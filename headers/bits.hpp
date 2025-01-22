#pragma once
#include <arpa/inet.h>	 // IWYU pragma: keep
#include <errno.h>		 // IWYU pragma: keep
#include <netdb.h>		 // IWYU pragma: keep
#include <netinet/in.h>	 // IWYU pragma: keep
#include <poll.h>		 // IWYU pragma: keep
#include <signal.h>		 // IWYU pragma: keep
#include <stdio.h>		 // IWYU pragma: keep
#include <sys/socket.h>	 // IWYU pragma: keep
#include <unistd.h>		 // IWYU pragma: keep

#include <algorithm>  // IWYU pragma: keep
#include <cctype>	  // IWYU pragma: keep
#include <cstdio>	  // IWYU pragma: keep
#include <cstdlib>	  // IWYU pragma: keep
#include <cstring>	  // IWYU pragma: keep
#include <fstream>	  // IWYU pragma: keep
#include <iomanip>	  // IWYU pragma: keep
#include <iostream>	  // IWYU pragma: keep
#include <map>		  // IWYU pragma: keep
#include <sstream>	  // IWYU pragma: keep
#include <string>	  // IWYU pragma: keep
#include <utility>	  // IWYU pragma: keep
#include <vector>	  // IWYU pragma: keep

#define BACKLOG		4
#define MAXCLIENT	5
#define BUFFER_SIZE 1024
#define PASS_LENGH	4
#define PASS_MAXLEN 16

#include "Responses.hpp"

#define PASS	"pass"
#define NICK	"nick"
#define USER	"user"
#define JOIN	"join"
#define KICK	"kick"
#define TOPIC	"topic"
#define MODE	"mode"
#define PRIVMSG "privmsg"
#define INVITE	"invite"
#define PING	"ping"
#define PONG	"pong"

using std::cerr;
using std::cout;
using std::endl;
using std::equal;
using std::find_if;
using std::getline;
using std::ifstream;
using std::istringstream;
using std::make_pair;
using std::map;
using std::memset;
using std::ostringstream;
using std::pair;
using std::setfill;
using std::setw;
using std::string;
using std::stringstream;
using std::to_string;
using std::tolower;
using std::transform;
using std::vector;

#define RED		"\x1b[1;31m"
#define GREEN	"\x1b[1;32m"
#define YELLOW	"\x1b[1;33m"
#define BLUE	"\x1b[1;34m"
#define MAGENTA "\x1b[1;35m"
#define CYAN	"\x1b[1;36m"
#define RESET	"\x1b[0m"

void handle_signal( int );
int	 parse_input( int&, char**& );
void error( const string&, int );

const vector< string > splitString( const string&, char );
const vector< string > ft_split_tokens( const string& );
void				   ft_send( int fd, const string& );
const string		   getText( const string&, vector< string >, int );
bool				   isNumber( const string& );
int					   stringToInt( const string& );
bool				   hasOnlyWhitespaces( const string& str );
string				   getTimestamp();
