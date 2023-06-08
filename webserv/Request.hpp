#ifndef REQUEST_HPP
#define REQUEST_HPP

# include "Webserv.hpp"
# include "Utility.hpp"

class Request
{
    private:
        int                                 _client_fd;
        std::string                         _method;
        std::string                         _path;
        std::map<std::string, std::string>  _headers;
        std::string                         _body;
        size_t                              _content_length;
        std::string                         _protocol_version;

    public:
        Request();
        ~Request();

        const int& getClientFd();
        const std::string& getMethod();
        const std::string& getPath();
        const std::map<std::string, std::string>& getHeaders();
        const std::string& getBody();
        const size_t& getContentLength();
        const std::string& getProtocolVersion();

        void setClientFd(int fd);
        void setMethod(std::string method);
        void setPath(std::string path);
        void pushHeader(std::string str1, std::string str2);
        void setBody(std::string body);
        void setContentLength(size_t content_length);
        void setProtocolVersion(std::string protocol_version);

        int parseRequest(std::string request);
        size_t parse_headers_line(std::string& request, size_t index);
        std::string parse_request_body_chunked(std::string& request, size_t index);
        std::string parse_request_body(std::string& request, size_t index);

        std::string getUriPath();
        std::string getUriQuery();
};

#endif