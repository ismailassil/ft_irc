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
			input.erase( 0, input.find( "nick" ) + 4 );
			erro = input.substr( input.find( tokens.at( 1 ) ) );
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
			ft_send( fd, ":" + cli[fd].getNickName() + " NICK " + nk + CRLF );
		cli[fd].setNickname( nk );
	}
	if ( isCmd( cmd, USER ) ) {
		if ( !cli[fd].getAuthenticated() )
			return ft_send( fd, ERR_NOTREGISTERED( string( "*" ) ) );
		if ( !cli[fd].getUserName().empty() )
			return ft_send( fd, ERR_ALREADYREGISTERED( ( cli[fd].getNickName().empty() ? string( "*" ) : cli[fd].getNickName() ) ) );
		if ( tokens.size() < 5 )
			return ft_send( fd, ERR_NEEDMOREPARAMS( ( cli[fd].getNickName().empty() ? string( "*" ) : cli[fd].getNickName() ) ) );

		string username = tokens.at( 1 );
		if ( username.length() > 10 )
			username = tokens.at( 0 ).substr( 0, 10 );
		cli[fd].setUsername( username );
	}
	if ( !cli[fd].getNickName().empty() && !cli[fd].getUserName().empty() ) {
		cli[fd].setRegistered( true );
		ft_send( fd, RPL_WELCOME( cli[fd].getNickName() ) );
	}
}

bool ClientManager::checkWhiteSpaces( int fd, string& input ) {
	if ( input.empty() ) return true;

	cli[fd].setBuffer( cli[fd].getBuffer() + input );
	string buffer = cli[fd].getBuffer();

	if ( buffer.find( CRLF ) == string::npos && buffer.find( LF ) == string::npos ) {
		return true;
	}

	if ( hasOnlyWhitespaces( buffer ) ) {
		cli[fd].setBuffer( "" );
		return true;
	}

	size_t pos = buffer.find( CRLF );
	if ( pos != string::npos ) {
		buffer.erase( pos );
	}

	size_t nPos = buffer.find_last_of( LF );
	if ( nPos != string::npos )
		buffer.erase( nPos, 1 );

	if ( buffer.empty() ) {
		return true;
	}

	cli[fd].setBuffer( buffer );

	return false;
}

void ClientManager::parse( int fd, string& input ) {
	if ( checkWhiteSpaces( fd, input ) ) return;

	string buffer = cli[fd].getBuffer();
	//////////////////////////////////////////////////////////////////////
	//////////////////////////// Check for cmd ///////////////////////////
	//////////////////////////////////////////////////////////////////////
	const char*			   cmdList[] = { PONG, QUIT, PART, NICK, JOIN, TOPIC,
										 MODE, PRIVMSG, KICK, INVITE, PING, USER, PASS };
	const vector< string > tokens	 = ft_split_tokens( buffer );
	if ( tokens.size() == 0 ) {
		cli[fd].setBuffer( "" );
		return;
	}

	string cmd = tokens.at( 0 );
	transform( cmd.begin(), cmd.end(), cmd.begin(), static_cast< int ( * )( int ) >( tolower ) );

	int	 len   = sizeof( cmdList ) / sizeof( cmdList[0] );
	bool found = false;
	for ( int i = 0; i < len; i++ ) {
		if ( isCmd( cmd, cmdList[i] ) ) {
			found = true;
			break;
		}
	}
	if ( !found ) {
		cli[fd].setBuffer( "" );
		return ft_send( fd, ERR_UNKNOWNCOMMAND( ( cli[fd].getNickName().empty() ? string( "*" ) : cli[fd].getNickName() ), cmd ) );
	}
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////

	if ( !cli[fd].getRegistered() ) {
		registerClient( fd, buffer );
		cli[fd].setBuffer( "" );
		return;
	}

	void ( ClientManager::* func[] )( int, string& ) = {
		&ClientManager::pongCmd,
		&ClientManager::quitCmd,
		&ClientManager::partCmd,
		&ClientManager::nickCmd,
		&ClientManager::joinCmd,
		&ClientManager::topicCmd,
		&ClientManager::modeCmd,
		&ClientManager::privmsgCmd,
		&ClientManager::kickCmd,
		&ClientManager::inviteCmd,
		&ClientManager::pingCmd };

	if ( isCmd( cmd, USER ) || isCmd( cmd, PASS ) ) {
		cli[fd].setBuffer( "" );
		return ft_send( fd, ERR_ALREADYREGISTERED( ( cli[fd].getNickName().empty() ? string( "*" ) : cli[fd].getNickName() ) ) );
	}

	for ( size_t i = 0; i < sizeof( func ) / sizeof( func[0] ); i++ ) {
		if ( isCmd( cmd, cmdList[i] ) ) {
			( this->*func[i] )( fd, buffer );
			if ( cli.find( fd ) == cli.end() ) return;
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
	if ( !isExist ) return;

	for ( vector< Channel >::iterator it = channels.begin(); it != channels.end(); ) {
		Channel& channel = *it;
		if ( channel.isInChannel( cli[fd].getNickName() ) ) {
			channel.removeClient( fd );
			if ( channel.isAdminInChannel( cli[fd].getNickName() ) )
				channel.removeAdmin( fd );
			if ( channel.getNumberOfClients() == 0 ) {
				channels.erase( it );
			} else {
				it++;
			}
		} else {
			it++;
		}
	}
	for ( map< int, Client >::iterator it = cli.begin(); it != cli.end(); it++ ) {
		if ( it->second.getFd() != fd && it->second.getFriends().size() > 0 ) {
			it->second.removeFriend( cli[fd].getNickName() );
		}
	}
	cli.erase( fd );
}

void ClientManager::addNewClient( int fd, struct in_addr ip ) {
	if ( cli.find( fd ) != cli.end() ) return;
	cli[fd] = Client( fd, inet_ntoa( ip ) );
}

string ClientManager::trim( const string& str ) {
	string::size_type start = str.find_first_not_of( " \t\n\r\f\v" );
	if ( start == string::npos )
		return "";

	string::size_type end = str.find_last_not_of( " \t\n\r\f\v" );

	return str.substr( start, end - start + 1 );
}
