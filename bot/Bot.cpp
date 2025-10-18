/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 13:08:36 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/17 13:08:49 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <stdlib.h>
#include <unistd.h>

//constructor and destructor

int  Bot::kennelMembers = 10;
Doggo   Bot:: kennelPool[10] = {
        {"Shiba", "GRR WOOF WOOF"},
        {"GoldenRetriever", "Bwaf Bwaf"},
        {"BorderCollie", "Bark Bark"},
        {"Husky", "AWOUWOUWOUWOUWOU"},
        {"AfghanHound", "KAIKAIKAI"},
        {"Pinscher", "YIP YAP BARK"},
        {"Spaniel", "WAF WAF WAF"},
        {"Daschund", "Grrrrr grrrrr"},
        {"BelgianShepperd", "WAAAAF WAFFFF"},
        {"GermanShepperd", "WEEF WOOF"},
};

Bot::Bot(const std::string& name, const std::string& pass, const std::string& port): _name(name), _pass(pass)
{
    _port = std::atoi(port.c_str());
    if (port.size() > 5 || _port < 1024 || _port > 49151)
        throw std::out_of_range("port number must be comprised between [1024, 49151]");
    _mustDie = false;
    _registrationDone = false;
    _KennelCall = 0;
}

Bot::~Bot(void)
{
    if (_kfd != -1)
        close(_kfd);
}

//init connection

void    Bot::connectKennel(void)
{
    struct sockaddr_in	server;
	     
	_kfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_kfd == -1)
        throw std::runtime_error("socketing has failed");
    server.sin_family = AF_INET;
	server.sin_port = htons(_port);
	server.sin_addr.s_addr =  inet_addr("127.0.0.1");
	if (connect(_kfd, (struct sockaddr*)&server, sizeof(server)) == -1)
        throw std::runtime_error("connecting to server has failed");
}

bool    Bot::initEpoll(void)
{
    struct epoll_event events;

    events.events = EPOLLOUT;
    events.data.fd = _kfd;
    _pollFd = epoll_create(1);
    if (_pollFd == -1)
    {
        std::cerr << "epoll create failed" << std::endl;
        return false;
    }
    epoll_ctl(_pollFd, EPOLL_CTL_ADD, _kfd, &events);
    _sendBuffer += "PASS " + _pass + "\r\n";
    _sendBuffer += "NICK " + _name + "\r\n";
    _sendBuffer += "USER BOT * 0 BOTDOG\r\n";
    return true;
}

//veil

void    Bot::sendMsg(void)
{
        std::istringstream  flux(_sendBuffer);
        std::string         line;
        epoll_event         ev; 

        ev.events = EPOLLIN;
        ev.data.fd = _kfd;
        while (getline(flux, line))
        {
            line += "\n";
            if (send(_kfd, line.c_str(), line.size(), 0) != - 1)
                std::cout << "sended" << std::endl;
            else
                std::cout << "send error" << std::endl;
            std::cout << "line sended" << line << std::endl;
        }
        _sendBuffer.erase();
        epoll_ctl(_pollFd, EPOLL_CTL_MOD, _kfd, &ev);
}

void    Bot::receiveMsg(void)
{
        epoll_event     ev;
        char            BUFFER[1024];
        int             n;

        std::cout << "received" << std::endl;

        n = recv(_kfd, BUFFER, 1024, MSG_DONTWAIT);
            while (n > 0)
            {
                std::string a(BUFFER, n);
                _receiveBuffer += a;
                n = recv(_kfd, BUFFER, 1024, MSG_DONTWAIT);
            }
            std::cout << _receiveBuffer << std::endl;
        if (n == 0)
        {
            std::cout << "n was equal to 0" << std::endl;
            _mustDie = true;
            return ;
        }
        act();
        std::cout << _sendBuffer << std::endl;
        if (!_sendBuffer.empty())
        {
            ev.events = EPOLLIN | EPOLLOUT;
            ev.data.fd = _kfd;
            epoll_ctl(_pollFd, EPOLL_CTL_MOD, _kfd, &ev);
        }

}

void    Bot::logSystem(void)
{
        std::stringstream   flux(_receiveBuffer);
        std::string line;
        std::string word2;

        if (!(flux >> line >> word2) || (word2 != "001"))
        {
            _mustDie = true;
            std::cerr << "cannot log onto the server, problem can be a wrong password or someone already took Kennel as a Nick" << std::endl;
        }
        else
            _registrationDone = true;
}

bool    extractName(std::string& line)
{
        size_t  n;

        if (line.size() < 3 || line[0] != ':')
            return false;
        line.erase(0, 1);
        if ((n = line.find('!')) == std::string::npos)
            return false;
        line = line.substr(0, n);
        if ( line.empty() || line[0] == '#')
            return false;
        std::cout << line << std::endl;
        return true;
}

void    Bot::releaseDog(std::string& target)
{
        if (target.size() < 2 || target[0] != ':')
            return;
        target.erase(0, 1);
        _sendBuffer += "NICK " + kennelPool[_KennelCall].species + "\r\n";
        _sendBuffer += "PRIVMSG " + target + " :" + kennelPool[_KennelCall].bark + "\r\n";
        _sendBuffer += "NICK Kennel\r\n";
        _KennelCall++;
        if (_KennelCall > 9)
            _KennelCall = 0;
}

void    Bot::barking(std::string& line)
{
        std::istringstream  flux(line);
        std::string         author;
        std::string         word2;
        std::string         word3;
        std::string         target;

        if (line.size() < 1 || *(line.end() - 1) != '\r')
            return ;
        if (!(flux >> author >> word2 >>word3 >> target) || word2 != "PRIVMSG")
            return ;
        if (!extractName(author))
            return ;
        if (author != "HoundMaster")
            _sendBuffer += ("PRIVMSG " + author + " :I dont recognize you as my master, get lost fool\r\n");
        else
            releaseDog(target);
}

void    Bot::act(void)
{
        std::istringstream  flux(_receiveBuffer);
        std::string         line;

        if (_registrationDone)
            std::cout << "registration done" << std::endl;
        if (!_registrationDone)
            logSystem();
        else
        {
            while (getline(flux, line))
                barking(line);
        }
        _receiveBuffer.erase();
}

void    Bot::veil(void)
{
    epoll_event surveil[1];
    int         n;

    std::cout << "veilling" << std::endl;
    connectKennel();
    if (!initEpoll())
        return ;
    while (1)
    {
        n = epoll_wait(_pollFd, surveil, 1, 1000000);
        if (n > 0)
        {
            if (surveil[0].events & EPOLLOUT)
                sendMsg();
            if (surveil[0].events & EPOLLIN)
                receiveMsg();
            if ((surveil[0].events & EPOLLHUP) || (surveil[0].events & EPOLLERR))
                _mustDie = true;
        }
        if (_mustDie)
            return ;
    }
}