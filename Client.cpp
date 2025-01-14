/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 09:10:53 by iassil            #+#    #+#             */
/*   Updated: 2025/01/14 09:25:53 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
	: fd( -1 ),
	  authenticated( false ),
	  registered( false ),
	  key( -1 ),
	  loggedIn( false ),
	  nickname( "" ),
	  userName( "" ),
	  buffer( "" ),
	  ipAdd( "" ) {
		cout << "Client created" << endl;
	  }

Client::Client( const string &nickname, const string &username, int key, int fd )
	: fd( fd ),
	  authenticated( false ),
	  registered( false ),
	  key( key ),
	  loggedIn( false ),
	  nickname( nickname ),
	  userName( username ),
	  buffer( "" ),
	  ipAdd( "" ) {}

Client::~Client() {}

Client::Client( Client const &src ) {
	*this = src;
}

Client &Client::operator=( Client const &src ) {
	if ( this != &src ) {
		this->fd			 = src.fd;
		this->registered	 = src.registered;
		this->nickname		 = src.nickname;
		this->loggedIn		 = src.loggedIn;
		this->userName		 = src.userName;
		this->buffer		 = src.buffer;
		this->ipAdd			 = src.ipAdd;
		this->ChannelsInvite = src.ChannelsInvite;
		this->channels		 = src.channels;
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

int Client::getKey() const {
	return key;
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

void Client::setKey( const int key ) {
	this->key = key;
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
