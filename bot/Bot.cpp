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

//constructor and destructor

Bot::Bot(const std::string& name, const std::string& pass, const std::string& port): _name(name), _pass(pass)
{
    _port = std::atoi(port.c_str());
    if (port.size() > 5 || _port < 1024 || _port > 49151)
        throw std::out_of_range("port number must be comprised between [1024, 49151]");
    _mustDie = false;
    _registrationDone = false;
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
    _sendBuffer += "USER BOT * 0 BOTCHIEN\r\n";
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
            send(_kfd, line.c_str(), line.size(), 0);
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
            _mustDie = true;
            return ;
        }
        act();
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

void    Bot::barking(std::string& line)
{
        std::istringstream  flux(line);
        std::string         word1;
        std::string         word2;
        std::string         word3;
        std::string         target;

        if (line.size() < 1 || *(line.end() + 1) != '+')
            return ;
        if (!(flux >> word1 >> word2 >>word3 >> target))
            return ;
        
}

void    Bot::act(void)
{
        std::istringstream  flux(_receiveBuffer);
        std::string         line;

        if (!_registrationDone)
            logSystem();
        else
        {
            while (getline(flux, line))
                barking(line);
        }
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