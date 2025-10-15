/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 16:59:33 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/11 17:03:26 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "PART.hpp"

//constructor & destructor

Part::Part(Server* base, Client* customer, const std::string& args): Action(base, customer, args) 
{
}
Part::~Part(void)
{
}



//parse split

    void    Part::parseSplitArgs()
    {
            size_t n;

            n = _cmdArgs.find(' ');
            if (n != std::string::npos)
            {
                _contextualArgs = _cmdArgs.substr(n + 1);
                _cmdArgs.erase(n);
            }

    }

//main action

void      Part::act(void)
{
        _cmdName = "PART";
        if (!globalParse())
            return;
        parseSplitArgs();
        proceedPart();

}
    //proceed
        void    Part::channelLeave(void)
        {
                if (!_chan->isMember(_customer->getNick()))
                {
                    _customer->addSend(generateMsg(442));
                    return;
                }
                _chan->broadcastMembers(generateMsg(-5));
                _chan->customerLeave(_customer);
                _customer->removeMembership(_chan->name);
        }

        void    Part::proceedPart(void)
        {
            std::string         line;
            std::stringstream   flux(_cmdArgs);

            while (getline(flux, line, ','))
            {
                _chan = _base->existingChan(line);
                if (!_chan)
                    _customer->addSend(generateMsg(403));
                else
                    channelLeave();
            }
        }

//out of scope

void    buildExecutePart(Server* base, Client* customer, const std::string& cmdArgs)
{
        Part a(base, customer, cmdArgs);

        a.act();
}