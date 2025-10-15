/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMsg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 16:26:32 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/09 16:30:56 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "PrivateMsg.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

//constructor && destructor

PrivateMsg::PrivateMsg(Server* Base, Client* Customer, const std::string& args): Action(Base, Customer, args)
{
}

PrivateMsg::~PrivateMsg(void)
{
}

//parse

bool   PrivateMsg::splitCmd(size_t n)
{
        _cmdTarget = _cmdArgs.substr(0, n);
        _contextualArgs = _cmdArgs.substr(n + 1);
        if (_contextualArgs.size() < 2 || _contextualArgs[0] != ':')
        {
            _customer->addSend(generateMsg(461));
            return false;
        }
        return true;
}

bool    PrivateMsg::parseCmd(void)
{
        size_t  n;

        n = _cmdArgs.find(' ');
        if (n == std::string::npos)
        {
            _customer->addSend(generateMsg(461));
            return false;
        }
        return splitCmd(n);
}

///main Action 

    //channelmsg
        bool    PrivateMsg::sendChanOk(void)
        {
                if (!_chan)
                {
                    _customer->addSend(generateMsg(403));
                    return false;
                }
                if (!_chan->isMember(_customer->getNick()))
                {
                    _customer->addSend(generateMsg(404));
                    return false;
                }
                return true;
        }

        void    PrivateMsg::channelMsg(void)
        {
            _chan = _base->existingChan(_cmdTarget);
            if (!sendChanOk())
                return;
            _chan->broadcastOthers(generateMsg(-3), _customer->getNick());
        }

    //personnal msg
        void    PrivateMsg::clientMsg(void)
        {
            Client* receiver;

            receiver = _base->existingClient(_cmdTarget);
            if (!receiver)
            {
                _customer->addSend(generateMsg(401));
                return ;
            }
            receiver->addSend(generateMsg(-3));
        }

        void    PrivateMsg::act(void)
{
        _cmdName = "PRIVMSG";
        if(!globalParse())
            return;
        if (!parseCmd())
            return;
        if (_cmdTarget[0] == '#')
            channelMsg();
        else
            clientMsg();
}


//outofscope

void    buildExecutePrivateMsg(Server* Base, Client *Customer, const std::string& args)
{
        PrivateMsg  a(Base, Customer, args);

        a.act();
}