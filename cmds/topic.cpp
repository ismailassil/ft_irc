#include "../headers/ClientManager.hpp"

void ClientManager::topicCmd( int fd, string& input ) {
	const vector< string > tokens = ft_split_tokens( input );

	if ( tokens.size() < 2 ) {
		return ft_send( fd, ERR_NEEDMOREPARAMS( string( "*" ) ) );
	}
	string channelName = tokens.at( 1 );
	if ( channelName[0] != '#' && channelName[0] != '&' ) {
		return ft_send( fd, ERR_NOSUCHCHANNEL( string( "*" ), channelName ) );
	}
	channelName = channelName.erase( 0, 1 );
	for ( vector< Channel >::iterator it = channels.begin(); it != channels.end(); it++ ) {
		if ( it->getName() == channelName ) {
			if ( it->isAdminInChannel( cli[fd].getNickName() ) ) {
				if ( tokens.size() == 2 ) {
					return ft_send( fd, RPL_TOPICIS( cli[fd].getNickName(), channelName, it->getTopic() ) );
				} else {
					if ( it->getModes().find( 't' ) != string::npos ) {
						return ft_send( fd, ERR_CHANOPRIVSNEEDED( cli[fd].getNickName(), channelName ) );
					}
					string topic;
					bool   found = false;
					if ( tokens.at( 2 )[0] == ':' ) {
						found = true;
						topic = tokens.at( 2 ).substr( 1 );
					}
					for ( vector< string >::const_iterator it = tokens.begin() + 3; found && it != tokens.end(); it++ ) {
						topic.append( *it + " " );
					}
					it->setTopic( topic );
					return it->broadcast( RPL_TOPIC( cli[fd].getNickName(), channelName, topic ), fd );
				}
			} else {
				return ft_send( fd, ERR_CHANOPRIVSNEEDED( cli[fd].getNickName(), channelName ) );
			}
			return;
		}
	}
	return ft_send( fd, ERR_NOSUCHCHANNEL( string( "*" ), channelName ) );
}
