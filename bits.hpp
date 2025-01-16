/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bits.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouchta <obouchta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 20:29:12 by iassil            #+#    #+#             */
/*   Updated: 2025/01/16 15:57:11 by obouchta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <stdio.h>		 // IWYU pragma: keep
#include <sys/socket.h>	 // IWYU pragma: keep
#include <unistd.h>		 // IWYU pragma: keep

#include <algorithm>  // IWYU pragma: keep
#include <cctype>	  // IWYU pragma: keep
#include <cstring>	  // IWYU pragma: keep
#include <iostream>	  // IWYU pragma: keep
#include <map>		  // IWYU pragma: keep
#include <sstream>	  // IWYU pragma: keep
#include <string>	  // IWYU pragma: keep
#include <utility>	  // IWYU pragma: keep
#include <vector>	  // IWYU pragma: keep
#include <string>	  // IWYU pragma: keep
#include <cstring>	  // IWYU pragma: keep
#include <sstream>   // IWYU pragma: keep
#include <iomanip>  // IWYU pragma: keep

#include "Responses.hpp"

#define PASS "pass"
#define NICK "nick"
#define USER "user"
#define QUIT "quit"
#define JOIN "join"
#define KICK "kick"
#define MODE "mode"

using std::cerr;
using std::cout;
using std::endl;
using std::equal;
using std::getline;
using std::istringstream;
using std::make_pair;
using std::map;
using std::pair;
using std::string;
using std::tolower;
using std::transform;
using std::vector;
using std::setw;
using std::setfill;

#define RED		"\x1b[31m"
#define GREEN	"\x1b[1;32m"
#define YELLOW	"\x1b[1;33m"
#define BLUE	"\x1b[1;34m"
#define MAGENTA "\x1b[1;35m"
#define CYAN	"\x1b[1;36m"
#define RESET	"\x1b[0m"

const vector< string > splitString( const string& str, char delim );
const vector< string > ft_split_tokens( const string& input );
int stringToInt(const std::string& str);
bool isNumber(const std::string& s);