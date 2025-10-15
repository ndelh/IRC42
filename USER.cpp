/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doley <doley@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:27:05 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/08 16:31:28 by doley            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "USER.hpp"
#include "Server.hpp"
#include "Client.hpp"


//constructor && destructor
User::User(Server* base, Client *customer, const std::string& cmdArgs): Action(base, customer, cmdArgs)
{
}

User::~User(void)
{
}

//parse

bool    User::argCount()
{
    std::istringstream      flux(_cmdArgs);
    std::string             line;
    int                     n;

    n = 0;
    while(getline(flux, line, ' '))
    {
        if (line.empty())
            break;
        n++;
    }
    std::cout << n << std::endl;
    return (n >= 4);
}

//pure utility
    void    User::getRName(std::string& realname)
    {
        int     n;

        n = -1;
        for (int i = 0; i < 3; i++)
            n += _cmdArgs.find(" ", n + 1);
        ++n;
        if (*(_cmdArgs.begin() + n) == ':')
            realname = _cmdArgs.substr(n + 1);
        else
            realname = _cmdArgs.substr(n + 1, _cmdArgs.find(" ", n + 1));
    }
//core function

void    User::act(void)
{
        std::string     username;
        std::string     realname;

        if (!argCount())
        {
            _customer->addSend(generateMsg(461));
            return;
        }
        if(_customer->userSet())
        {
            _customer->addSend(generateMsg(462));
            return;
        }
        if (!argCount())
            return ;
        username = _cmdArgs.substr(0, _cmdArgs.find(" "));
        getRName(realname);
        _customer->setUser(username, realname);
}

//out of scope

void    buildExecuteUser(Server* base, Client* customer, const std::string& args)
{
        User    a(base, customer, args);

        a.act();
}
