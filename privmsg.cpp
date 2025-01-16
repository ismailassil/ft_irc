#include "ClientManager.hpp"
#include "bits.hpp"

void ClientManager::privmsg(int fd, string& input) {
    vector<string> tokens = ft_split_tokens(input);

    if (tokens.size() < 2)
        return ft_send(fd, ERR_NEEDMOREPARAMS(cli[fd].getNickName()));
    if (tokens.size() != 3)
        return ft_send(fd, ERR_NOTEXTTOSEND(cli[fd].getNickName()));

    string message = "";
    for (size_t i = 2; i < tokens.size(); i++) {
        if (i == 2 && tokens[i][0] == ':')
            tokens[i] = tokens[i].substr(1);
        else {
            message = tokens[i];
            break ;
        }
        message += tokens[i];
        if (i + 1 < tokens.size())
            message += " ";
    }

    vector<string> target_tokens = splitString(tokens[1], ',');
    if (target_tokens.size() > 6)
        return ft_send(fd, ERR_TOOMANYTARGETS(cli[fd].getNickName(), tokens[1]));
    for (size_t i = 0; i < target_tokens.size(); i++) {
        string target = target_tokens[i];
        if (target.empty()) {
            ft_send(fd, ERR_NORECIPIENT(cli[fd].getNickName(), tokens[0]));
            continue;
        }
        if (target[0] == '#' || target[0] == '&') {
            const Channel* channel = getChannel(target.substr(1));
            if (!channel) {
                ft_send(fd, ERR_NOSUCHCHANNEL(cli[fd].getNickName(), target));
                continue;
            }
            if (!channel->isInChannel(cli[fd].getNickName())) {
                ft_send(fd, ERR_CANNOTSENDTOCHAN(cli[fd].getNickName(), target));
                continue;
            }
            string reply = ":" + getPrefix(fd) + " PRIVMSG " + target + " :" + message + CRLF;
            channel->broadcast(reply, fd);
        } else {
            const Client* client = getClient(target);
            if (!client) {
                ft_send(fd, ERR_NOSUCHNICK(cli[fd].getNickName(), target));
                continue;
            }
            ft_send(fd, ":" + cli[fd].getNickName() + " PRIVMSG " + target + " :" + message + CRLF);
        }
    }
}
