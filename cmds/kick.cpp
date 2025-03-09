#include "../headers/ClientManager.hpp"

void ClientManager::kickCmd( int fd, string& input ) {
	bool			 chFound = false;
	vector< string > tokens	 = ft_split_tokens( input );

	if ( tokens.size() < 3 )
		return ft_send( fd, ERR_NEEDMOREPARAMS( cli[fd].getNickName() ) );
	if ( tokens.at( 1 ).at( 0 ) != '#' && tokens.at( 1 ).at( 0 ) != '&' ) {
		return ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), tokens.at( 1 ).substr( 1 ) ) );
	}

	string reason = "Yeet Out";
	if ( tokens.size() > 3 && tokens.at( 3 ) != ":" )
		reason = getText( input, tokens, 3 );

	vector< string > chName = splitString( tokens.at( 1 ), ',' );
	if ( chName.size() != 1 || chName[0].empty() )
		return ft_send( fd, ERR_NEEDMOREPARAMS( cli[fd].getNickName() ) );

	if ( chName[0] == "#" || chName[0] == "&" )
		return ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), "" ) );

	vector< string > nicknames = splitString( tokens.at( 2 ), ',' );
	if ( nicknames.size() > 6 )
		return ft_send( fd, ERR_TOOMANYTARGETS( cli[fd].getNickName(), nicknames[6] ) );

	const string& channelName = chName[0];

	for ( vector< Channel >::iterator it = channels.begin(); it != channels.end(); ) {
		if ( channelName == it->getName() ) {
			chFound = true;

			if ( !it->isInChannel( cli[fd].getNickName() ) )
				return ft_send( fd, ERR_NOTONCHANNEL( cli[fd].getNickName(), channelName ) );
			if ( !it->isAdminInChannel( cli[fd].getNickName() ) )
				return ft_send( fd, ERR_NOTOPERATOR( cli[fd].getNickName() ) );

			for ( size_t i = 0; i < nicknames.size(); i++ ) {
				if ( nicknames[i].empty() ) {
					ft_send( fd, ERR_NOSUCHNICK( nicknames[i], cli[fd].getNickName() ) );
					continue;
				}

				if ( it->isInChannel( nicknames[i] ) ) {
					int client_fd = it->getClientInChannel( nicknames[i] )->getFd();

					string reply = ":" + getPrefix( fd ) + " KICK " + channelName + " " + nicknames[i];
					reply += " :" + reason + CRLF;
					it->broadcast( reply );

					it->removeClient( client_fd );

					if ( it->isAdminInChannel( nicknames[i] ) ) {
						it->removeAdmin( client_fd );
					}

				} else {
					ft_send( fd, ERR_USERNOTINCHANNEL( cli[fd].getNickName(), nicknames[i], channelName ) );
					continue;
				}
			}

			if ( it->getNumberOfClients() == 0 ) {
				it = channels.erase( it );
			} else {
				++it;
			}
			return;
		} else {
			++it;
		}
	}
	if ( !chFound )
		return ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), channelName ) );
}