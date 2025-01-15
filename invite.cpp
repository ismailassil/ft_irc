#include "ClientManager.hpp"
#include "bits.hpp"
#include "Responses.hpp"
#include "Channel.hpp"

void    ClientManager::inviteCmd(int fd, string& cmd) {
    vector<string> splited = ft_split_tokens(cmd);

    if (splited.size() < 3) {
        ClientManager::ft_send(fd, ERR_NEEDMOREPARAMS(cli[fd].getNickName()));
        return;
    }

    if (splited[2][0] != '#' && splited[2][0] != '&') {
        ClientManager::ft_send(fd, ERR_NOSUCHCHANNEL(cli[fd].getNickName(), splited[2]));
        return;
    }
    string channelName = splited[2].substr(1);
    if (!isValidChannel(splited[2])) {
        ClientManager::ft_send(fd, ERR_NOSUCHCHANNEL(cli[fd].getNickName(), splited[2]));
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

    if (!channel->isAdminInChannel(cli[fd].getNickName())) {
        ClientManager::ft_send(fd, ERR_NOTOPERATOR(channelName));
        return;
    }

    Client* targetClient = (Client*)channel->getClientInChannel(splited[1]);
    if (!targetClient) {
        ClientManager::ft_send(fd, ERR_NOSUCHNICK(channelName, splited[1]));
        return;
    }

    if (targetClient->getNickName() == cli[fd].getNickName()) {
        ClientManager::ft_send(fd, ERR_NOSUCHNICK(channelName, splited[1]));
        return;
    }

    string reply = RPL_INVITING(cli[fd].getNickName(), targetClient->getNickName(), channelName);
    ClientManager::ft_send(targetClient->getFd(), reply);
    reply = RPL_INVITING(cli[fd].getNickName(), targetClient->getNickName(), channelName);
    channel->broadcast(reply, fd);
}