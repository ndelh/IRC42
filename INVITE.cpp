/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 15:02:42 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/15 15:14:45 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "INVITE.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

//constructor && destructor

Invite::Invite(Server* base, Client* customer, const std::string& cmdArg): Action(base, customer, cmdArg), _flux(cmdArg)
{
	_cmdName = "INVITE";
}

Invite::~Invite(void)
{
}

//act
	//parsing
		bool	Invite::parse(void)
		{
			if (!(_flux >> _cmdTarget >> _contextualArgs))
			{
				_customer->addSend(generateMsg(461));
				return false;
			}
			return true;
		}
	//check permission and existence
		bool	Invite::checkChannel(void)
		{
				_chan = _base->existingChan(_contextualArgs);
				if (!_chan)
				{
					_customer->addSend(generateMsg(403));	
					return false;
				}
				if (!_chan->isMember(_customer->getNick()))
				{
					_customer->addSend(generateMsg(442));
					return false;
				}
				if (!_chan->isOperator(_customer->getNick()))
				{
					_customer->addSend(generateMsg(482));
					return false;
				}
				return true;

		}

		bool	Invite::checkClient(void)
		{
				if (_chan->isMember(_cmdTarget))
				{
					_customer->addSend(generateMsg(443));
					return false;
				}
				if (!_base->existingClient(_cmdTarget))
				{
					_customer->addSend(generateMsg(401));
					return false;
				}
				return true;
		}
	//act
		void	Invite::sendInvitation(void)
		{
				Client* receiver;

				receiver = _base->existingClient(_cmdTarget);
				_chan->addInviteList(_cmdTarget);
				_customer->addSend(generateMsg(341));
				receiver->addSend(generateMsg(-9));
		}

void	Invite::act(void)
{
	std::cout << "inviteLaunched" << std::endl;
	if (!parse())
		return;
	if (!checkChannel())
		return;
	if (!checkClient())
		return;
	sendInvitation();
}


//external

void	buildExecuteInvite(Server* base, Client* customer, const std::string& cmdArgs)
{
	Invite	a(base, customer, cmdArgs);

	a.act();
}
