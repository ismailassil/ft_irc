#include "../headers/ClientManager.hpp"

void ClientManager::pongCmd( int fd, string& buffer ) {
	// NOTHING SPECIAL TO DO HERE
	// JUST IGNORE THE PONG MESSAGE
	(void)fd;
	(void)buffer;
}