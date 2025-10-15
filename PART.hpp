/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 17:03:31 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/11 17:06:50 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once 
#include <iostream>
#include "Action.hpp"

class Server;
class Client;
class Channel;

class Part : public Action
{
	public:
		Part(Server* base, Client* Customer, const std::string& args);
		~Part(void);
		//main action
			void	act(void);
	private:
		//parsing 	
			void			parseSplitArgs(void);
		//excuting
			void			proceedPart(void);
			void			channelLeave(void);		
};

void	buildExecutePart(Server* base, Client* customer, const std::string& cmdArgs);