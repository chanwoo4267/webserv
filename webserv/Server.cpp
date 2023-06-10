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

/*
    print all Server private values
*/
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

/*
    initialize and set server socket(_server_listen_socket)

    socket(), fcntl(O_NONBLOCK), setsockopt(SO_REUSEADDR), bind(), listen()
*/
void Server::setupServerSocket()
{
    struct sockaddr_in server_addr;

    if ((_server_listen_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        throw std::runtime_error("Error on setupServerSocket : socket() error");
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(ft_stoi(this->getServerPort()));

    if (bind(_server_listen_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
        throw std::runtime_error("Error on setupServerSocket : socket() error");
    
    if (listen(_server_listen_socket, BACKLOG_QUEUE_SIZE) < 0)
        throw std::runtime_error("Error on setupServerSocket : listen() error");
}
// void Server::setupServerSocket()
// {
//     struct addrinfo hints, *info;

//     memset(&hints, 0, sizeof(hints));
//     hints.ai_family = AF_INET;
// 	hints.ai_socktype = SOCK_STREAM;
// 	hints.ai_flags = AI_PASSIVE;

//     // getaddrinfo : dns resolving (domain 주소를 ip 주소로 변환, addrinfo의 linked list로 반환)
//     if (getaddrinfo(this->_server_host.c_str(), this->_server_port.c_str(), &hints, &info) >= 0)
//     {
//         if ((this->_server_listen_socket = socket(info->ai_family, info->ai_socktype, info->ai_protocol)) < 0)
//             throw std::runtime_error("Error on setupServerSocket : socket() error");

//         fcntl(this->_server_listen_socket, F_SETFL, O_NONBLOCK);
//         if (this->_server_listen_socket < 0)
//             throw std::runtime_error("Error on setupServerSocket : fcntl() error");
        
//         int option = 1;
//         if (setsockopt(this->_server_listen_socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&option, sizeof(option)) < 0)
// 			throw std::runtime_error("Error on setupServerSocket : setsockopt() error");
        
//         if (bind(this->_server_listen_socket, info->ai_addr, info->ai_addrlen))
//             throw std::runtime_error("Error on setupServerSocket : bind() error");
//         freeaddrinfo(info);
    
//         if (listen(this->_server_listen_socket, BACKLOG_QUEUE_SIZE) < 0)
//             throw std::runtime_error("Error on setupServerSocket : listen() error");
//     }
//     else
//         throw std::runtime_error("Error on setupServerSocket : getaddrinfo() error");
// }

