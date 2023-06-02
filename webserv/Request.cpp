#include "Request.hpp"

Request::Request()
{
    this->_client_fd = 0;
    this->_method = "";
    this->_path = "";
    this->_body = "";
    this->_content_length = 0;
}