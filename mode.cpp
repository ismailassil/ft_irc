#include "ClientManager.hpp"
#include "bits.hpp"
#include "Responses.hpp"
#include "Channel.hpp"

void handleModeO(Channel& channel, const string& mode, const string& nick, int fd, Client& client) {
    Client* targetClient = (Client*)channel.getClientInChannel(nick);
    if (!targetClient) {
        ClientManager::ft_send(fd, ERR_NOSUCHNICK(channel.getName(), nick));
        return;
    }

    if (mode[0] == '+') {
        channel.addAdmin(*targetClient);
    } else {
        channel.removeAdmin(targetClient->getFd());
    }

    string reply = RPL_CHANGEMODE(client.getNickName(), channel.getName(), mode, nick);
    channel.broadcast(reply, fd);
}

void handleModeK(Channel& channel, const string& mode, const string& key, int fd, Client& client) {
    if (mode[0] == '+') {
        channel.setKey(1);
        channel.setPassword(key);
    } else {
        channel.setKey(0);
    }

    string reply = RPL_CHANGEMODE(client.getNickName(), channel.getName(), mode, key);
    channel.broadcast(reply, fd);
}

void handleModeL(Channel& channel, const string& mode, const string& limit, int fd, Client& client) {
    if (mode[0] == '+') {
        channel.setLimit(stringToInt(limit));
    } else {
        channel.setLimit(0);
    }

    string reply = RPL_CHANGEMODE(client.getNickName(), channel.getName(), mode, limit);
    channel.broadcast(reply, fd);
}

void handleModeI(Channel& channel, const string& mode, int fd, Client& client) {
    channel.setInviteOnly(mode[0] == '+');
    string reply = RPL_CHANGEMODE(client.getNickName(), channel.getName(), mode, "");
    channel.broadcast(reply, fd);
}

void handleModeT(Channel& channel, const string& mode, int fd, Client& client) {
    channel.setTopicRestrict(mode[0] == '+');
    string reply = RPL_CHANGEMODE(client.getNickName(), channel.getName(), mode, "");
    channel.broadcast(reply, fd);
}

bool isValidMode(const string& mode) {
    const string validModes = "itko";
    for (size_t i = 1; i < mode.length(); ++i) {
        if (validModes.find(mode[i]) == string::npos) {
            return false;
        }
    }
    return true;
}

void processMode(Channel& channel, const string& mode, const vector<string>& splited, int fd, Client& client) {
    if (!isValidMode(mode)) {
        ClientManager::ft_send(fd, ERR_UNKNOWNMODE(client.getNickName(), channel.getName(), mode));
        return;
    }
    for (size_t i = 1; i < mode.length(); ++i) {
        switch (mode[i]) {
            case 'o':
                if (splited.size() < 4) {
                    ClientManager::ft_send(fd, ERR_NEEDMOREPARAMS(client.getNickName()));
                } else {
                    handleModeO(channel, mode, splited[3], fd, client);
                }
                break;

            case 'k':
                if (splited.size() < 4) {
                    ClientManager::ft_send(fd, ERR_NEEDMODEPARM(channel.getName(), mode));
                } else {
                    handleModeK(channel, mode, splited[3], fd, client);
                }
                break;

            case 'l':
                if (splited.size() < 4) {
                    ClientManager::ft_send(fd, ERR_NEEDMODEPARM(channel.getName(), mode));
                } else {
                    handleModeL(channel, mode, splited[3], fd, client);
                }
                break;

            case 'i':
                handleModeI(channel, mode, fd, client);
                break;
            case 't':
                handleModeT(channel, mode, fd, client);

            default:
                ClientManager::ft_send(fd, ERR_UNKNOWNMODE(client.getNickName(), channel.getName(), mode));
                break;
        }
    }
}

void ClientManager::modeCmd(int fd, string& cmd) {
    vector<string> splited = ft_split_tokens(cmd);

    if (splited.size() < 2) {
        ClientManager::ft_send(fd, ERR_NEEDMOREPARAMS(cli[fd].getNickName()));
        return;
    }

    if (splited[1][0] != '#' && splited[1][0] != '&') {
        ClientManager::ft_send(fd, ERR_NOSUCHCHANNEL(cli[fd].getNickName(), splited[1]));
        return;
    }
    string channelName = splited[1].substr(1);
    if (!isValidChannel(splited[1])) {
        ClientManager::ft_send(fd, ERR_NOSUCHCHANNEL(cli[fd].getNickName(), splited[1]));
        return;
    }

    Channel* channel = findChannelByName(channels, channelName);
    if (!channel) {
        ClientManager::ft_send(fd, ERR_NOSUCHCHANNEL(cli[fd].getNickName(), channelName));
        return;
    }

    if (!channel->isClientInChannel(cli[fd].getNickName())) {
        ClientManager::ft_send(fd, ERR_NOTONCHANNEL(cli[fd].getNickName(), channelName));
        return;
    }

    if (splited.size() == 2) {
        printf("Channel modes: %s\n", channel->getModes().c_str());
        return;
    }
    string mode = splited[2];
    if (mode[0] != '+' && mode[0] != '-') {
        ClientManager::ft_send(fd, ERR_UNKNOWNMODE(cli[fd].getNickName(), channelName, mode));
        return;
    }
    processMode(*channel, mode, splited, fd, cli[fd]);
}
