#ifndef SERVER_HPP

# define SERVER_HPP
#include "bits.hpp"

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
		void	printCurrentDateTime();
};

#endif