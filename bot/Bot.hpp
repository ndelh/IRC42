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

class Bot
{
    public:
            Bot(const std::string& name, const std::string& pass, const std::string& port);
            ~Bot(void);
            void    veil(void);
        //variable
            const std::string   _name;
    private:
        //variable
            const std::string&      _pass;
            int                     _port;
            int                     _kfd;
        //init connection 
            void    connectKennel(void);
            

};