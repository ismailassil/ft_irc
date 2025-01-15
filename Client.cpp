#include "Client.hpp"

Client::Client()
	: fd( -1 ),
	  authenticated( false ),
	  registered( false ),
	  loggedIn( false ),
	  nickname( "" ),
	  userName( "" ),
	  buffer( "" ),
	  ipAdd( "" ),
	  ChannelsInvite( 0 ),
	  channels( 0 ) {}

Client::Client( const string &nickname, const string &username, int fd )
	: fd( fd ),
	  authenticated( false ),
	  registered( false ),
	  loggedIn( false ),
	  nickname( nickname ),
	  userName( username ),
	  buffer( "" ),
	  ipAdd( "" ),
	  ChannelsInvite( 0 ),
	  channels( 0 ) {}

Client::~Client() {}

Client::Client( Client const &src ) {
	*this = src;
}

Client &Client::operator=( Client const &src ) {
	if ( this != &src ) {
		fd			   = src.fd;
		authenticated  = src.authenticated;
		registered	   = src.registered;
		loggedIn	   = src.loggedIn;
		nickname	   = src.nickname;
		userName	   = src.userName;
		buffer		   = src.buffer;
		ipAdd		   = src.ipAdd;
		ChannelsInvite = src.ChannelsInvite;
		channels	   = src.channels;
	}
	return *this;
}

int Client::getFd() const {
	return fd;
}

bool Client::getAuthenticated() const {
	cout << authenticated << endl;
	return authenticated;
}

bool Client::getRegistered() const {
	return registered;
}

bool Client::getInviteChannel( string &ChName ) const {
	return find( ChannelsInvite.begin(), ChannelsInvite.end(), ChName ) !=
		   ChannelsInvite.end();
}

const string Client::getNickName() const {
	return nickname;
}

bool Client::getLoggedIn() const {
	return loggedIn;
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

void Client::setLoggedIn( const bool value ) {
	this->loggedIn = value;
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
