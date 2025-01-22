#ifndef BOT_HPP
#define BOT_HPP

#include "../headers/bits.hpp"

class Bot {
		int				   socket_fd;
		struct sockaddr_in server_addr;
		string			   password;
		int				   port;
		vector< string >   facts;

		void		 initVars();
		void		 openConfigFile();
		void		 authentificate();
		void		 send_msg( const string &msg );
		void		 error( string str, int exit_status );
		const string read_msg();

		void printCurrentDateTime();

	public:
		Bot( void );
		Bot( Bot const &copy );
		~Bot( void );
		Bot &operator=( const Bot &other );

		void run();
};

#endif