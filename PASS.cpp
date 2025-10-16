/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doley <doley@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:25:46 by doley             #+#    #+#             */
/*   Updated: 2025/10/08 16:26:18 by doley            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PASS.hpp"
#include "Server.hpp"
#include "Client.hpp"

Pass::Pass(Server *base, Client *customer, const std::string &args) : Action(base, customer, args)
{
}

Pass::~Pass(void)
{
}

void Pass::act(void)
{
	if (_customer->isRegistered() == false)
		first_connection();
	else
		_customer->addSend(generateMsg(462));
}

void	Pass::first_connection(void)
{
	if (_base->isPassCorrect(_cmdArgs))
	{
		_customer->setPassChecked();
		return;
	}
	_customer->addSend(generateMsg(464));
	_customer->setMustKill();
}

void buildExecutePass(Server *base, Client *customer, const std::string &args)
{
	Pass a(base, customer, args);

	a.act();
}
