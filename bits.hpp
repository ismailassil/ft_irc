/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bits.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 20:29:12 by iassil            #+#    #+#             */
/*   Updated: 2025/01/13 20:29:13 by iassil           ###   ########.fr       */
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

using std::cerr;
using std::cout;
using std::endl;
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