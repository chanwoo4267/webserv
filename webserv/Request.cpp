#include "Request.hpp"

Request::Request()
{
    this->_client_fd = 0;
    this->_method = "";
    this->_path = "";
    this->_body = "";
    this->_content_length = 0;
}

Request::~Request() {}

const int& Request::getClientFd()
{
    return this->_client_fd;
}

const std::string& Request::getMethod()
{
    return this->_method;
}

const std::string& Request::getPath()
{
    return this->_path;
}

const std::map<std::string, std::string>& Request::getHeaders()
{
    return this->_headers;
}

const std::string& Request::getBody()
{
    return this->_body;
}

const size_t& Request::getContentLength()
{
    return this->_content_length;
}

void Request::setClientFd(int fd)
{
    this->_client_fd = fd;
}

void Request::setMethod(std::string method)
{
    this->_method = method;
}

void Request::setPath(std::string path)
{
    this->_path = path;
}

void Request::setBody(std::string body)
{
    this->_body = body;
}

void Request::setContentLength(size_t content_length)
{
    this->_content_length = content_length;
}

void Request::pushHeader(std::string str1, std::string str2)
{
    this->_headers[str1] = str2;
}

/*
    parse Request message
*/
int Request::parseRequest(std::string request)
{
    size_t  i, j;

    // request Message starts with method type (ex GET POST ..)
    i = request.find_first_of(" ", 0);
    this->setMethod(request.substr(0, i));
    if (checkMethodType(this->_method) == INVALID)
        return 400;
    
    // if there's no other data
    j = request.find_first_of(" ", i + 1);
    if (j == std::string::npos)
        return 400;
    
    this->setPath(request.substr(i + 1, j - i - 1));
    //progressing
}