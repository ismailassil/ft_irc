/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 10:26:41 by iassil            #+#    #+#             */
/*   Updated: 2025/01/14 10:37:55 by iassil           ###   ########.fr       */
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

const vector<string> splitString(const string& str, char delim)
{
    vector<string> split;
    string token = "";
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == delim)
        {
            split.push_back(token);
            token = "";
        }
        else
            token += str[i];
    }
    split.push_back(token);
    return split;
}
