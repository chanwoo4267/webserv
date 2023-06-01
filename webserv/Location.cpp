#include "Location.hpp"

Location::Location()
{
    this->_location_client_max_body_size = 0;
    this->_location_path = "";
    this->_location_root = "";
    this->_location_index = "";
    this->_location_return = "";
    this->_location_autoindex = false;
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