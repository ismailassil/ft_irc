#include "../headers/ClientManager.hpp"

void ClientManager::kickCmd( int fd, string& input ) {
	bool			 chFound = false;
	vector< string > tokens	 = ft_split_tokens( input );

	if ( tokens.size() < 3 )
		return ft_send( fd, ERR_NEEDMOREPARAMS( string( "*" ) ) );
	if ( tokens.at( 1 ).at( 0 ) != '#' || tokens.at( 1 ).at( 0 ) != '&' )
		return ft_send( fd, ERR_NOSUCHCHANNEL( tokens.at( 2 ), tokens.at( 1 ) ) );

	const string	 reason = getComment(tokens, 3);

	vector< string > chName = splitString( tokens.at( 1 ), ',' );
	if ( chName.size() != 1 || chName[0].empty() || chName[0] == "#" || chName[0] == "&" )
		return ft_send( fd, ERR_NEEDMOREPARAMS( string( "*" ) ) );

	vector< string > nicknames = splitString( tokens.at( 2 ), ',' );
	if (nicknames.size() > 6)
		return ft_send( fd, ERR_TOOMANYTARGETS( cli[fd].getNickName(), nicknames[6] ) );

	const string& channelName = chName[0].substr( 1 );

	for ( vector< Channel >::iterator it = channels.begin(); it != channels.end(); it++ ) {
		if ( channelName == it->getName() ) {
			chFound = true;
			if ( !it->isClientInChannel( cli[fd].getNickName() ) )
				return ft_send( fd, ERR_NOTONCHANNEL( cli[fd].getNickName(), channelName ) );
			if ( it->getClient( fd ) )
				return ft_send( fd, ERR_NOTOPERATOR( cli[fd].getNickName() ) );

			for ( size_t i = 0; i < nicknames.size(); i++ ) {
				if ( nicknames[i].empty() ) {
					ft_send( fd, ERR_NOSUCHNICK( nicknames[i], cli[fd].getNickName() ) );
					continue;
				}
				if ( it->isClientInChannel( nicknames[i] ) )
					it->removeClient( it->getClientInChannel( nicknames[i] )->getFd() );
				else if ( it->isAdminInChannel( nicknames[i] ) )
					it->removeAdmin( it->getAdminInChannel( nicknames[i] )->getFd() );
				else {
					ft_send( fd, ERR_USERNOTINCHANNEL( cli[fd].getNickName(), channelName, nicknames[i] ) );
					continue;
				}
				string reply = ":" + cli[fd].getNickName() + "!~" + cli[fd].getUserName() + "@" + cli[fd].getIpAdd() + " KICK #" + channelName + " " + nicknames[i] + " :" + reason;
				it->broadcast( reply, fd );
				if ( it->getNumberOfClients() == 0 && it->getNumberOfAdmins() == 0 )
					channels.erase( it );
				return;
			}
		}
	}
	if ( !chFound )
		return ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), channelName ) );
}