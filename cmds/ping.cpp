#include "../headers/ClientManager.hpp"

void ClientManager::pingCmd( int fd, string& input ) {
	vector< string > tokens = ft_split_tokens( input );

	string message = "";
	if ( tokens.size() > 2 )
		message = getText( input, tokens, 1 );
	ft_send( fd, RPL_PONG( cli[fd].getNickName(), message ) );
}