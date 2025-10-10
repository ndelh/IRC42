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

bool    PrivateMsg::parseCmd(void)
{
        size_t  n;

        n = _cmdArgs.find(' ');
        if (n == std::string::npos)
        {
            //display error;
            return false;
        }
        _cmdTarget = _cmdArgs.substr(0, n);
        _contextualArgs = _cmdArgs.substr(n + 1);
        std::cout << "target:" << _cmdTarget << std::endl;
        std::cout << "text msg:" << _contextualArgs << std::endl;
        if (_cmdTarget.empty() || _contextualArgs.size() < 2)
        {
            _customer->addSend(generateMsg(461));
            return false;
        }
        if (_contextualArgs[0] != ':')
        {
            //display error
            return false;
        }
        std::cout << "success on parse" << std::endl;
        return true;
}

///main Action 

    //chabbelmsg
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
                _customer->addSend(generateMsg(401));
            else
                receiver->addSend(generateMsg(-3));
        }
void    PrivateMsg::act(void)
{

        std::cout << "begin of act pvmsg" << std::endl;
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