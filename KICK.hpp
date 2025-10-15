#pragma once
#include "Action.hpp"

class Kick : public Action
{
	public:

		Kick(Server *base, Client *customer, const std::string &args);
		~Kick(void);
		void	act(void);

	private:
};

void    buildExecuteKick(Server* base, Client* customer, const std::string& args);
