/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 11:42:13 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/04 11:42:15 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "QUIT.hpp"
#include <algorithm>
#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>


volatile sig_atomic_t SIG = 0;

//constructor && destructor

Server::Server(char *port, char *password):servName("DoleyDel"), version("1.0"), userMode("O"), channelMode("sagdhasg"),  _port(std::atoi(port)), _servPass(password), _rFlags(EPOLLIN | EPOLLET), _wrFlags(EPOLLIN | EPOLLOUT | EPOLLET)
{
    if (strlen(port) > 5 || _port < 1024 || _port > 49151)
        throw std::domain_error("acceptable port range is [1024, 49151]");
    launchServ();
}

Server::~Server(void)
{
    std::cout << std::endl << servName  << " is now shutdowning" << std::endl;
    cleanMapsAlloc();
    close(_serverSocket);
    close(_epollFd);
}

//Server init + signals handling + epollmanagement

void    Server::SIGINT_handler(int)
{
        SIG = SIGINT;
}

void    Server::signals_init(void)
{
        struct sigaction    sa;

        sa.sa_handler = &Server::SIGINT_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGINT, &sa, NULL);
}

void    Server::setHostname(void)
{
    if (gethostname(_hostname, sizeof(_hostname)) != 0)
        throw std::runtime_error("error while retrieving the machine name");
}

void    Server::launchServ(void)
{
    _serverSocket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    _servAddr.sin_family = AF_INET;
    _servAddr.sin_port = htons(_port);
    _servAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(_serverSocket, (sockaddr*)&_servAddr, sizeof(_servAddr)) == -1)
    {
        close(_serverSocket);
        throw std::runtime_error("cannot bind server socket, this happens mostly when the kernel is protecting a port");
    }
    signals_init();
    epollInit();
    listen(_serverSocket, 1000);
}
    //epoll gestion
    void    Server::epollInit(void)
    {
        struct epoll_event ev;

        ev.events = _rFlags;
        ev.data.fd = _serverSocket;

        _epollFd = epoll_create(1);
        epoll_ctl(_epollFd, EPOLL_CTL_ADD , _serverSocket, &ev);
    }

    void    Server::addWFlag(int fd)
    {
        epoll_event ev;

        ev.data.fd = fd;
        ev.events = _wrFlags;
        epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &ev);
    }

    void    Server::removeWFlag(int fd)
    {
        epoll_event ev;

        ev.data.fd = fd;
        ev.events = _rFlags;
        epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &ev);
    }
//server clean

void    Server::cleanMapsAlloc()
{
    std::map<int, Client*>::iterator it;
    std::map<std::string, Channel*>::iterator ite;
    
    for (it = _clientList.begin(); it != _clientList.end(); it++)
            delete(it->second);
    for (ite = _channelList.begin(); ite != _channelList.end(); ite++)
            delete(ite->second);
}

//Client gestion

void    Server::acceptClient(void)
{
    int         clientFd;
    std::string ipClient;
    socklen_t   size;
    struct      epoll_event ev;
    struct      sockaddr_in ClientId = {AF_INET, 0, {0}, {0}};

    Client* newcommer;

    ev.events = _rFlags;
    size = sizeof(ClientId);
    while ((clientFd = accept(_serverSocket, (struct sockaddr*)&ClientId, &size)) != -1)
    {
        ev.data.fd = clientFd;
        epoll_ctl(_epollFd, EPOLL_CTL_ADD, clientFd, &ev);
        ipClient = inet_ntoa(ClientId.sin_addr);
        newcommer = new Client(clientFd, *this, ipClient);
        _clientList.insert(std::make_pair(clientFd, newcommer));
    }
}

void    Server::forceDisconnect(int fd)
{
    std::map<int, Client*>::iterator  it;

    it = _clientList.find(fd);
    if (it != _clientList.end())
        killClient(it);
}

std::map<int, Client*>::iterator   Server::killClient(std::map<int, Client*>::iterator& itClient)
{
    std::map<int, Client*>::iterator it;
    
    it = itClient;
    itClient++;
    std::cout << "client from fd: " << it->first << "disconnected" <<  std::endl;
    epoll_ctl(_epollFd, EPOLL_CTL_DEL, it->first, NULL);
    removePhonebook(it->second->getNick());
    delete(it->second);
    _clientList.erase(it);
    return itClient;
}

//channel gestion

