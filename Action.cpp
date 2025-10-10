/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Action.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 16:28:20 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/05 16:29:22 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Action.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

//outofscope const struct;
const int              Action::_tablesize = 20;
const functionTable    Action::_table[20] = {
        {-3, &Action::privmessage},
        {-2, &Action::joinMessage},
        {-1, &Action::displayNick},
        {1, &Action::rplWelcome1},
        {2, &Action::rplYourHost2},
        {3, &Action::rplCreatead3},
        {4, &Action::rplMyInfo4},
        {332, &Action::rplTopic332},
        {353, &Action::rplNameply353},
        {366, &Action::rplEON366},
        {401, &Action::rplNoSuchNick401},
        {403, &Action::rplNoSuchChannel403},
        {404, &Action::rplCannotSendToChan404},
        {432, &Action::erroneusNick432},
        {433, &Action::nickTaken433},
        {461, &Action::needMoreParams461},
        {462, &Action::alreadyRegister462},
        {464, &Action::badPass464},
        {473, &Action::inviteOnlyChan473},
        {475 ,&Action::badChannelKey475},
};

Action::Action(Server* base, Client* customer, const std::string& cmdArgs): _base(base), _customer(customer), _cmdArgs(cmdArgs)
{
    _chan = NULL;
}

Action::~Action(void)
{
}

//command factory utilitary

std::string  Action::getprefix(void)
{
    return ":" + _base->servName + " ";
}

std::string Action::getUserHost(void)
{
    return "!" + _customer->getUser() + "@"+ _customer->getHost();
}

std::string Action::getPrefixTrio(const std::string& code)
{
    return getprefix() + code + " " + _customer->getNick() + " ";
}
//act

void        Action::act()
{
}
//command factory

std::string  Action::generateMsg(int code)
{
    int             i;

    i = 0;
    for(i = 0; i < _tablesize; i++)
    {
        if (_table[i].code == code)
            break;
    }
    return ((this->*_table[i].funct)());
}

    //personalized one
        std::string     Action::privmessage(void) //need to add a cutting system here
        {
            return (":" + _customer->getNick() + getUserHost() + " PRIVMSG " + _cmdTarget + " " + _contextualArgs);
        }

        std::string     Action::displayNick(void)
        {
            return ":" + _contextualArgs + getUserHost() + " NICK :" + _customer->getNick();
        }

        std::string     Action::joinMessage(void)
        {
            return ":" + _customer->getNick() + getUserHost() + " " + "JOIN :" + _chan->name;
        }
    //standardized rpl

        std::string     Action::rplWelcome1(void)
        {
            return getPrefixTrio("001") + ":Welcome to the " + _base->servName + " Network, " + _customer->getNick() + getUserHost();
        }

        std::string     Action::rplYourHost2(void)
        {
            return getPrefixTrio("002") + ":Your host is " + _base->servName + ", running version " + _base->version;
        }

        std::string     Action::rplCreatead3(void)
        {
            return getPrefixTrio("003") + ":This server was created on " + _base->get_time();
        }

        std::string     Action::rplMyInfo4(void)
        {
            return getPrefixTrio("004") + _base->servName + " " + _base->version + " " + _base->userMode + " " + _base->channelMode;
        }

    //join related one
        //join utilitary

        //join main;
        std::string     Action::rplTopic332(void)
        {
            return  getPrefixTrio("332") +  " " + _chan->name + " :";
        }

        std::string     Action::rplNameply353(void)
        {
            std::string     prefix;
            std::string     namelist;

            prefix = getPrefixTrio("353") + "= " + _chan->name + " :";
            namelist = _chan->displayMembers();
            return (prefix + namelist);
        }

        std::string     Action::rplEON366(void)
        {
            return (getPrefixTrio("366") + _chan->name + " :End of /NAMES list");
        }
    //client related one
        std::string     Action::rplNoSuchNick401(void)
        {
            return (getPrefixTrio("401") + _cmdTarget + " :No such Nick");

        }

        std::string     Action::rplNoSuchChannel403(void)
        {
            return (getPrefixTrio("403") + _cmdTarget + " :No such Channel");
        }

        std::string     Action::rplCannotSendToChan404(void)
        {
            return (getPrefixTrio("404") + _cmdTarget + " :Cannot send to channel");
        }
    //nick related one
        std::string     Action::erroneusNick432(void)
        {
            std::string errormsg(":Erroneus nickname");

            return getPrefixTrio("432") + _cmdArgs.substr(0, _cmdArgs.find(' ')) + " " + errormsg;
        }

        std::string     Action::nickTaken433(void)
        {
            std::string errormsg(":Nickname is already in use");

            return getPrefixTrio("433") + _cmdArgs.substr(0, _cmdArgs.find(' ')) + " " + errormsg;
        }
    //general
        std::string     Action::needMoreParams461(void)
        {
            std::string errormsg(":Not enough parameters");

            return getPrefixTrio("461") + errormsg;
        }
    //user related one
        std::string     Action::alreadyRegister462(void)
        {
            std::string errormsg(":You may not register");

            return getPrefixTrio("462") + errormsg;
        }

    //pass related one
        std::string     Action::badPass464(void)
        {
            std::string errormsg(":Password incorrect");

            return getPrefixTrio("464") + _customer->getNick()+ " " + errormsg;
        }
    //channel related one
        std::string     Action::inviteOnlyChan473(void)
        {
            return getPrefixTrio("473") + _chan->name + " :Cannot join channel (+i)";
        }

        std::string     Action::badChannelKey475(void)
        {
            return getPrefixTrio("475") + _chan->name + " :Cannot join channel (+k)";
        }
