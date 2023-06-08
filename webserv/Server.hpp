#ifndef SERVER_HPP
#define SERVER_HPP

# include "Webserv.hpp"
# include "Location.hpp"

/*
    server subject rules

    # mandatory
    1. client can upload files
    2. At least feature GET, POST, DELETE methods
    3. can listen to multiple port

    # config
    1. port, host, server_names
    2. first host:port server is default (?)
    3. default error pages
    4. client_max_body_size
*/

class Server
{
    private:
        size_t                          _server_client_max_body_size;
        std::string                     _server_name;
        std::string                     _server_root;
        std::string                     _server_index;
        std::string                     _server_return;
        int                             _server_return_status;
        std::map<short, std::string>    _server_error_pages;
        std::vector<Location>           _server_locations;
        bool                            _server_autoindex;
        int                             _server_listen_socket;
        std::string                     _server_host; //in_addr_t
        std::string                     _server_port; //uint16_t

    public:
        Server();
	    ~Server();
	    Server(const Server& obj);
	    Server& operator=(const Server& obj);

        /* get methods */
        const size_t& getServerClientMaxBodySize();
        const std::string& getServerName();
        const std::string& getServerRoot();
        const std::string& getServerIndex();
        const std::string& getServerReturn();
        const int& getServerReturnStatus();
        const std::string& getServerHost();
        const std::string& getServerPort();

        /* set methods */
        void setServerClientMaxBodySize(size_t size);
        void setServerName(std::string name);
        void setServerRoot(std::string root);
        void setServerIndex(std::string index);
        void setServerReturn(std::string ret);
        void setServerReturnStatus(int status);
        void setServerAutoindex(bool autoindex);
        void setServerHost(std::string host);
        void setServerPort(std::string port);

        void pushServerLocation(Location loc);
        void pushServerErrorPage(short error_code, std::string error_path);

        void setupServerSocket();

        void printServer();
};

#endif