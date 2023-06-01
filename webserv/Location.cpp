#include "Location.hpp"

Location::Location()
{
    this->_location_client_max_body_size = 0;
    this->_location_path = "";
    this->_location_root = "";
    this->_location_index = "";
    this->_location_return = "";
    this->_location_autoindex = false;
    this->_location_cgi_extension = "";
    this->_location_cgi_path = "";
    this->_location_return_status = 0;
}

Location::~Location() {}

Location::Location(const Location &obj)
{
    this->_location_client_max_body_size = obj._location_client_max_body_size;
    this->_location_path = obj._location_path;
    this->_location_root = obj._location_root;
    this->_location_index = obj._location_index;
    this->_location_autoindex = obj._location_autoindex;
    this->_location_allow_methods = obj._location_allow_methods;
    this->_location_cgi_path = obj._location_cgi_path;
    this->_location_cgi_extension = obj._location_cgi_extension;
    this->_location_return = obj._location_return;
    this->_location_return_status = obj._location_return_status;
}

Location& Location::operator=(const Location &obj)
{
    if (this != &obj)
    {
        this->_location_client_max_body_size = obj._location_client_max_body_size;
        this->_location_path = obj._location_path;
        this->_location_root = obj._location_root;
        this->_location_index = obj._location_index;
        this->_location_autoindex = obj._location_autoindex;
        this->_location_allow_methods = obj._location_allow_methods;
        this->_location_cgi_path = obj._location_cgi_path;
        this->_location_cgi_extension = obj._location_cgi_extension;
        this->_location_return = obj._location_return;
        this->_location_return_status = obj._location_return_status;
    }
    return *this;
}

/* -------------------------------------------------- */
/* -------------------get methods-------------------- */
/* -------------------------------------------------- */

const size_t& Location::getLocationClientMaxBodySize()
{
    return this->_location_client_max_body_size;
}

const std::string& Location::getLocationPath()
{
    return this->_location_path;
}

const std::string& Location::getLocationRoot()
{
    return this->_location_root;
}

const std::string& Location::getLocationIndex()
{
    return this->_location_index;
}

const std::string& Location::getLocationReturn()
{
    return this->_location_return;
}

const int& Location::getLocationReturnStatus()
{
    return this->_location_return_status;
}

const bool& Location::getLocationAutoindex()
{
    return this->_location_autoindex;
}

const std::vector<MethodType>& Location::getLocationAllowMethods()
{
    return this->_location_allow_methods;
}

const std::string& Location::getLocationCgiPath()
{
    return this->_location_cgi_path;
}

const std::string& Location::getLocationCgiExtension()
{
    return this->_location_cgi_extension;
}

/* -------------------------------------------------- */
/* -------------------set methods-------------------- */
/* -------------------------------------------------- */

void Location::setLocationClientMaxBodySize(size_t size)
{
    this->_location_client_max_body_size = size;
}

void Location::setLocationPath(std::string path)
{
    this->_location_path = path;
}

void Location::setLocationRoot(std::string root)
{
    this->_location_root = root;
}

void Location::setLocationIndex(std::string index)
{
    this->_location_index = index;
}

void Location::setLocationReturn(std::string ret)
{
    this->_location_return = ret;
}

void Location::setLocationReturnStatus(int status)
{
    this->_location_return_status = status;
}

void Location::setLocationAutoindex(bool autoindex)
{
    this->_location_autoindex = autoindex;
}

void Location::setLocationAllowMethods(std::vector<MethodType> methods)
{
    this->_location_allow_methods = methods;
}

void Location::setLocationCgiPath(std::string cgi_path)
{
    this->_location_cgi_path = cgi_path;
}

void Location::setLocationCgiExtension(std::string cgi_extension)
{
    this->_location_cgi_extension = cgi_extension;
}

void Location::printLocation()
{
    std::cout << "******************" << std::endl;
    std::cout << "client_max_body_size = " << _location_client_max_body_size << std::endl;
    std::cout << "path = " << _location_path << std::endl;
    std::cout << "root = " << _location_root << std::endl;
    std::cout << "index = " << _location_index << std::endl;
    std::cout << "return = " << _location_return_status << " " << _location_return << std::endl;
    std::cout << "autoindex = " << _location_autoindex << std::endl;
    std::cout << "cgi_path = " << _location_cgi_path << std::endl;
    std::cout << "cgi_extension = " << _location_cgi_extension << std::endl;
    std::cout << "allow_methods = " << std::endl;
    for (std::vector<MethodType>::iterator it = _location_allow_methods.begin(); it != _location_allow_methods.end(); it++)
        std::cout << *it << " ";
    std::cout << std::endl;
    std::cout << "******************" << std::endl;
}