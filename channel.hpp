#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <string>
#include <vector>
#include <utility>

class Channel
{
private:
    int invit_only;
    std::string topic;
    int key;
    int limit;
    std::string name;
    std::string password;
    std::vector<Client> clients;
    std::vector<Client> admins;
    std::vector<std::pair<char, bool> > modes;

public:
    Channel();
    ~Channel();
    Channel(Channel const &src);
    Channel &operator=(Channel const &src);

    // Setters
    void SetInvitOnly(int invit_only);
    void SetTopic(std::string topic);
    void SetKey(int key);
    void SetLimit(int limit);
    void SetPassword(std::string password);
    void SetName(std::string name);
    void setModeAtindex(size_t index, bool mode);

    // Getters
    int GetInvitOnly();
    std::string GetTopic();
    int GetKey();
    int GetLimit();
    int GetClientsNumber();
    bool getModeAtindex(size_t index);
    bool clientInChannel(std::string &nick);
    std::string GetPassword();
    std::string GetName();
    std::string getModes();
    std::string clientChannel_list();
    Client *get_client(int fd);
    Client *get_admin(int fd);
    Client *GetClientInChannel(std::string name);

    // Methods
    void add_client(Client newClient);
    void add_admin(Client newClient);
    void remove_client(int fd);
    void remove_admin(int fd);
    bool change_clientToAdmin(std::string &nick);
    bool change_adminToClient(std::string &nick);

    // Send to All
    void sendTo_all(std::string rpl1);
    void sendTo_all(std::string rpl1, int fd);
};

#endif
