/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 13:08:10 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/17 13:08:28 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <exception>
#include <string>
#include <sstream>
#include <errno.h>


struct Doggo
{
    std::string     species;
    std::string     bark;
};


class Bot
{
    public:
            Bot(const std::string& name, const std::string& pass, const std::string& port);
            ~Bot(void);
            void    veil(void);
        //variable
            const std::string   _name;
    private:
        //function
            void                    sendMsg(void);
            void                    receiveMsg(void);
            void                    act(void);
            void                    logSystem(void);
            void                    barking(std::string& line);
            void                    releaseDog(std::string& target);
        //variable
            const std::string&      _pass;
            int                     _port;
            int                     _kfd;
            int                     _pollFd;
            bool                    _mustDie;
            int                     _KennelCall;
            bool                    _registrationDone;
            std::string             _sendBuffer;
            std::string             _receiveBuffer;
        //tab
            static Doggo                   kennelPool[10];
            static int                     kennelMembers;

        //init connection 
            void    connectKennel(void);
            bool    initEpoll(void);

};