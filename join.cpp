#include "ClientManager.hpp"
#include "bits.hpp"
#include "Responses.hpp"
#include "Channel.hpp"

void ClientManager::joinCmd(int fd, string& cmd)
{
    cout << "joinCmd" << endl;
    vector<string> passwords;
    bool hasPasswords = false;
    vector<string> splited = ft_split_tokens(cmd);
    if (splited.size() < 2)
    {
        ft_send( fd, ERR_NEEDMOREPARAMS( string( "*" ) ) );
        return;
    }
    hasPasswords = splited.size() == 3;
    // splited[0]: cmd
    // splited[1]: channels
    // splited[2]: passwords
    vector<string> chans = splitString(splited[1], ',');
    if (hasPasswords)
        passwords = splitString(splited[2], ',');
    int j = 0;
    for (size_t i = 0; i < chans.size(); i++)
    {
        if (chans[i][0] != '#' || chans[i][0] != '&')
            return ft_send(fd, ERR_NOSUCHCHANNEL(cli[fd].getNickName(), chans[i]));
        // check if channel already exists
        bool exists = false;
        for (size_t k = 0; k < channels.size(); k++)
        {
            if (channels[k].getName() == chans[i])
            {
                exists = true;
                break;
            }
        }
        if (!exists)
        {
            Channel newChannel;
            newChannel.setName(chans[i]);
            if (hasPasswords)
                newChannel.setPassword(passwords[j]);
            newChannel.addClient(cli[fd]);
            newChannel.addAdmin(cli[fd]);
            channels.push_back(newChannel);
            j++;
        }
        else
        {
            for (size_t k = 0; k < channels.size(); k++)
            {
                if (channels[k].getName() == chans[i])
                {
                    if (channels[k].isClientInChannel(cli[fd].getNickName()))
                        return;
                    break;
                }
            }
            // join client to channel if not invite only and password is correct
            for (size_t k = 0; k < channels.size(); k++)
            {
                if (channels[k].getName() == chans[i])
                {
                    if (channels[k].getInviteOnly() && !channels[k].getModeAtIndex(0))
                        return ft_send(fd, ERR_INVITEONLYCHAN(string("*"), chans[i]));
                    if (hasPasswords && channels[k].getPassword() != passwords[j])
                    {
                        ft_send(fd, ERR_BADCHANNELKEY(string("*"), chans[i]));
                        return;
                    }
                    channels[k].addClient(cli[fd]);
                    string joinMsg = ":" + cli[fd].getNickName() + " JOIN :" + channels[k].getName() + "\r\n";
                    ft_send(fd, joinMsg);
                    channels[k].broadcast(joinMsg, fd);
                    string topic = channels[k].getTopic();
                    if (!topic.empty()) {
                        ft_send(fd, RPL_TOPIC(cli[fd].getNickName(), channels[k].getName(), topic));
                    }
                    string names = channels[k].getClientChannelList(); // collect all members’ nicks
                    ft_send(fd, RPL_NAMREPLY(cli[fd].getNickName(), channels[k].getName(), names));
                    // 5. Send RPL_ENDOFNAMES (366)
                    ft_send(fd, RPL_ENDOFNAMES(cli[fd].getNickName(), channels[k].getName()));

                    return;
                }
            }
        }
    }
}