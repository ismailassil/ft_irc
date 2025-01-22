#ifndef BOT_HPP
# define BOT_HPP

#include "../headers/bits.hpp"

class Bot
{
	int					socket_fd;
	struct sockaddr_in	server_addr;
	string 				pas;
	int					prt;
	std::vector<string> facts;

	void	open_config_file();
	void	send_msg(const string& msg);
	void	authentificate();
	string	read_msg();
	void	send_fact();
	void	initVars();

	public :
		Bot(void);
		Bot(Bot const &copy);
		~Bot(void);
		Bot& operator=(const Bot &other);
		void	run();
};

#endif