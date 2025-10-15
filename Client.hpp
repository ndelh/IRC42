/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doley <doley@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 15:35:24 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/15 19:40:41 by doley            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <map>
#include <sstream>
#include "NICK.hpp"
class Server;
class Client;
class Channel;
class Action;

typedef  void (*function)(Server*, Client*, const std::string&);
struct  repartitor
{
    std::string name;
    function    tolaunch;
};

class Client
{
        public:
            //constructor & destructor
                Client(int fd, Server& base, const std::string& ip);
                ~Client(void);
            //setters
                //status changer
                    void    setDisconnected(void);
                    void    setNick(const std::string& name);
                    void    updateNick(const std::string& oldname);
                    void    setUser(const std::string& username, const std::string& realname);
                    void    setPassChecked(void);
                    void    setMustKill(void);
                //adders
                    void    addReceived(const std::string& toAdd);
                    void    addSend(const std::string& toAdd);
                    void    addMembership(const std::string& name, Channel* chan);
                //remover
                    void    eraseTrace();
                    void    removeMembership(const std::string& name);
                    void    eraseWbuffer();
                //getters
                    //boolean
                        bool    mustSend(void);
                        bool    mustKill(void);
                        bool    userSet(void);
                        bool    isRegistered(void);
                    //identities
                        int                 getFd(void);
                        const std::string&  getNick(void);
                        const std::string&  getUser(void);
                        const std::string&  getHost(void);
            //broadcast
                void    broadcastToLinked(const std::string& msg);
            //action related command
                //standard system
                    void    act(void);
                //send system
                    void    sendmsg(void);
            //variables


        private:
            //variable
                //identity
                     const int          fd;
                     const std::string  _host;
                     Server*            _base;
                     std::string        _Nick;
                     std::string        _username;
                     std::string        _realname;
                //state of Client
                    bool    _mustKill;
                    bool    _disconnected;
                    bool    _passChecked;
                    bool    _fullyRegistered;
                //buffer
                    std::string     _received;
                    std::string     _tosend;
            //container
                    std::map<std::string, Channel*>     _membership;
            //function map
                static repartitor         functMap[11];
                static const int          functMapSize;
            //action related command
                //log system
                    bool    securityCheck(std::istringstream& flux);
                    void    registration(std::istringstream& flux);
                    void    registrationEnd(void);
                    void    clientLog(std::istringstream& flux);
                //action system
                    void    clientCmd(std::string& line);
};
