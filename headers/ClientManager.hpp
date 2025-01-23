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

		bool		 checkWhiteSpaces( int, string& );
		bool		 isCmd( const string&, const char* );
		bool		 isValid( const string& str );
		bool		 isChannel( const string& channel );
		const string getPrefix( int );
		Client*		 getClient( const string& );
		Channel*	 findChannelByName( const vector< Channel >& channels, const string& name );
		Channel*	 getChannel( const string& );

		void pongCmd( int, string& );
		void quitCmd( int, string& );
		void partCmd( int, string& );
		void nickCmd( int, string& );
		void joinCmd( int, string& );
		void kickCmd( int, string& );
		void topicCmd( int, string& );
		void modeCmd( int, string& );
		void privmsgCmd( int, string& );
		void inviteCmd( int, string& );
		void pingCmd( int, string& );

	public:
		void parse( int, string& );
		void registerClient( int, string& );
		void removeClient( int );
		void addNewClient( int, struct in_addr ip );

		void		 setPass( const string& );
		const string getPass() const;
};
