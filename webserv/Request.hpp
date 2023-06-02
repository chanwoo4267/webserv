#ifndef REQUEST_HPP
#define REQUEST_HPP

# include "Webserv.hpp"

class Request
{
    private:
        int                                 _client_fd;
        std::string                         _method;
        std::string                         _path;
        std::map<std::string, std::string>  _headers;
        std::string                         _body;
        size_t                              _content_length;

    public:
        Request();
        ~Request();

        const int& getClientFd();
        const std::string& getMethod();
        const std::string& getPath();
        const std::map<std::string, std::string>& getHeaders();
        const std::string& getBody();
        const size_t& getContentLength();

        void setClientFd(int fd);
        void setMethod(std::string method);
        void setPath(std::string path);
        void pushHeader(std::string str1, std::string str2);
        void setBody(std::string body);
        void setContentLength(size_t content_length);
};

#endif