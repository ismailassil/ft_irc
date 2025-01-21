#include "channel.hpp"

Channel::Channel()
{
    invit_only = 0;
    topic = "";
    key = 0;
    limit = 0;
    name = "";
    password = "";
}

Channel::~Channel()
{
}

Channel::Channel(Channel const &src)
{
    *this = src;
}

Channel &Channel::operator=(Channel const &src)
{
    invit_only = src.invit_only;
    topic = src.topic;
    key = src.key;
    limit = src.limit;
    name = src.name;
    password = src.password;
    clients = src.clients;
    admins = src.admins;
    modes = src.modes;
    return *this;
}

void Channel::SetInvitOnly(int invit_only)
{
    this->invit_only = invit_only;
}

void Channel::SetTopic(std::string topic)
{
    this->topic = topic;
}

void Channel::SetKey(int key)
{
    this->key = key;
}

void Channel::SetLimit(int limit)
{
    this->limit = limit;
}

void Channel::SetPassword(std::string password)
{
    this->password = password;
}

void Channel::SetName(std::string name)
{
    this->name = name;
}

void Channel::setModeAtindex(size_t index, bool mode)
{
    modes[index].second = mode;
}

int Channel::GetInvitOnly()
{
    return invit_only;
}

std::string Channel::GetTopic()
{
    return topic;
}

int Channel::GetKey()
{
    return key;
}

int Channel::GetLimit()
{
    return limit;
}

int Channel::GetClientsNumber()
{
    return clients.size();
}

bool Channel::getModeAtindex(size_t index)
{
    return modes[index].second;
}

bool Channel::clientInChannel(std::string &nick)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].GetNickName() == nick)
            return true;
    }
    return false;
}

std::string Channel::GetPassword()
{
    return password;
}

std::string Channel::GetName()
{
    return name;
}

std::string Channel::getModes()
{
    std::string modes_str = "";
    for (size_t i = 0; i < modes.size(); i++)
    {
        if (modes[i].second)
            modes_str += modes[i].first;
    }
    return modes_str;
}

std::string Channel::clientChannel_list()
{
    std::string list = "";
    for (size_t i = 0; i < clients.size(); i++)
    {
        list += clients[i].GetNickName();
        if (i + 1 < clients.size())
            list += " ";
    }
    return list;
}

Client *Channel::get_client(int fd)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].GetFd() == fd)
            return &clients[i];
    }
    return NULL;
}

Client *Channel::get_admin(int fd)
{
    for (size_t i = 0; i < admins.size(); i++)
    {
        if (admins[i].GetFd() == fd)
            return &admins[i];
    }
    return NULL;
}

Client *Channel::GetClientInChannel(std::string name)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].GetNickName() == name)
            return &clients[i];
    }
    return NULL;
}

void Channel::add_client(Client newClient)
{
    clients.push_back(newClient);
}

void Channel::add_admin(Client newClient)
{
    admins.push_back(newClient);
}

void Channel::remove_client(int fd)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].GetFd() == fd)
        {
            clients.erase(clients.begin() + i);
            return;
        }
    }
}

void Channel::remove_admin(int fd)
{
    for (size_t i = 0; i < admins.size(); i++)
    {
        if (admins[i].GetFd() == fd)
        {
            admins.erase(admins.begin() + i);
            return;
        }
    }
}

bool Channel::change_clientToAdmin(std::string &nick)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].GetNickName() == nick)
        {
            admins.push_back(clients[i]);
            clients.erase(clients.begin() + i);
            return true;
        }
    }
    return false;
}

bool Channel::change_adminToClient(std::string &nick)
{
    for (size_t i = 0; i < admins.size(); i++)
    {
        if (admins[i].GetNickName() == nick)
        {
            clients.push_back(admins[i]);
            admins.erase(admins.begin() + i);
            return true;
        }
    }
    return false;
}

void Channel::sendTo_all(std::string rpl1)
{
    (void)rpl1;
}

void Channel::sendTo_all(std::string rpl1, int fd)
{
    (void)rpl1;
    (void)fd;
}