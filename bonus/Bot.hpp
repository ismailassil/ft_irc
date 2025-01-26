#ifndef BOT_HPP
#define BOT_HPP

#include "../headers/bits.hpp"

class Bot {
		static int		   socket_fd;
		struct sockaddr_in server_addr;
		string			   password;
		int				   port;
		string			   server_ip;
		vector< string >   facts;
		vector< string >   jokes;
		vector< string >   quotes;
		string			   nick;
		string			   user;

		void		 initVars();
		void		 openConfigFile();
		void		 authentificate();
		void		 send_msg( const string &msg );
		void		 error( string str, int exit_status );
		static void	 handle_signal( int signum );
		const string read_msg();

		string getNickName();
		string getUserName();

		void printCurrentDateTime();

	public:
		Bot( void );
		Bot( Bot const &copy );
		~Bot( void );
		Bot &operator=( const Bot &other );

		void run();
};

#endif