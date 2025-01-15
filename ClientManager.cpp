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

void ClientManager::ft_send( int fd, const string& str ) {
	if ( send( fd, str.c_str(), str.size(), 0 ) == -1 )
		cerr << "send() failed" << endl;
}

bool ClientManager::rNewLine( string& input ) {
	size_t pos = input.find( "\r\n" );
	if ( pos == string::npos ) {
		return false;
		// return true;
	}
	input.erase( 0, pos );
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
	static int id = 1;
	if ( cli[fd].getRegistered() )
		return true;
	const vector< string > tokens = ft_split_tokens( input );

	if ( tokens.size() == 0 ) return false;
	string cmd = tokens.at( 0 );
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
		if ( tokens.size() < 5 )
			return ft_send( fd, ERR_NEEDMOREPARAMS( string( "*" ) ) ), false;

		string username = tokens.at( 0 );
		if ( username.length() > 10 )
			username = tokens.at( 0 ).substr( 0, 10 );
		cli[fd].setUsername( username );
	}
	if ( !cli[fd].getNickName().empty() && !cli[fd].getUserName().empty() ) {
		cli[fd].setKey(id++);
		cli[fd].setRegistered( true );

		std::stringstream ss;
		ss << setw(3) << setfill('0') << cli[fd].getKey();
		ft_send( fd, RPL_CONNECTED( ss.str(), cli[fd].getNickName() ) );
		return true;
	}
	return false;
}

void ClientManager::setPass( const string& p ) {
	pass = p;
}

void displayInfo(const map<int, Client>& cli, const vector<Channel>& channels, int fd) {
    cout << CYAN << "============================================================" << RESET << endl;
    cout << GREEN << "Clients: " << RESET << endl;
    for (map<int, Client>::const_iterator it = cli.begin(); it != cli.end(); it++) {
        cout << YELLOW << "Client <" << it->first << "> " << RESET;
        cout << MAGENTA << "Nickname: " << it->second.getNickName() << RESET << endl;
    }
    cout << GREEN << "Channels: " << RESET << endl;
    for (vector<Channel>::const_iterator it = channels.begin(); it != channels.end(); it++) {
        cout << BLUE << "Name: " << it->getName() << RESET << endl;
		if (it->getPassword().empty())
			cout << "Password: " << "None" << endl;
		else
			cout << "Password: " << it->getPassword() << endl;
        cout << "Members: " << RESET << endl;
        cout << it->getClientChannelList() << endl;
    }
    cout << RED << "Current Client: " << fd << RESET << endl;
    cout << CYAN << "============================================================" << RESET << endl;
}

void ClientManager::parse( int fd, string& input ) {
	static int i = 1;
	if (i == 1) {
		Channel tmp;
		tmp.setName("test");
		tmp.setPassword("haha");
		channels.push_back(tmp);
		i = 0;
	}
	// if ( input.empty() ) return;

	// if ( rNewLine( input ) ) return;
	// size_t pos = input.find_first_of( "\t\v" );
	// if ( pos != string::npos ) {
	// 	input.erase( pos );
	// 	string buff = cli[fd].getBuffer();
	// 	buff.append( input );
	// 	cli[fd].setBuffer( buff );
	// 	return;
	// }

	// string buffer = cli[fd].getBuffer();

	// if ( cli[fd].getRegistered() )
	// 	if ( !registerClient( fd, buffer ) )
	// 		return;

	cli[fd].setNickname( "iassil" );
	cli[fd].setFd( fd );
	cli[fd].setIpAdd("");
	cli[fd].setUsername( "iassil" );
	void ( ClientManager::* func[] )( int, string& ) = {
		&ClientManager::nickCmd,
		&ClientManager::quitCmd,
		&ClientManager::joinCmd,
		&ClientManager::kickCmd,
	};

	const char* cmdList[] = { NICK, QUIT, JOIN, KICK };

	const vector< string > tokens = ft_split_tokens( input );
	if ( tokens.size() == 0 ) return;

	string cmd = tokens.at( 0 );
	transform( cmd.begin(), cmd.end(), cmd.begin(), static_cast< int ( * )( int ) >( tolower ) );

	// for ( size_t i = 0; i < sizeof( func ) / sizeof( func[0] ); i++ ) {
	// 	if ( isCmd( cmd, cmdList[i] ) ) {
	// 		( this->*func[i] )( fd, input );
	// 		return;
	// 	}
	// }
	if ( isCmd( cmd, JOIN ) ) {
		joinCmd( fd, input );
	}
	displayInfo( cli, channels, fd );

	/*
		- Remove CRNL and Check for \t\v

		if ( !rNewLine( input ) ) return;
		size_t pos = input.find_first_of( "\t\v" );
		if ( pos != string::npos ) {
			return ft_send( fd, ERR_NOSUCHCHANNEL( string( "*" ) ) );
		}
	*/
	size_t nPos = input.find( "\n" );
	input.erase( nPos, 1 );
	if ( input.empty() ) return;
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
			it->removeClient( fd );
			if ( it->getNumberOfClients() == 0 ) {
				channels.erase( it );
			} else {
				string reply = ":" + cli[fd].getNickName() + "!~" + cli[fd].getUserName() + "@" + cli[fd].getIpAdd() + " QUIT :" + reason;
				it->broadcast( reply, fd );
			}
		} else if ( it->getAdmin( fd ) ) {
			it->removeAdmin( fd );
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

Channel* ClientManager::findChannelByName(const vector<Channel>& channels, const string& name) {
    for (size_t i = 0; i < channels.size(); ++i) {
        if (channels[i].getName() == name) {
            return (Channel*)&channels[i];
        }
    }
    return NULL;
}