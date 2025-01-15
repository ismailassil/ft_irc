/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 16:43:17 by iassil            #+#    #+#             */
/*   Updated: 2025/01/15 13:13:24 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Channel.hpp"
#include "Client.hpp"
#include "bits.hpp"

class ClientManager {
	private:
		vector< int >	   fds;
		map< int, Client > cli;
		vector< Channel >  channels;
		string			   pass;
		bool			   rNewLine( string& );
		bool			   isCmd( const string&, const char* );
		bool			   isValid( const string& str );

		void nickCmd( int, string& );
		void quitCmd( int, string& );
		void joinCmd( int, string& );
		void kickCmd( int, string& );
		void topicCmd( int, string& );
		void modeCmd( int, string& );

	public:
		void parse( int, string& );
		void registerClient( int, string& );
		void setPass( const string& );
};
