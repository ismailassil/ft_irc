#include <cctype>

#include "../headers/bits.hpp"

const vector< string > ft_split_tokens( const string& input ) {
	istringstream	 stream( input );
	vector< string > tokens;
	string			 token;

	while ( stream >> token ) {
		tokens.push_back( token );
		token.clear();
	}
	return tokens;
}

const vector< string > splitString( const string& str, char delim ) {
	vector< string > split;
	string			 token = "";

	for ( size_t i = 0; i < str.size(); i++ ) {
		if ( str[i] == delim ) {
			split.push_back( token );
			token = "";
		} else
			token += str[i];
	}
	split.push_back( token );
	return split;
}

void ft_send( int fd, const string& str ) {
	ssize_t sent;

	while ( 1 ) {
		sent = send( fd, str.c_str(), str.size(), 0 );

		if ( sent >= 0 ) {
			return;
		}

		if ( errno == EAGAIN || errno == EWOULDBLOCK ) {
			continue;
		} else if ( errno == EINTR ) {
			continue;
		} else {
			perror( "send()" );
			return;
		}
	}
}

const string getText( const string& input, vector< string > tokens, int index ) {
	string msg;

	if ( tokens.at( index )[0] == ':' ) {
		size_t pos = input.find( tokens.at( index ) );
		msg		   = input.substr( pos + 1 );
	} else {
		msg = tokens.at( index );
	}
	return msg;
}

int stringToInt( const string& str ) {
	stringstream ss( str );
	int			 result;

	ss >> result;
	if ( ss.fail() ) {
		return -1;
	}
	return result;
}

bool isNumber( const string& s ) {
	if ( s.empty() )
		return false;

	for ( size_t i = 0; i < s.length(); i++ ) {
		if ( !isdigit( s[i] ) )
			return false;
	}
	return true;
}

extern int stop_server;

void error( const string& str, int exit_status ) {
	perror( str.c_str() );
	exit( exit_status );
}

void handle_signal( int signal ) {
	(void)signal;
	stop_server = 1;
}

int parse_input( int& ac, char**& av ) {
	string port;
	string password;
	int	   port_n;

	if ( ac != 3 ) {
		cerr << "ERROR: ./ircserv <port> <password>" << endl;
		return ( 1 );
	}
	port	 = av[1];
	password = av[2];
	if ( port.empty() || password.empty() ) {
		cerr << "ERROR: Empty argument is not allowed" << endl;
		return ( 1 );
	}
	for ( size_t i = 0; i < password.size(); i++ ) {
		if ( isspace( password.at( i ) ) ) {
			cerr << "ERROR: port must not contains white-spaces" << endl;
			return ( 1 );
		}
	}
	for ( size_t i = 0; i < port.size(); i++ ) {
		if ( !isdigit( port.at( i ) ) ) {
			cerr << "ERROR: port must only contains numbers" << endl;
			return ( 1 );
		}
	}
	port_n = atoi( port.c_str() );
	if ( port_n > 65535 || port_n < 1024 ) {
		cerr << "ERROR: port number must be between [1024 - 65535]" << endl;
		return ( 1 );
	}
	if ( password.size() < PASS_LENGH && password.size() > PASS_MAXLEN ) {
		cerr << "ERROR: password size must be between [" << PASS_LENGH << " - " << PASS_MAXLEN << "]" << endl;
		return ( 1 );
	}
	return ( 0 );
}

bool hasOnlyWhitespaces( const string& str ) {
	for ( string::const_iterator it = str.begin(); it != str.end(); ++it ) {
		if ( !isspace( static_cast< unsigned char >( *it ) ) ) {
			return false;
		}
	}
	return true;
}

string getTimestamp() {
	time_t		 currentTime = time( NULL );
	stringstream timestamp;
	timestamp << currentTime;

	return timestamp.str();
}
