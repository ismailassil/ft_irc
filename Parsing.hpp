#ifndef __PARSING__
#	define __PARSING__
#	define JOIN "JOIN"
#	define

#	include <iostream>
using std::string;

class Parsing {
	private:
		int	 isDone;
		bool rNewLine( string& );
		bool isCMD( const string&, const char* );

	public:
		Parsing() { isDone = 0; }
		void parse( string& );
		void registerClient( string& );
};

#endif	// __PARSING__