#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>

class Client
{
private:
    int fd;
    bool registered;
    std::string nickname;
    bool logedin;
    std::string username;
    std::string buffer;
    std::string ipadd;
    std::vector<std::string> ChannelsInvite;
    std::vector<std::pair<std::string, bool> > channels;

public:
    // Constructors and Destructor
    Client();
    Client(std::string nickname, std::string username, int fd);
    ~Client();
    Client(Client const &src);
    Client &operator=(Client const &src);

    // Getters
    int GetFd();
    bool getRegistered();
    bool GetInviteChannel(std::string &ChName);
    std::string GetNickName();
    bool GetLogedIn();
    std::string GetUserName();
    std::string getIpAdd();
    std::string getBuffer();

    // Setters
    void SetFd(int fd);
    void SetNickname(std::string &nickName);
    void setLogedin(bool value);
    void SetUsername(std::string &username);
    void setBuffer(std::string received);
    void setRegistered(bool value);
    void setIpAdd(std::string ipadd);

    // Methods
    void addChannel(std::string channelName);
    void removeChannel(std::string channelName);
    void addInviteChannel(std::string channelName);
    void removeInviteChannel(std::string channelName);
    void clearChannels();
    
};

#endif
