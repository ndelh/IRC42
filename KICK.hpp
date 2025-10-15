#pragma once
#include "Action.hpp"
#include <iostream>
#include <sstream>

class Kick : public Action
{
	public:

		Kick(Server *base, Client *customer, const std::string &args);
		~Kick(void);
		void	act(void);
		bool	parse_args();

	private:

		std::string			_chanName;
		std::string			_reason;
		std::istringstream	_flux;
};

void    buildExecuteKick(Server* base, Client* customer, const std::string& args);
