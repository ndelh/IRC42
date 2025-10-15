/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 18:51:35 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/12 19:15:15 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MODE.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <stdlib.h>
// #include <algorithm>

int			Mode::_fNumber = 5;
partitioner Mode::_functionMap[5] =
{
		{'t', &Mode::modeT},
		{'o', &Mode::modeO},
		{'l', &Mode::modeL},
		{'i', &Mode::modeI},
		{'k', &Mode::modeK},
};

Mode::Mode(Server* base, Client* customer, const std::string& cmdArgs): Action(base, customer, cmdArgs), _flux(cmdArgs)
{
	_passChanged = false;
}

Mode::~Mode(void)
{
}


//act

	//display and generate result

		void	Mode::modifyFlags(char c, bool b)
		{
			if (b)
				return addFlags(c);
			else
				return removeFlags(c);
		}
		void	Mode::removeFlags(char c)
		{
			std::set<char>::iterator	it;
			
			it = _currentFlags.find(c);
			if (it == _currentFlags.end())
				return;
			else
			{
				_currentFlags.erase(it);
			}
		}

		void	Mode::addFlags(char c)
		{
			std::set<char>::iterator it;
			
			it = _currentFlags.find(c);
			if (it == _currentFlags.end())
				_currentFlags.insert(c);

		}

		void	Mode::genMsgList(std::string& added, std::string& removed)
		{
				std::set<char>::iterator it;

				it = _currentFlags.begin();
				for (; it != _currentFlags.end(); it++)
				{
					if (_oldFlags.find(*it) == _oldFlags.end() || (*it == 'k' && _passChanged))
						added += *it;
				}
				it = _oldFlags.begin();
				for (; it != _oldFlags.end(); it++)
				{
					if (_currentFlags.find(*it) == _currentFlags.end())
						removed += *it;
				}
		}

		void	Mode::sendmessage(void)
		{
			std::string	added("+");
			std::string removed("-");

			_contextualArgs = "";
			genMsgList(added, removed);
			if (added.size() > 1)
				_contextualArgs = added;
			if (removed.size() > 1)
				_contextualArgs += removed;
			if (_passChanged)
				_contextualArgs += " #####";
			if (!_newlimit.empty())
				_contextualArgs += " " + _newlimit;
			if (!_contextualArgs.empty())
				_chan->broadcastMembers(generateMsg(-7));
		}
	//parse
		bool	Mode::checkPermission(void)
		{
				if (!_chan->isMember(_customer->getNick()))
				{
					_customer->addSend(generateMsg(442));
					return false;
				}
				if (!_chan->isOperator(_customer->getNick()))
				{
					std::string	line;
					if (!getline(_flux, line))
						noFlagMode();
					else
						_customer->addSend(generateMsg(482));
					return false;
				}
				return true;
		}
		bool	Mode::retrieveChan(void)
		{
			std::string		line;

			std::getline(_flux, line, ' ');
			_cmdArgs.erase(0, line.size() + 1);
			_chan = _base->existingChan(line);
			if (!_chan)
			{
				_customer->addSend(generateMsg(403));
				return false;
			}
			_oldFlags = _chan->activeFlags();
			_currentFlags = _oldFlags;
			return (checkPermission());
		}

		bool	Mode::checkEmptyTarget()
		{
				if (!_cmdTarget.empty())
					return true;
				_customer->addSend(generateMsg(461));
				return false;
		}
		void	Mode::treatFlagLine(const std::string& line)
		{
			std::string::const_iterator	it;
			bool						mode;

			it = line.begin();
			mode = (*it == '+');
			it++;
			for (; it != line.end(); it++)
			{
				if (*it == '+' || *it == '-')
					mode = (*it == '+');
				else
					_flagList.push_back(std::make_pair(mode, *it));
			}

		}
		void	Mode::genModifyList(void)
		{
			std::string line;
	
			while (getline(_flux, line, ' '))
			{
				if (line[0] != '+' && line[0] != '-')
					break;
				treatFlagLine(line);
			}
			if (line.empty())
			{
				std::cout << "catched" << std::endl;
				return ;	
			}
			if (line[0] != '+' && line[0] != '-')
			{
				_targetList.push_back(line);
				while (getline(_flux, line, ' '))
					if(!line.empty())
						_targetList.push_back(line);
			}
		}

	//sub mode
	void	Mode::setTarget(std::vector<std::string>::iterator& it)
	{
			if (it != _targetList.end())
			{	
				_cmdTarget = *it;
				it++;
			}
			else
				_cmdTarget = "";
	}
	void	Mode::useList(void)
	{
		std::vector<std::pair<bool, char> >::iterator	itFlag;
		std::vector<std::string>::iterator				itTarget;
		int 											i;
		
		itTarget = _targetList.begin();
		for (itFlag = _flagList.begin(); itFlag != _flagList.end(); itFlag++)
		{
			if (itFlag->second == 'k' || itFlag->second == 'o' || itFlag->second == 'l')
				setTarget(itTarget);
			for (i = 0; i < _fNumber; i++)
			{
				if (itFlag->second == _functionMap[i].mode)
					(this->*(_functionMap[i].function))(itFlag->first);
			}
		}
	}

	void	Mode::modeI(bool b)
	{
			modifyFlags('i', b);
			_chan->changeInvMode(b);
	}

	void	Mode::modeK(bool b)
	{
			if (!checkEmptyTarget())
				return ;
			if (_cmdTarget.size() > 15)
			{
				_cmdTarget.erase(15);
				_customer->addSend(generateMsg(461));
				return ;
			}
			modifyFlags('k', b);
			_chan->setPassword(_cmdTarget, b);
			if (!_passChanged)
				_passChanged = true;

	}

	void	Mode::modeL(bool b)
	{
			int 	i;
			
			if (!checkEmptyTarget())
				return ;
			i = std::atoi(_cmdTarget.c_str());
			if((_cmdTarget.find_first_not_of("0123456789")) != std::string::npos || _cmdTarget.size() > 4 || (i > 500 || i < 0))
			{
				_customer->addSend(generateMsg(461));
				return ;
			}
			_chan->setSize(i, b);
			_newlimit = _cmdTarget;
			modifyFlags('l', b);
			

	}

	void	Mode::modePromote(void)
	{
			if (_chan->isOperator(_cmdTarget))
				return ;
			_contextualArgs = "+";
			_chan->customerPromote(_cmdTarget);
			_chan->broadcastMembers(generateMsg(-6));
	}

	void	Mode::modeDemote(void)
	{
			if (!_chan->isOperator(_cmdTarget))
				return;
			_contextualArgs = "-";
			_chan->customerDemote(_cmdTarget);
			_chan->broadcastMembers(generateMsg(-6));
	}
	void	Mode::modeO(bool b)
	{
			if (!checkEmptyTarget())
				return;
			if(!_chan->isMember(_cmdTarget))
			{
				_customer->addSend(generateMsg(401));
				return;
			}
			if (!b)
				modeDemote();
			else
				modePromote();
	}

	void	Mode::modeT(bool b)
	{
			modifyFlags('t', b);
			_chan->changeTopicMode(b);
	}

	std::string		convertInt(int a)
	{
		std::ostringstream oss;
		oss << a;
		return (oss.str());
	}
	void	Mode::noFlagMode(void)
	{
			std::set<char> 				activeFlags;
			std::set<char>::iterator	it;

			_contextualArgs = "";
			activeFlags = _chan->activeFlags();
			for (it = activeFlags.begin(); it != activeFlags.end(); it++)
				_contextualArgs += *it;
			_contextualArgs += " :";
			if (activeFlags.find('k') != activeFlags.end())
				_contextualArgs += "####";
			if (activeFlags.find('l') != activeFlags.end())
			{
				_contextualArgs += " " + convertInt(_chan->getPlaceNb());
			}
			_customer->addSend(generateMsg(324));
	}

void	Mode::act(void)
{
	std::cout << "call to mode sucessfull" << std::endl;
	_cmdName = "MODE";
	if (!globalParse())
		return;
	if (!retrieveChan())
		return;
	genModifyList();
	if (_flagList.empty())
	{	
		if (_targetList.empty())
			noFlagMode();
		else
			_customer->addSend(generateMsg(461));
		return;
	}
	useList();
	sendmessage();
}

//out of scope

void	buildExecuteMode(Server* base, Client* customer, const std::string& cmdArgs)
{
	Mode a(base, customer, cmdArgs);

	a.act();
}
