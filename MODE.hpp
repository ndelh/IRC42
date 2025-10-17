/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doley <doley@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 18:55:07 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/17 11:36:11 by doley            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Action.hpp"
#include <sstream>
#include <vector>
#include <set>
#include <utility>

class Server;
class Client;
class Channel;

//constructor & destructor
class Mode;
typedef	void (Mode::*modal)(bool);
struct partitioner
{
	char		mode;
	modal		function;
};

class Mode : public Action
{
	public:
		//constructor destructor
			Mode(Server* base, Client* customer, const std::string& args);
			~Mode(void);
		//act
			void	act(void);



	private:
		//parse
				bool	retrieveChan(void);
				bool	checkPermission(void);
				bool	checkEmptyTarget(void);
				void	genModifyList(void);
				void	treatFlagLine(const std::string& line);
		//display/generate result
				void		removeFlags(char c);
				void		addFlags(char c);
				void		modifyFlags(char c, bool b);
				void		genMsgList(std::string& a, std::string& b);
				void		sendmessage();
		//submode
				void	useList(void);
				void	setTarget(std::vector<std::string>::iterator& it);
				void	modeI(bool b);
				void	modeT(bool b);
				void	modeK(bool b);
				void	modeDemote(void);
				void	modePromote(void);
				void	modeO(bool b);
				void	modeL(bool b);
				void	noFlagMode(void);
		//variable
			//flux
				std::istringstream	_flux;
			//pseudo map
				static partitioner	_functionMap[5];
				static int			_fNumber;
			//container
				std::vector<std::pair<bool, char> >	_flagList;
				std::vector<std::string>			_targetList;
				std::set<char>						_currentFlags;
				std::set<char>						_oldFlags;
			//classic
				bool								_passChanged;
				std::string							_newlimit;
			//flag error
				bool								_invalidFlagFound;

};

void	buildExecuteMode(Server* base, Client* customer ,const std::string& cmdArgs);
