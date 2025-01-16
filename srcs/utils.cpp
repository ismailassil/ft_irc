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
