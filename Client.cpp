/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 09:10:53 by iassil            #+#    #+#             */
/*   Updated: 2025/01/14 15:05:03 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
	: fd( -1 ),
	 authenticated( false ),
	  registered( false ),
	  loggedIn( false ),
	  nickname( "" ),
	  userName( "" ),
	  buffer( "" ),
	  ipAdd( "" ) {}

Client::Client( const string &nickname, const string &username, int fd )
	: fd( fd ),
	  authenticated( false ),
	  registered( false ),
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

int Client::getFd() {
	return fd;
}

bool Client::getRegistered() {
	return registered;
}

bool Client::getInviteChannel( string &ChName ) {
	return find( ChannelsInvite.begin(), ChannelsInvite.end(), ChName ) !=
		   ChannelsInvite.end();
}

const string Client::getNickName() {
	return nickname;
}

bool Client::getLoggedIn() {
	return loggedIn;
}

const string Client::getUserName() {
	return userName;
}

const string Client::getIpAdd() {
	return ipAdd;
}

const string Client::getBuffer() {
	return buffer;
}

bool Client::getAuthenticated() {
	return authenticated;
}

void Client::setFd( int fd ) {
	this->fd = fd;
}

void Client::setAuthenticated( const bool value ) {
	this->authenticated = value;
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

void Client::setRegistered( const bool value ) {
	this->registered = value;
}

void Client::setIpAdd( const string &ipAdd ) {
	this->ipAdd = ipAdd;
}
