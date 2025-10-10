/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:28:41 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/06 14:32:46 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include "Action.hpp"

class Server;
class Client;

class User : public Action
{
    public:
        //destructor and constructor
            User(Server* base, Client* customer, const std::string& args);
            ~User(void);
        //main function
            void    act(void);
    private:
        //parse
            bool    argCount(void);
        //utiliy
            void    getRName(std::string& realname);
};

void    buildExecuteUser(Server* base, Client* customer, const std::string& args);