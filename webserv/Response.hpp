#ifndef RESPONSE_HPP
#define RESPONSE_HPP

# include "Webserv.hpp"

class Response
{
    private:
        std::map<std::string, std::string>  _headers;
        std::string                         _status_code;
        std::string                         _status_phrase;
        std::string                         _body;

    public:
        Response();
        ~Response();

        const std::map<std::string, std::string>& getHeaders();
        const std::string& getStatusCode();
        const std::string& getStatusPhrase();
        const std::string& getBody();

        void pushHeader(std::string str1, std::string str2);
        void setStatusCode(std::string status_code);
        void setStatusPhrase(std::string status_phrase);
        void setBody(std::string body);
        void setStatus(std::string status);

        std::string makeHeader();
        void makeStatusBody();
        std::string serialize();
};

#endif