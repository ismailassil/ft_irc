#pragma once
#include "Client.hpp"

class Channel {
	private:
		int							 invite_only;
		string						 topic;
		int							 key;
		int							 limit;
		string						 name;
		string						 password;
		vector< Client >			 clients;
		vector< Client >			 admins;
		vector< pair< char, bool > > modes;

	public:
		Channel();
		~Channel();
		Channel( Channel const & );
		Channel &operator=( Channel const & );

		// Setters
		void setInviteOnly( const int );
		void setTopic( const string );
		void setKey( const int );
		void setLimit( const int );
		void setPassword( const string & );
		void setName( const string & );
		void setModeAtIndex( const size_t, const bool );

		// Getters
		const int	  getInviteOnly();
		const string  getTopic();
		const int	  getKey();
		const int	  getLimit();
		const int	  getNumberOfClients();
		const bool	  getModeAtIndex( const size_t );
		const bool	  isClientInChannel( const string & );
		const string  getPassword();
		const string  getName();
		const string  getModes();
		const string  getClientChannelList();
		const Client *getClient( const int );
		const Client *getAdmin( const int );
		const Client *getClientInChannel( const string & );

		// Methods
		void add_client( const Client );
		void add_admin( const Client );
		void remove_client( const int );
		void remove_admin( const int );
		bool change_clientToAdmin( const string & );
		bool change_adminToClient( const string & );

		// Send to All
		void sendBroadcast( const string & );
		void sendBroadcast( const string &, int );
};
