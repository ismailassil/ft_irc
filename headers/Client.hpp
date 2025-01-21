#pragma once
#include "bits.hpp"

class Client {
	private:
		int							   fd;
		bool						   authenticated;
		bool						   registered;
		string						   nickname;
		string						   userName;
		string						   buffer;
		string						   ipAdd;
		vector< string >			   ChannelsInvite;
		vector< pair< string, bool > > channels;

	public:
		// Constructors and Destructor
		Client();
		Client( const string &nickname, const string &username, int fd );
		~Client();
		Client( Client const &src );
		Client &operator=( Client const &src );

		// Getters
		int			 getFd() const;
		bool		 getAuthenticated() const;
		bool		 getRegistered() const;
		bool		 getInviteChannel( const string &ChName ) const;
		const string getNickName() const;
		const string getUserName() const;
		const string getIpAdd() const;
		const string getBuffer() const;

		// Setters
		void setFd( const int fd );
		void setAuthenticated( const bool value );
		void setNickname( const string &nickName );
		void setUsername( const string &username );
		void setBuffer( const string &received );
		void setRegistered( const bool value );
		void setIpAdd( const string &ipadd );

		// Methods
		void addInvitedChannel( const string &channelName );
};
