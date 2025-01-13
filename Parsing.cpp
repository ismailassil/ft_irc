#include "Parsing.hpp"

#include <algorithm>
#include <algorithm>  // For std::transform
#include <cctype>
#include <cctype>  // For ::tolower
#include <sstream>
#include <string>  // For std::string
#include <utility>
#include <vector>

#define PASS						  "pass"
#define ERR_NEEDMOREPARAMS( client )  // 461

using std::equal;
using std::getline;
using std::istringstream;
using std::tolower;
using std::transform;
using std::vector;

bool Parsing::isCMD( const string& str, const char* cmd ) {
	if ( str.length() != std::strlen( cmd ) ) return false;
	string tmp = str;
	for ( size_t i = 0; i < tmp.length(); i++ ) {
		if ( tolower( tmp[i] ) != cmd[i] ) return false;
	}
	return true;
}

bool Parsing::rNewLine( string& input ) {
	size_t pos = input.find( "\n" );
	if ( pos == string::npos ) return true;
	input.erase( 0, pos );
	return false;
}

void Parsing::registerClient( string& input ) {
	if ( rNewLine( input ) ) return;
	istringstream	 stream( input );
	vector< string > tokens;
	string			 token;

	while ( stream >> token ) tokens.push_back( token );
	if ( tokens.size() == 0 ) return;
	if ( tokens.size() > 2 ) {
		if ( isCMD( tokens.at( 0 ), PASS ) ) {
			if ( isDone == 1 ) {
				// Create a function to handle the response to the client using custom ft_send();
				std::cerr << "ERR_ALREADYREGISTRED" << std::endl;
				return;
			}
		}
	}
}

void Parsing::parse( string& input ) {
	if ( rNewLine( input ) ) return;
	istringstream	 stream( input );
	vector< string > tokens;
	string			 token;

	while ( stream >> token ) tokens.push_back( token );
}