/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doley <doley@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:25:57 by doley             #+#    #+#             */
/*   Updated: 2025/10/08 16:26:29 by doley            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Action.hpp"

class Server;
class Client;

class Pass : public Action
{
	public:
		// destructor and constructor
		Pass(Server *base, Client *customer, const std::string &args);
		~Pass(void);
		// main function
		void	act(void);
		void	first_connection(void);
};

void buildExecutePass(Server *base, Client *customer, const std::string &args);
