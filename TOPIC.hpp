/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 08:54:59 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/15 09:07:50 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Action.hpp"
#include <iostream>
#include <sstream>

class Server;
class Client;
class Channel;

class Topic : public Action
{
	public:
		Topic(Server* base, Client* customer, const std::string& cmdArgs);
		~Topic(void);
		void	act(void);
	private:
		//parsing
			bool	parseBasicRights(void);
		
		//sub act
			//displayTopic
				void	displayTopic(void);
			//modifyTopic
				bool	changeRights(void);
				void	changeTopic(void);

		//variables
			//flux
				std::istringstream	_flux;
		
};

void	buildExecuteTopic(Server* base, Client* Customer, const std::string& args);
