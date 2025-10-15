/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doley <doley@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 15:35:32 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/15 19:41:47 by doley            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "NICK.hpp"
#include "USER.hpp"
#include "JOIN.hpp"
#include "PASS.hpp"
#include "PrivateMsg.hpp"
#include "QUIT.hpp"
#include "PART.hpp"
#include "MODE.hpp"
#include "TOPIC.hpp"
#include "KICK.hpp"
#include "INVITE.hpp"
#include <unistd.h>

// static data

const int Client::functMapSize = 11;

repartitor Client::functMap[11] = {
    {"NICK", &buildExecuteNick},
    {"USER", &buildExecuteUser},
    {"JOIN", &buildExecuteJoin},
    {"PASS", &buildExecutePass},
    {"PRIVMSG", &buildExecutePrivateMsg},
    {"QUIT", &buildExecuteQuit},
    {"PART", &buildExecutePart},
    {"MODE", &buildExecuteMode},
    {"TOPIC", &buildExecuteTopic},
    {"KICK", &buildExecuteKick},
    {"INVITE", &buildExecuteInvite},
};

// constructor && destructor
Client::Client(int fd, Server &base, const std::string &ip) : fd(fd), _host(ip), _base(&base), _Nick("*")
{
    _mustKill = false;
    _passChecked = false;
    _fullyRegistered = false;
    _disconnected = false;
}

Client::~Client(void)
{
    close(fd);
}

// getter
// boolean returns
bool Client::mustSend(void)
{
    return (!_tosend.empty());
}

bool Client::mustKill(void)
{
    return (_mustKill);
}

bool Client::userSet(void)
{
    return (!_username.empty());
}

bool Client::isRegistered(void)
{
    return (_fullyRegistered);
}

// identity return
const std::string &Client::getNick(void)
{
    return (_Nick);
}

const std::string &Client::getUser(void)
{
    return (_username);
}

const std::string &Client::getHost(void)
{
    return (_host);
}

int                Client::getFd(void)
{
    return fd;
}

// setter
// status updaters
void Client::setDisconnected(void)
{
    _disconnected = true;
}

void Client::setNick(const std::string &name)
{
    _Nick = name;
}
void Client::setUser(const std::string &username, const std::string &realname)
{
    _realname = realname;
    _username = username;
}
void Client::setPassChecked()
{
    _passChecked = true;
}
void Client::setMustKill()
{
    _mustKill = true;
}

void Client::updateNick(const std::string& Oldnick)
{
    std::map<std::string, Channel*>::iterator   it;

    for (it = _membership.begin(); it != _membership.end(); it++)
            it->second->updateNick(Oldnick, this);
}

// adders
void    Client::addReceived(const std::string &toAdd)
{
    _received += toAdd;
}

void    Client::addSend(const std::string &toAdd)
{
    if (!_disconnected)
        _tosend += toAdd + "\r\n";
}

void    Client::addMembership(const std::string& name, Channel* chan)
{
    _membership.insert(std::make_pair(name, chan));
}

//remover

void     Client::removeMembership(const std::string& name)
{
        _membership.erase(_membership.find(name));
}

void    Client::eraseWbuffer(void)
{
        _tosend.erase();
}

void    Client::eraseTrace(void)
{
    std::map<std::string, Channel*>::iterator   it;

    for (it = _membership.begin(); it != _membership.end(); it++)
    {
        it->second->customerLeave(this);
    }
}

// command related function

// pure utilitary
std::string extractCmd(std::string &line)
{
    size_t n;
    std::string cmd;
    while (!line.empty())
    {
        char last = line[line.size() - 1];
        if (last == '\r' || last == '\n')
            line.erase(line.size() - 1);
        else
            break;
    }
    if ((n = line.find(' ')) == std::string::npos)
    {
        cmd = line;
        line.clear();
    }
    else
    {
        cmd = line.substr(0, n);
        line.erase(0, n + 1);
    }
    return (cmd);


}
// log part
bool Client::securityCheck(std::istringstream &flux)
{
    std::string line;

    getline(flux, line);
    if (line == "CAP LS 302\r")
        getline(flux, line);
    if (extractCmd(line) == "PASS")
        buildExecutePass(_base, this, line);
    else
        _mustKill = true;
    return (_passChecked);
}

void Client::registrationEnd(void)
{
    Action a(_base, this, "");

    if (_Nick == "*" || _username.empty())
        return;
    _fullyRegistered = true;
    addSend(a.generateMsg(1));
    addSend(a.generateMsg(2));
    addSend(a.generateMsg(3));
    addSend(a.generateMsg(4));
}

void Client::registration(std::istringstream &flux)
{
    std::string line;
    std::string cmd;

    while (!_fullyRegistered && getline(flux, line))
    {
        cmd = extractCmd(line);
        std::cout << "cmd: [" << cmd << "]" << std::endl;
        if (cmd == "NICK")
            buildExecuteNick(_base, this, line);
        else if (cmd == "USER")
            buildExecuteUser(_base, this, line);
        else
            addSend("ERROR :you are not registered");
        registrationEnd();
    }
}

void Client::clientLog(std::istringstream &flux)
{
    if (!_passChecked)
        if (securityCheck(flux) == false)
            return;
    if (!_fullyRegistered)
        registration(flux);
}
// act system
void Client::clientCmd(std::string &line)
{
    std::string cmd;

    cmd = extractCmd(line);
    for (int i = 0; i < functMapSize; i++)
    {
        if (cmd == functMap[i].name)
        {
            (functMap[i].tolaunch)(_base, this, line);
            return;
        }
    }
    std::cout << "command invalid command" << std::endl;
}

void Client::act(void)
{
    std::istringstream flux(_received);
    std::string line;

    std::cout << "currenty working with this buffer \n"
              << _received << std::endl;
    _received.erase();
    if (!_fullyRegistered)
        clientLog(flux);
    if (_disconnected)
    {
        _tosend.erase();
        _mustKill = true;
    }
    while (getline(flux, line) && !_mustKill)
        clientCmd(line);

}

// sending msg
void Client::sendmsg(void)
{
    std::istringstream flux(_tosend);
    std::string line;

    std::cout << "send buffer" << std::endl
              << _tosend << std::endl;
    while (getline(flux, line))
    {
        line += '\n';
        send(fd, line.c_str(), line.size(), 0);
    }
    _tosend.erase();
}

//broadcasting

void    Client::broadcastToLinked(const std::string& msg)
{
    std::map<std::string, Channel*>::iterator   itChan;
    std::set<std::string>                       alreadyDiffused;

    addSend(msg);
    alreadyDiffused.insert(getNick());
    for (itChan = _membership.begin(); itChan != _membership.end(); itChan++)
    {
        itChan->second->partialBroadcast(alreadyDiffused, msg);
    }
}
