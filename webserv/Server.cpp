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
    this->_server_return_status = 0;
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
    this->_server_return_status = obj._server_return_status;
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
        this->_server_return_status = obj._server_return_status;
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

const int& Server::getServerReturnStatus()
{
    return this->_server_return_status;
}

const std::string& Server::getServerHost()
{
    return this->_server_host;
}

const std::string& Server::getServerPort()
{
    return this->_server_port;
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

void Server::setServerReturnStatus(int status)
{
    this->_server_return_status = status;
}

void Server::setServerAutoindex(bool autoindex)
{
    this->_server_autoindex = autoindex;
}

void Server::setServerHost(std::string host)
{
    this->_server_host = host;
}

void Server::setServerPort(std::string port)
{
    this->_server_port = port;
}

void Server::pushServerLocation(Location loc)
{
    this->_server_locations.push_back(loc);
}

void Server::pushServerErrorPage(short error_code, std::string error_path)
{
    this->_server_error_pages[error_code] = error_path;
}

void Server::printServer()
{
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "client_max_body_size = " << _server_client_max_body_size << std::endl;
    std::cout << "name = " << _server_name << std::endl;
    std::cout << "root = " << _server_root << std::endl;
    std::cout << "index = " << _server_index << std::endl;
    std::cout << "return = " << this->_server_return_status << " " << _server_return << std::endl;
    std::cout << "autoindex = " << (_server_autoindex ? "on" : "off") << std::endl;
    std::cout << "host:port = " << _server_host << ":" << _server_port << std::endl;
    std::cout << "error_pages = " << std::endl;
    for (std::map<short, std::string>::iterator it = _server_error_pages.begin(); it != _server_error_pages.end(); it++)
    {
        std::cout << it->first << " " << it->second << std::endl;
    }
    std::cout << "locations = " << std::endl;
    for (std::vector<Location>::iterator it = _server_locations.begin(); it != _server_locations.end(); it++)
    {
        it->printLocation();
    }
    std::cout << "--------------------------------------" << std::endl;
}