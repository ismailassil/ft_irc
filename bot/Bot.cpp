#include "Bot.hpp"

Bot::Bot(void)
{
	initVars();
	open_config_file();

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
		error("socket()", 1);
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(prt);
	if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		error("connect()", 1);
	
	char buffer[BUFFER_SIZE];
	ssize_t byte = recv(socket_fd, buffer, BUFFER_SIZE - 1, 0);
	
	authentificate();
}

Bot::Bot(const Bot &copy)
{
	*this = copy;
}

Bot::~Bot(void)
{
	close(socket_fd);
	std::cout << "Bot is shutting down" << std::endl;
}

Bot& Bot::operator=(const Bot &other)
{
	if (this == &other)
		return (*this);
	socket_fd = other.socket_fd;
	server_addr = other.server_addr;
	pas = other.pas;
	prt = other.prt;
	facts = other.facts;
	return (*this);
}

void Bot::open_config_file()
{
	std::ifstream file("botConfig");
	if (!file.is_open())
		error("botconfig file", 1);
	string line, port;
	while (getline(file, line)) {
		if (line.empty()) {
			line.clear();
			continue;
		}
		size_t port_pos = line.find("PORT=");
		size_t pass_pos = line.find("PASSWORD=");
		if (port_pos == string::npos && pass_pos == string::npos) {
			file.close();
			cerr << "ERROR: arg not found" << endl;
			exit(1);
		}
		if (port_pos != string::npos) {
			port = line.substr(port_pos + 5 );
		}
		if (pass_pos != string::npos) {
			pas = line.substr( pass_pos + 9 );
		}
	}
	file.close();
	if (port.empty() || pas.empty()) {
		cerr << "ERROR: arg is empty" << endl;
		exit(1);
	}

	char *end1;
	prt = strtol(port.c_str(), &end1, 10);
	if (*end1) {
		cerr << "ERROR: invalid arg" << endl;
		exit(1);
	}
}

void Bot::authentificate()
{
	const string& msg1 = "pass " + pas + CRLF;
	send_msg(msg1);
	usleep(100);
	////
	string message = read_msg();
	if (!message.empty()) {
		cerr << "ERROR: invalid password" << endl;
		close(socket_fd);
		exit(1);
	}
	message.clear();
	////
	const string& msg2 = string("nick bot") + CRLF;
	send_msg(msg2);
	usleep(100);
	////
	message = read_msg();
	if (!message.empty()) {
		cerr << "ERROR: invalid nick" << endl;
		close(socket_fd);
		exit(1);
	}
	const string& msg3 = string("user bot 0 * :bot") + CRLF;
	send_msg(msg3);
}

void Bot::send_msg(const string& msg)
{
	if (send(socket_fd, msg.c_str(), msg.size(), 0) < 0)
		error("send()", 1);
}

string Bot::read_msg() {
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100;
    setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    char buffer[BUFFER_SIZE];
    ssize_t byte = recv(socket_fd, buffer, BUFFER_SIZE - 1, 0);
    if (byte == 0) {
        return std::string();
    }

    if (byte < 0) {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            return std::string();
        } else {
            perror("recv()");
            exit(1);
        }
    }
    buffer[byte] = '\0';
    return std::string(buffer);
}

void Bot::initVars()
{
	facts.push_back("The Eiffel Tower can grow more than six inches during the summer.");
	facts.push_back("Octopuses have three hearts and blue blood.");
	facts.push_back("Bananas are berries, but strawberries aren't.");
	facts.push_back("Sharks existed before trees did.");
	facts.push_back("The shortest war in history lasted only 38 minutes.");
	facts.push_back("There are more stars in the universe than grains of sand on Earth.");
	facts.push_back("A group of flamingos is called a 'flamboyance.'");
	facts.push_back("Honey never spoils. Archaeologists have found edible honey in ancient tombs.");
	facts.push_back("The human brain generates enough electricity to power a small light bulb.");
	facts.push_back("Water can boil and freeze at the same time under certain conditions.");
	facts.push_back("The longest-living cells in the human body are brain cells, which can last an entire lifetime.");
	facts.push_back("Cows have best friends and get stressed when separated from them.");
	facts.push_back("Mount Everest grows about 4 millimeters every year due to geological activity.");
	facts.push_back("A single teaspoon of honey represents the life work of 12 bees.");
	facts.push_back("Wombat poop is cube-shaped to prevent it from rolling away.");
	facts.push_back("Antarctica is the only continent without native species of ants.");
	facts.push_back("Saturn's moon Titan has a thick atmosphere and weather similar to Earth.");
	facts.push_back("Sloths can hold their breath longer than dolphins can—up to 40 minutes.");
	facts.push_back("The dot over a lowercase 'i' or 'j' is called a 'tittle.'");
	facts.push_back("Butterflies can taste with their feet.");
}

void Bot::run()
{
	string message = read_msg();
	message.clear();

	while (1) {
		message = read_msg();
		if (message.empty()) {
			continue;
		}
		if (message.find("PRIVMSG") != string::npos) {
			size_t preStart = message.find(':') + 1;
            size_t preEnd = message.find(' ', preStart);

            const string& clientName = message.substr(preStart, preEnd - preStart);

			string response;
			transform(message.begin(), message.end(), message.begin(), static_cast<int (*)(int)>(tolower));
			
			if (message.find(PING) != string::npos) {
				response = "PONG";
			} else {
				response = facts[rand() % facts.size()];
			}
			const string& msg = "PRIVMSG " + clientName  + " :" + response + CRLF;
			send_msg(msg);
		}
	}
}