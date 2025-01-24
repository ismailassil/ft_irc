#include "Bot.hpp"

int stop_bot	   = 0;
int Bot::socket_fd = -1;

Bot::Bot( void ) : password( "" ), port( 0 ), nick( "bot" ), user( "bot" ), lastActivity( time( NULL ) ) {
	signal( SIGINT, &Bot::handle_signal );
	signal( SIGTERM, &Bot::handle_signal );
	signal( SIGHUP, &Bot::handle_signal );
	signal( SIGPIPE, SIG_IGN );

	cout << GREEN << "[Bot] is Starting" << RESET << endl;
	initVars();
	openConfigFile();

	socket_fd = socket( AF_INET, SOCK_STREAM, 0 );
	if ( socket_fd == -1 )
		error( "socket()", 1 );

	int opt = 1;
	if ( setsockopt( socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof( opt ) ) < 0 )
		error( "setsockopt()", 1 );

	memset( &server_addr, 0, sizeof( server_addr ) );
	server_addr.sin_family		= AF_INET;
	server_addr.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	server_addr.sin_port		= htons( port );
	if ( connect( socket_fd, (struct sockaddr*)&server_addr, sizeof( server_addr ) ) < 0 )
		error( "connect()", 1 );

	authentificate();
}

Bot::Bot( const Bot& copy ) {
	*this = copy;
}

Bot::~Bot( void ) {
	close( socket_fd );
	cout << RED << "[Bot] is Stopping" << RESET << endl;
}

Bot& Bot::operator=( const Bot& other ) {
	if ( this == &other )
		return ( *this );
	socket_fd	 = other.socket_fd;
	server_addr	 = other.server_addr;
	password	 = other.password;
	port		 = other.port;
	facts		 = other.facts;
	nick		 = other.nick;
	user		 = other.user;
	lastActivity = other.lastActivity;
	return ( *this );
}

void Bot::openConfigFile() {
	ifstream file( "bonus/botConfig.conf" );
	if ( !file.is_open() )
		error( "Conf file", 1 );

	string line, prt;
	while ( getline( file, line ) ) {
		if ( line.empty() ) {
			line.clear();
			continue;
		}
		size_t port_pos = line.find( "PORT=" );
		size_t pass_pos = line.find( "PASSWORD=" );
		if ( port_pos == string::npos && pass_pos == string::npos ) {
			file.close();
			cerr << "ERROR: arg not found" << endl;
			exit( 1 );
		}
		if ( port_pos != string::npos ) {
			prt = line.substr( port_pos + 5 );
		}
		if ( pass_pos != string::npos ) {
			password = line.substr( pass_pos + 9 );
		}
	}
	file.close();

	if ( prt.empty() || password.empty() ) {
		cerr << "ERROR: arg is empty" << endl;
		exit( 1 );
	}

	char* end;
	port = strtol( prt.c_str(), &end, 10 );
	if ( *end ) {
		cerr << "ERROR: invalid arg" << endl;
		exit( 1 );
	}
}

void Bot::authentificate() {
	const string& msg1 = "pass " + password + CRLF;
	send_msg( msg1 );
	usleep( 10000 );
	////
	string message = read_msg();
	if ( !message.empty() ) {
		cerr << "ERROR: invalid password" << endl;
		close( socket_fd );
		exit( 1 );
	}
	message.clear();
	////
	const string& msg2 = string( "nick " + getNickName() ) + CRLF;
	send_msg( msg2 );
	usleep( 10000 );
	////
	message = read_msg();
	if ( !message.empty() ) {
		cerr << "ERROR: invalid nick" << endl;
		close( socket_fd );
		exit( 1 );
	}
	const string& msg3 = string( "user " + getUserName() + " 0 * :bot" ) + CRLF;
	send_msg( msg3 );

	read_msg();
}

void Bot::send_msg( const string& msg ) {
	if ( send( socket_fd, msg.c_str(), msg.size(), 0 ) < 0 )
		error( "send()", 1 );
}

const string Bot::read_msg() {
	struct timeval timeout;
	timeout.tv_sec	= 0;
	timeout.tv_usec = 10000;
	setsockopt( socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof( timeout ) );

	char	buffer[BUFFER_SIZE];
	ssize_t byte = recv( socket_fd, buffer, BUFFER_SIZE - 1, 0 );
	if ( byte == 0 ) {
		return string();
	}

	if ( byte < 0 ) {
		if ( errno == EWOULDBLOCK || errno == EAGAIN ) {
			return string();
		} else {
			exit( 1 );
		}
	}
	buffer[byte] = '\0';
	return string( buffer );
}

