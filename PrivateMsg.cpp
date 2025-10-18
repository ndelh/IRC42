/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMsg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doley <doley@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 16:26:32 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/18 12:46:19 by doley            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "PrivateMsg.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

//constructor && destructor

PrivateMsg::PrivateMsg(Server* Base, Client* Customer, const std::string& args): Action(Base, Customer, args)
{
    _cmdName = "PRIVMSG";
}

PrivateMsg::~PrivateMsg(void)
{
}

//parse

bool   PrivateMsg::splitCmd(void)
{
        std::istringstream  flux(_cmdArgs);
        std::string         line;
        size_t              n;

        while (getline(flux, line, ' '))
        {
            if (!line.empty() && line[0] == ':')
                break;
            if (!line.empty())
                _receiverList.push_back(line);
        }
        n = _cmdArgs.find(':');
        if (n != std::string::npos)
            _contextualArgs = _cmdArgs.substr(n);
        if (_contextualArgs.empty() || _receiverList.empty())
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
        return splitCmd();
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
            if (_contextualArgs.length() > 450)
            {
                std::string tmp = _contextualArgs.substr(450);
                _contextualArgs.erase(450, _contextualArgs.length());
                _chan->broadcastOthers(generateMsg(-3), _customer->getNick());
                _contextualArgs = tmp;
                _chan->broadcastOthers(generateMsg(-3), _customer->getNick());
            }
            else
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
            if (_contextualArgs.length() > 450)
            {
                std::string tmp = _contextualArgs.substr(450);
                _contextualArgs.erase(450, _contextualArgs.length());
                receiver->addSend(generateMsg(-3));
                _contextualArgs = tmp;
                receiver->addSend(generateMsg(-3));
            }
            else
                receiver->addSend(generateMsg(-3));
        }

void    PrivateMsg::executeSend(void)
{
        std::vector<std::string>::iterator  it;

        it = _receiverList.begin();
        for (; it != _receiverList.end(); it++)
        {
            _cmdTarget = *it;
            if (_cmdTarget[0] == '#')
                channelMsg();
            else
                clientMsg();
        }

}

void    PrivateMsg::act(void)
{
        if(!globalParse())
            return;
        if (!parseCmd())
            return;
        executeSend();
}


//outofscope

void    buildExecutePrivateMsg(Server* Base, Client *Customer, const std::string& args)
{
        PrivateMsg  a(Base, Customer, args);

        a.act();
}
