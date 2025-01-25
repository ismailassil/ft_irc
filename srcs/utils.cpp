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
	if ( send( fd, str.c_str(), str.size(), 0 ) < 0 )
		cerr << "ERROR: send() failed" << endl;
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

string intToString( int n ) {
	stringstream ss;
	ss << n;
	return ss.str();
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

void printCurrentDateTime() {
	time_t curr_time;
	curr_time = time( NULL );

	tm *tm_local = localtime( &curr_time );
	if ( tm_local == NULL )
		return;
	cout << "[" << tm_local->tm_mday << "-"
		 << setfill( '0' ) << setw( 2 ) << tm_local->tm_mon + 1 << "-"
		 << tm_local->tm_year + 1900 << "] ["
		 << setw( 2 ) << tm_local->tm_hour << ":"
		 << setw( 2 ) << tm_local->tm_min << ":"
		 << setw( 2 ) << tm_local->tm_sec << "] ";
}

void handle_signal( int signal ) {
	(void)signal;
	stop_server = 1;
	cout << "\r";
	printCurrentDateTime();
	cout << CYAN << "[Server] Signal received" << RESET << endl;
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
		if ( isprint( password.at( i ) ) == 0 ) {
			cerr << "ERROR: port must only contains printable characters" << endl;
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
	if ( password.size() < PASS_LENGH || password.size() > PASS_MAXLEN ) {
		cerr << "ERROR: password size must be between " << PASS_LENGH << " and " << PASS_MAXLEN << " characters" << endl;
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
