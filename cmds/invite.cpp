#include "../headers/ClientManager.hpp"

void    ClientManager::inviteCmd(int fd, string& cmd) {
    vector<string> splited = ft_split_tokens(cmd);

    if (splited.size() < 3) {
        ft_send(fd, ERR_NEEDMOREPARAMS(cli[fd].getNickName()));
        return;
    }

    if (splited[2][0] != '#' && splited[2][0] != '&') {
        ft_send(fd, ERR_NOSUCHCHANNEL(cli[fd].getNickName(), splited[2]));
        return;
    }
    string channelName = splited[2].substr(1);
    if (!isChannel(splited[2])) {
        ft_send(fd, ERR_NOSUCHCHANNEL(cli[fd].getNickName(), splited[2]));
        return;
    }

    Channel* channel = findChannelByName(channels, channelName);
    if (!channel) {
        ft_send(fd, ERR_NOSUCHCHANNEL(cli[fd].getNickName(), channelName));
        return;
    }

    if (!channel->isClientInChannel(cli[fd].getNickName())) {
        ft_send(fd, ERR_NOTONCHANNEL(cli[fd].getNickName(), channelName));
        return;
    }

    if (!channel->isAdminInChannel(cli[fd].getNickName())) {
        ft_send(fd, ERR_NOTOPERATOR(channelName));
        return;
    }

    Client* targetClient = getClient(splited[1]);

    if (!targetClient)
    {
        ft_send(fd, ERR_NOSUCHNICK( ("#" + channelName), splited[1]));
        return ;
    }

    if (targetClient->getNickName() == cli[fd].getNickName()) {
        return;
    }

    targetClient->addInvitedChannel(channelName);
    string reply = RPL_INVITING(cli[fd].getNickName(), targetClient->getNickName(), channelName);
    ft_send(targetClient->getFd(), reply);

    reply = RPL_INVITING(getPrefix(fd), targetClient->getNickName(), channelName);
    channel->broadcast(reply);
}
