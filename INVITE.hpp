/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 14:56:22 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/15 15:01:28 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma	once
#include "Action.hpp"
#include <iostream>
#include <sstream>

class Server;
class Client;
class Channel;

class Invite : public Action
{
	public:
		Invite(Server* base, Client* customer, const std::string& cmdArgs);
		~Invite(void);
		void	act(void);

	private:
		//variable
			//stream
				std::istringstream	_flux;
		//act related
			void	sendInvitation(void);
		//parsing
			bool	parse(void);
		//checking perm and existence
			bool	checkClient(void);
			bool	checkChannel(void);
		
};

void	buildExecuteInvite(Server* base, Client* customer, const std::string& cmdArgs);
