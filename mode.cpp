#include <cstddef>

#include "Channel.hpp"
#include "ClientManager.hpp"
#include "Responses.hpp"
#include "bits.hpp"

std::string displayChannelModes( const std::string&				   clientNickName,
                                const std::string&				   channelName,
                                const std::string&				   modes,
                                const std::vector< std::string >& modeParams ) {
	std::ostringstream response;
	response << ":server 324 " << clientNickName << " " << channelName << " ";
	response << modes;
    for ( std::vector<std::string>::const_iterator it = modeParams.begin(); it != modeParams.end(); ++it ) {
        response << " " << *it;
    }
	return response.str();
}

void handleModeO( Channel& channel, char sign, const string& nick, int fd, Client& client ) {
	Client* targetClient = (Client*)channel.getClientInChannel( nick );
	if ( !targetClient ) {
		ClientManager::ft_send( fd, ERR_NOSUCHNICK( channel.getName(), nick ) );
		return;
	}

	if ( sign == '+' ) {
		channel.addAdmin( *targetClient );
	} else {
		channel.removeAdmin( targetClient->getFd() );
	}

	string mode	 = sign == '+' ? "+o" : "-o";
	string reply = RPL_CHANGEMODE( client.getNickName(), channel.getName(), mode, nick );
	channel.broadcast( reply, fd );
}

void handleModeK( Channel& channel, char sign, const string& key, int fd, Client& client ) {
	if ( sign == '+' ) {
		channel.setKey( 1 );
		channel.setPassword( key );
	} else {
		channel.setKey( 0 );
	}
	string mode	 = sign == '+' ? "+k" : "-k";
	string reply = sign == '+'
					   ? RPL_CHANGEMODE( client.getNickName(), channel.getName(), mode, "*****" )
					   : RPL_CHANGEMODE( client.getNickName(), channel.getName(), mode, "" );

	channel.broadcast( reply, fd );
}

void handleModeL( Channel& channel, char sign, const string& limit, int fd, Client& client ) {
	if ( sign == '+' && !isNumber( limit ) ) {
		ClientManager::ft_send( fd, ERR_NEEDMODEPARM( channel.getName(), "l" ) );
		return;
	}
	if ( sign == '+' ) {
        if ( channel.getNumberOfClients() > channel.getLimit() ) {
            ClientManager::ft_send( fd, ERR_CHANNELISFULL( client.getNickName(), channel.getName() ) );
            return;
        }
		channel.setLimit( stringToInt( limit ) );
		channel.setModeAtIndex( 1, true );
	} else {
		channel.setLimit( 0 );
		channel.setModeAtIndex( 1, false );
	}
	string mode	 = sign == '+' ? "+l" : "-l";
	string reply = RPL_CHANGEMODE( client.getNickName(), channel.getName(), mode, limit );
	channel.broadcast( reply, fd );
}

void handleModeI( Channel& channel, char sign, int fd, Client& client ) {
	channel.setInviteOnly( sign == '+' );
	channel.setModeAtIndex( 2, sign == '+' );
	string mode	 = sign == '+' ? "+i" : "-i";
	string reply = RPL_CHANGEMODE( client.getNickName(), channel.getName(), mode, "" );
	channel.broadcast( reply, fd );
}

void handleModeT( Channel& channel, char sign, int fd, Client& client ) {
	channel.setTopicRestrict( sign == '+' );
	channel.setModeAtIndex( 3, sign == '+' );
	string mode	 = sign == '+' ? "+t" : "-t";
	string reply = RPL_CHANGEMODE( client.getNickName(), channel.getName(), mode, "" );
	channel.broadcast( reply, fd );
}

void processMode( vector< string > splited, Channel& channel, int fd, Client& cli ) {
	string mode = splited[2];
    for ( size_t i = 0; i < mode.length(); ++i ) {
        char c = mode[i];
        if ( c != '+' && c != '-' && string( "oklit" ).find( c ) == string::npos ) {
            ClientManager::ft_send( fd, ERR_UNKNOWNMODE( cli.getNickName(), channel.getName(), mode ) );
            return;
        }
    }
	char sign = mode[0];
	if ( sign != '+' && sign != '-' ) {
		ClientManager::ft_send( fd, ERR_UNKNOWNMODE( cli.getNickName(), channel.getName(), mode ) );
		return;
	}
	vector< string > params = splited.size() > 3 ? vector< string >( splited.begin() + 3, splited.end() ) : vector< string >();
	size_t			 j		= 0;
	for ( size_t i = 1; i < mode.length(); ++i ) {
		if ( mode[i] == '+' || mode[i] == '-' ) {
			sign = mode[i];
			continue;
		}
		switch ( mode[i] ) {
			case 'o':
				if ( params.size() < j + 1 ) {
					ClientManager::ft_send( fd, ERR_NEEDMOREPARAMS( cli.getNickName() ) );
				} else {
					handleModeO( channel, sign, params[j], fd, cli );
					j++;
				}
				break;
			case 'k':
				if ( params.size() < j + 1 ) {
					ClientManager::ft_send( fd, ERR_NEEDMODEPARM( channel.getName(), mode ) );
				} else {
					handleModeK( channel, sign, params[j], fd, cli );
				}
				j++;
				break;
			case 'l':
				if ( params.size() < j + 1 ) {
					ClientManager::ft_send( fd, ERR_NEEDMODEPARM( channel.getName(), mode ) );
				} else {
					handleModeL( channel, sign, params[j], fd, cli );
					j++;
				}
				break;
			case 'i':
				handleModeI( channel, sign, fd, cli );
				break;
			case 't':
				handleModeT( channel, sign, fd, cli );
				break;
			default:
				ClientManager::ft_send( fd, ERR_UNKNOWNMODE( cli.getNickName(), channel.getName(), mode ) );
				break;
		}
	}
}

void ClientManager::modeCmd( int fd, string& cmd ) {
	vector< string > splited = ft_split_tokens( cmd );

	if ( splited.size() < 2 ) {
		ClientManager::ft_send( fd, ERR_NEEDMOREPARAMS( cli[fd].getNickName() ) );
		return;
	}

	if ( splited[1][0] != '#' && splited[1][0] != '&' ) {
		ClientManager::ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), splited[1] ) );
		return;
	}
	string channelName = splited[1].substr( 1 );
	if ( !isValidChannel( splited[1] ) ) {
		ClientManager::ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), splited[1] ) );
		return;
	}

	Channel* channel = findChannelByName( channels, channelName );
	if ( !channel ) {
		ClientManager::ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), channelName ) );
		return;
	}

	if ( !channel->isClientInChannel( cli[fd].getNickName() ) ) {
		ClientManager::ft_send( fd, ERR_NOTONCHANNEL( cli[fd].getNickName(), channelName ) );
		return;
    }

    if ( splited.size() == 2 ) {
        std::vector<std::string> emptyParams;
        ClientManager::ft_send( fd, displayChannelModes( cli[fd].getNickName(), channelName, channel->getModes(), emptyParams ) );
        return;
    }
	processMode( splited, *channel, fd, cli[fd] );
	// display the channels with their modes
	for ( size_t i = 0; i < channels.size(); i++ ) {
		printf( "Channel: %s, modes: %s\n", channels[i].getName().c_str(), channels[i].getModes().c_str() );
	}
}
