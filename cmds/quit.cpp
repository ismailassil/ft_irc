#include "../headers/ClientManager.hpp"

void ClientManager::quitCmd( int fd, string& input ) {
	const vector< string > tokens = ft_split_tokens( input );

	string reason = input;
	size_t pos	  = reason.find( tokens.at( 0 ) );
	if ( pos != string::npos ) {
		reason.erase( pos, tokens.at( 0 ).length() );
		reason = "Quit: " + reason;
	}

	for ( vector< Channel >::iterator it = channels.begin(); it != channels.end(); it++ ) {
		if ( it->getClient( fd ) ) {
			it->removeClient( fd );
			if ( it->getNumberOfClients() == 0 && it->getNumberOfAdmins() == 0 ) {
				channels.erase( it );
			} else {
				string reply = ":" + cli[fd].getNickName() + "!~" + cli[fd].getUserName() + "@" + cli[fd].getIpAdd() + " QUIT :" + reason;
				it->broadcast( reply, fd );
			}
		} else if ( it->getAdmin( fd ) ) {
			it->removeAdmin( fd );
			if ( it->getNumberOfClients() == 0 && it->getNumberOfAdmins() == 0 ) {
				channels.erase( it );
			} else {
				string reply = ":" + cli[fd].getNickName() + "!~" + cli[fd].getUserName() + "@" + cli[fd].getIpAdd() + " QUIT :" + reason;
				it->broadcast( reply, fd );
			}
		}
	}
	cli.erase( fd );								  // Remove client from the Server
	fds.erase( find( fds.begin(), fds.end(), fd ) );  // Remove the fd from the fd_set
	close( fd );									  // Close the connection
	cout << RED << "Client <" << fd << "> Disconnected" << RESET << endl;
}
