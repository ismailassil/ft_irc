#define once

#define CRLF "\r\n"
#define LF	 "\n"

#define WELCOME_MSG											"Welcome to the server!\n"
#define RPL_CONNECTED( nickname )							( ": 001 " + nickname + " : Welcome to the IRC server!" + CRLF )
#define RPL_UMODEIS( hostname, chName, mode, user )			":" + hostname + " MODE " + chName + " " + mode + " " + user + CRLF
#define RPL_CREATIONTIME( nickname, chName, creationtime )	": 329 " + nickname + " #" + chName + " " + creationtime + CRLF
#define RPL_CHANNELMODES( nickname, chName, modes )			": 324 " + nickname + " #" + chName + " " + modes + CRLF
#define RPL_CHANGEMODE( hostname, chName, mode, arguments ) ( ":" + hostname + " MODE #" + chName + " " + mode + " " + arguments + CRLF )
#define RPL_NICKCHANGE( oldnickname, nickname )				( ":" + oldnickname + " NICK " + nickname + CRLF )
#define RPL_JOINMSG( nick, username, ipaddress, chName )	( ":" + nick + "!" + username + "@" + ipaddress + " JOIN #" + chName + CRLF )
#define RPL_NAMREPLY( nickname, chName, clientslist )		( ": 353 " + nickname + " @ #" + chName + " :" + clientslist + CRLF )
#define RPL_ENDOFNAMES( nickname, chName )					( ": 366 " + nickname + " #" + chName + " :END of /NAMES list" + CRLF )
#define RPL_TOPICIS( nickname, chName, topic )				( ": 332 " + nickname + " #" + chName + " :" + topic + "\r\n" )
#define RPL_TOPIC( nickname, chName, topic )				( ": 332 " + nickname + " #" + chName + " :" + topic + CRLF )
#define RPL_KICK( chName, nickname, reason )				( ": " + nickname + " KICK #" + chName + " " + nickname + " :" + reason + CRLF )
#define RPL_INVITING( nickname, target, chName )			( ": 341 " + nickname + " INVITE " + target + " #" + chName + CRLF )

/////////////////////////////////
//////////// ERRORS /////////////
/////////////////////////////////
#define ERR_INVALIDMODEPARM( chName, mode )			   ": 696 #" + chName + " Invalid mode parameter. " + mode + CRLF
#define ERR_KEYSET( chName )						   ": 467 #" + chName + " Channel key already set. " + CRLF
#define ERR_UNKNOWNMODE( nickname, chName, mode )	   ": 472 " + nickname + " #" + chName + " " + mode + " :is not a recognised channel mode" + CRLF
#define ERR_NEEDMOREPARAMS( nickname )				   ( ": 461 " + nickname + " :Not enough parameters." + CRLF )
#define ERR_CHANNELNOTFOUND( nickname, chName )		   ( ": 403 " + nickname + " " + chName + " :No such channel" + CRLF )
#define ERR_NOTOPERATOR( chName )					   ( ": 482 #" + chName + " :You're not a channel operator" + CRLF )
#define ERR_NOSUCHNICK( chName, name )				   ( ": 401 #" + chName + " " + name + " :No such nick/channel" + CRLF )
#define ERR_PASSWDMISMATCH( nickname )				   ( ": 464 " + nickname + ":Password incorrect !" + CRLF )
#define ERR_ALREADYREGISTERED( nickname )			   ( ": 462 " + nickname + " :You may not reregister !" + CRLF )
#define ERR_NONICKNAME( nickname )					   ( ": 431 " + nickname + " :No nickname given" + CRLF )
#define ERR_NICKNAMEINUSE( nickname )				   ( ": 433 " + nickname + " :Nickname is already in use" + CRLF )
#define ERR_ERRONEUSNICKNAME( nickname )			   ( ": 432 " + nickname + " :Erroneus nickname" + CRLF )
#define ERR_NOTREGISTERED( nickname )				   ( ": 451 " + nickname + " :You have not registered!" + CRLF )
#define ERR_UNKNOWNCOMMAND( nickname, command )		   ( ": 421 " + nickname + " " + command + " :Unknown command" + CRLF )
#define ERR_NOSUCHCHANNEL( nickname, chName )		   ( ": 403 " + nickname + " #" + chName + " :No such channel" + CRLF )
#define ERR_INVITEONLYCHAN( nickname, chName )		   ( ": 403 " + nickname + " #" + chName + " :Cannot join channel (+i)" + CRLF )
#define ERR_BADCHANNELKEY( nickname, chName )		   ( ": 475 " + nickname + " #" + chName + " :Cannot join channel (+k)" + CRLF )
#define ERR_NOTONCHANNEL( nickname, chName )		   ( ": 442 " + nickname + " #" + chName + " :You're not on that channel" + CRLF )
#define ERR_USERNOTINCHANNEL( nickname, nick, chName ) ( ": 441 " + nickname + " " + nick + " #" + chName + " :You're not on that channel" + CRLF )
#define ERR_CHANOPRIVSNEEDED( nickname, chName )	   ( ": 482 " + nickname + " #" + chName + " :You're not a channel operator" + CRLF )
#define ERR_NORECIPIENT( nickname, command )		   ( ": 411 " + nickname + " " + command + " :No recipient given" + CRLF )
#define ERR_TOOMANYTARGETS( nickname, targets )		   ( ": 407 " + nickname + " " + targets + " :Too many recipients" + CRLF )
#define ERR_CANNOTSENDTOCHAN( nickname, chName )	   ( ": 404 " + nickname + " #" + chName + " :Cannot send to channel" + CRLF )
#define ERR_NOTEXTTOSEND( nickname )				   ( ": 412 " + nickname + " :No text to send" + CRLF )
#define ERR_CHANNELISFULL( nickname, chName )		   ( ": 471 " + nickname + " #" + chName + " :Cannot join channel (+l)" + CRLF )
