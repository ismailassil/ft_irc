#include "../headers/Client.hpp"

#include <vector>

Client::Client()
	: fd( -1 ),
	  authenticated( false ),
	  registered( false ),
	  nickname( "" ),
	  userName( "" ),
	  buffer( "" ),
	  ipAdd( "" ),
	  ChannelsInvite( 0 ),
	  channels( 0 ),
	  friends( 0 ) {}

Client::Client( const int fd, const string &ip )
	: fd( fd ),
	  authenticated( false ),
	  registered( false ),
	  nickname( "" ),
	  userName( "" ),
	  buffer( "" ),
	  ipAdd( ip ),
	  ChannelsInvite( 0 ),
	  channels( 0 ),
	  friends( 0 ) {}

Client::~Client() {}

Client::Client( Client const &src ) {
	*this = src;
}

Client &Client::operator=( Client const &src ) {
	if ( this != &src ) {
		fd			   = src.fd;
		authenticated  = src.authenticated;
		registered	   = src.registered;
		nickname	   = src.nickname;
		userName	   = src.userName;
		buffer		   = src.buffer;
		ipAdd		   = src.ipAdd;
		ChannelsInvite = src.ChannelsInvite;
		channels	   = src.channels;
		friends		   = src.friends;
	}
	return *this;
}

int Client::getFd() const {
	return fd;
}

bool Client::getAuthenticated() const {
	return authenticated;
}

bool Client::getRegistered() const {
	return registered;
}

bool Client::getInviteChannel( const string &ChName ) const {
	return find( ChannelsInvite.begin(), ChannelsInvite.end(), ChName ) !=
		   ChannelsInvite.end();
}

const string Client::getNickName() const {
	return nickname;
}

const string Client::getUserName() const {
	return userName;
}

const string Client::getIpAdd() const {
	return ipAdd;
}

const string Client::getBuffer() const {
	return buffer;
}

void Client::setFd( const int fd ) {
	this->fd = fd;
}

void Client::setAuthenticated( const bool value ) {
	this->authenticated = value;
}

void Client::setRegistered( const bool value ) {
	this->registered = value;
}

void Client::setNickname( const string &nickName ) {
	this->nickname = nickName;
}

void Client::setUsername( const string &userName ) {
	this->userName = userName;
}

void Client::setBuffer( const string &received ) {
	this->buffer = received;
}

void Client::setIpAdd( const string &ipAdd ) {
	this->ipAdd = ipAdd;
}

void Client::addInvitedChannel( const string &channelName ) {
	ChannelsInvite.push_back( channelName );
}

void Client::addFriend( const string &friendName ) {
	if ( find( friends.begin(), friends.end(), friendName ) != friends.end() )
		return;
	friends.push_back( friendName );
}

bool Client::isFriend( const string &friendName ) {
	return find( friends.begin(), friends.end(), friendName ) != friends.end();
}

void Client::setFriendNickName( const string &old, const string &nickName ) {
	for ( vector< string >::iterator it = friends.begin(); it != friends.end(); it++ ) {
		if ( *it == old ) {
			*it = nickName;
			break;
		}
	}
}

void Client::removeFriend( const string &friendName ) {
	for ( vector< string >::iterator it = friends.begin(); it != friends.end(); it++ ) {
		if ( *it == friendName ) {
			friends.erase( it );
			break;
		}
	}
}

vector< string > Client::getFriends() const {
	return friends;
}
