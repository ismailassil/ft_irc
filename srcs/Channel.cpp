#include "../headers/Channel.hpp"

Channel::Channel() : invite_only( 0 ), topic( "" ), topicAuthor( "" ), topicDate( "" ), key( 0 ), limit( 0 ), topicRestrict( 0 ), name( "" ), password( "" ), creationDate( "" ) {
	modes['k'] = false;
	modes['l'] = false;
	modes['i'] = false;
	modes['t'] = false;
}

Channel::~Channel() {}

Channel::Channel( Channel const &src ) {
	*this = src;
}

Channel &Channel::operator=( Channel const &src ) {
	if ( this != &src ) {
		invite_only	  = src.invite_only;
		topic		  = src.topic;
		key			  = src.key;
		limit		  = src.limit;
		name		  = src.name;
		password	  = src.password;
		topicRestrict = src.topicRestrict;
		clients		  = src.clients;
		admins		  = src.admins;
		modes		  = src.modes;
		topicAuthor	  = src.topicAuthor;
		topicDate	  = src.topicDate;
		creationDate  = src.creationDate;
	}
	return *this;
}

bool Channel::isInChannel( const string &nick ) const {
	for ( size_t i = 0; i < clients.size(); i++ ) {
		if ( clients[i].getNickName() == nick )
			return true;
	}
	for ( size_t i = 0; i < admins.size(); i++ ) {
		if ( admins[i].getNickName() == nick )
			return true;
	}
	return false;
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

void Channel::setModeAtIndex( const char index, const bool mode ) {
	modes.at( index ) = mode;
}

void Channel::setTopicRestrict( const int topicRestrict ) {
	this->topicRestrict = topicRestrict;
}

int Channel::getInviteOnly() const {
	return invite_only;
}

const string Channel::getTopic() const {
	return topic;
}

int Channel::getKey() const {
	return key;
}

int Channel::getLimit() const {
	return limit;
}

int Channel::getNumberOfClients() const {
	return clients.size();
}

bool Channel::getModeAtIndex( char mode ) const {
	return modes.at( mode );
}

bool Channel::isClientInChannel( const string &nick ) const {
	for ( size_t i = 0; i < clients.size(); i++ ) {
		if ( clients[i].getNickName() == nick )
			return true;
	}
	return false;
}

bool Channel::isAdminInChannel( const string &nick ) const {
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
	string mode = "";
	if ( modes.at( 'k' ) )
		mode += "k";
	if ( modes.at( 'l' ) )
		mode += "l";
	if ( modes.at( 'i' ) )
		mode += "i";
	if ( modes.at( 't' ) )
		mode += "t";
	if ( getAdminChannelList().size() > 0 )
		mode += "o";
	return mode;
}

const string Channel::getClientChannelList() const {
	string list = "";
	for ( size_t i = 0; i < clients.size(); i++ ) {
		if ( isAdminInChannel( clients[i].getNickName() ) )
			list += "@" + clients[i].getNickName() + " ";
		else
			list += clients[i].getNickName() + " ";
	}
	return list;
}

const string Channel::getAdminChannelList() const {
	string list = "";
	for ( size_t i = 0; i < admins.size(); i++ ) {
		list += "@" + admins[i].getNickName() + " ";
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

const Client *Channel::getClientInChannel( const string &name ) const {
	for ( size_t i = 0; i < clients.size(); i++ ) {
		if ( clients[i].getNickName() == name )
			return &clients[i];
	}
	return NULL;
}

const Client *Channel::getAdminInChannel( const string &name ) const {
	for ( size_t i = 0; i < admins.size(); i++ ) {
		if ( admins[i].getNickName() == name )
			return &admins[i];
	}
	return NULL;
}

const string Channel::getTopicAuthor() const {
	return topicAuthor;
}

const string Channel::getTopicDate() const {
	return topicDate;
}

const string Channel::getCreationDate() const {
	return creationDate;
}

void Channel::setCreationDate() {
	time_t		  _time = time( NULL );
	ostringstream oss;

	oss << _time;
	creationDate = string( oss.str() );
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

void Channel::changeMemberNick( const string &oldNick, const string &newNick ) {
	for ( size_t i = 0; i < clients.size(); i++ ) {
		if ( clients[i].getNickName() == oldNick ) {
			clients[i].setNickname( newNick );
			break;
		}
	}
	for ( size_t i = 0; i < admins.size(); i++ ) {
		if ( admins[i].getNickName() == oldNick ) {
			admins[i].setNickname( newNick );
			return;
		}
	}
}

void Channel::broadcast( const string &reply, const int fd ) const {
	for ( size_t i = 0; i < clients.size(); i++ ) {
		if ( clients[i].getFd() != fd )
			ft_send( clients[i].getFd(), reply );
	}
}

void Channel::broadcast( const string &reply ) const {
	for ( size_t i = 0; i < clients.size(); i++ ) {
		ft_send( clients[i].getFd(), reply );
	}
}

bool Channel::getTopicRestrict() const {
	return topicRestrict;
}

void Channel::setTopicAuthor( const string &author ) {
	topicAuthor = author;
}

void Channel::setTopicDate( const string &date ) {
	topicDate = date;
}
