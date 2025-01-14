#include "Channel.hpp"

Channel::Channel() {
	invite_only = 0;
	topic		= "";
	key			= 0;
	limit		= 0;
	name		= "";
	password	= "";
}

Channel::~Channel() {
}

Channel::Channel( Channel const &src ) {
	*this = src;
}

Channel &Channel::operator=( Channel const &src ) {
	invite_only = src.invite_only;
	topic		= src.topic;
	key			= src.key;
	limit		= src.limit;
	name		= src.name;
	password	= src.password;
	clients		= src.clients;
	admins		= src.admins;
	modes		= src.modes;
	return *this;
}

void Channel::setInviteOnly( const int invite_only ) {
	this->invite_only = invite_only;
}

void Channel::setTopic( const string topic ) {
	this->topic = topic;
}

void Channel::setKey( const int key ) {
	this->key = key;
}

void Channel::setLimit( const int limit ) {
	this->limit = limit;
}

void Channel::setPassword( const string &password ) {
	this->password = password;
}

void Channel::setName( const string &name ) {
	this->name = name;
}

void Channel::setModeAtIndex( const size_t index, const bool mode ) {
	modes[index].second = mode;
}

const int Channel::getInviteOnly() const {
	return invite_only;
}

const string Channel::getTopic() const {
	return topic;
}

const int Channel::getKey() const {
	return key;
}

const int Channel::getLimit() const {
	return limit;
}

const int Channel::getNumberOfClients() const {
	return clients.size();
}

const bool Channel::getModeAtIndex( const size_t index ) const {
	return modes[index].second;
}

const bool Channel::isClientInChannel( const string &nick ) const {
	for ( size_t i = 0; i < clients.size(); i++ ) {
		if ( clients[i].getNickName() == nick )
			return true;
	}
	return false;
}

const bool Channel::isAdminInChannel( const string &nick ) const {
	for ( size_t i = 0; i < admins.size(); i++ ) {
		if ( admins[i].getNickName() == nick )
			return true;
	}
	return false;
}

const string Channel::getPassword() const {
	return password;
}

const string Channel::getName() const {
	return name;
}

const string Channel::getModes() const {
	string modes_str = "";
	for ( size_t i = 0; i < modes.size(); i++ ) {
		if ( modes[i].second )
			modes_str += modes[i].first;
	}
	return modes_str;
}

const string Channel::getClientChannelList() const {
	string list = "";
	for ( size_t i = 0; i < clients.size(); i++ ) {
		list += clients[i].getNickName();
		if ( i + 1 < clients.size() )
			list += " ";
	}
	return list;
}

const Client *Channel::getClient( const int fd ) const {
	for ( size_t i = 0; i < clients.size(); i++ ) {
		if ( clients[i].getFd() == fd )
			return &clients[i];
	}
	return NULL;
}

const Client *Channel::getAdmin( const int fd ) const {
	for ( size_t i = 0; i < admins.size(); i++ ) {
		if ( admins[i].getFd() == fd )
			return &admins[i];
	}
	return NULL;
}

const Client *Channel::getClientInChannel( const string &name )const  {
	for ( size_t i = 0; i < clients.size(); i++ ) {
		if ( clients[i].getNickName() == name )
			return &clients[i];
	}
	return NULL;
}

const Client *Channel::getAdminInChannel( const string &name )const  {
	for ( size_t i = 0; i < admins.size(); i++ ) {
		if ( admins[i].getNickName() == name )
			return &admins[i];
	}
	return NULL;
}

void Channel::addClient( const Client newClient ) {
	clients.push_back( newClient );
}

void Channel::addAdmin( const Client newClient ) {
	admins.push_back( newClient );
}

void Channel::removeClient( const int fd ) {
	for ( size_t i = 0; i < clients.size(); i++ ) {
		if ( clients[i].getFd() == fd ) {
			clients.erase( clients.begin() + i );
			return;
		}
	}
}

void Channel::removeAdmin( const int fd ) {
	for ( size_t i = 0; i < admins.size(); i++ ) {
		if ( admins[i].getFd() == fd ) {
			admins.erase( admins.begin() + i );
			return;
		}
	}
}

bool Channel::changeClientToAdmin( const string &nick ) {
	for ( size_t i = 0; i < clients.size(); i++ ) {
		if ( clients[i].getNickName() == nick ) {
			admins.push_back( clients[i] );
			clients.erase( clients.begin() + i );
			return true;
		}
	}
	return false;
}

bool Channel::changeAdminToClient( const string &nick ) {
	for ( size_t i = 0; i < admins.size(); i++ ) {
		if ( admins[i].getNickName() == nick ) {
			clients.push_back( admins[i] );
			admins.erase( admins.begin() + i );
			return true;
		}
	}
	return false;
}

// void Channel::sendBroadcast( const string& reply ) {
// 	(void)reply;
// }

void Channel::broadcast( const string &reply, const int fd ) {
	(void)reply;
	(void)fd;
}