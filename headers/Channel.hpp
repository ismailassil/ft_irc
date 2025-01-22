#pragma once
#include "Client.hpp"

class Channel {
	private:
		int				  invite_only;
		string			  topic;
		int				  key;
		int				  limit;
		int				  topicRestrict;
		string			  name;
		string			  password;
		vector< Client >  clients;
		vector< Client >  admins;
		map< char, bool > modes;

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
		void setModeAtIndex( const char, const bool );
		void setTopicRestrict( const int topicRestrict );

		bool isClientInChannel( const string & ) const;
		bool isAdminInChannel( const string & ) const;
		bool isInChannel( const string & ) const;

		// Getters
		int			  getInviteOnly() const;
		const string  getTopic() const;
		int			  getKey() const;
		int			  getLimit() const;
		int			  getNumberOfClients() const;
		bool		  getModeAtIndex( char ) const;
		const string  getPassword() const;
		const string  getName() const;
		const string  getModes() const;
		const string  getClientChannelList() const;
		const Client *getClient( const int ) const;
		const Client *getAdmin( const int ) const;
		const Client *getClientInChannel( const string & ) const;
		const Client *getAdminInChannel( const string & ) const;
		bool		  getTopicRestrict() const;

		// Methods
		void addAdmin( const Client );
		void removeAdmin( const int );
		void addClient( const Client );
		void removeClient( const int );
		bool changeClientToAdmin( const string & );
		bool changeAdminToClient( const string & );

		// Send to All Clients
		void broadcast( const string & ) const;
		void broadcast( const string &, int ) const;
};
