/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:49:28 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/10 11:54:49 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Action.hpp"

class Server;
class Client;
class Channel;

class Quit: public Action
{
        public:
            //constructor & destructor
                Quit(Server* base, Client* customer, const std::string& args);
                ~Quit(void);
            //main function
                void    act(void);

        private:
};

void    buildExecuteQuit(Server *base, Client* customer, const std::string& args);