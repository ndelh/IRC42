/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 16:47:06 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/05 16:54:08 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include "Action.hpp"

class Server;
class Client;

class Nick: public Action
{
	public :
			Nick(Server* base, Client* customer, const std::string& cmdargs);
			~Nick(void);
			//main function
				virtual void	act(void);
				
	private:
			//errorcheck
			bool	validNick(const std::string& line);
			//subfunction
			void	changeNick(const std::string& line);
			void	registerNick(const std::string& line);
			void	updateInformation();

};

void	buildExecuteNick(Server* base, Client* customer, const std::string& cmdargs);
