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
#include <string>

//outofscope const struct;
const int              Action::_tablesize = 37;
const functionTable    Action::_table[37] = {
        {-10, &Action::kickMessage},
        {-9, &Action::inviteMessage},
        {-8, &Action::topicChangeMessage},
        {-7, &Action::flagChangeMessage},
        {-6, &Action::operatorChangeMessage},
        {-5, &Action::partmessage},
        {-4, &Action::quitmessage},
        {-3, &Action::privmessage},
        {-2, &Action::joinMessage},
        {-1, &Action::displayNick},
        {1, &Action::rplWelcome1},
        {2, &Action::rplYourHost2},
        {3, &Action::rplCreatead3},
        {4, &Action::rplMyInfo4},
        {324, &Action::rplChannelModeIs324},
        {331, &Action::rplNoTopic331},
        {332, &Action::rplTopic332},
        {341, &Action::rplInviting341},
        {353, &Action::rplNameply353},
        {366, &Action::rplEON366},
        {401, &Action::rplNoSuchNick401},
        {403, &Action::rplNoSuchChannel403},
        {404, &Action::rplCannotSendToChan404},
        {432, &Action::erroneusNick432},
        {433, &Action::nickTaken433},
        {441, &Action::rplUserNotInChannel441},
        {442, &Action::rplNotOnChannel442},
        {443, &Action::rplUserOnChannel443},
        {461, &Action::needMoreParams461},
        {462, &Action::alreadyRegister462},
        {464, &Action::badPass464},
        {471, &Action::rplChannelIsFull471},
        {473, &Action::inviteOnlyChan473},
        {475 ,&Action::badChannelKey475},
        {476, &Action::badChanMask476},
        {482, &Action::rplChanOPrivIsNeeded482},
        {501, &Action::rplUModeUnknownFlag501},
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

void        Action::shortCutContext(size_t i)
{
    if (_contextualArgs.size() < i)
        return;
    _contextualArgs.erase(i - 1);
    _contextualArgs += "...";
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

    std::string     Action::inviteMessage(void)
    {
            return (":" + _customer->getNick() + getUserHost() + " INVITE " + _cmdTarget + " :" + _chan->name);
    }

    std::string     Action::topicChangeMessage(void)
        {
            return (":" +  _customer->getNick() + " TOPIC " + _chan->name + " :" + _contextualArgs);
        }

        std::string     Action::kickMessage(void)
        {
            return (":" + _customer->getNick() + getUserHost() + " " + "KICK " + _chan->name + " " + _cmdTarget + _contextualArgs);
        }
        std::string     Action::flagChangeMessage(void)
        {
            return (":" + _customer->getNick() + " MODE " + _chan->name + " " + _contextualArgs);
        }
        std::string     Action::operatorChangeMessage(void)
        {
            return (":" + _customer->getNick() + " " + _cmdName + " " + _chan->name + " " + _contextualArgs + "o " + _cmdTarget);
        }

        std::string     Action::partmessage(void)
        {
            return (":" + _customer->getNick() + getUserHost() + " PART " + _chan->name + " " + _contextualArgs);
        }

        std::string     Action::quitmessage(void)
        {
            return (":" + _customer->getNick() + getUserHost() + " QUIT :" + _cmdArgs);
        }
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

        std::string     Action::rplChannelModeIs324(void)
        {
            return getPrefixTrio("324") +  _chan->name + " +" + _contextualArgs;
        }

        std::string     Action::rplNoTopic331(void)
        {
            return  getPrefixTrio("331") + _chan->name + " :No topic is set";
        }

        std::string     Action::rplTopic332(void)
        {
            return  getPrefixTrio("332") + _chan->name + " :" + _chan->getTopic();
        }

        std::string     Action::rplInviting341(void)
        {
            return getPrefixTrio("341") + _cmdTarget + " " + _chan->name;
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
            _contextualArgs = _cmdTarget;
            shortCutContext(50);
            return (getPrefixTrio("403") + _cmdTarget + " :No such Channel");
        }

        std::string     Action::rplCannotSendToChan404(void)
        {
            _contextualArgs = _cmdTarget;
            shortCutContext(15);
            return (getPrefixTrio("404") + _contextualArgs + " :Cannot send to channel");
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

            return getPrefixTrio("433") + _cmdArgs.substr(0, _cmdArgs
                .find(' ')) + " " + errormsg;
        }

        std::string     Action::rplUserNotInChannel441(void)
        {
             return  getPrefixTrio("441") + _customer->getNick() + " " + _cmdTarget + " " + _chan->name + " :They aren't on that channel";
        }

        std::string     Action::rplNotOnChannel442(void)
        {
            return getPrefixTrio("442") + _chan->name + " :You're not on that channel";
        }

        std::string     Action::rplUserOnChannel443(void)
        {
            return  getPrefixTrio("443") + _customer->getNick() + " " + _cmdTarget + " " + _chan->name + " :is already on channel";
        }
    //general
        std::string     Action::needMoreParams461(void)
        {
            std::string errormsg(" :Not enough parameters");

            return getPrefixTrio("461") + _cmdName + errormsg;
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

            return getPrefixTrio("464") + errormsg;
        }
    //channel related one

        std::string     Action::rplChannelIsFull471(void)
        {
            return getPrefixTrio("471") + _chan->name + " :Cannot join Channel (+l)";
        }
        std::string     Action::inviteOnlyChan473(void)
        {
            return getPrefixTrio("473") + _chan->name + " :Cannot join channel (+i)";
        }

        std::string     Action::badChannelKey475(void)
        {
            return getPrefixTrio("475") + _chan->name + " :Cannot join channel (+k)";
        }

        std::string     Action::badChanMask476(void)
        {
            shortCutContext(50);
            return getPrefixTrio("476") +  _contextualArgs + " :Bad channel mask";
        }

        std::string     Action::rplChanOPrivIsNeeded482(void)
        {
            return getPrefixTrio("482") + _chan->name + ":You're not channel operator";
        }
    //mode related one
        std::string     Action::rplUModeUnknownFlag501(void)
        {
            return getPrefixTrio("501") + ": Unknown MODE flag";
        }
    //protected
        bool    Action::onlySpace(void)
        {
                return (_cmdArgs.find_first_not_of(' ') == std::string::npos);
        }

        bool    Action::globalParse(void)
        {
            size_t  n;
            if (_cmdArgs.empty() || onlySpace())
            {
                _customer->addSend(generateMsg(461));
                return false;
            }
            n = _cmdArgs.find(' ');
            if (n == 0)
            {
                _customer->addSend(generateMsg(461));
                    return false;
            }
            return true;
        }