void Bot::initVars() {
	facts.push_back( "The Eiffel Tower can grow more than six inches during the summer." );
	facts.push_back( "Octopuses have three hearts and blue blood." );
	facts.push_back( "Bananas are berries, but strawberries aren't." );
	facts.push_back( "Sharks existed before trees did." );
	facts.push_back( "The shortest war in history lasted only 38 minutes." );
	facts.push_back( "There are more stars in the universe than grains of sand on Earth." );
	facts.push_back( "A group of flamingos is called a 'flamboyance.'" );
	facts.push_back( "Honey never spoils. Archaeologists have found edible honey in ancient tombs." );
	facts.push_back( "The human brain generates enough electricity to power a small light bulb." );
	facts.push_back( "Water can boil and freeze at the same time under certain conditions." );
	facts.push_back( "The longest-living cells in the human body are brain cells, which can last an entire lifetime." );
	facts.push_back( "Cows have best friends and get stressed when separated from them." );
	facts.push_back( "Mount Everest grows about 4 millimeters every year due to geological activity." );
	facts.push_back( "A single teaspoon of honey represents the life work of 12 bees." );
	facts.push_back( "Wombat poop is cube-shaped to prevent it from rolling away." );
	facts.push_back( "Antarctica is the only continent without native species of ants." );
	facts.push_back( "Saturn's moon Titan has a thick atmosphere and weather similar to Earth." );
	facts.push_back( "Sloths can hold their breath longer than dolphins can—up to 40 minutes." );
	facts.push_back( "The dot over a lowercase 'i' or 'j' is called a 'tittle.'" );
	facts.push_back( "Butterflies can taste with their feet." );
	facts.push_back( "The unicorn is the national animal of Scotland." );
	facts.push_back( "The world's largest snowflake was 15 inches wide and 8 inches thick." );
	facts.push_back( "The shortest war in history was between Britain and Zanzibar on August 27, 1896. Zanzibar surrendered after 38 minutes." );

	jokes.push_back( "Why did the scarecrow win an award? Because he was outstanding in his field!" );
	jokes.push_back( "Why did the tomato turn red? Because it saw the salad dressing!" );
	jokes.push_back( "Why don't skeletons fight each other? They don't have the guts!" );
	jokes.push_back( "Why don't eggs tell jokes? They might crack up!" );
	jokes.push_back( "Why can't your nose be 12 inches long? Because then it would be a foot!" );
	jokes.push_back( "What do you call fake spaghetti? An impasta!" );
	jokes.push_back( "Why did the bicycle fall over? Because it was two-tired!" );
	jokes.push_back( "Why don't oysters donate to charity? Because they are shellfish!" );
	jokes.push_back( "What did the ocean say to the beach? Nothing, it just waved!" );
	jokes.push_back( "Why did the coffee file a police report? It got mugged!" );
	jokes.push_back( "Why did the golfer bring two pairs of pants? In case he got a hole in one!" );
	jokes.push_back( "What do you call cheese that isn't yours? Nacho cheese!" );

	quotes.push_back( "The only way to do great work is to love what you do. – Steve Jobs" );
	quotes.push_back( "Success is not final, failure is not fatal: It is the courage to continue that counts. – Winston Churchill" );
	quotes.push_back( "It does not matter how slowly you go as long as you do not stop. – Confucius" );
	quotes.push_back( "The journey of a thousand miles begins with one step. – Lao Tzu" );
	quotes.push_back( "In the middle of every difficulty lies opportunity. – Albert Einstein" );
	quotes.push_back( "Believe you can and you're halfway there. – Theodore Roosevelt" );
	quotes.push_back( "The only limit to our realization of tomorrow is our doubts of today. – Franklin D. Roosevelt" );
	quotes.push_back( "Life is what happens when you're busy making other plans. – John Lennon" );
	quotes.push_back( "You must be the change you wish to see in the world. – Mahatma Gandhi" );
	quotes.push_back( "The best way to predict the future is to create it. – Abraham Lincoln" );
	quotes.push_back( "The best time to plant a tree was 20 years ago. The second best time is now. – Chinese Proverb" );
	quotes.push_back( "The future belongs to those who believe in the beauty of their dreams. – Eleanor Roosevelt" );
}

