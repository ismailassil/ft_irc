/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.cpp                                        :+:      :+: :+:
 */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 16:43:15 by iassil            #+#    #+#             */
/*   Updated: 2025/01/13 20:15:57 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientManager.hpp"

bool ClientManager::isCmd( const string& str, const char* cmd ) {
	if ( str.length() != std::strlen( cmd ) ) return false;
	string tmp = str;
	for ( size_t i = 0; i < tmp.length(); i++ ) {
		if ( tolower( tmp[i] ) != cmd[i] ) return false;
	}
	return true;
}

void ClientManager::ft_send( int fd, const string& str ) {
	if ( send( fd, str.c_str(), str.size(), 0 ) == -1 )
		std::cerr << "send() failed" << std::endl;
}

bool ClientManager::rNewLine( string& input ) {
	size_t pos = input.find( "\r\n" );
	if ( pos != string::npos ) {
		input.erase( 0, pos );
		return true;
	}
	return false;
}

bool ClientManager::isValid( const string& str ) {
	if ( str.empty() || str[0] == '#' || str[0] == '&' || str[0] == ':' ||
		 isdigit( str[0] ) )
		return false;
	size_t pos = str.find_first_not_of(
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789{}[]|"
		"\\_" );
	if ( pos != string::npos ) return false;
	return true;
}

bool ClientManager::registerClient( int fd, string& input ) {
	if ( cli[fd].getRegistered() )
		return true;
	const vector< string > tokens = ft_split_tokens( input );

	if ( tokens.size() == 0 ) return false;
	string cmd = tokens.at( 0 );
	if ( !isCmd( cmd, PASS ) && !isCmd( cmd, NICK ) && !isCmd( cmd, USER ) ) {
		return ft_send( fd, ERR_UNKNOWNCOMMAND( string( "*" ), cmd.c_str() ) ), false;
	}
	if ( isCmd( cmd, PASS ) ) {
		if ( cli[fd].getAuthenticated() )
			return ft_send( fd, ERR_ALREADYREGISTERED( string( "" ) ) ), false;
		if ( tokens.size() == 1 )
			return ft_send( fd, ERR_NEEDMOREPARAMS( string( "*" ) ) ), false;
		if ( tokens.size() > 2 || tokens.at( 1 ) != pass ) {
			return ft_send( fd, ERR_PASSWDMISMATCH( string( "*" ) ) ), false;
		}
		cli[fd].setAuthenticated( true );
	}
	if ( isCmd( cmd, NICK ) ) {
		if ( !cli[fd].getAuthenticated() )
			return ft_send( fd, ERR_NOTREGISTERED( string( "*" ) ) ), false;
		if ( tokens.size() == 1 )
			return ft_send( fd, ERR_NEEDMOREPARAMS( string( "*" ) ) ), false;
		if ( tokens.size() > 2 ) {
			string erro;
			for ( vector< string >::const_iterator it = tokens.begin() + 1; it != tokens.end(); it++ )
				erro.append( *it + " " );
			return ft_send( fd, ERR_ERRONEUSNICKNAME( erro ) ), false;
		}
		string nk = tokens.at( 1 );
		if ( !isValid( nk ) )
			return ft_send( fd, ERR_ERRONEUSNICKNAME( nk ) ), false;
		for ( map< int, Client >::iterator it = cli.begin(); it != cli.end(); it++ ) {
			if ( it->second.getNickName() == nk )
				return ft_send( fd, ERR_NICKNAMEINUSE( nk ) ), false;
		}
		if ( !cli[fd].getNickName().empty() )
			ft_send( fd, ": " + cli[fd].getNickName() + " NICK " + nk );
		cli[fd].setNickname( nk );
	}
	if ( isCmd( cmd, USER ) ) {
		if ( !cli[fd].getAuthenticated() )
			return ft_send( fd, ERR_NOTREGISTERED( string( "*" ) ) ), false;
		if ( !cli[fd].getUserName().empty() )
			return ft_send( fd, ERR_ALREADYREGISTERED( string( "*" ) ) ), false;
		if ( tokens.size() < 4 )
			return ft_send( fd, ERR_NEEDMOREPARAMS( string( "*" ) ) ), false;
		size_t semiColonPos = input.find( ":" );
		if ( semiColonPos == string::npos && tokens.size() != 4 )
			return ft_send( fd, ERR_NEEDMOREPARAMS( string( "*" ) ) ), false;

		string username = tokens.at( 0 );
		if ( username.length() > 10 )
			username = tokens.at( 0 ).substr( 0, 10 );
		cli[fd].setUsername( username );
	}
	if ( !cli[fd].getNickName().empty() && !cli[fd].getUserName().empty() )
		cli[fd].setRegistered( true );
	return false;
}

void ClientManager::setPass( const string& p ) {
	// Remove CRNL and Check for \t\v i
	// if ( !rNewLine( input ) ) return;
	// size_t pos = input.find_first_of( "\t\v" );
	// if ( pos != string::npos ) {
	// 	return ft_send( fd, ERR_NOSUCHCHANNEL( string( "*" ) ) );
	// }
	pass = p;
}

void ClientManager::parse( int fd, string& input ) {

	cout << "parse: " << input << std::endl;

	// Remove CRNL and Check for \t\v
	// if ( !rNewLine( input ) ) return;
	// size_t pos = input.find_first_of( "\t\v" );
	// if ( pos != string::npos ) {
	// 	return ft_send( fd, ERR_NOSUCHCHANNEL( string( "*" ) ) );
	// }
	
	size_t pos = input.find( "\n" );
	input.erase( pos, 1 );
	if ( input.empty() ) return;

	if (!registerClient( fd, input ))
		return ;
	cout << "Client <" << fd << "> Nickname: " << cli[fd].getNickName() << std::endl;
	joinCmd(fd, input);
	// if ( rNewLine( input ) ) return;
}

void ClientManager::nickCmd( int fd, string& input ) {
	const vector< string > tokens = ft_split_tokens( input );

	if ( tokens.size() > 2 ) {
		string erro;
		for ( vector< string >::const_iterator it = tokens.begin() + 1; it != tokens.end(); it++ )
			erro.append( *it + " " );
		return ft_send( fd, ERR_ERRONEUSNICKNAME( erro ) );
	}
	if ( tokens.size() == 2 )
		return ft_send( fd, ERR_NEEDMOREPARAMS( string( "*" ) ) );
	string nick = tokens.at( 1 );
	if ( !isValid( nick ) )
		return ft_send( fd, ERR_ERRONEUSNICKNAME( nick ) );
	for ( map< int, Client >::iterator it = cli.begin(); it != cli.end(); it++ ) {
		if ( it->second.getNickName() == nick )
			return ft_send( fd, ERR_NICKNAMEINUSE( nick ) );
	}
	cli[fd].setNickname( nick );
}

void ClientManager::quitCmd( int fd, string& input ) {
	const vector< string > tokens = ft_split_tokens( input );

	string reason = input;
	size_t pos	  = reason.find( tokens.at( 0 ) );
	if ( pos != string::npos ) {
		reason.erase( pos, tokens.at( 0 ).length() );
		reason = "Quit: " + reason;
	}

	for ( vector< Channel >::iterator it = channels.begin(); it != channels.end(); it++ ) {
		if ( it->getClient( fd ) ) {
			it->remove_client( fd );
			if ( it->getNumberOfClients() == 0 ) {
				channels.erase( it );
			} else {
				string reply = ":" + cli[fd].getNickName() + "!~" + cli[fd].getUserName() + "@" + cli[fd].getIpAdd() + " QUIT :" + reason;
				it->broadcast( reply, fd );
			}
		} else if ( it->getAdmin( fd ) ) {
			it->remove_admin( fd );
			if ( it->getNumberOfClients() == 0 ) {
				channels.erase( it );
			} else {
				string reply = ":" + cli[fd].getNickName() + "!~" + cli[fd].getUserName() + "@" + cli[fd].getIpAdd() + " QUIT :" + reason;
				it->broadcast( reply, fd );
			}
		}
	}
	cli.erase( fd );								  // Remove client from the Server
	fds.erase( find( fds.begin(), fds.end(), fd ) );  // Remove the fd from the fd_set
	close( fd );									  // Close the connection
	cout << RED << "Client <" << fd << "> Disconnected" << RESET << endl;
}
