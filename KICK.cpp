#include "ClientManager.hpp"
#include "bits.hpp"

void    ClientManager::JOIN(int fd, std::string cmd)
{
    std::string channel = cmd.substr(5);
    if (channel[0] != '#')
    {
        ft_send(fd, "403 ERR_NOSUCHCHANNEL " + channel + " :No such channel\r\n");
        return;
    }
    if (cli[fd].getRegistered() == false)
    {
        ft_send(fd, "451 ERR_NOTREGISTERED :You have not registered\r\n");
        return;
    }
    if (cli[fd].getLoggedIn() == false)
    {
        ft_send(fd, "451 ERR_NOTREGISTERED :You have not logged in\r\n");
        return;
    }
    if (cli[fd].getInviteChannel(channel) == false)
    {
        ft_send(fd, "473 ERR_INVITEONLYCHAN " + channel + " :Cannot join channel (+i)\r\n");
        return;
    }
    if (cli[fd].getChannel(channel) == true)
    {
        ft_send(fd, "405 ERR_TOOMANYCHANNELS " + channel + " :You have joined too many channels\r\n");
        return;
    }
    cli[fd].addChannel(channel);
    ft_send(fd, ":" + cli[fd].getNickName() + " JOIN " + channel + "\r\n");
    ft_send(fd, ":" + cli[fd].getNickName() + " MODE " + channel + " +o " + cli[fd].getNickName() + "\r\n");
    ft_send(fd, ":" + cli[fd].getNickName() + " MODE " + channel + " +v " + cli[fd].getNickName() + "\r\n");
    ft_send(fd, ":" + cli[fd].getNickName() + " MODE " + channel + " +t\r\n");
    ft_send(fd, ":" + cli[fd].getNickName() + " MODE " + channel + " +n\r\n");
    ft_send(fd, ":" + cli[fd].getNickName() + " MODE " + channel + " +m\r\n");
    ft_send(fd, ":" + cli[fd].getNickName() + " MODE " + channel + " +l 50\r\n");
    ft_send(fd, ":" + cli[fd].getNickName() + " MODE " + channel + " +k 1234\r\n");
    // ft_send(fd, ":" + cli[fd
}