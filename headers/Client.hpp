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
		vector< string >			   friends;

	public:
		// Constructors and Destructor
		Client();
		Client( const int fd, const string &ip );
		Client( Client const &src );
		Client &operator=( Client const &src );
		~Client();

		// Getters
		int				 getFd() const;
		bool			 getAuthenticated() const;
		bool			 getRegistered() const;
		bool			 getInviteChannel( const string &ChName ) const;
		const string	 getNickName() const;
		const string	 getUserName() const;
		const string	 getIpAdd() const;
		const string	 getBuffer() const;
		vector< string > getFriends() const;

		// Setters
		void setFd( const int fd );
		void setAuthenticated( const bool value );
		void setNickname( const string &nickName );
		void setUsername( const string &username );
		void setBuffer( const string &received );
		void setRegistered( const bool value );
		void setIpAdd( const string &ipadd );
		void setFriendNickName( const string &old, const string &nickName );

		// Methods
		void addInvitedChannel( const string &channelName );
		void addFriend( const string &friendName );
		bool isFriend( const string &friendName );
		void removeFriend( const string &friendName );
};