const vector< string > splitMessage( const string& str, char delim ) {
	vector< string > split;
	string			 token = "";

	for ( size_t i = 0; i < str.size(); i++ ) {
		if ( str[i] == delim ) {
			split.push_back( token );
			token = "";
		} else
			token += str[i];
	}
	split.push_back( token );
	return split;
}

void Bot::checkInactivity() {
	static const int idleThreshold = 10;

	time_t currentTime = time( NULL );
	if ( difftime( currentTime, lastActivity ) > idleThreshold ) {
		const string pingMsg = "PING :Are you alive?" + string( CRLF );
		send_msg( pingMsg );
		lastActivity = currentTime;
	}
}

void Bot::run() {
	string message;

	srand( time( NULL ) );
	cout << YELLOW << "Bot is running" << RESET << endl;
	while ( stop_bot == 0 ) {
		message = read_msg();
		if ( message.empty() ) {
			checkInactivity();
			continue;
		}

		lastActivity = time( NULL );
		printCurrentDateTime();
		cout << "Received from: " << message.erase( message.find( CRLF ) ) << endl;

		transform( message.begin(), message.end(), message.begin(), static_cast< int ( * )( int ) >( tolower ) );
		if ( message.find( "privmsg" ) != string::npos ) {
			vector< string > tokens = splitMessage( message, ' ' );
			if ( tokens.size() < 4 ) {
				continue;
			}
			string clientName = tokens[0].substr( 1, tokens[0].find( '!' ) - 1 );

			string response;
			if ( tokens.at( 3 ) == ":ping" ) {
				response = "PONG";
			} else if ( tokens.at( 3 ) == ":pong" ) {
				response = "PING";
			} else if ( tokens.at( 3 ) == ":time" ) {
				time_t curr_time;
				curr_time = time( NULL );
				response  = ctime( &curr_time );
			} else if ( tokens.at( 3 ) == ":hello" ) {
				response = "Hello, " + clientName + "!";
			} else if ( tokens.at( 3 ) == ":fact" ) {
				response = facts[rand() % facts.size()];
			} else if ( tokens.at( 3 ) == ":roll" ) {
				response = "You rolled a " + intToString( rand() % 6 + 1 ) + "!";
			} else if ( tokens.at( 3 ) == ":flip" ) {
				response = "You flipped a " + string( rand() % 2 ? "heads" : "tails" ) + "!";
			} else if ( tokens.at( 3 ) == ":dice" ) {
				response = "You rolled a " + intToString( rand() % 6 + 1 ) + " and a " + intToString( rand() % 6 + 1 ) + "!";
			} else if ( tokens.at( 3 ) == ":joke" ) {
				response = jokes[rand() % jokes.size()];
			} else if ( tokens.at( 3 ) == ":quote" ) {
				response = quotes[rand() % quotes.size()];
			} else if ( tokens.at( 3 ) == ":help" ) {
				response = "Available commands: :hello, :ping, :pong, :time, :fact, :roll, :flip, :dice, :joke, :quote, :help";
			} else {
				response = "Unknown command. Type :help to see available commands.";
			}

			const string& msg = "PRIVMSG " + clientName + " :" + response + CRLF;
			send_msg( msg );
		}
	}
}

void Bot::error( string str, int exit_status ) {
	perror( str.c_str() );
	exit( exit_status );
}

void Bot::printCurrentDateTime() {
	time_t curr_time;
	curr_time = time( NULL );

	tm* tm_local = localtime( &curr_time );
	if ( tm_local == NULL )
		return;
	cout << "[" << tm_local->tm_mday << "-"
		 << setfill( '0' ) << setw( 2 ) << tm_local->tm_mon + 1 << "-"
		 << tm_local->tm_year + 1900 << "] ["
		 << setw( 2 ) << tm_local->tm_hour << ":"
		 << setw( 2 ) << tm_local->tm_min << ":"
		 << setw( 2 ) << tm_local->tm_sec << "] ";
}

void Bot::handle_signal( int signum ) {
	(void)signum;
	stop_bot = 1;
	if ( socket_fd != -1 ) {
		send( socket_fd, "QUIT :Bye!\r\n", 11, 0 );
		close( socket_fd );
	}
}

string Bot::getNickName() {
	return nick;
}

string Bot::getUserName() {
	return user;
}

string intToString( int n ) {
	stringstream ss;
	ss << n;
	return ss.str();
}
