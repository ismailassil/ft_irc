#include "../headers/ClientManager.hpp"

bool ClientManager::isChannel( const string& channel ) {
	string channelName = channel.substr( 1 );
	if ( channel.empty() || ( channel[0] != '#' && channel[0] != '&' ) ) {
		return false;
	}
	for ( vector< Channel >::const_iterator it = channels.begin(); it != channels.end(); it++ ) {
		if ( it->getName() == channelName ) {
			return true;
		}
	}
	return false;
}

Channel* ClientManager::findChannelByName( const vector< Channel >& channels, const string& name ) {
	for ( size_t i = 0; i < channels.size(); ++i ) {
		if ( channels[i].getName() == name ) {
			return (Channel*)&channels[i];
		}
	}
	return NULL;
}

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
			return ft_send( fd, ERR_NEEDMOREPARAMS( ( cli[fd].getNickName().empty() ? string( "*" ) : cli[fd].getNickName() ) ) );
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
			return;
		if ( !cli[fd].getNickName().empty() )
			ft_send( fd, ":" + cli[fd].getNickName() + " NICK " + nk + "\n" );
		cli[fd].setNickname( nk );
	}
	if ( isCmd( cmd, USER ) ) {
		if ( !cli[fd].getAuthenticated() )
			return ft_send( fd, ERR_NOTREGISTERED( string( "*" ) ) );
		if ( !cli[fd].getUserName().empty() )
			return ft_send( fd, ERR_ALREADYREGISTERED( ( cli[fd].getNickName().empty() ? string( "*" ) : cli[fd].getNickName() ) ) );
		if ( tokens.size() < 5 )
			return ft_send( fd, ERR_NEEDMOREPARAMS( ( cli[fd].getNickName().empty() ? string( "*" ) : cli[fd].getNickName() ) ) );

		string username = tokens.at( 0 );
		if ( username.length() > 10 )
			username = tokens.at( 0 ).substr( 0, 10 );
		cli[fd].setUsername( username );
	}
	if ( !cli[fd].getNickName().empty() && !cli[fd].getUserName().empty() ) {
		cli[fd].setRegistered( true );
		ft_send( fd, RPL_CONNECTED( cli[fd].getNickName() ) );
	}
}

bool ClientManager::removeWhiteSpace( string& input ) {
	if ( input.empty() ) return true;

	size_t pos = input.find( CRLF );
	if ( pos != string::npos ) {
		input.erase( pos );
	}

	size_t nPos = input.find_last_of( LF );
	if ( nPos != string::npos )
		input.erase( nPos, 1 );

	return false;
}

void ClientManager::parse( int fd, string& input ) {
	if ( input.empty() ) return;

	if ( removeWhiteSpace( input ) ) return;

	// size_t isExist = cli.find( fd ) != cli.end();
	// if ( !isExist ) {
	// 	cli[fd] = Client();
	// 	cli[fd].setFd( fd );
	// }

	string buffer = cli[fd].getBuffer();

	size_t pos = input.find_first_of( "\t\v" );
	if ( pos != string::npos ) {
		input.erase( pos );
		return;
	}
	buffer.append( input );
	cli[fd].setBuffer( buffer );

	//////////////////////////////////////////////////////////////////////
	//////////////////////////// Check for cmd ///////////////////////////
	//////////////////////////////////////////////////////////////////////
	const char*			   cmdList[] = { NICK, USER, PASS, QUIT, JOIN, KICK,
										 PART, TOPIC, MODE, PRIVMSG, INVITE };
	const vector< string > tokens	 = ft_split_tokens( input );
	if ( tokens.size() == 0 ) return;

	string cmd = tokens.at( 0 );
	transform( input.begin(), input.end(), cmd.begin(), static_cast< int ( * )( int ) >( tolower ) );

	int	 len   = sizeof( cmdList ) / sizeof( cmdList[0] );
	bool found = false;
	for ( int i = 0; i < len; i++ ) {
		if ( isCmd( cmd, cmdList[i] ) ) {
			found = true;
			break;
		}
	}
	if ( !found )
		return ft_send( fd, ERR_UNKNOWNCOMMAND( string( "*" ), cmd ) );
	//////////////////////////////////////////////////////////////////////
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
		&ClientManager::partCmd,
		&ClientManager::topicCmd,
		&ClientManager::modeCmd,
		&ClientManager::privmsgCmd,
		&ClientManager::inviteCmd };

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

void ClientManager::setPass( const string& p ) {
	pass = p;
}

const string ClientManager::getPass() const {
	return pass;
}

Client* ClientManager::getClient( const string& nick ) {
	for ( map< int, Client >::iterator it = cli.begin(); it != cli.end(); it++ ) {
		if ( it->second.getNickName() == nick )
			return &it->second;
	}
	return NULL;
}

Channel* ClientManager::getChannel( const string& name ) {
	for ( vector< Channel >::iterator it = channels.begin(); it != channels.end(); it++ ) {
		Channel& channel = *it;
		if ( channel.getName() == name )
			return &channel;
	}
	return NULL;
}

const string ClientManager::getPrefix( int fd ) {
	return cli[fd].getNickName() + "!" + cli[fd].getUserName() + "@" + cli[fd].getIpAdd();
}

void ClientManager::removeClient( int fd ) {
	int isExist = cli.find( fd ) != cli.end();
	if ( isExist ) {
		cli.erase( fd );
	}
	for ( vector< Channel >::iterator it = channels.begin(); it != channels.end(); it++ ) {
		Channel& channel = *it;
		if ( channel.isInChannel( cli[fd].getNickName() ) ) {
			if ( channel.isAdminInChannel( cli[fd].getNickName() ) )
				channel.removeAdmin( fd );
			else
				channel.removeClient( fd );
		}
	}
}

void ClientManager::addNewClient( int fd, struct in_addr ip ) {
	size_t isExist = cli.find( fd ) != cli.end();
	if ( !isExist ) {
		cli[fd] = Client();
		cli[fd].setFd( fd );
		cli[fd].setIpAdd( inet_ntoa( ip ) );
	}
}
