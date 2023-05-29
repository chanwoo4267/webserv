#include "Server.hpp"

Server::Server()
{
    this->_server_client_max_body_size = 0;
    this->_server_name = "";
    this->_server_root = "";
    this->_server_index = "";
    this->_server_return = "";
    this->_server_autoindex = false;
}