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
	if ( send( fd, str.c_str(), str.size(), 0 ) == -1 )
		cerr << "send() failed" << endl;
}

const string getComment( vector<string> tokens, int index ) {
	string topic;

	if ( tokens.at( index )[0] == ':' ) {
		tokens.at( index ).erase( 0, 1 );
		for ( size_t i = index; i < tokens.size(); i++ ) {
			topic += tokens.at( i ) + " ";
		}
	} else {
		topic = tokens.at( index );
	}
	return topic;
}

int stringToInt(const string& str) {
    stringstream ss(str);
    int result;

    ss >> result;
    if (ss.fail()) {
        return -1;
    }
    return result;
}

bool isNumber(const string& s) {
    if (s.empty())
        return false;

    for (size_t i = 0; i < s.length(); i++) {
        if (!isdigit(s[i]))
            return false;
    }
    return true;
}

extern int  stop_server;

void error(string str, int exit_status) {
	perror(str.c_str());
	exit(exit_status);
}

void    handle_signal(int signal) {
	(void)signal;
    stop_server = 1;
}

int parse_input(int &ac, char **&av) {
    string port;
    string password;
    int         port_n;

	if (ac != 3)
    {
        cerr << "ERROR : ./ircserv <port> <password>" << endl;
		return (1);
    }
	port = av[1];
	password = av[2];
    if (port.empty() || password.empty())
    {
        cerr << "ERROR : Empty parametre is not allowed" << endl;
        return (1);
    }
    for (size_t i = 0; i < port.size(); i++)
    {
        if (!isdigit(port.at(i)))
        {
            cerr << "ERROR : port must only contains numbers" << endl;
            return (1);
        }
    }
    port_n = atoi(port.c_str());
    if (port_n > 65535 || port_n < 1024)
    {
        cerr << "ERROR : port number must be between [1024 - 65535]" << endl;
        return (1);
    }
    if (password.size() < PASS_LENGH)
    {
        cerr << "ERROR : password size must be over " << PASS_LENGH << endl;
        return (1);
    }
    return (0);
}

