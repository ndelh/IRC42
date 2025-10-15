/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 18:02:20 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/05 18:04:26 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <algorithm>

//constructor & destructor

Channel::Channel(const std::string& name): name(name)
{
	_passProtected = false;
	_invitationOnly = false;
	_topicrestricted = true;
	_limitedNumber = false;
	_membersNumber = 0;
	_maxMembers = 500;
}

Channel::Channel(const std::string& name, const std::string& pass): name(name), _pass(pass)
{
	_passProtected = true;
	_invitationOnly = false;
	_topicrestricted = true;
	_limitedNumber = false;
	_membersNumber = 0;
	_maxMembers = 500;
}

Channel::~Channel(void)
{
}

//getters
	//boolean return
		bool	Channel::isEmpty(void)
		{
				return (_memberlist.empty());
		}
		
		bool	Channel::isMember(const std::string& customerName)
		{
				return ( _memberlist.find(customerName) != _memberlist.end());
		}
		
		bool	Channel::isOperator(const std::string& customerName)
		{
				return (_operatorList.find(customerName) != _operatorList.end());
		}

		bool	Channel::isPassProtected(void)
		{
				return (_passProtected);
		}

		bool	Channel::correctPass(const std::string& test)
		{
				return (_pass == test);
		}

		bool	Channel::isInvitationOnly(void)
		{
				return (_invitationOnly);
		}
		
		bool	Channel::isInvited(const std::string& name)
		{
				return (_invitationList.find(name) != _invitationList.end());
		}

		bool	Channel::isTopicRestricted(void)
		{
				return _topicrestricted;
		}
	//returners
		int		Channel::getPlaceNb(void)
		{
				return _maxMembers;
		}
		
	//listers
		std::string	Channel::displayMembers(void)
		{
			std::map<std::string, Client*>::iterator	it;
			std::string	to_ret;
			std::string	name;
			bool		first;

			first = true;			
			for (it = _memberlist.begin(); it != _memberlist.end(); it++)
			{
				if (_operatorList.find(it->first) != _operatorList.end())
					name = "@" + it->first;
				else
					name = it->first;
				if (!first)
					to_ret += " ";
				to_ret += name;
				first = false;
			}
			return to_ret;
		}

		std::set<char> Channel::activeFlags(void)
		{
			std::set<char> to_ret;

			if (_passProtected)
				to_ret.insert('k');
			if (_invitationOnly)
				to_ret.insert('i');
			if (_limitedNumber)
				to_ret.insert('l');
			if (_topicrestricted)
				to_ret.insert('t');
			return to_ret;
		}
//setters
	//adders
		void	Channel::customerJoin(Client* customer)
		{
				_memberlist.insert(std::make_pair(customer->getNick(), customer));
				++_membersNumber;
		}
		void	Channel::customerPromote(const std::string& customerName)
		{
				_operatorList.insert(customerName);
		}
	//removers
		void	Channel::customerDemote(const std::string& customerName)
		{
				_operatorList.erase(_operatorList.find(customerName));
		}
		void	Channel::customerLeave(Client* customer)
		{
				_memberlist.erase(_memberlist.find(customer->getNick()));
				if (isOperator(customer->getNick()))
					customerDemote(customer->getNick());
				--_membersNumber;
		}
	//updater

		void	Channel::updateNick(const std::string& oldName, Client* customer)
		{
				_memberlist.erase(_memberlist.find(oldName));
				_memberlist.insert(std::make_pair(customer->getNick(), customer));
		}

		void	Channel::changeInvMode(bool b)
		{
				if (b != _invitationOnly)
					_invitationOnly = b;
		}
		
		void	Channel::changeTopicMode(bool b)
		{
				if (b != _topicrestricted)
					_topicrestricted = b;
		}

		void	Channel::setSize(size_t n, bool b)
		{
				if (b != _limitedNumber)
					_limitedNumber = b;
				_maxMembers = (int)n;		
		}

		void	Channel::setPassword(std::string& pass, bool b)
		{
				if (b != _passProtected)
					_passProtected = b;
				if (b == true)
					_pass = pass;
				
		}

		void	Channel::setTopic(const std::string& topic)
		{
				_topic = topic;
		}
//broadcaster
	//to members
		void	Channel::broadcastMembers(const std::string& msg)
		{
				std::map<std::string, Client*>::iterator it;
				
				for (it = _memberlist.begin(); it != _memberlist.end(); it++)
					it->second->addSend(msg);
		}
		void	Channel::broadcastOthers(const std::string& msg, const std::string& customerName)
		{
				std::map<std::string, Client*>::iterator it;
				
				for (it = _memberlist.begin(); it != _memberlist.end(); it++)
				{	
					if (it->first != customerName)
						it->second->addSend(msg);
				}
		}

		void	Channel::partialBroadcast(std::set<std::string>& alreadyInformed, const std::string& msg)
		{
				std::map<std::string, Client*>::iterator it;
				
				for (it = _memberlist.begin(); it != _memberlist.end(); it++)
				{	
					if (alreadyInformed.find(it->first) == alreadyInformed.end())
					{
						it->second->addSend(msg);
						alreadyInformed.insert(it->first);
					}
				}
		}