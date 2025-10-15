/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doley <doley@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 16:46:01 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/08 17:11:31 by doley            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NICK.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include <string>
// constructor && destructor
Nick::Nick(Server *base, Client *customer, const std::string &cmdArgs) : Action(base, customer, cmdArgs)
{
}
Nick::~Nick(void)
{
}

// main function

void Nick::act(void)
{

    std::istringstream flux(_cmdArgs);
    std::string line;

    _cmdName = "NICK";
    if (!globalParse())
        return;
    getline(flux, line, ' ');
    if (!validNick(line))
        return;
    _contextualArgs = _customer->getNick();
    if (_contextualArgs != "*")
        changeNick(line);
    else
        registerNick(line);
    updateInformation();
}

// parseFunction

bool Nick::validNick(const std::string &line)
{
    std::string invalid = " ,:*?!@.";

    if (line.size() > 15 || line.find_first_of(invalid) != std::string::npos)
    {
        _customer->addSend(generateMsg(432));
        return false;
    }
    if (!_base->nickAvailable(line))
    {
        _customer->addSend(generateMsg(433));
        return false;
    }
    return true;
}
// sub function

void Nick::registerNick(const std::string &line)
{
    _customer->setNick(line);
    _base->addPhonebook(line, _customer);
}

void Nick::changeNick(const std::string &line)
{
    _base->removePhonebook(_customer->getNick());
    _customer->setNick(line);
    _base->addPhonebook(line, _customer);
    _customer->updateNick(_contextualArgs);
}

void Nick::updateInformation(void)
{

    if (!_customer->isRegistered())
        return;
    _customer->broadcastToLinked(generateMsg(-1));
}
// out of Class scope

void buildExecuteNick(Server *base, Client *customer, const std::string &cmdargs)
{
    Nick a(base, customer, cmdargs);

    a.act();
}
