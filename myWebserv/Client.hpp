#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Webserv.hpp"
# include "Config.hpp"

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

        void setSocket(int &socket);
        void setAddress(struct sockaddr_in &address);
        void setServer(Config &config);

        void buildResponse();
        void updateTime();
        void clearClient();

        const int &getSocket();
        const struct sockaddr_in &getAddress();
        const time_t &getLastCommuTime();
};

#endif