/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 09:07:54 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/15 09:16:57 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TOPIC.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

//constructor & destructor

Topic::Topic(Server* base, Client* customer, const std::string& cmdArgs): Action(base, customer, cmdArgs), _flux(cmdArgs)
{
	_cmdName = "TOPIC";
}

Topic::~Topic(void)
{
}

//act
	//parse
		bool	Topic::parseBasicRights(void)
		{
			std::string	line;

			getline(_flux, line, ' ');
			_chan = _base->existingChan(line);
			if (!_chan)
			{
				_customer->addSend(generateMsg(403));
				return (false);
			}
			if (!_chan->isMember(_customer->getNick()))
			{
				_customer->addSend(generateMsg(442));
				return (false);
			}
			getline(_flux, _contextualArgs);
			return true;
		}
	//subAct
		//displayTopic
			void	Topic::displayTopic(void)
			{
					if (!_chan->emptyTopic())
						_customer->addSend(generateMsg(332));
					else
						_customer->addSend(generateMsg(331));
			}

		//changeTopid
			bool	Topic::changeRights(void)
			{
				if (!_chan->isTopicRestricted())
					return true;
				if (!_chan->isOperator(_customer->getNick()))
				{
					_customer->addSend(generateMsg(482));
					return false;
				}
				return true;
			}
			void	Topic::changeTopic(void)
			{
				if (!changeRights())
					return ;
				if (_contextualArgs[0] != ':')
				{
					_customer->addSend(generateMsg(461));
					return ;
				}
				if (_contextualArgs.size() > 390)
					_contextualArgs = _contextualArgs.substr(390);
				_contextualArgs.erase(0, 1);
				_chan->setTopic(_contextualArgs);
				_chan->broadcastMembers(generateMsg(-8));
			}
void	Topic::act(void)
{
	if (!globalParse())
		return;
	if (!parseBasicRights())
		return ;
	if (_contextualArgs.empty())
		displayTopic();
	else
		changeTopic();
	
}


//out of scope

void	buildExecuteTopic(Server* base, Client* customer, const std::string& cmdArgs)
{
	Topic a(base, customer, cmdArgs);

	a.act();
}
