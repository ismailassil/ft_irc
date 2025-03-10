#include "../headers/ClientManager.hpp"

void ClientManager::nickCmd( int fd, string& input ) {
	const vector< string > tokens = ft_split_tokens( input );

	if ( tokens.size() != 2 || tokens.at(1) == ":" ) {
		if ( tokens.size() > 2 ) {
			string erro;
			input.erase( 0, input.find( "nick" ) + 4 );
			erro = input.substr( input.find( tokens.at( 1 ) ) );
			return ft_send( fd, ERR_ERRONEUSNICKNAME( erro ) );
		}
		return ft_send( fd, ERR_NONICKNAMEGIVEN( cli[fd].getNickName() ) );
	}
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
			it->changeMemberNick( cli[fd].getNickName(), nick );
			if ( it->getTopicAuthor() == cli[fd].getNickName() )
				it->setTopicAuthor( nick );
			it->broadcast( RPL_NICKCHANGE( getPrefix( fd ), nick ), fd );
		}
	}
	vector< string > friends = cli[fd].getFriends();
	for ( vector< string >::iterator it = friends.begin(); it != friends.end(); it++ ) {
		const Client* client = getClient( *it );
		if ( client )
			ft_send( client->getFd(), RPL_NICKCHANGE( getPrefix( fd ), nick ) );
	}
	for ( map< int, Client >::iterator it = cli.begin(); it != cli.end(); it++ ) {
		if ( it->second.getFd() != fd && it->second.getFriends().size() > 0 ) {
			it->second.setFriendNickName( cli[fd].getNickName(), nick );
		}
	}
	cli[fd].setNickname( nick );
}
