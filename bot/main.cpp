#include "Bot.hpp"

void error( string str, int exit_status ) {
	perror( str.c_str() );
	exit( exit_status );
}

int main()
{
    Bot facts;
	facts.run();
}