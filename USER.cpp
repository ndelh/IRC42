/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doley <doley@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:27:05 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/18 12:09:18 by doley            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "USER.hpp"
#include "Server.hpp"
#include "Client.hpp"


//constructor && destructor
User::User(Server* base, Client *customer, const std::string& cmdArgs): Action(base, customer, cmdArgs)
{
    _cmdName = "USER";
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
        if (line[0] == ':')
            break;
    }
    return (n >= 4);
}

//pure utility
    void    User::getRName(std::string& realname)
    {
        int     n;

        n = -1;
        for (int i = 0; i < 3; i++)
            n = _cmdArgs.find(" ", n + 1);
        ++n;
        if (*(_cmdArgs.begin() + n) == ':')
            realname = _cmdArgs.substr(n + 1, 100);
        else
        {
            size_t  space = _cmdArgs.find(" ", n + 1);
            if (space != std::string::npos)
                realname = _cmdArgs.substr(n, std::min(space - n, (size_t)100));
            else
                realname = _cmdArgs.substr(n, 100);

        }
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
        size_t  space = _cmdArgs.find(" ");
        if (space != std::string::npos)
            username = _cmdArgs.substr(0, std::min(space, (size_t)10));
        else
            username = _cmdArgs.substr(0, 10);
        getRName(realname);
        _customer->setUser(username, realname);
        std::cout << "Username: " << username << "\nlen: " << username.length() <<
        "\nRealname: " << realname << "\nlen: " << realname.length() <<
        "\nNickname: " << _customer->getNick() << "\nlen: " << _customer->getNick().length() << std::endl;
}

//out of scope

void    buildExecuteUser(Server* base, Client* customer, const std::string& args)
{
        User    a(base, customer, args);

        a.act();
}
