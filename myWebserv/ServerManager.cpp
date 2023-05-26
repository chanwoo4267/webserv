#include "ServerManager.hpp"

ServerManager::ServerManager()
{

}

ServerManager::~ServerManager() {}



Config &ServerManager::getServerConfig()
{
    return this->_server_config;
}