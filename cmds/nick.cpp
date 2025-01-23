#include "../headers/ClientManager.hpp"

void ClientManager::nickCmd( int fd, string& input ) {
	const vector< string > tokens = ft_split_tokens( input );

	if ( tokens.size() > 2 ) {
		string erro;
		for ( vector< string >::const_iterator it = tokens.begin() + 1; it != tokens.end(); it++ )
			erro.append( *it + " " );
		return ft_send( fd, ERR_ERRONEUSNICKNAME( erro ) );
	}
	if ( tokens.size() != 2 )
		return ft_send( fd, ERR_NEEDMOREPARAMS( cli[fd].getNickName() ) );
	string nick = tokens.at( 1 );
	if ( !isValid( nick ) )
		return ft_send( fd, ERR_ERRONEUSNICKNAME( nick ) );
	for ( map< int, Client >::iterator it = cli.begin(); it != cli.end(); it++ ) {
		if ( it->second.getFd() != fd && it->second.getNickName() == nick )
			return ft_send( fd, ERR_NICKNAMEINUSE( nick ) );
	}
	if ( cli[fd].getNickName() == nick )
		return;
	if ( !cli[fd].getNickName().empty() )
		ft_send( fd, ":" + cli[fd].getNickName() + " NICK " + nick + CRLF );
	for ( vector< Channel >::iterator it = channels.begin(); it != channels.end(); it++ ) {
		if ( it->isInChannel( cli[fd].getNickName() ) ) {
			Client* client = it->getClient( cli[fd].getNickName() );
			if ( !client )
				client->setNickname( nick );
			if ( it->isAdminInChannel( cli[fd].getNickName() ) ) {
				Client* admin = it->getAdmin( cli[fd].getNickName() );
				if ( !admin )
					admin->setNickname( nick );
			}
			if ( topicAuthor == cli[fd].getNickName() )
				setTopicAuthor( nick );
			it->broadcast( ":" + getPrefix( fd ) + " NICK " + nick + CRLF, fd );
		}
	}
	cli[fd].setNickname( nick );
}
