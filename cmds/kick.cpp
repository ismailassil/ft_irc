#include "../headers/ClientManager.hpp"

void ClientManager::kickCmd( int fd, string& input ) {
	bool			 chFound = false;
	vector< string > tokens	 = ft_split_tokens( input );

	if ( tokens.size() < 3 )
		return ft_send( fd, ERR_NEEDMOREPARAMS( cli[fd].getNickName() ) );
	if ( tokens.at( 1 ).at( 0 ) != '#' && tokens.at( 1 ).at( 0 ) != '&' ) {
		return ft_send( fd, ERR_NOSUCHCHANNEL( tokens.at( 2 ), tokens.at( 1 ).substr( 1 ) ) );
	}

	string reason = "";
	if ( tokens.size() > 3 )
		reason = getText( input, tokens, 3 );

	vector< string > chName = splitString( tokens.at( 1 ), ',' );
	if ( chName.size() != 1 || chName[0].empty() )
		return ft_send( fd, ERR_NEEDMOREPARAMS( cli[fd].getNickName() ) );

	if ( chName[0] == "#" || chName[0] == "&" )
		return ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), "" ) );

	vector< string > nicknames = splitString( tokens.at( 2 ), ',' );
	if ( nicknames.size() > 6 )
		return ft_send( fd, ERR_TOOMANYTARGETS( cli[fd].getNickName(), nicknames[6] ) );

	const string& channelName = chName[0].substr( 1 );

	for ( vector< Channel >::iterator it = channels.begin(); it != channels.end(); it++ ) {
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
					it->removeClient( it->getClientInChannel( nicknames[i] )->getFd() );
					if ( it->isAdminInChannel( nicknames[i] ) )
						it->removeAdmin( it->getAdminInChannel( nicknames[i] )->getFd() );
					ft_send( it->getClientInChannel( nicknames[i] )->getFd(), RPL_KICK( channelName, cli[fd].getNickName(), reason ) );
				} else {
					ft_send( fd, ERR_USERNOTINCHANNEL( cli[fd].getNickName(), nicknames[i], channelName ) );
					continue;
				}
				string reply = ":" + cli[fd].getNickName() + "!~" + cli[fd].getUserName() + "@" + cli[fd].getIpAdd() + " KICK #" + channelName + " " + nicknames[i];
				if ( !reason.empty() )
					reply += " :" + reason;
				reply += CRLF;
				it->broadcast( reply, fd );
				if ( it->getNumberOfClients() == 0 )
					channels.erase( it );
				return;
			}
		}
	}
	if ( !chFound )
		return ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), channelName ) );
}