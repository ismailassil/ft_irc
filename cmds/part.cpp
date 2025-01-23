#include "../headers/ClientManager.hpp"

void ClientManager::partCmd( int fd, string& input ) {
	vector< string > tokens = ft_split_tokens( input );

	if ( tokens.size() < 2 )
		return ft_send( fd, ERR_NEEDMOREPARAMS( cli[fd].getNickName() ) );

	string message = "Leaving";
	if ( tokens.size() > 2 )
		message = getText( input, tokens, 2 );

	vector< string > target_tokens = splitString( tokens[1], ',' );
	for ( size_t i = 0; i < target_tokens.size(); i++ ) {
		string target = target_tokens[i];
		if ( target.empty() ) {
			ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), target ) );
			continue;
		}
		if ( target[0] == '#' || target[0] == '&' ) {
			string channelName = target.substr( 1 );
			Channel* channel = getChannel( channelName );
			if ( !channel ) {
				ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), channelName ) );
				continue;
			}
			if ( !channel->isInChannel( cli[fd].getNickName() ) ) {
				ft_send( fd, ERR_NOTONCHANNEL( cli[fd].getNickName(), channelName ) );
				continue;
			}
			string reply = ":" + getPrefix( fd ) + " PART #" + channelName + " :" + message + CRLF;
			channel->broadcast( reply );

			channel->removeClient( fd );
			if ( channel->isAdminInChannel( cli[fd].getNickName() ) )
				channel->removeAdmin( fd );
			if ( channel->getNumberOfClients() == 0 ) {
				for ( size_t i = 0; i < channels.size(); i++ ) {
					if ( channels[i].getName() == channel->getName() ) {
						channels.erase( channels.begin() + i );
						break;
					}
				}
			}
		} else {
			ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), target ) );
		}
	}
}