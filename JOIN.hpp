/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 11:11:00 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/08 11:19:20 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Action.hpp"

class Server;
class Client;
class Channel;

class Join : public Action
{
	public:
		//constructor && destructor
			Join(Server* base, Client* customer, const std::string& args);
			~Join(void);
		//main function
		void	act(void);
			//parsing/slashing
				void	genString(std::string& chanName, std::string& chanPass);
				void	genMap(std::istringstream& nameFlux, std::istringstream& passFlux);
				void	parseArgs(void);
			//communicating information
				void	CommunicateSuccess(void);
			//trying to join
				bool	checkInvite(void);
				bool	checkPass(const std::string& pass);
				void	creatingChan(const std::string& name, const std::string& pass);
				void	joinChan(const std::string& pass);
				void	treatIt(std::map<std::string, std::string>::iterator& it);
				
	private:
		std::map<std::string, std::string>	_pairs;
};

void	buildExecuteJoin(Server* base, Client* customer, const std::string& args);
