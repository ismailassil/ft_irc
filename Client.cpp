#include "Client.hpp"
#include <algorithm>

Client::Client()
    : fd(-1), registered(false), logedin(false), nickname(""), username(""), buffer(""), ipadd("") {}

Client::Client(std::string nickname, std::string username, int fd)
    : fd(fd), registered(false), logedin(false), nickname(nickname), username(username), buffer(""), ipadd("") {}

Client::~Client() {}

Client::Client(Client const &src)
{
    *this = src;
}

Client &Client::operator=(Client const &src)
{
    if (this != &src)
    {
        this->fd = src.fd;
        this->registered = src.registered;
        this->nickname = src.nickname;
        this->logedin = src.logedin;
        this->username = src.username;
        this->buffer = src.buffer;
        this->ipadd = src.ipadd;
        this->ChannelsInvite = src.ChannelsInvite;
        this->channels = src.channels;
    }
    return *this;
}

int Client::GetFd()
{
    return fd;
}

bool Client::getRegistered()
{
    return registered;
}

bool Client::GetInviteChannel(std::string &ChName)
{
    return std::find(ChannelsInvite.begin(), ChannelsInvite.end(), ChName) != ChannelsInvite.end();
}

std::string Client::GetNickName()
{
    return nickname;
}

bool Client::GetLogedIn()
{
    return logedin;
}

std::string Client::GetUserName()
{
    return username;
}

std::string Client::getIpAdd()
{
    return ipadd;
}

std::string Client::getBuffer()
{
    return buffer;
}

void Client::SetFd(int fd)
{
    this->fd = fd;
}

void Client::SetNickname(std::string &nickName)
{
    this->nickname = nickName;
}

void Client::setLogedin(bool value)
{
    this->logedin = value;
}

void Client::SetUsername(std::string &username)
{
    this->username = username;
}

void Client::setBuffer(std::string received)
{
    this->buffer = received;
}

void Client::setRegistered(bool value)
{
    this->registered = value;
}

void Client::setIpAdd(std::string ipadd)
{
    this->ipadd = ipadd;
}
