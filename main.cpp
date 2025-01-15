#include "server.hpp"

void error(std::string str, int exit_status)
{
	perror(str.c_str());
	exit(exit_status);
}

int server_init(char *port)
{
	int					socket_fd;
	struct sockaddr_in	server_addr;


	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
		error("socker() : ", 1);
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(8080);
	if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
	{
		close(socket_fd);
		error("bind() : ", 1);
	}
	if (listen(socket_fd, BACKLOG) < 0)
	{
		close(socket_fd);
		error("listen() : ", 1);
	}
	std::cout << "Server listening on port " << port << std::endl;
	return (socket_fd);
}

int	add_new_client(int &socket_fd, int &nfds, struct pollfd *fds)
{
	int	client_fd;
	struct	sockaddr_in	client_addr;
	socklen_t			addrelen;

	std::cout << "New client" << std::endl;
	memset(&client_addr, 0, sizeof(client_addr));
	addrelen = sizeof(client_addr);
	client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &addrelen);
	if (client_fd < 0)
		error("accept : ", 1);
	else
	{
		fds[nfds].fd = client_fd;
		fds[nfds].events = POLLIN;
		nfds++;
	}
	return (client_fd);
}

void	delete_client(int &i, struct pollfd *fds, int &nfds)
{
	close(fds[i].fd);
	for (int j = i; j < nfds - 1; j++)
		fds[j] = fds[j + 1];
	nfds--;
}

void	server(int socket_fd)
{
	
	struct pollfd		fds[MAXCLIENT + 1];
	int					nfds;
	int					client_fd;

	fds[0].fd = socket_fd;
	fds[0].events = POLLIN;
	nfds = 1;

		int poll_res = poll(fds, nfds, -1);
		if (poll_res < 0)
		{
			close(socket_fd);
			error("poll() : ", 1);
		}
		if (fds[0].revents & POLLIN)
			client_fd = add_new_client(socket_fd, nfds, fds);
		for (int i = 1; i < nfds; i++)
		{
			if (fds[i].revents & POLLIN)
			{
				// if byte > 1024
				char buffer[BUFFER_SIZE];
				int byte = read(fds[i].fd, buffer, BUFFER_SIZE);
				if (byte == 0)
				{
					delete_client(i, fds, nfds);
					continue;
				}
				buffer[byte] = '\0';
				std::cout << "fd = " << fds[i].fd << "Received: " << buffer << std::endl;
			}
			if (fds[i].revents & POLLHUP)
				delete_client(i, fds, nfds);
		}
}
void print_server_ip()
{
    char hostname[256];
    struct hostent *host_entry;
    char *IPbuffer;

    // Get the hostname
    gethostname(hostname, sizeof(hostname));
    
    // Get the host information
    host_entry = gethostbyname(hostname);
    
    // Convert the first address from the host entry to a string
    IPbuffer = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));

    // Print the IP address
    std::cout << "Server IP Address: " << IPbuffer << std::endl;
}


int main(int ac,char **av)
{
	int	socket_fd;

	if (ac != 3)
		return (1);
	std::cout << "port = " << av[1] << std::endl;
	std::cout << "pass = " << av[2] << std::endl;
	print_server_ip();
	socket_fd = server_init(av[1]);
	server(socket_fd);
	return 0;
}
