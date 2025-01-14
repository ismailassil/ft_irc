/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 20:29:16 by iassil            #+#    #+#             */
/*   Updated: 2025/01/14 10:19:27 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "bits.hpp"

class Client {
	private:
		int							   fd;
		bool						   registered;
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
		int			 getFd();
		bool		 getRegistered();
		bool		 getInviteChannel( string &ChName );
		const string getNickName();
		bool		 getLoggedIn();
		const string getUserName();
		const string getIpAdd();
		const string getBuffer();

		// Setters
		void setFd( int fd );
		void setNickname( const string &nickName );
		void setLoggedIn( const bool value );
		void setUsername( const string &username );
		void setBuffer( const string &received );
		void setRegistered( const bool value );
		void setIpAdd( const string &ipadd );
};
