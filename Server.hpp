/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 11:42:21 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/04 11:42:33 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <cstring>
#include <stdexcept>
#include <map>
#include <vector>

class Client;
class Channel;

class Server{

    public:

        Server(char* port, char* password);
        ~Server(void);
        //getter
            //verification
                bool        isPassCorrect(const std::string& test);
                bool        nickAvailable(const std::string& name);
                Channel*    existingChan(const std::string& name);
                Client*     existingClient(const std::string& name);

        //core function
            //routine
                void    routine(void);
        //setters
                //adders
                    void    addPhonebook(const std::string& name, Client* customer);
                    void    addChannelList(const std::string& name, Channel* chan);
                //removers
                    void    removePhonebook(const std::string&  name);
                    void    removeChannelList(const std::string& name);
                    void    eraseClient(int fd);
                    
        //public variables
            const std::string   servName;
            const std::string   version;
            const std::string   userMode;
            const std::string   channelMode;
    private:
    
        //private function 
            //server intialization/ epoll management and signals
                void        launchServ(void);
                void        signals_init(void);
                static void SIGINT_handler(int);
                void        setHostname(void);
                void        epollInit(void);
                void        addWFlag(int fd);
                void        removeWFlag(int fd);
                
            //server destruction
                void        cleanMapsAlloc(void);
            //client gestion
                void                                    acceptClient(void);
                void                                    forceDisconnect(int fd);
                void                                    removeOnIncident(int fd);
                
            //channel gestion
                std::map<std::string, Channel*>::iterator   killChannel(std::map<std::string, Channel*>::iterator& itChannel);
            //sub routine
                void        processRead(int fd);
                void        processWrite(int fd);
                void        watchRoutine(void);
                void        watchClient(void);
                void        watchChannel(void);
                std::map<int, Client*>::iterator        killClient(std::map<int, Client*>::iterator& itClient);
        //private variables
            //simple variables
                const int           _port;
                const std::string   _servPass;
                const uint32_t       _rFlags;
                const uint32_t       _wrFlags;
                int                 _epollFd;
                int                 _serverSocket;
                sockaddr_in         _servAddr;
                char                _hostname[256];
            //container
                std::map<int, Client*> _clientList;
                std::map<std::string, Client*> _phoneBook;
                std::map<std::string, Channel*> _channelList;
};