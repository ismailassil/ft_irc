#ifndef SERVER_HPP

# define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <signal.h> 
#include <poll.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>




#include <arpa/inet.h>
#include <netdb.h>

#define BACKLOG 4
#define MAXCLIENT 5
#define BUFFER_SIZE 1024
#define Welcome_msg "Welcome to the server!\n"
#define PASS_LENGH 4

class Server
{
	private :
	int					socket_fd;
	struct sockaddr_in	server_addr;	
	std::string			port;
	std::string			password;
	struct pollfd		fds[MAXCLIENT + 1];
	int					nfds;
	int					stop;

	public :
		Server(void);
		Server(Server const &copy);
		~Server(void);
		Server& operator=(const Server &other);

		Server (std::string port, std::string password);
		void	server_init();
		void	add_client();
		void	remove_client(int &fd);
		void	read_msg(int &i);
};

#endif