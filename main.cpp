#include "headers/Server.hpp"

int main(int ac,char **av)
{
    if (parse_input(ac, av)) {
        return (1);
    }
	Server server(av[1], av[2]);
	return 0;
}
