/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 16:43:17 by iassil            #+#    #+#             */
/*   Updated: 2025/01/14 09:23:46 by iassil           ###   ########.fr       */
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

	public:
		void parse( string& );
		bool registerClient( int, string& );
		void setPass( const string& );
};
