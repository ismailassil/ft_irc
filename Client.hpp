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
		bool						   authenticated;
		bool						   registered;
		int							   key;
		bool						   loggedIn;
		string						   nickname;
		string						   userName;
		string						   buffer;
		string						   ipAdd;
		vector< string >			   ChannelsInvite;
		vector< pair< string, bool > > channels;

	public:
		// Constructors and Destructor
		Client();
		Client( const string &nickname, const string &username, int key, int fd );
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

		void printAll () {
			cout << "fd: " << fd << endl;
			cout << "authenticated: " << authenticated
				 << endl;
			cout << "registered: " << registered << endl;
			cout << "key: " << key << endl;
			cout << "loggedIn: " << loggedIn << endl;
			cout << "nickname: " << nickname << endl;
			cout << "userName: " << userName << endl;
			cout << "buffer: " << buffer << endl;
			cout << "ipAdd: " << ipAdd << endl;
			cout << "ChannelsInvite: ";
			for ( size_t i = 0; i < ChannelsInvite.size(); i++ )
				cout << ChannelsInvite[i] << " ";
			cout << endl;
			cout << "channels: ";
			for ( size_t i = 0; i < channels.size(); i++ )
				cout << channels[i].first << " ";
			cout << endl;
		}
};
