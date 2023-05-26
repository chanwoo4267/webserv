#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include "Webserv.hpp"
# include "Config.hpp"
# include "Client.hpp"

class ServerManager
{
    private:
        Config                     _server_config;
        std::map<int, ServerBlock> _server_map;
        std::map<int, Client>      _client_map;

    public:
        ServerManager();
        ~ServerManager();
        ServerManager(const ServerManager &obj);
        ServerManager &operator=(const ServerManager &obj);

        Config &getServerConfig();
};

#endif