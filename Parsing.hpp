/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 16:43:17 by iassil            #+#    #+#             */
/*   Updated: 2025/01/13 18:26:27 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/socket.h>	 // IWYU pragma: keep

#include <algorithm>  // IWYU pragma: keep
#include <cctype>	  // IWYU pragma: keep
#include <iostream>	  // IWYU pragma: keep
#include <map>		  // IWYU pragma: keep
#include <sstream>	  // IWYU pragma: keep
#include <string>	  // For std::string
#include <utility>	  // IWYU pragma: keep
#include <vector>	  // IWYU pragma: keep

#define PASS "pass"
#define NICK "nick"
#define USER "user"

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
using std::cout;
using std::cerr;
using std::endl;

struct userInfo {
		string nickname;
		string user;
};

enum flags {
	NONE				  = 0,
	ERR_PASSWDMISMATCH	  = 464,
	ERR_NEEDMOREPARAMS	  = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_UNKNOWNCOMMAND	  = 421,
	ERR_NOTREGISTERED	  = 451,
	ERR_ERRONEUSNICKNAME  = 432,
	ERR_NICKNAMEINUSE	  = 433,
};

class Parsing {
	private:
		static vector< userInfo > users;
		static vector< string >	  nicknames;
		userInfo				  user;
		flags					  irc_flag;
		string					  pass;
		int						  isDone;
		bool					  rNewLine( string& );
		bool					  isCmd( const string&, const char* );
		static void				  ft_send( int, const string&, int );
		bool					  isValid( const string& str );

	public:
		Parsing() { isDone = 0; }
		void parse( string& );
		bool registerClient( int, string& );
		void setPass( const string& );
};
