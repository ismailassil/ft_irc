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
					if ( it->getTopic().empty() ) {
						return ft_send( fd, ERR_NOTOPIC( cli[fd].getNickName(), channelName ) );
					}
					ft_send( fd, RPL_TOPIC( cli[fd].getNickName(), channelName, it->getTopic() ) );
					ft_send( fd, RPL_TOPICWHOTIME( cli[fd].getNickName(), channelName, it->getTopicAuthor(), it->getTopicDate() ) );
					return;
				} else {
					if ( it->getTopicRestrict() && !it->isAdminInChannel( cli[fd].getNickName() ) ) {
						return ft_send( fd, ERR_CHANOPRIVSNEEDED( cli[fd].getNickName(), channelName ) );
					}
					string topic = getText( input, tokens, 2 );
					it->setTopic( topic );
					it->setTopicAuthor( cli[fd].getNickName() );
					it->setTopicDate( getTimestamp() );
					it->broadcast( RPL_TOPIC( getPrefix( fd ), channelName, topic ) );
					return;
				}
			}
			return;
		}
	}
	return ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), channelName ) );
}
