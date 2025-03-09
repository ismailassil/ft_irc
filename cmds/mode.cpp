
#include "../headers/ClientManager.hpp"

string displayChannelModes( const string& clientNickName, const Channel& channel ) {
	string mode = channel.getModes();
	string args = "";
	if ( channel.getModeAtIndex( 'k' ) ) {
		args += channel.getPassword() + " ";
	}
	if ( channel.getModeAtIndex( 'l' ) ) {
		args += intToString( channel.getLimit() ) + " ";
	}
	if (channel.getAdminChannelList().size() > 0) {
		args += channel.getAdminChannelList() + " ";
	}
	return RPL_CHANNELMODEIS( clientNickName, channel.getName(), mode, args );
}

void handleModeO( Channel& channel, char sign, const string& nick, int fd, string& replyPrefix ) {
	Client* targetClient = (Client*)channel.getClientInChannel( nick );
	if ( !targetClient ) {
		ft_send( fd, ERR_NOTONCHANNEL(targetClient->getNickName(), channel.getName()) );
		return;
	}

	if ( sign == '+' && !channel.isAdminInChannel( targetClient->getNickName() ) ) {
		channel.addAdmin( *targetClient );
	} else {
		channel.removeAdmin( targetClient->getFd() );
	}

	string mode	 = sign == '+' ? "+o" : "-o";
	string reply = RPL_CHANGEMODE( replyPrefix, channel.getName(), mode, targetClient->getNickName() );
	channel.broadcast( reply );
}

void handleModeK( Channel& channel, char sign, const string& key, int fd, string& replyPrefix ) {
	if ( sign == '+' ) {
		if (!isValidPassword( key )) {
			ft_send( fd, ERR_INVALIDMODEPARM( channel.getName(), "k" ) );
			return;
		}
		channel.setKey( 1 );
		channel.setPassword( key );
	} else {
		channel.setKey( 0 );
	}
	channel.setModeAtIndex( 'k', sign == '+' );
	string mode	 = sign == '+' ? "+k" : "-k";
	string reply = sign == '+' ? RPL_CHANGEMODE( replyPrefix, channel.getName(), mode, key ) : RPL_CHANGEMODE( replyPrefix, channel.getName(), mode, "" );

	channel.broadcast( reply );
}

void handleModeL( Channel& channel, char sign, const string& limit, int fd, Client& client, string& replyPrefix ) {
	if ( sign == '+' && !isNumber( limit ) ) {
		ft_send( fd, ERR_NEEDMOREPARAMS( client.getNickName() ) );
		return;
	}
	if ( sign == '+' ) {
		int limitInt = stringToInt( limit );
		if ( channel.getNumberOfClients() > limitInt ) {
			ft_send( fd, ERR_INVALIDMODEPARM( channel.getName(), "l" ) );
			return;
		}
		channel.setLimit( stringToInt( limit ) );
	} else {
		channel.setLimit( 0 );
	}
	channel.setModeAtIndex( 'l', sign == '+' );
	string mode	 = sign == '+' ? "+l " + limit : "-l";
	string reply = RPL_CHANGEMODE( replyPrefix, channel.getName(), mode, "" );
	channel.broadcast( reply );
}

void handleModeI( Channel& channel, char sign, string& replyPrefix ) {
	channel.setInviteOnly( sign == '+' );
	channel.setModeAtIndex( 'i', sign == '+' );
	string mode	 = sign == '+' ? "+i" : "-i";
	string reply = RPL_CHANGEMODE( replyPrefix, channel.getName(), mode, "" );
	channel.broadcast( reply );
}

void handleModeT( Channel& channel, char sign, string& replyPrefix ) {
	channel.setTopicRestrict( sign == '+' );
	channel.setModeAtIndex( 't', sign == '+' );
	string mode	 = sign == '+' ? "+t" : "-t";
	string reply = RPL_CHANGEMODE( replyPrefix, channel.getName(), mode, "" );
	channel.broadcast( reply );
}

void processMode( vector< string > splited, Channel& channel, int fd, Client& cli, string replyPrefix ) {
	string mode = splited[2];
	for ( size_t i = 0; i < mode.length(); ++i ) {
		char c = mode[i];
		if ( c != '+' && c != '-' && string( "oklit" ).find( c ) == string::npos ) {
			ft_send( fd, ERR_UNKNOWNMODE( cli.getNickName(), channel.getName(), mode ) );
			return;
		}
	}
	char sign = mode[0];
	if ( sign != '+' && sign != '-' ) {
		ft_send( fd, ERR_UNKNOWNMODE( cli.getNickName(), channel.getName(), mode ) );
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
				if ( sign == '+' && params.size() < j + 1 ) {
					ft_send( fd, ERR_NEEDMOREPARAMS( cli.getNickName() ) );
				} else {
					handleModeO( channel, sign, params[j], fd, replyPrefix );
					j++;
				}
				break;
			case 'k':
				if ( sign == '+' && params.size() < j + 1  ) {
					ft_send( fd, ERR_NEEDMOREPARAMS( cli.getNickName() ) );
				} else {
					handleModeK( channel, sign, params[j], fd, replyPrefix );
					j++;
				}
				break;
			case 'l':
				if ( sign == '+' && params.size() < j + 1 ) {
					ft_send( fd, ERR_NEEDMOREPARAMS( cli.getNickName() ) );
				} else {
					handleModeL( channel, sign, params[j], fd, cli, replyPrefix );
					j++;
				}
				break;
			case 'i':
				handleModeI( channel, sign, replyPrefix );
				break;
			case 't':
				handleModeT( channel, sign, replyPrefix );
				break;
			default:
				ft_send( fd, ERR_UNKNOWNMODE( cli.getNickName(), channel.getName(), mode ) );
				break;
		}
	}
}

void ClientManager::modeCmd( int fd, string& cmd ) {
	vector< string > splited = ft_split_tokens( cmd );

	if ( splited.size() < 2 ) {
		ft_send( fd, ERR_NEEDMOREPARAMS( cli[fd].getNickName() ) );
		return;
	}

	if ( splited[1][0] != '#' && splited[1][0] != '&' ) {
		ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), splited[1] ) );
		return;
	}
	string channelName = splited[1];
	if ( !isChannel( splited[1] ) ) {
		ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), channelName ) );
		return;
	}

	Channel* channel = findChannelByName( channels, channelName );
	if ( !channel ) {
		ft_send( fd, ERR_NOSUCHCHANNEL( cli[fd].getNickName(), channelName ) );
		return;
	}

	if ( !channel->isClientInChannel( cli[fd].getNickName() ) ) {
		ft_send( fd, ERR_NOTONCHANNEL( cli[fd].getNickName(), channelName ) );
		return;
	}

	if ( splited.size() == 2 ) {
		ft_send( fd, displayChannelModes( cli[fd].getNickName(), *channel ) );
		ft_send( fd, RPL_CREATIONTIME( cli[fd].getNickName(), channelName, channel->getCreationDate() ) );
		return;
	}

	if ( !channel->isAdminInChannel( cli[fd].getNickName() ) ) {
		ft_send( fd, ERR_CHANOPRIVSNEEDED( cli[fd].getNickName(), channelName ) );
		return;
	}

	processMode( splited, *channel, fd, cli[fd], cli[fd].getNickName() + "!" + cli[fd].getUserName() );
}
