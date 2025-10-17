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
    (void)_pass;
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
	if (connect(_kfd, (struct sockaddr*)&server, sizeof(server)))
        throw std::runtime_error("connecting to irc server has failed");


}

//veil
void    Bot::veil(void)
{
    connectKennel();
    std::cout << "veilling" << std::endl;
}