#include "Bot.hpp"

int stop_bot = 0;

void	handle_signal( int signum ) {
	(void)signum;
	stop_bot = 1;
}

int main() {
	Bot facts;
	facts.run();
}
