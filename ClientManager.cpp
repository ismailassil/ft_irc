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
	if ( str.length() != strlen( cmd ) ) return false;
	string tmp = str;
	for ( size_t i = 0; i < tmp.length(); i++ ) {
		if ( tolower( tmp[i] ) != cmd[i] ) return false;
	}
	return true;
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

void ClientManager::registerClient( int fd, string& input ) {
	static int			   id	  = 1;
	const vector< string > tokens = ft_split_tokens( input );

	if ( tokens.size() == 0 ) return;
	string cmd = tokens.at( 0 );
	if ( !isCmd( cmd, USER ) && !isCmd( cmd, NICK ) && !isCmd( cmd, PASS ) )
		return ft_send( fd, ERR_NOTREGISTERED( string( "*" ) ) );
	if ( isCmd( cmd, PASS ) ) {
		if ( cli[fd].getAuthenticated() )
			return ft_send( fd, ERR_ALREADYREGISTERED( cli[fd].getNickName() ) );
		if ( tokens.size() == 1 )
			return ft_send( fd, ERR_NEEDMOREPARAMS( string( "*" ) ) );
		if ( tokens.size() > 2 || tokens.at( 1 ) != pass ) {
			return ft_send( fd, ERR_PASSWDMISMATCH( string( "*" ) ) );
		}
		cli[fd].setAuthenticated( true );
	}
	if ( isCmd( cmd, NICK ) ) {
		if ( !cli[fd].getAuthenticated() )
			return ft_send( fd, ERR_NOTREGISTERED( string( "*" ) ) );
		if ( tokens.size() == 1 )
			return ft_send( fd, ERR_NEEDMOREPARAMS( string( "*" ) ) );
		if ( tokens.size() > 2 ) {
			string erro;
			for ( vector< string >::const_iterator it = tokens.begin() + 1; it != tokens.end(); it++ )
				erro.append( *it + " " );
			return ft_send( fd, ERR_ERRONEUSNICKNAME( erro ) );
		}
		string nk = tokens.at( 1 );
		if ( !isValid( nk ) )
			return ft_send( fd, ERR_ERRONEUSNICKNAME( nk ) );
		for ( map< int, Client >::iterator it = cli.begin(); it != cli.end(); it++ ) {
			if ( it->second.getFd() != fd && it->second.getNickName() == nk )
				return ft_send( fd, ERR_NICKNAMEINUSE( nk ) );
		}
		if ( cli[fd].getNickName() == nk )
			return ;
		if ( !cli[fd].getNickName().empty() )
			ft_send( fd, ":" + cli[fd].getNickName() + " NICK " + nk + "\n" );
		cli[fd].setNickname( nk );
	}
	if ( isCmd( cmd, USER ) ) {
		if ( !cli[fd].getAuthenticated() )
			return ft_send( fd, ERR_NOTREGISTERED( string( "*" ) ) );
		if ( !cli[fd].getUserName().empty() )
			return ft_send( fd, ERR_ALREADYREGISTERED( cli[fd].getNickName() ) );
		if ( tokens.size() < 5 )
			return ft_send( fd, ERR_NEEDMOREPARAMS( string( "*" ) ) );

		string username = tokens.at( 0 );
		if ( username.length() > 10 )
			username = tokens.at( 0 ).substr( 0, 10 );
		cli[fd].setUsername( username );
	}
	if ( !cli[fd].getNickName().empty() && !cli[fd].getUserName().empty() ) {
		cli[fd].setKey( id++ );
		cli[fd].setRegistered( true );

		std::stringstream ss;
		ss << setw( 3 ) << setfill( '0' ) << cli[fd].getKey();
		ft_send( fd, RPL_CONNECTED( ss.str(), cli[fd].getNickName() ) );
	}
}

void ClientManager::setPass( const string& p ) {
	pass = p;
}

// void ClientManager::parse( int fd, string& input ) {
// 	size_t pos = input.find( "\n" );
// 	if ( pos != string::npos )
// 		input.erase( pos, 1 );
// 	if ( input.empty() ) return;

// 	if ( cli.find( fd ) == cli.end() ) {
// 		cli[fd] = Client();
// 	}

// 	if ( !cli[fd].getRegistered() )
// 		if ( !registerClient( fd, input ) )
// 			return;
// 	// check if join cmd
// 	const vector< string > tokens = ft_split_tokens( input );

