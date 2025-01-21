#ifndef SERVER_HPP
#define SERVER_HPP

#include "ClientManager.hpp"
#include "bits.hpp"

class Server {
	private:
		int				   socket_fd;
		struct sockaddr_in server_addr;
		string			   port;
		string			   password;
		struct pollfd	   fds[MAXCLIENT + 1];
		int				   nfds;
		ClientManager	   clientManager;

		void add_client();
		void remove_client( int & );
		void read_msg( int & );
		void printCurrentDateTime();

	public:
		Server( void );
		Server( Server const &copy );
		Server &operator=( const Server &other );
		~Server( void );

		Server( const string &port, const string &password );
		void server_init();
};

#endif