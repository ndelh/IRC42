/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 17:59:14 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/05 18:05:10 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <set>
#include <map>
class	Server;
class	Client;

class	Channel
{
	public:
		//constructor & destructor
			Channel(const std::string& name);
			Channel(const std::string& name, const std::string& passport);
			~Channel(void);
		//getter
			//boolean
				bool	isEmpty(void);
				bool	isPassProtected(void);
				bool	isInvitationOnly(void);
				bool	isMember(const std::string& name);
				bool	isOperator(const std::string& name);
				bool	isInvited(const std::string& name);
				bool	correctPass(const std::string& test);
			//lister
				std::string									displayMembers(void);
		//setter
			//adder
				void	customerJoin(Client* customer);
				void	customerPromote(const std::string& customerName);
			//remover
				void	customerDemote(const std::string& customerName);
				void	customerLeave(Client* customer);
		//broadcater
				void	broadcastMembers(const std::string& msg);
				void	broadcastOthers(const std::string &msg, const std::string& senderName);
				void	partialBroadcast(std::set<std::string>& alreadyInformed, const std::string& msg);
		//variables
			const std::string name;
		
	private:
		//containers
			std::map<std::string, Client*>	_memberlist;
			std::set<std::string>			_operatorList;
			std::set<std::string>			_invitationList;
		//status
			bool							_passProtected;
			bool							_invitationOnly;
			bool							_topicActive;
			int								_membersNumber;
		//variable
			std::string						_pass;
};
