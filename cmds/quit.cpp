#include "../headers/ClientManager.hpp"
#include "../headers/Server.hpp"

void ClientManager::quitCmd( int fd, string& input ) {
	const vector< string > tokens = ft_split_tokens( input );

	string reason = "Quit: ";
	if ( tokens.size() > 1 ) {
		reason += getText( input, tokens, 1 );
	}

	for ( vector< Channel >::iterator it = channels.begin(); it != channels.end(); ) {
		if ( it->isInChannel( cli[fd].getNickName() ) ) {
			string reply = ":" + getPrefix( fd ) + " QUIT :" + reason + CRLF;
			it->broadcast( reply );

			it->removeClient( fd );
			if ( it->isAdminInChannel( cli[fd].getNickName() ) )
				it->removeAdmin( fd );
			if ( it->getNumberOfClients() == 0 )
				it = channels.erase( it );
			else
				it++;
		} else {
			it++;
		}
	}
	cli.erase( fd );
	Server::remove_fd( fd );
}