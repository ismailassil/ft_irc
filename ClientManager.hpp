/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 16:43:17 by iassil            #+#    #+#             */
/*   Updated: 2025/01/14 10:37:18 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Client.hpp"
#include "bits.hpp"

class ClientManager {
	private:
		map< int, Client > cli;
		string			   pass;
		bool			   rNewLine( string& );
		bool			   isCmd( const string&, const char* );
		static void		   ft_send( int, const string& );
		bool			   isValid( const string& str );

		void nickCmd( int, string& );

	public:
		void parse( string& );
		bool registerClient( int, string& );
		void setPass( const string& );
};
