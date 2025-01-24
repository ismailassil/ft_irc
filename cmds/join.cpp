#include "../headers/ClientManager.hpp"

bool isValidChannelName(const string& channel) {
    return !channel.empty() && (channel[0] == '#' || channel[0] == '&') && channel != "#" && channel != "&";
}

void createAndJoinChannel(vector<Channel>& channels, Client& client, int fd, const string& name, const string& password = "") {
    Channel newChannel;
    newChannel.setName(name);
    if (!password.empty()) {
        newChannel.setPassword(password);
        newChannel.setKey(1);
        newChannel.setModeAtIndex('k', true);
    }
    newChannel.addClient(client);
    newChannel.addAdmin(client);
	newChannel.setCreationDate();
	newChannel.setTopic("");
    channels.push_back(newChannel);
    newChannel.broadcast(RPL_JOINMSG(client.getNickName(), client.getUserName(), client.getIpAdd(), newChannel.getName()));
	ft_send(fd, RPL_NAMREPLY(client.getNickName(), newChannel.getName(), newChannel.getClientChannelList()));
    ft_send(fd, RPL_ENDOFNAMES(client.getNickName(), newChannel.getName()));
}

void handleExistingChannel(Channel& channel, Client& client, int fd, const string& password) {
    if (channel.isClientInChannel(client.getNickName())) {
        return;
    }

    if (channel.getInviteOnly() && !client.getInviteChannel(channel.getName())) {
        ft_send(fd, ERR_INVITEONLYCHAN(client.getNickName(), channel.getName()));
        return;
    }

    if (channel.getKey() && channel.getPassword() != password) {
        ft_send(fd, ERR_BADCHANNELKEY(client.getNickName(), channel.getName()));
        return;
    }

    if (channel.getLimit() != 0 && channel.getNumberOfClients() >= channel.getLimit()) {
        ft_send(fd, ERR_CHANNELISFULL(client.getNickName(), channel.getName()));
        return;
    }

    channel.addClient(client);
    channel.broadcast(RPL_JOINMSG(client.getNickName(), client.getUserName(), client.getIpAdd(), channel.getName()));
    if (!channel.getTopic().empty()) {
        ft_send(fd, RPL_TOPIC(client.getNickName(), channel.getName(), channel.getTopic()));
    }
    ft_send(fd, RPL_NAMREPLY(client.getNickName(), channel.getName(), channel.getClientChannelList()));
    ft_send(fd, RPL_ENDOFNAMES(client.getNickName(), channel.getName()));
}

void ClientManager::joinCmd(int fd, string& cmd) {
    vector<string> splited = ft_split_tokens(cmd);

    if (splited.size() < 2)
        return ft_send(fd, ERR_NEEDMOREPARAMS(cli[fd].getNickName()));
    vector<string> chans = splitString(splited[1], ',');
    vector<string> passwords = (splited.size() == 3) ? splitString(splited[2], ',') : vector<string>();

    for (size_t i = 0, j = 0; i < chans.size(); ++i) {

        if (!isValidChannelName(chans[i])) {
            ft_send(fd, ERR_NOSUCHCHANNEL(cli[fd].getNickName(), chans[i]));
            j++;
            continue;
        }

        string chanName = chans[i].substr(1);
        Channel* channel = findChannelByName(channels, chanName);
        if (!channel) {
            string password = (j < passwords.size()) ? passwords[j] : "";
			createAndJoinChannel(channels, cli[fd], fd, chanName, password);
        } else {
            string password = (j < passwords.size()) ? passwords[j] : "";
            handleExistingChannel(*channel, cli[fd], fd, password);
        }
        j++;
    }
}
