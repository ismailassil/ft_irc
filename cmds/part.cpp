#include "../headers/ClientManager.hpp"

void ClientManager::partCmd( int fd, string& input ) {
	vector< string > tokens = ft_split_tokens( input );

	if ( tokens.size() < 2 )
		return ft_send( fd, ERR_NEEDMOREPARAMS( cli[fd].getNickName() ) );

	string message = getText( input, tokens, 2 );

	vector< string > target_tokens = splitString( tokens[1], ',' );
	for ( size_t i = 0; i < target_tokens.size(); i++ ) {
		string target = target_tokens[i];
		if ( target.empty() ) {
			ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), target ) );
			continue;
		}
		if ( target[0] == '#' || target[0] == '&' ) {
			Channel* channel = getChannel( target.substr( 1 ) );
			if ( !channel ) {
				ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), target ) );
				continue;
			}
			if ( !channel->isInChannel( cli[fd].getNickName() ) ) {
				ft_send( fd, ERR_NOTONCHANNEL( cli[fd].getNickName(), target ) );
				continue;
			}
			channel->removeClient( fd );
			if ( channel->isAdminInChannel( cli[fd].getNickName() ) )
				channel->removeAdmin( fd );
			string reply = ":" + getPrefix( fd ) + " PART " + target + " :" + message + CRLF;
			channel->broadcast( reply );
			if ( channel->getNumberOfClients() == 0 ) {
				for ( vector< Channel >::iterator it = channels.begin(); it != channels.end(); it++ ) {
					if ( it->getName() == channel->getName() ) {
						channels.erase( it );
						break;
					}
				}
			}
		} else {
			ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), target ) );
		}
	}
}