// 	if ( tokens.size() == 0 ) return;
// 	string cmd = tokens.at( 0 );
// 	cout << "Command: " << cmd << std::endl;
// 	if ( cmd == "join" )
// 		joinCmd( fd, input );
// 	cout << CYAN << "============================================================" << RESET << endl;
// 	cout << GREEN << "Clients: " << RESET << endl;
// 	for ( map< int, Client >::const_iterator it = cli.begin(); it != cli.end(); it++ ) {
// 		cout << YELLOW << "\tClient <" << it->first << "> " << RESET;
// 		cout << MAGENTA << "\tNickname: " << it->second.getNickName() << RESET << endl;
// 	}
// 	cout << GREEN << "Channels: " << RESET << endl;
// 	for ( vector< Channel >::const_iterator it = channels.begin(); it != channels.end(); it++ ) {
// 		cout << BLUE << "\tName: " << it->getName() << RESET << endl;
// 		if ( !it->getPassword().empty() )
// 			cout << YELLOW << "\tPassword: " << it->getPassword() << RESET << endl;
// 		cout << "\tMembers: " << RESET;
// 		cout << it->getClientChannelList() << endl;
// 		cout << "\tAdmins: " << RESET;
// 		for ( int i = 0; i < it->getNumberOfClients(); i++ ) {
// 			if ( it->getAdmin( it->getClient( i )->getFd() ) )
// 				cout << it->getClient( i )->getNickName() << " ";
// 		}
// 	}
// 	cout << RED << "Current Client: " << fd << RESET << endl;
// 	cout << CYAN << "============================================================" << RESET << endl;
// 	// if ( rNewLine( input ) ) return;
// }

bool ClientManager::rNewLine( string& input ) {
	if ( input.empty() ) return true;

	size_t pos = input.find( "\r\n" );
	if ( pos != string::npos ) {
		input.erase( pos );
	}

	size_t nPos = input.find_last_of( "\n" );
	if ( nPos != string::npos )
		input.erase( nPos, 1 );
	return false;
}

void ClientManager::parse( int fd, string& input ) {
	if ( input.empty() ) return;

	if ( rNewLine( input ) ) return;

	size_t isExist = cli.find( fd ) != cli.end();
	if ( !isExist ) {
		cli[fd] = Client();
		cli[fd].setFd(fd);
	}

	string buffer = cli[fd].getBuffer();

	size_t pos = input.find_first_of( "\t\v" );
	if ( pos != string::npos ) {
		input.erase( pos );
		return;
	}
	buffer.append( input );
	cli[fd].setBuffer( buffer );
	
	//////////////////////////////////////////////////////////////////////
	/////////////////////////// Search for cmd ///////////////////////////
	const char* cmdList[] = { NICK, USER, PASS, QUIT, JOIN, KICK };
	const vector< string > tokens = ft_split_tokens( input );
	if ( tokens.size() == 0 ) return;

	string cmd = tokens.at( 0 );
	transform( input.begin(), input.end(), cmd.begin(), static_cast< int ( * )( int ) >( tolower ) );
	
	int len = sizeof(cmdList) / sizeof(cmdList[0]);
	bool found = false;
	for ( int i = 0; i < len; i++) {
		if (isCmd(cmd, cmdList[i])) {
			found = true;
			break ;
		}
	}
	if ( !found )
		return ft_send( fd, ERR_UNKNOWNCOMMAND( string( "*" ), cmd ) );
	//////////////////////////////////////////////////////////////////////

	if ( !cli[fd].getRegistered() ) {
		registerClient( fd, buffer );
		cli[fd].setBuffer( "" );
		return;
	}

	void ( ClientManager::* func[] )( int, string& ) = {
		&ClientManager::nickCmd,
		&ClientManager::quitCmd,
		&ClientManager::joinCmd,
		&ClientManager::kickCmd,
	};

	if ( isCmd( cmd, USER ) || isCmd( cmd, PASS ) )
		return ft_send( fd, ERR_ALREADYREGISTERED( cli[fd].getNickName() ) );

	for ( size_t i = 0; i < sizeof( func ) / sizeof( func[0] ); i++ ) {
		if ( isCmd( cmd, cmdList[i] ) ) {
			found = true;
			( this->*func[i] )( fd, input );
			cli[fd].setBuffer( "" );
			return;
		}
	}
}
