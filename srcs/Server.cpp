#include "../headers/Server.hpp"

#include <vector>

int			  stop_server  = 0;
int			  Server::nfds = 0;
struct pollfd Server::fds[MAXCLIENT + 1];

Server::Server() {}

Server::~Server() {
	for ( int i = 0; i < nfds; i++ ) {
		close( fds[i].fd );
	}
	cout << "\r";
	printCurrentDateTime();
	cout << YELLOW << "[Server] Shutting down" << RESET << endl;
}

Server::Server( const Server &copy ) {
	*this = copy;
}

Server &Server::operator=( const Server &other ) {
	if ( this == &other )
		return *this;
	socket_fd	= other.socket_fd;
	server_addr = other.server_addr;
	port		= other.port;
	password	= other.password;
	nfds		= other.nfds;
	for ( int i = 0; i < nfds; i++ ) {
		fds[i] = other.fds[i];
	}
	return ( *this );
}

void Server::add_client() {
	int				   client_fd;
	struct sockaddr_in client_addr;
	socklen_t		   addrlen;

	memset( &client_addr, 0, sizeof( client_addr ) );
	addrlen	  = sizeof( client_addr );
	client_fd = accept( socket_fd, (struct sockaddr *)&client_addr, &addrlen );
	if ( client_fd < 0 )
		return perror( "accept()" );
	if ( this->nfds >= MAXCLIENT + 1 ) {
		cout << "[Server] Max clients reached. Closing connection." << endl;
		close( client_fd );
		return;
	}
	if ( fcntl( client_fd, F_SETFL, O_NONBLOCK ) < 0 ) {
		error( "fcntl()", 1 );
	}
	clientManager.addNewClient( client_fd, client_addr.sin_addr );
	fds[nfds].fd	  = client_fd;
	fds[nfds].events  = POLLIN;
	fds[nfds].revents = 0;
	++nfds;
	printCurrentDateTime();
	cout << GREEN << "[Server] Client <ID: " << WHITE << client_fd << GREEN << ", IP: " << WHITE << inet_ntoa( client_addr.sin_addr ) << GREEN << "> Connected" << RESET << endl;
}

void Server::server_init() {
	signal( SIGINT, handle_signal );
	signal( SIGTERM, handle_signal );
	signal( SIGHUP, handle_signal );
	signal( SIGPIPE, SIG_IGN );

	socket_fd = socket( AF_INET, SOCK_STREAM, 0 );
	if ( socket_fd == -1 )
		error( "socket()", 1 );
	int reuse = 1;
	if ( setsockopt( socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof( reuse ) ) < 0 ) {
		error( "setsockopt()", 1 );
	}
	if ( fcntl( socket_fd, F_SETFL, O_NONBLOCK ) < 0 ) {
		error( "fcntl()", 1 );
	}
	memset( &this->server_addr, 0, sizeof( this->server_addr ) );
	server_addr.sin_family		= AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port		= htons( static_cast< uint16_t >( atoi( port.c_str() ) ) );
	if ( bind( socket_fd, (struct sockaddr *)&this->server_addr, sizeof( this->server_addr ) ) != 0 ) {
		close( socket_fd );
		error( "bind()", 1 );
	}
	printCurrentDateTime();
	cout << YELLOW << "[Server] running....." << RESET << endl;
	if ( listen( socket_fd, SOMAXCONN ) < 0 ) {
		close( socket_fd );
		error( "listen()", 1 );
	}
	printCurrentDateTime();
	cout << YELLOW << "[Server] listening on port " << BLUE << port << RESET << endl;
	clientManager.setPass( password );
}

Server::Server( const string &_port, const string &_password )
	: port( _port ), password( _password ) {
	int status;
	server_init();
	fds[0].fd	   = socket_fd;
	fds[0].events  = POLLIN;
	fds[0].revents = 0;
	nfds		   = 1;
	while ( !stop_server ) {
		if ( ( status = poll( fds, nfds, -1 ) ) < 0 && stop_server == 0 ) {
			if ( stop_server == 0 )
				break;
			perror( "poll()" );
			break;
		}
		if ( stop_server == 0 && fds[0].revents & POLLIN )
			add_client();
		for ( int i = 1; i < nfds; i++ ) {
			if ( stop_server == 0 && fds[i].revents & POLLIN )
				read_msg( fds[i].fd );
		}
	}
}

void Server::read_msg( int &fd ) {
	char   buffer[BUFFER_SIZE];
	string message;
	int	   byte;

	while ( ( byte = recv( fd, buffer, BUFFER_SIZE - 1, 0 ) ) > 0 ) {
		buffer[byte] = '\0';
		message += buffer;
		if ( byte < BUFFER_SIZE - 1 )
			break;
	}
	if ( !message.empty() ) {
		printCurrentDateTime();
		cout << "[Client] Message received from client <" << fd << "> : " << GREEN << message << RESET;
		vector< string > msg = ft_split_message( message );
		for ( vector< string >::iterator it = msg.begin(); it != msg.end(); it++ ) {
			clientManager.parse( fd, *it );
		}
	}
	if ( byte == 0 )
		remove_client( fd );
	else if ( byte < 0 )
		perror( "recv() " );
}

vector< string > Server::ft_split_message( string &msg ) {
	vector< string > res;

	size_t pos = msg.find_first_of( CRLF );
	if ( pos == string::npos ) {
		res.push_back( msg );
		return res;
	}
	while ( pos != string::npos ) {
		size_t pos_ = msg.find( CRLF );
		if (pos_ != string::npos)
			pos_ += 1;
		res.push_back( msg.substr( 0, pos + 1 ) );
		msg.erase( 0, pos + 1 );
		pos = msg.find_first_of( CRLF );
	}
	return res;
}

void Server::remove_client( int &fd ) {
	if ( close( fd ) != -1 ) {
		printCurrentDateTime();
		cout << CYAN << "[Server] Client <ID: " << WHITE << fd << CYAN << "> Disconnected" << RESET << endl;
	}
	//////////////////////////////////////////
	clientManager.removeClient( fd );
	//////////////////////////////////////////
	for ( int i = 1; i < nfds; i++ ) {
		if ( fds[i].fd == fd ) {
			for ( int j = i; j < nfds - 1; j++ ) {
				fds[j] = fds[j + 1];
			}
			--nfds;
			break;
		}
	}
}

void Server::remove_fd( int &fd ) {
	if ( close( fd ) != -1 ) {
		printCurrentDateTime();
		cout << CYAN << "[Server] Client <ID: " << WHITE << fd << CYAN << "> Disconnected" << RESET << endl;
	}
	for ( int i = 1; i < nfds; i++ ) {
		if ( fds[i].fd == fd ) {
			for ( int j = i; j < nfds - 1; j++ ) {
				fds[j] = fds[j + 1];
			}
			--nfds;
			break;
		}
	}
}
