#pragma once
#include "Client.hpp"

class Channel {
	private:
		int				  invite_only;
		string			  topic;
		string			  topicAuthor;
		string			  topicDate;
		int				  key;
		int				  limit;
		int				  topicRestrict;
		string			  name;
		string			  password;
		vector< Client >  clients;
		vector< Client >  admins;
		map< char, bool > modes;
		string			  creationDate;

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
		void setTopicAuthor( const string & );
		void setTopicDate( const string & );
		void setCreationDate();

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
		const string  getAdminChannelList() const;
		const Client *getClient( const int ) const;
		const Client *getAdmin( const int ) const;
		const Client *getClientInChannel( const string & ) const;
		const Client *getAdminInChannel( const string & ) const;
		bool		  getTopicRestrict() const;
		const string  getTopicAuthor() const;
		const string  getTopicDate() const;
		const string  getCreationDate() const;

		// Methods
		void addAdmin( const Client );
		void removeAdmin( const int );
		void addClient( const Client );
		void removeClient( const int );
		void changeMemberNick( const string &, const string & );

		// Send to All Clients
		void broadcast( const string & ) const;
		void broadcast( const string &, int ) const;
};
