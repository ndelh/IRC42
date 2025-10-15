#include "KICK.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Kick::Kick(Server *base, Client *customer, const std::string &args)
: Action(base, customer, args), _flux(args)
{
	_cmdName = "KICK";
}

Kick::~Kick(void)
{
}

void	Kick::act(void)
{
	if (!parse_args());
		return;
	_chan = _base->existingChan(_chanName);
	if (!_chan)
		return (_customer->addSend(generateMsg(403)));
	else if (!_chan->isMember(_customer->getNick()))
		return (_customer->addSend(generateMsg(442)));
	else if (!_chan->isOperator(_customer->getNick()))
		return (_customer->addSend(generateMsg(482)));
	else if (!_base->existingClient(_cmdTarget))
		return (_customer->addSend(generateMsg(401)));
	else if (!_chan->isMember(_cmdTarget))
		return (_customer->addSend(generateMsg(441)));
	else
	{
		_contextualArgs = _reason;
		
	}

}

bool	Kick::parse_args(void)
{
	if (!(_flux >> _chanName >> _cmdTarget))
	{
		_customer->addSend(generateMsg(461));
		return (false);
	}
	getline(_flux, _reason);
	if (!_reason.empty())
	{
		while (!_reason.empty() && isspace(_reason[0]))
			_reason.erase(0, 1);
		if (!_reason.empty() && _reason[0] == ':')
		{
			_reason.erase(0, 1);
		}
	}
	return (true);
}

// out of scope

void	buildExecuteKick(Server* base, Client* customer, const std::string& args)
{
	Kick	a(base, customer, args);

	a.act();
}
