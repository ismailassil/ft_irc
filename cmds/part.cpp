#include "../headers/ClientManager.hpp"

void ClientManager::partCmd( int fd, string& input ) {
	vector< string > tokens = ft_split_tokens( input );

	if ( tokens.size() < 2 )
		return ft_send( fd, ERR_NEEDMOREPARAMS( cli[fd].getNickName() ) );

	string message = "";
	for ( size_t i = 2; i < tokens.size(); i++ ) {
		if ( i == 2 && tokens[i][0] == ':' )
			tokens[i] = tokens[i].substr( 1 );
		else {
			message = tokens[i];
			break;
		}
		message += tokens[i];
		if ( i + 1 < tokens.size() )
			message += " ";
	}

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
			if (channel->isClientInChannel(cli[fd].getNickName()))
				channel->removeClient( fd );
			else if (channel->isAdminInChannel(cli[fd].getNickName()))
				channel->removeAdmin( fd );

			string reply = ":" + getPrefix( fd ) + " PART " + target + " :" + message + CRLF;
			channel->broadcast( reply, fd );
			if ( channel->getNumberOfClients() == 0 && channel->getNumberOfAdmins() == 0 )
				channels.erase( find( channels.begin(), channels.end(), *channel ) );
		} else {
			ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), target ) );
		}
	}
}