#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Webserv.hpp"

class Client
{
    private:
        int                 _socket;
        struct sockaddr_in  _address;
        time_t              _last_commu_time;
    
    public:
        Client();
        ~Client();
        Client(const Client &obj);
        Client &operator=(const Client &obj);

        const int &getSocket();
        const struct sockaddr_in &getAddress();
        const time_t &getLastCommuTime();
};

#endif