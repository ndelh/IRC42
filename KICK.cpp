#include "KICK.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Kick::Kick(Server *base, Client *customer, const std::string &args)
: Action(base, customer, args)
{
	_cmdName = "KICK";

}

Kick::~Kick(void)
{
}

void	Kick::act(void)
{
	// Channel	target_chan = _base->existingChan(chan_name);

}

// out of scope

void	buildExecuteKick(Server* base, Client* customer, const std::string& args)
{
	Kick	a(base, customer, args);

	a.act();
}
