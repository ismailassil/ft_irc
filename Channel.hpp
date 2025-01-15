#pragma once
#include "Client.hpp"

class Channel {
	private:
		int							 invite_only;
		string						 topic;
		int							 topicRestrict;
		int							 key;
		int							 limit;
		string						 name;
		string						 password;
		vector< Client >			 clients;
		vector< Client >			 admins;

	public:
		Channel();
		~Channel();
		Channel( Channel const & );
		Channel &operator=( Channel const & );

		// Setters
		void setInviteOnly( const int );
		void setTopic( const string );
		void setTopicRestrict( const int );
		void setKey( const int );
		void setLimit( const int );
		void setPassword( const string & );
		void setName( const string & );

		// Getters
		const int	  getInviteOnly() const;
		const string  getTopic() const;
		const int	  getTopicRestrict() const;
		const int	  getKey() const;
		const int	  getLimit() const;
		const int	  getNumberOfClients() const;
		const bool	  isClientInChannel( const string & ) const;
		const bool	  isAdminInChannel( const string & ) const;
		const string  getPassword() const;
		const string  getName() const;
		const string  getClientChannelList() const;
		const Client *getClient( const int ) const;
		const Client *getAdmin( const int ) const;
		const Client *getClientInChannel( const string & ) const;
		const Client *getAdminInChannel( const string & ) const;

		// Methods
		void addAdmin( const Client );
		void removeAdmin( const int );
		void addClient( const Client );
		void removeClient( const int );
		bool changeClientToAdmin( const string & );
		bool changeAdminToClient( const string & );

		// Send to All
		void broadcast( const string &, int );
};
