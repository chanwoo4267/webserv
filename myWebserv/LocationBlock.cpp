#include "LocationBlock.hpp"

LocationBlock::LocationBlock()
{
	this->_path = "";
	this->_root = "";
	this->_index = "";
	this->_return = "";
	this->_alias = "";

    this->_autoindex = false;
	this->_client_max_body_size = 1000000; // nginx default : 1MB

	this->_methods["GET"] = true;
    this->_methods["HEAD"] = true;
    this->_methods["POST"] = true;
    this->_methods["PUT"] = true;
    this->_methods["PATCH"] = true;
    this->_methods["DELETE"] = true;
    this->_methods["OPTIONS"] = true;
}

LocationBlock::LocationBlock(const LocationBlock &obj)
{
	this->_path = obj._path;
	this->_root = obj._root;
	this->_autoindex = obj._autoindex;
	this->_index = obj._index;
	this->_cgi_path = obj._cgi_path;
	this->_cgi_ext = obj._cgi_ext;
	this->_return = obj._return;
	this->_alias = obj._alias;
    this->_methods = obj._methods;
	this->_ext_path = obj._ext_path;
	this->_client_max_body_size = obj._client_max_body_size;
}

LocationBlock &LocationBlock::operator=(const LocationBlock &obj)
{
	if (this != &obj)
	{
		this->_path = obj._path;
		this->_root = obj._root;
		this->_autoindex = obj._autoindex;
		this->_index = obj._index;
		this->_cgi_path = obj._cgi_path;
		this->_cgi_ext = obj._cgi_ext;
		this->_return = obj._return;
		this->_alias = obj._alias;
		this->_methods = obj._methods;
		this->_ext_path = obj._ext_path;
		this->_client_max_body_size = obj._client_max_body_size;
    }
	return (*this);
}

LocationBlock::~LocationBlock() { }

/* --------------------------------------------------------------------------- */
/* ------------------------------- set methods ------------------------------- */
/* --------------------------------------------------------------------------- */

void LocationBlock::setPath(std::string parametr)
{
	this->_path = parametr;
}

void LocationBlock::setRootLocation(std::string parametr)
{
    // directory 가 맞는지 확인, directory가 아니면 예외 throw
	if (getTypePath(parametr) != 2)
		throw ErrorHandler::ErrorException("root of location");
	this->_root = parametr;
}

void LocationBlock::setMethods(std::vector<std::string> methods)
{
    for(std::map<std::string, bool>::iterator it = this->_methods.begin(); it != this->_methods.end(); it++)
        it->second = false;
	// this->_methods["GET"] = false;
    // this->_methods["HEAD"] = false;
    // this->_methods["POST"] = false;
    // this->_methods["PUT"] = false;
    // this->_methods["PATCH"] = false;
    // this->_methods["DELETE"] = false;
    // this->_methods["OPTIONS"] = false;

	for (size_t i = 0; i < methods.size(); i++)
	{
		if (methods[i] == "GET")
			this->_methods["GET"] = true;
		else if (methods[i] == "HEAD")
			this->_methods["HEAD"] = true;
		else if (methods[i] == "POST")
			this->_methods["POST"] = true;
        else if (methods[i] == "PUT")
			this->_methods["PUT"] = true;
        else if (methods[i] == "PATCH")
			this->_methods["PATCH"] = true;
        else if (methods[i] == "DELETE")
			this->_methods["DELETE"] = true;
        else if (methods[i] == "OPTIONS")
			this->_methods["OPTIONS"] = true;
		else
			throw ErrorHandler::ErrorException("Allow Methods invalid input : " + methods[i]);
	}
}

void LocationBlock::setAutoindex(std::string parametr)
{
    // 인자가 on이면 1, off면 0, 둘다 아니면 예외 throw
	if (parametr == "on" || parametr == "off")
		this->_autoindex = (parametr == "on");
	else
		throw ErrorHandler::ErrorException("Autoindex invalid input : " + parametr);
}

void LocationBlock::setIndexLocation(std::string parametr)
{
	this->_index = parametr;
}

void LocationBlock::setReturn(std::string parametr)
{
	this->_return = parametr;
}

void LocationBlock::setAlias(std::string parametr)
{
	this->_alias = parametr;
}

void LocationBlock::setCgiPath(std::vector<std::string> path)
{
	this->_cgi_path = path;
}

void LocationBlock::setCgiExtension(std::vector<std::string> extension)
{
	this->_cgi_ext = extension;
}

void LocationBlock::setMaxBodySize(std::string parametr)
{
	long body_size = 0;

    // 인자에 숫자가 아닌 값이 들어올경우 예외처리
	if (!isNumber(parametr))
		throw ErrorHandler::ErrorException("Client_Max_Body_Size invalid input : " + parametr);

    // string을 int로 변환. 실패시 (음수 반환) 예외 throw
    body_size = ft_stoi(parametr);
    if (body_size < 0)
        throw ErrorHandler::ErrorException("Client_Max_Body_Size invalid input : " + parametr);

	this->_client_max_body_size = body_size;
}

void LocationBlock::setMaxBodySize(long parametr)
{
	this->_client_max_body_size = parametr;
}

/* --------------------------------------------------------------------------- */
/* ------------------------------- get methods ------------------------------- */
/* --------------------------------------------------------------------------- */

const std::string &LocationBlock::getPath() const
{
	return (this->_path);
}

const std::string &LocationBlock::getRootLocation() const
{
	return (this->_root);
}

const std::string &LocationBlock::getIndexLocation() const
{
	return (this->_index);
}

const std::map<std::string, bool> &LocationBlock::getMethods() const
{
	return (this->_methods);
}

const std::vector<std::string> &LocationBlock::getCgiPath() const
{
	return (this->_cgi_path);
}

const std::vector<std::string> &LocationBlock::getCgiExtension() const
{
	return (this->_cgi_ext);
}

const bool &LocationBlock::getAutoindex() const
{
	return (this->_autoindex);
}

const std::string &LocationBlock::getReturn() const
{
	return (this->_return);
}

const std::string &LocationBlock::getAlias() const
{
	return (this->_alias);
}

const std::map<std::string, std::string> &LocationBlock::getExtensionPath() const
{
	return (this->_ext_path);
}

const long &LocationBlock::getMaxBodySize() const
{
	return (this->_client_max_body_size);
}

/* test functions */
const void LocationBlock::printLocationBlock()
{
	std::cout << RED << "---------- Location ----------" << CYN << std::endl;
	std::cout << "path :\t" << this->_path << std::endl;
	std::cout << "root :\t" << this->_root << std::endl;
	std::cout << "autoindex :\t" << this->_autoindex << std::endl;
	std::cout << "index :\t" << this->_index << std::endl;
	std::cout << "return :\t" << this->_return << std::endl;
	std::cout << "alias :\t" << this->_alias << std::endl;
	std::cout << "cmaxbsize : \t" << this->_client_max_body_size << std::endl;
	std::cout << "methods :\t" << std::endl;
	for (std::map<std::string, bool>::iterator it = this->_methods.begin(); it != this->_methods.end(); it++)
		std::cout << it->first << " " << it->second << std::endl;
	std::cout << "cgi_path :\t" << std::endl;
	for (std::vector<std::string>::iterator it = this->_cgi_path.begin(); it != this->_cgi_path.end(); it++)
		std::cout << *it << std::endl;
	std::cout << "cgi_ext :\t" << std::endl;
	for (std::vector<std::string>::iterator it = this->_cgi_ext.begin(); it != this->_cgi_ext.end(); it++)
		std::cout << *it << std::endl;
	std::cout << NC;
}