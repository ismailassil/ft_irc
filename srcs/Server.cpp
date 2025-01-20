#include "../headers/Server.hpp"

int stop_server = 0;

Server::Server() { }

Server::Server(const Server &copy)
{
	*this = copy;
}

Server::~Server() {
    for(int i = 0; i < nfds; i++){
        close(fds[i].fd);
    }
	cout << "\nServer is shutting down" << endl;
}

Server& Server::operator=(const Server &other)
{
    socket_fd = other.socket_fd;
    server_addr = other.server_addr;
    port = other.port;
    password = other.password;
    nfds = other.nfds;
    for (int i = 0; i < nfds; i++){
        fds[i] = other.fds[i];
    }
	return (*this);
}

void Server::add_client()
{
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t addrlen;

    memset(&client_addr, 0, sizeof(client_addr));
    addrlen = sizeof(client_addr);
    cout << "client ip = " << inet_ntoa(client_addr.sin_addr) << endl;
    client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &addrlen);
    if (client_fd < 0)
        return perror("accept : ");
    if (this->nfds >= MAXCLIENT + 1)
    {
        cout << "Max clients reached. Closing connection." << endl;
        close(client_fd);
        return;
    }
    fds[nfds].fd = client_fd;
    fds[nfds].events = POLLIN | POLLPRI;
    ++nfds;
    cout << "Client <" << client_fd << "> is Connected" << endl;
    send(client_fd, WELCOME_MSG, sizeof(WELCOME_MSG), 0);
}

void    Server::read_msg(int &i)
{
    char buffer[BUFFER_SIZE];
    string message;
    int byte;

    while ((byte = recv(fds[i].fd, buffer, BUFFER_SIZE - 1, 0)) > 0)
    {
        buffer[byte] = '\0';
        message += buffer;
        if (byte < BUFFER_SIZE - 1)
            break;
    }
    if (!message.empty())
        cout << "fd = " << fds[i].fd << " Received: " << message;
    if (byte == 0)
        remove_client(i);
    else if (byte < 0)
        perror("recv() ");
}

void    Server::server_init()
{
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    signal(SIGHUP, handle_signal);
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
        error("socket() : ", 1);
    int reuse = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    memset(&fds, 0, sizeof(fds));
    memset(&this->server_addr, 0, sizeof(this->server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(static_cast<uint16_t>(atoi(port.c_str())));
    if (bind(socket_fd, (struct sockaddr *)&this->server_addr, sizeof(this->server_addr)) != 0)
    {
        close(socket_fd);
        error("bind()", 1);
    }
    if (listen(socket_fd, BACKLOG) < 0)
    {
        close(socket_fd);
        error("listen()", 1);
    }
    cout << "Server listening on port " << port << endl;
}

Server::Server(string _port, string _password): port(_port), password(_password)
{
    int status;
    server_init();
    fds[0].fd = socket_fd;
    fds[0].events = POLLIN | POLLPRI;
    nfds = 1;
    while (!stop_server)
    {
        status = poll(fds, nfds, 1000);
        if (status == 0)
            continue;
        if (status < 0 && stop_server == 0)
            perror("poll()");
        if (fds[0].revents & POLLIN)
            add_client();
        for (int i = 1; i < nfds; i++)
        {
            if (fds[i].revents & POLLIN)
                read_msg(i);
            if (fds[i].revents & POLLHUP)
                remove_client(i);
        }
    }
}

void	Server::remove_client(int &i)
{
    cout << "Client <" << fds[i].fd << "> is Disconnected" << endl;
	close(fds[i].fd);
	for (int j = i; j < nfds - 1; j++)
		fds[j] = fds[j + 1];
	nfds--;
}
