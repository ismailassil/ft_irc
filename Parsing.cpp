/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 16:43:15 by iassil            #+#    #+#             */
/*   Updated: 2025/01/13 18:35:33 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"
#include <string>

vector< userInfo > Parsing::users;
vector< string >   Parsing::nicknames;

bool Parsing::isCmd( const string& str, const char* cmd ) {
	if ( str.length() != std::strlen( cmd ) ) return false;
	string tmp = str;
	for ( size_t i = 0; i < tmp.length(); i++ ) {
		if ( tolower( tmp[i] ) != cmd[i] ) return false;
	}
	return true;
}

void Parsing::ft_send( int fd, const string& str, int flag ) {
	static map< int, string > flagMap;
	if ( flagMap.empty() ) {
		flagMap[NONE]				   = "\n";
		flagMap[ERR_PASSWDMISMATCH]	   = " :Password incorrect\n";
		flagMap[ERR_NEEDMOREPARAMS]	   = " :Not enough parameters\n";
		flagMap[ERR_ALREADYREGISTERED] = " :You may not reregister\n";
		flagMap[ERR_UNKNOWNCOMMAND]	   = " :Unknown command\n";
		flagMap[ERR_NOTREGISTERED]	   = " :You have not registered\n";
		flagMap[ERR_ERRONEUSNICKNAME]  = " :Erroneus nickname\n";
		flagMap[ERR_NICKNAMEINUSE]	   = " :Nickname is already in use\n";
	}
	if ( flagMap.find( flag ) != flagMap.end() ) {
		string toSend = str + std::to_string(flag) + flagMap[flag];
		if ( send( fd, toSend.c_str(), toSend.size(), 0 ) == -1 )
			std::cerr << "send() failed" << std::endl;
	}
}

bool Parsing::rNewLine( string& input ) {
	size_t pos = input.find( "\n" );
	if ( pos == string::npos ) return true;
	input.erase( 0, pos );
	return false;
}

bool Parsing::isValid( const string& str ) {
	if ( str[0] == '#' || str[0] == '&' || str[0] == ':' || isdigit( str[0] ) )
		return false;
	size_t pos = str.find_first_not_of(
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789{}[]|"
		"\\" );
	if ( pos != string::npos ) return false;
	return true;
}

bool Parsing::registerClient( int fd, string& input ) {
	if ( rNewLine( input ) ) return false;
	istringstream	 stream( input );
	vector< string > tokens;
	string			 token;

	while ( stream >> token ) tokens.push_back( token );
	if ( tokens.size() == 0 ) return false;
	string cmd = tokens.at( 0 );
	if ( !isCmd( cmd, PASS ) && !isCmd( cmd, NICK ) && !isCmd( cmd, USER ) ) {
		return ft_send( fd, cmd, ERR_UNKNOWNCOMMAND ), false;
	}
	if ( isCmd( cmd, PASS ) ) {
		if ( isDone == 1 )
			return ft_send( fd, "", ERR_ALREADYREGISTERED ), false;
		if ( tokens.size() == 1 )
			return ft_send( fd, cmd, ERR_NEEDMOREPARAMS ), false;
		if ( tokens.size() > 2 || tokens.at( 1 ) != pass )
			return ft_send( fd, "", ERR_PASSWDMISMATCH ), false;
		isDone = 1;
	}
	if ( isCmd( cmd, NICK ) ) {
		if ( isDone == 0 ) return ft_send( fd, "", ERR_NOTREGISTERED ), false;
		if ( user.nickname.empty() )
			user.nickname = tokens.at( 1 );
		else {
			ft_send( fd, ": " + user.nickname + " NICK " + tokens.at( 1 ),
					 NONE );
			user.nickname = tokens.at( 1 );
		}
		if ( !isValid( tokens.at( 1 ) ) )
			return ft_send( fd, user.nickname, ERR_ERRONEUSNICKNAME ), false;
		if ( find( nicknames.begin(), nicknames.end(), user.nickname ) !=
			 nicknames.end() )
			return ft_send( fd, user.nickname, ERR_NICKNAMEINUSE ), false;
		isDone = 2;
	}
	if ( isCmd( cmd, USER ) ) {
		if ( isDone == 0 ) return ft_send( fd, "", ERR_NOTREGISTERED ), false;
	}
	if ( isDone == 3 ) {
		users.push_back( user ), nicknames.push_back( user.nickname );
		return true;
	}
	cout << "OK" << endl;
	return false;
}

void Parsing::setPass( const string& p ) { pass = p; }

void Parsing::parse( string& input ) {
	if ( rNewLine( input ) ) return;
	istringstream	 stream( input );
	vector< string > tokens;
	string			 token;

	while ( stream >> token ) tokens.push_back( token );
}