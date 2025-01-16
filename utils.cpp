/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouchta <obouchta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 10:26:41 by iassil            #+#    #+#             */
/*   Updated: 2025/01/16 15:57:00 by obouchta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bits.hpp"

const vector< string > ft_split_tokens( const string& input ) {
	istringstream	 stream( input );
	vector< string > tokens;
	string			 token;

	while ( stream >> token ) {
		tokens.push_back( token );
		token.clear();
	}
	return tokens;
}

const vector< string > splitString( const string& str, char delim ) {
	vector< string > split;
	string			 token = "";
	for ( size_t i = 0; i < str.size(); i++ ) {
		if ( str[i] == delim ) {
			split.push_back( token );
			token = "";
		} else
			token += str[i];
	}
	split.push_back( token );
	return split;
}

int stringToInt(const std::string& str) {
    std::stringstream ss(str);
    int result;
    ss >> result;
    if (ss.fail()) {
        throw std::invalid_argument("Invalid conversion from string to int");
    }
    return result;
}

bool isNumber(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun((int(*)(int))std::isdigit))) == s.end();
}