std::map<std::string, Channel*>::iterator   Server::killChannel(std::map<std::string, Channel*>::iterator& itChannel)
{
    std::map<std::string, Channel*>::iterator it;

    it = itChannel;
    itChannel++;
    delete(it->second);
    removeChannelList(it->first);
    return itChannel;
}
//server routine
    //sub routine
    void    Server::processRead(int fd)
    {
            Client* customer;
            char    BUFFER[1024];
            int     n;
 
            std::cout << "incoming detected from fd" << fd << std::endl;
            customer = _clientList[fd];
            n = recv(fd, BUFFER, 1024, MSG_DONTWAIT);
            while (n > 0)
            {
                BUFFER[n] = 0;
                std::string a(BUFFER);
                customer->addReceived(a);
                n = recv(fd, BUFFER, 1024, MSG_DONTWAIT);
            }
            if (n == 0)
                customer->setDisconnected();
            customer->act();
    }

    void    Server::processWrite(int fd)
    {
            _clientList[fd]->sendmsg();
            removeWFlag(fd);
    }

void    Server::routine(void)
{
    epoll_event     surveil[128];
    epoll_event     cursor;
    int             n;
    int             i;

    while (!SIG)
    {
        n = epoll_wait(_epollFd, surveil, 128, 10);
        for (i = 0; i < n; i++)
        {
            cursor = surveil[i];
            if (cursor.data.fd == _serverSocket)
                acceptClient();
            else
            {
                if (cursor.events & EPOLLOUT)
                    processWrite(cursor.data.fd);
                if (cursor.events & EPOLLIN)
                    processRead(cursor.data.fd);
                if ((cursor.events & EPOLLHUP) || (cursor.events & EPOLLERR))
                    forceDisconnect(cursor.data.fd);
            }
        }
        watchRoutine();
    }

}
//subroutine

void    Server::watchClient(void)
{
        std::map<int, Client*>::iterator itClient;
        std::map<int, Client*>::iterator itTemp;
        itClient = _clientList.begin();
        while (itClient != _clientList.end())
        {
            if (itClient->second->mustSend())
                addWFlag(itClient->first);
            if (itClient->second->mustKill() && !itClient->second->mustSend())
            {
                itTemp = itClient;
                itClient++;
                buildExecuteQuit(this, itTemp->second, ": error with connection detected");
            }
            else
                    itClient++;
        }
}

void    Server::watchChannel(void)
{
        std::map<std::string, Channel*>::iterator   it;
        
        it = _channelList.begin();
        while (it != _channelList.end())
        {
            if (it->second->isEmpty())
                it = killChannel(it);
            else
                it++;
        }
}

void    Server::watchRoutine(void)
{
        watchClient();
        watchChannel();
}

//getter
    //verification
    bool    Server::isPassCorrect(const std::string& test){
            return (test == _servPass);
    }

    bool    Server::nickAvailable(const std::string& test){
            return (_phoneBook.find(test) == _phoneBook.end());
    }

    Channel*    Server::existingChan(const std::string& name)
    {
            std::map<std::string, Channel*>::iterator it;

            it = _channelList.find(name);
            if (it == _channelList.end())
                return NULL;
            return it->second;
    }

    Client* Server::existingClient(const std::string& name)
    {
            std::map<std::string, Client*>::iterator    it;

            it = _phoneBook.find(name);
            if (it == _phoneBook.end())
                return NULL;
            return it->second;
    }
    //time
    const std::string   Server::get_time() const
    {
        return (_timeCreation);
    }

//setters
    //adders
        void    Server::addPhonebook(const std::string& name, Client* customer)
        {
                _phoneBook.insert(std::make_pair(name, customer));
        }
        
        void    Server::addChannelList(const std::string& name, Channel* chan)
        {
                _channelList.insert(std::make_pair(name, chan));
        }
    //removers
        void    Server::removePhonebook(const std::string& name)
        {
                if (name == "*")
                    return;
                _phoneBook.erase(_phoneBook.find(name));
        }
        
        void    Server::removeChannelList(const std::string& name)
        {
                _channelList.erase(_channelList.find(name));
        }
        void    Server::eraseClient(int fd)
        {
                std::map<int, Client*>::iterator   it;

                it = _clientList.find(fd);
                killClient(it);
        }

        void        Server::removeOnIncident(int fd)
        {
                std::map<int, Client*>::iterator    it;

                it = _clientList.find(fd);
                buildExecuteQuit(this, it->second, "incident detected");
        }
    //time
        void Server::set_time()
        {
                time_t      now = time(NULL);
                struct tm   *timestamp = localtime(&now);
                char        buffer[20];

                strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timestamp);
                _timeCreation = std::string(buffer);
        }