/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:48:35 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/10 11:49:18 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "QUIT.hpp"

Quit::Quit(Server* base, Client* customer, const std::string& args): Action(base, customer, args)
{
}

Quit::~Quit(void)
{
}

//Main function

void    Quit::act(void)
{
        _customer->eraseTrace();
        _customer->setMustKill();
        _customer->broadcastToLinked(generateMsg(-4));
        _base->eraseClient(_customer->getFd());

}

//out of scope

void    buildExecuteQuit(Server* base, Client* customer, const std::string& args)
{
        Quit    a(base, customer, args);

        a.act();
}
