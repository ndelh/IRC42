/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 11:09:30 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/08 11:19:25 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JOIN.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

//constructor & destructor

Join::Join(Server* base, Client* customer, const std::string& args): Action(base, customer, args)
{
}

Join::~Join()
{
}

//action

	//slashing argument
		void	Join::genString(std::string& chanName, std::string& chanPass)
		{
			size_t	splitIndex;

			if ((splitIndex = _cmdArgs.find(" ")) == std::string::npos)
				chanPass = "";
			else
				chanPass = _cmdArgs.substr(splitIndex + 1);
			chanName = _cmdArgs.substr(0, splitIndex);
		}
		
		void	Join::genMap(std::istringstream& nameFlux, std::istringstream& passFlux)
		{
			std::string pass;
			std::string name;

			while (getline(nameFlux, name, ','))
			{
				if (name[0] != '#')
					name = "#" + name;
				if (getline(passFlux, pass, ','))
					_pairs.insert(std::make_pair(name, pass));
				else
					_pairs.insert(std::make_pair(name, ""));
			}
		}

		void	Join::parseArgs(void)
		{
			std::string		chanName;
			std::string		chanPass;

			genString(chanName, chanPass);
			std::istringstream	nameFlux(chanName);
			std::istringstream	passFlux(chanPass);
			genMap(nameFlux, passFlux);
		}

		//join try

		bool	Join::checkInvite(void)
		{
				if (!_chan->isInvited(_chan->name))
				{
					_customer->addSend(generateMsg(473));
					return (false);
				}
				return true;
		}

		bool	Join::checkPass(const std::string& pass)
		{
				if (!_chan->correctPass(pass))
				{
					_customer->addSend(generateMsg(475));
					return (false);
				}
				return true;
		}
		void	Join::creatingChan(const std::string& name, const std::string& pass)
		{
				Channel* created;

				if (pass.empty())
					created = new Channel(name);
				else
					created = new Channel(name, pass);
				_base->addChannelList(created->name ,created);
				created->customerJoin(_customer);
				created->customerPromote(_customer->getNick());
				_chan = created;
				CommunicateSuccess();
		}

		void	Join::joinChan(const std::string& pass)
		{
				if (_chan->isInvitationOnly())
					if (!checkInvite())
						return;
				if	(_chan->isPassProtected())
					if (!checkPass(pass))
						return;
				_chan->customerJoin(_customer);
				CommunicateSuccess();
				
		}

		void	Join::treatIt(std::map<std::string, std::string>::iterator& it)
		{

				_chan = _base->existingChan(it->first);
				if (!_chan)
					creatingChan(it->first, it->second);
				else
					joinChan(it->second);

		}
	
//communicating on sucess
		void	Join::CommunicateSuccess(void)
		{
				_customer->addMembership(_chan->name, _chan);
				_chan->broadcastMembers(generateMsg(-2));
				_customer->addSend(generateMsg(332));
				_customer->addSend(generateMsg(353));
				_customer->addSend(generateMsg(366));
		}

void	Join::act(void)
{
		
		parseArgs();
		for (std::map<std::string, std::string>::iterator it = _pairs.begin(); it != _pairs.end(); it++)
			treatIt(it);
}

//out of scope

void	buildExecuteJoin(Server* base, Client* customer, const std::string& args)
{
	Join	a(base, customer, args);

	a.act();
}
