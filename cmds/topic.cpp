#include "../headers/ClientManager.hpp"

void ClientManager::topicCmd( int fd, string& input ) {
	const vector< string > tokens = ft_split_tokens( input );

	if ( tokens.size() < 2 ) {
		return ft_send( fd, ERR_NEEDMOREPARAMS( cli[fd].getNickName() ) );
	}
	string channelName = tokens.at( 1 );
	if ( channelName[0] != '#' && channelName[0] != '&' ) {
		return ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), channelName ) );
	}
	channelName.erase( 0, 1 );
	for ( vector< Channel >::iterator it = channels.begin(); it != channels.end(); it++ ) {
		if ( it->getName() == channelName ) {
			if ( it->isInChannel( cli[fd].getNickName() ) ) {
				if ( tokens.size() == 2 ) {
					return ft_send( fd, RPL_TOPICIS( cli[fd].getNickName(), channelName, it->getTopic() ) );
				} else {
					if ( it->getTopicRestrict() && !it->isAdminInChannel( cli[fd].getNickName() ) ) {
						return ft_send( fd, ERR_CHANOPRIVSNEEDED( cli[fd].getNickName(), channelName ) );
					}
					string topic = getText( input, tokens, 2 );
					it->setTopic( topic );
					return it->broadcast( RPL_TOPIC( cli[fd].getNickName(), channelName, topic ) );
				}
			}
			return;
		}
	}
	return ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), channelName ) );
}
