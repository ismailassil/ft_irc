#include "../headers/ClientManager.hpp"

void ClientManager::quitCmd( int fd, string& input ) {
	const vector< string > tokens = ft_split_tokens( input );

	string reason = "Quit: ";
	if ( tokens.size() > 1 ) {
		reason += getText( input, tokens, 1 );
	}

	for ( vector< Channel >::iterator it = channels.begin(); it != channels.end(); it++ ) {
		if ( it->getClient( fd ) ) {
			it->removeClient( fd );
			if ( it->getAdmin( fd ) )
				it->removeAdmin( fd );
			if ( it->getNumberOfClients() == 0 ) {
				channels.erase( it );
			} else {
				string reply = getPrefix( fd ) + " QUIT :" + reason;
				it->broadcast( reply );
			}
		}
	}
	cli.erase( fd );		  // Remove client from the Server
	/*
	* Server::remove_fd( fd );  // Remove the fd from the pollfd array
	* close( fd );			    // Close the connection
	*/
}