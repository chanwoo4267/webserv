#include "Server.hpp"

Server::Server()
{
    this->_server_client_max_body_size = 0;
    this->_server_name = "";
    this->_server_root = "";
    this->_server_index = "";
    this->_server_return = "";
    this->_server_autoindex = false;
    this->_server_listen_socket = 0;
    this->_server_host = "";
    this->_server_port = "";
}

Server::~Server() {}

Server::Server(const Server& obj)
{
    this->_server_client_max_body_size = obj._server_client_max_body_size;
    this->_server_name = obj._server_name;
    this->_server_root = obj._server_root;
    this->_server_index = obj._server_index;
    this->_server_return = obj._server_return;
    this->_server_error_pages = obj._server_error_pages;
    this->_server_locations = obj._server_locations;
    this->_server_autoindex = obj._server_autoindex;
    this->_server_listen_socket = obj._server_listen_socket;
    this->_server_host = obj._server_host;
    this->_server_port = obj._server_port;
}

Server& Server::operator=(const Server& obj)
{
    if (this != &obj)
    {
        this->_server_client_max_body_size = obj._server_client_max_body_size;
        this->_server_name = obj._server_name;
        this->_server_root = obj._server_root;
        this->_server_index = obj._server_index;
        this->_server_return = obj._server_return;
        this->_server_error_pages = obj._server_error_pages;
        this->_server_locations = obj._server_locations;
        this->_server_autoindex = obj._server_autoindex;
        this->_server_listen_socket = obj._server_listen_socket;
        this->_server_host = obj._server_host;
        this->_server_port = obj._server_port;
    }
    return *this;
}

/* -------------------------------------------------- */
/* -------------------get methods-------------------- */
/* -------------------------------------------------- */

const size_t& Server::getServerClientMaxBodySize()
{
    return this->_server_client_max_body_size;
}

const std::string& Server::getServerName()
{
    return this->_server_name;
}

const std::string& Server::getServerRoot()
{
    return this->_server_root;
}

const std::string& Server::getServerIndex()
{
    return this->_server_index;
}

const std::string& Server::getServerReturn()
{
    return this->_server_return;
}

/* -------------------------------------------------- */
/* -------------------set methods-------------------- */
/* -------------------------------------------------- */

void Server::setServerClientMaxBodySize(size_t size)
{
    this->_server_client_max_body_size = size;
}

void Server::setServerName(std::string name)
{
    this->_server_name = name;
}

void Server::setServerRoot(std::string root)
{
    this->_server_root = root;
}

void Server::setServerIndex(std::string index)
{
    this->_server_index = index;
}

void Server::setServerReturn(std::string ret)
{
    this->_server_return = ret;
}

void Server::setServerAutoindex(bool autoindex)
{
    this->_server_autoindex = autoindex;
}