/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 16:43:17 by iassil            #+#    #+#             */
/*   Updated: 2025/01/15 12:05:00 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Channel.hpp"
#include "Client.hpp"
#include "bits.hpp"

class ClientManager {
	private:
		vector<int>		   fds;
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
		void modeCmd( int, string& );

	public:
		static void		   ft_send( int, const string& );
		void parse( int, string& );
		bool registerClient( int, string& );
		void setPass( const string& );
		Channel* findChannelByName(const vector<Channel>& channels, const string& name);
};
