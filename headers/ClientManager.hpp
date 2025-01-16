#pragma once
#include "Channel.hpp"
#include "Client.hpp"
#include "bits.hpp"

class ClientManager {
	private:
		vector< int >	   fds;
		map< int, Client > cli;
		vector< Channel >  channels;
		string			   pass;

		bool		rNewLine( string& );
		bool		isCmd( const string&, const char* );
		bool		isValid( const string& str );

		void nickCmd( int, string& );
		void quitCmd( int, string& );
		void joinCmd( int, string& );
		void kickCmd( int, string& );
		void topicCmd( int, string& );
		void modeCmd( int, string& );
		void privmsg( int, string& );
		void partCmd( int, string& );
 
		Channel* getChannel(const string&);
		Client* getClient(const string&);

		const string getPrefix(int);

	public:
		void parse( int, string& );
		void registerClient( int, string& );
		
		void setPass( const string& );
		const string getPass() const;
};
