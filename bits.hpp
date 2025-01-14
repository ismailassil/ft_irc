/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bits.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 20:29:12 by iassil            #+#    #+#             */
/*   Updated: 2025/01/14 18:31:53 by codespace        ###   ########.fr       */
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
#include <iomanip>

#include "Responses.hpp"

#define PASS "pass"
#define NICK "nick"
#define USER "user"
#define JOIN "JOIN"

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

#define RED		"\x1b[31m"
#define GREEN	"\x1b[1;32m"
#define YELLOW	"\x1b[1;33m"
#define BLUE	"\x1b[1;34m"
#define MAGENTA "\x1b[1;35m"
#define CYAN	"\x1b[1;36m"
#define RESET	"\x1b[0m"
#define BOLD "\033[1m"
#define WHITE "\033[37m"

const vector< string > splitString( const string& str, char delim );
const vector< string > ft_split_tokens( const string& input );