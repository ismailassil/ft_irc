/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 20:29:16 by iassil            #+#    #+#             */
/*   Updated: 2025/01/14 17:50:08 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "bits.hpp"

class Client {
	private:
		int							   fd;
		bool						   authenticated;
		bool						   registered;
		int							   key;
		string						   nickname;
		bool						   loggedIn;
		string						   userName;
		string						   buffer;
		string						   ipAdd;
		vector< string >			   ChannelsInvite;
		vector< pair< string, bool > > channels;

	public:
		// Constructors and Destructor
		Client();
		Client( const string &nickname, const string &username, int fd );
		~Client();
		Client( Client const &src );
		Client &operator=( Client const &src );

		// Getters
		int			 getFd() const;
		bool		 getAuthenticated() const;
		bool		 getRegistered() const;
		int			 getKey() const;
		bool		 getInviteChannel( string &ChName ) const;
		const string getNickName() const;
		bool		 getLoggedIn() const;
		const string getUserName() const;
		const string getIpAdd() const;
		const string getBuffer() const;

		// Setters
		void setFd( const int fd );
		void setAuthenticated( const bool value );
		void setNickname( const string &nickName );
		void setKey( const int key );
		void setLoggedIn( const bool value );
		void setUsername( const string &username );
		void setBuffer( const string &received );
		void setRegistered( const bool value );
		void setIpAdd( const string &ipadd );
};
