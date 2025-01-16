#include "../headers/ClientManager.hpp"

void ClientManager::kickCmd( int fd, string& cmd ) {
	string			 reason;
	bool			 chFound = false;
	vector< string > tokens	 = ft_split_tokens( cmd );

	if ( tokens.size() < 3 )
		return ft_send( fd, ERR_NEEDMOREPARAMS( string( "*" ) ) );
	if ( tokens.at( 1 ).at( 0 ) != '#' || tokens.at( 1 ).at( 0 ) != '&' )
		return ft_send( fd, ERR_NOSUCHCHANNEL( tokens.at( 2 ), tokens.at( 1 ) ) );

	if ( tokens.size() > 3 ) {
		if ( tokens.at( 3 ).at( 0 ) == ':' ) {
			tokens.at( 3 ).erase( 0, 1 );
			for ( size_t i = 3; i < tokens.size(); i++ ) {
				reason += tokens.at( i ) + " ";
			}
		} else {
			reason = tokens.at( 3 );
		}
	}

	vector< string > chName = splitString( tokens.at( 1 ), ',' );
	if ( chName.size() != 1 )
		return ft_send( fd, ERR_NEEDMOREPARAMS( string( "*" ) ) );
	vector< string > nicknames = splitString( tokens.at( 2 ), ',' );

	for ( vector< Channel >::iterator it = channels.begin(); it != channels.end(); it++ ) {
		if ( chName.at( 0 ) == it->getName() ) {
			chFound = true;
			if ( it->getClient( fd ) )
				return ft_send( fd, ERR_NOTOPERATOR( cli[fd].getNickName() ) );
			if ( !it->getAdmin( fd ) )
				return ft_send( fd, ERR_NOTONCHANNEL( cli[fd].getNickName(), chName.at( 0 ) ) );
			for ( size_t i = 0; i < nicknames.size(); i++ ) {
				if ( it->isClientInChannel( nicknames[i] ) )
					it->removeClient( it->getClientInChannel( nicknames[i] )->getFd() );
				else if ( it->isAdminInChannel( nicknames[i] ) )
					it->removeAdmin( it->getAdminInChannel( nicknames[i] )->getFd() );
				else
					ft_send( fd, ERR_USERNOTINCHANNEL( cli[fd].getNickName(), chName.at( 0 ), nicknames[i] ) );
				string reply = ":" + cli[fd].getNickName() + "!~" + cli[fd].getUserName() + "@" + cli[fd].getIpAdd() + " KICK #" + chName.at( 0 ) + " " + nicknames[i] + " :" + reason;
				it->broadcast( reply, fd );
				if ( it->getNumberOfClients() )
					channels.erase( it );
				return;
			}
		}
	}
	if ( !chFound )
		return ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), chName.at( 0 ) ) );
}