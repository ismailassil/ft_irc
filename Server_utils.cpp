#include "Server.hpp"

extern int  stop_server;

void error(std::string str, int exit_status)
{
	perror(str.c_str());
	exit(exit_status);
}

void    handle_signal(int signal)
{
    stop_server = 1;
}

int pars_input(int &ac, char **&av)
{
    std::string port;
    std::string password;
    int         port_n;

	if (ac != 3)
    {
        std::cerr << "ERROR : ./ircserv <port> <password>" << std::endl;
		return (1);
    }
	port = av[1];
	password = av[2];
    if (port.empty() || password.empty())
    {
        std::cerr << "ERROR : Empty parametre is not allowed" << std::endl;
        return (1);
    }
    for (int i = 0; i < port.size(); i++)
    {
        if (!isdigit(port.at(i)))
        {
            std::cerr << "ERROR : port must only contains numbers" << std::endl;
            return (1);
        }
    }
    port_n = atoi(port.c_str());
    if (port_n > 65535 || port_n < 1024)
    {
        std::cerr << "ERROR : port number must be between [1024 - 65535]" << std::endl;
        return (1);
    }
    if (password.size() < PASS_LENGH)
    {
        std::cerr << "ERROR : password size must be over " << PASS_LENGH << std::endl;
        return (1);
    }
    return (0);
}

