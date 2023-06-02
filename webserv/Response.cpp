#include "Response.hpp"

Response::Response()
{
    this->_status_code = "";
    this->_status_phrase = "";
    this->_body = "";
}

Response::~Response()
{

}

const std::map<std::string, std::string>& Response::getHeaders()
{
    return this->_headers;
}

const std::string& Response::getStatusCode()
{
    return this->_status_code;
}

const std::string& Response::getStatusPhrase()
{
    return this->_status_phrase;
}

const std::string& Response::getBody()
{
    return this->_body;
}

void Response::pushHeader(std::string str1, std::string str2)
{
    this->_headers[str1] = str2;
}

void Response::setStatusCode(std::string status_code)
{
    this->_status_code = status_code;
}

void Response::setStatusPhrase(std::string status_phrase)
{
    this->_status_phrase = status_phrase;
}

void Response::setBody(std::string body)
{
    this->_body = body;
}

void Response::setStatus(std::string status)
{
    this->setStatusCode(status.substr(0, 3));
    this->setStatusPhrase(status.substr(4));
}

std::string Response::makeHeader()
{
    std::string result = "HTTP/1.1 " + this->_status_code + " " + this->_status_phrase + "\r\n";

	for (std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != this->_headers.end(); it++)
		result.append((*it).first + ": " + (*it).second + "\r\n");
	result.append("\r\n");

	return result;
}

void Response::makeStatusBody()
{
    std::string result;

	result.append("<!DOCTYPE html><html><head><meta charset=\"UTF-8\"/><title>webserv</title></head>");
	result.append("<body>");
	result.append("<h1>" + this->_status_code + "</h1>");
	result.append("<h3>" + this->_status_phrase + "</h3>");
	result.append("<p>Click <a href=\"/\">here</a> to return home.</p>");
	result.append("</body></html>");

	this->_body = result;
}

std::string Response::serialize()
{
    std::string result = this->makeHeader();
    result.append(this->_body);

    return result;
}