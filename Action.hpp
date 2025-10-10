/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Action.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doley <doley@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 16:27:25 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/08 16:22:46 by doley            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <map>

class Server;
class Client;
class Channel;
class Action;

typedef std::string (Action::*pointer)(void);
struct functionTable
{
        int     code;
        pointer    funct;
};


class Action
{
    public:
            Action(Server* serv, Client* customer, const std::string& cmdArgs);
            virtual~Action(void);
            //function
            virtual void        act(void);
            std::string         generateMsg(int code);
    private:
        //factorising function
                //customized one;
                        std::string     privmessage(void);
                        std::string     joinMessage(void);
                        std::string     displayNick(void);
                //standardized rpl
                        std::string     rplWelcome1(void);
                        std::string     rplYourHost2(void);
                        std::string     rplCreatead3(void);
                        std::string     rplMyInfo4(void);
                //join related
                        std::string     rplTopic332(void);
                        std::string     rplNameply353(void);
                        std::string     rplEON366(void);
                //client related
                        std::string     rplNoSuchNick401(void);
                        std::string     rplNoSuchChannel403(void);
                        std::string     rplCannotSendToChan404(void);
                //nick related
                        std::string     erroneusNick432(void);
                        std::string     nickTaken433(void);
                //general
                        std::string     needMoreParams461(void);
                //user related
                        std::string     alreadyRegister462(void);
                //pass related
                        std::string     badPass464(void);
                //chan related
                        std::string     inviteOnlyChan473(void);
                        std::string     badChannelKey475(void);

        //factory utilitary fonction
                std::string     getprefix(void);
                std::string     getUserHost(void);
                std::string     getPrefixTrio(const std::string& code);

        //container
                static const int                _tablesize;
                static const functionTable      _table[20];


    protected:
            Server*             _base;
            Client*             _customer;
            Channel*            _chan;
            std::string         _cmdArgs;
            std::string         _contextualArgs;
            std::string         _cmdTarget;

};
