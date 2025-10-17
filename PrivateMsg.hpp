/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMsg.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 16:31:03 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/09 16:37:32 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <vector>
#include "Action.hpp"

class Server;
class Client;
class Channel;

class PrivateMsg : public Action
{
	public:
		//constructor && destructor
			PrivateMsg(Server* Base, Client* Customer, const std::string& args);
			~PrivateMsg(void);
		void	act(void);
	
	private:
		//parse / slice
			bool	parseCmd(void);
			bool	splitCmd(void);
		//function
			void	executeSend(void);
			//privateMsg
				void	clientMsg(void);
			//chanelmsg
				bool	sendChanOk();
				void	channelMsg(void);
		//container
			std::vector<std::string>	_receiverList;
};

void	buildExecutePrivateMsg(Server* Base, Client* Customer, const std::string& args);
