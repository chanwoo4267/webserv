#include "Location.hpp"

Location::Location()
{
	this->_path = "";
	this->_root = "";
	this->_autoindex = false;
	this->_index = "";
	this->_return = "";
	this->_alias = "";
	this->_client_max_body_size = MAX_CONTENT_LENGTH;
	this->_methods.reserve(5); // 메서드는 5가지 (GET+ POST- DELETE- PUT- HEAD-) 만 사용
	this->_methods.push_back(1);
	this->_methods.push_back(0);
	this->_methods.push_back(0);
	this->_methods.push_back(0);
	this->_methods.push_back(0);
}

Location::Location(const Location &other)
{
	this->_path = other._path;
	this->_root = other._root;
	this->_autoindex = other._autoindex;
	this->_index = other._index;
	this->_cgi_path = other._cgi_path;
	this->_cgi_ext = other._cgi_ext;
	this->_return = other._return;
	this->_alias = other._alias;
    this->_methods = other._methods;
	this->_ext_path = other._ext_path;
	this->_client_max_body_size = other._client_max_body_size;
}

Location &Location::operator=(const Location &rhs)
{
	if (this != &rhs)
	{
		this->_path = rhs._path;
		this->_root = rhs._root;
		this->_autoindex = rhs._autoindex;
		this->_index = rhs._index;
		this->_cgi_path = rhs._cgi_path;
		this->_cgi_ext = rhs._cgi_ext;
		this->_return = rhs._return;
		this->_alias = rhs._alias;
		this->_methods = rhs._methods;
		this->_ext_path = rhs._ext_path;
		this->_client_max_body_size = rhs._client_max_body_size;
    }
	return (*this);
}

Location::~Location() { }

/* --------------------------------------------------------------------------- */
/* ------------------------------- set methods ------------------------------- */
/* --------------------------------------------------------------------------- */

void Location::setPath(std::string parametr)
{
	this->_path = parametr;
}

void Location::setRootLocation(std::string parametr)
{
    // directory 가 맞는지 확인, directory가 아니면 예외 throw
	if (ConfigFile::getTypePath(parametr) != 2)
		throw ServerConfig::ErrorException("root of location");
	this->_root = parametr;
}

void Location::setMethods(std::vector<std::string> methods)
{
    // _methods는 생성자에서 size=5 로 reserve 되어있음
	this->_methods[0] = 0;
	this->_methods[1] = 0;
	this->_methods[2] = 0;
	this->_methods[3] = 0;
	this->_methods[4] = 0;

    // 들어오는 methods의 내용물에 맞는 index 값을 1로 set, 잘못된 method일 경우 예외 throw
	for (size_t i = 0; i < methods.size(); i++)
	{
		if (methods[i] == "GET")
			this->_methods[0] = 1;
		else if (methods[i] == "POST")
			this->_methods[1] = 1;
		else if (methods[i] == "DELETE")
			this->_methods[2] = 1;
		else if (methods[i] == "PUT")
			this->_methods[3] = 1;
		else if (methods[i] == "HEAD")
			this->_methods[4] = 1;
		else
			throw ServerConfig::ErrorException("Allow method not supported " + methods[i]);
	}
}

void Location::setAutoindex(std::string parametr)
{
    // 인자가 on이면 1, off면 0, 둘다 아니면 예외 throw
	if (parametr == "on" || parametr == "off")
		this->_autoindex = (parametr == "on");
	else
		throw ServerConfig::ErrorException("Wrong autoindex");
}

void Location::setIndexLocation(std::string parametr)
{
	this->_index = parametr;
}

void Location::setReturn(std::string parametr)
{
	this->_return = parametr;
}

void Location::setAlias(std::string parametr)
{
	this->_alias = parametr;
}

void Location::setCgiPath(std::vector<std::string> path)
{
	this->_cgi_path = path;
}

void Location::setCgiExtension(std::vector<std::string> extension)
{
	this->_cgi_ext = extension;
}

void Location::setMaxBodySize(std::string parametr)
{
	unsigned long body_size = 0;

    // 인자에 숫자가 아닌 값이 들어올경우 예외처리
	for (size_t i = 0; i < parametr.length(); i++)
	{
		if (parametr[i] < '0' || parametr[i] > '9')
			throw ServerConfig::ErrorException("Wrong syntax: client_max_body_size");
	}
    // custom stoi function으로 string을 int로 변환. 실패시 예외 throw
    body_size = ft_stoi(parameter);
    if (!body_size)
        throw ServerConfig::ErrorException("Wrong syntax: client_max_body_size");
	this->_client_max_body_size = body_size;

    // refactor : ft_stoi를 불필요하게 두번 호출하는 부분 수정
    // if (!ft_stoi(parametr))
	// 	throw ServerConfig::ErrorException("Wrong syntax: client_max_body_size");
	// body_size = ft_stoi(parametr);
}

void Location::setMaxBodySize(unsigned long parametr)
{
	this->_client_max_body_size = parametr;
}

/* --------------------------------------------------------------------------- */
/* ------------------------------- get methods ------------------------------- */
/* --------------------------------------------------------------------------- */

const std::string &Location::getPath() const
{
	return (this->_path);
}

const std::string &Location::getRootLocation() const
{
	return (this->_root);
}

const std::string &Location::getIndexLocation() const
{
	return (this->_index);
}

const std::vector<short> &Location::getMethods() const
{
	return (this->_methods);
}

const std::vector<std::string> &Location::getCgiPath() const
{
	return (this->_cgi_path);
}

const std::vector<std::string> &Location::getCgiExtension() const
{
	return (this->_cgi_ext);
}

const bool &Location::getAutoindex() const
{
	return (this->_autoindex);
}

const std::string &Location::getReturn() const
{
	return (this->_return);
}

const std::string &Location::getAlias() const
{
	return (this->_alias);
}

const std::map<std::string, std::string> &Location::getExtensionPath() const
{
	return (this->_ext_path);
}

const unsigned long &Location::getMaxBodySize() const
{
	return (this->_client_max_body_size);
}

/* 허용된 methods를 <METHOD, METHOD, METHOD> 형식으로 반환하는 메서드 */
std::string Location::getPrintMethods() const
{
	std::string res;
	if (_methods[4])
		res.insert(0, "HEAD");
	if (_methods[3])
	{
		if (!res.empty())
			res.insert(0, ", ");
		res.insert(0, "PUT");
	}
	if (_methods[2])
	{
		if (!res.empty())
			res.insert(0, ", ");
		res.insert(0, "DELETE");
	}
	if (_methods[1])
	{
		if (!res.empty())
			res.insert(0, ", ");
		res.insert(0, "POST");
	}
	if (_methods[0])
	{
		if (!res.empty())
			res.insert(0, ", ");
		res.insert(0, "GET");
	}
	return (res);
}