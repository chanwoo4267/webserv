#include "ServerBlock.hpp"

ServerBlock::ServerBlock()
{
	this->_port = 0;
	this->_host = 0;
	this->_server_name = "";
	this->_root = "";
	this->_client_max_body_size = 1000000;
	this->_index = "";
	this->_listen_fd = 0;
	this->_autoindex = false;
	this->initErrorPages();
}

ServerBlock::~ServerBlock() { }

ServerBlock::ServerBlock(const ServerBlock &obj)
{
	if (this != &obj)
	{
		this->_server_name = obj._server_name;
		this->_root = obj._root;
		this->_host = obj._host;
		this->_port = obj._port;
		this->_client_max_body_size = obj._client_max_body_size;
		this->_index = obj._index;
		this->_error_pages = obj._error_pages;
		this->_locations = obj._locations;
		this->_listen_fd = obj._listen_fd;
		this->_autoindex = obj._autoindex;
		this->_server_address = obj._server_address;
	}
	return ;
}

ServerBlock &ServerBlock::operator=(const ServerBlock & obj)
{
	if (this != &obj)
	{
		this->_server_name = obj._server_name;
		this->_root = obj._root;
		this->_port = obj._port;
		this->_host = obj._host;
		this->_client_max_body_size = obj._client_max_body_size;
		this->_index = obj._index;
		this->_error_pages = obj._error_pages;
		this->_locations = obj._locations;
		this->_listen_fd = obj._listen_fd;
		this->_autoindex = obj._autoindex;
		this->_server_address = obj._server_address;
	}
	return (*this);
}

/* 모든 에러 페이지에 대해 map 요소 추가 및 초기화 */
void ServerBlock::initErrorPages(void)
{
    int i;
    for(i = 300; i <= 308; i++)
        _error_pages[i] = "";
    
    for (i = 400; i <= 418; i++)
        _error_pages[i] = "";
    
    for (i = 421; i <= 426; i++)
        _error_pages[i] = "";
    
	_error_pages[428] = "";
    _error_pages[429] = "";
    _error_pages[431] = "";
    _error_pages[451] = "";

    for (i = 500; i <= 508; i++)
        _error_pages[i] = "";
	
	_error_pages[510] = "";
	_error_pages[511] = "";
}

/* --------------------------------------------------------------------------- */
/* ------------------------------- set methods ------------------------------- */
/* --------------------------------------------------------------------------- */

/* check */ //ServerBlock의 set함수들에서 세미콜론을 제거하지 말고, 세미콜론을 미리 제거한 뒤 set함수를 호출하도록 변경

void ServerBlock::setServerName(std::string server_name)
{
	checkToken(server_name); // 끝에 세미콜론이 있는지 확인 후 없으면 예외 throw, 있으면 제거해줌
	this->_server_name = server_name;
}

void ServerBlock::setIndex(std::string index)
{
	checkToken(index);
	this->_index = index;
}

void	ServerBlock::setFd(int fd)
{
	this->_listen_fd = fd;
}

void ServerBlock::setHost(std::string parametr)
{
	checkToken(parametr);
	if (parametr == "localhost") // localhost는 자동으로 127.0.0.1로 변환 후 저장
		parametr = "127.0.0.1";
	if (!isValidHost(parametr)) // host가 valid한지 확인, valid하지 않다면 예외를 throw
		throw ErrorHandler::ErrorException("Wrong syntax: host");

    this->_host = inet_addr(parametr.c_str());
    // string.c_str() vs string.data()
    // c_str()는 항상 널 문자로 끝나지만, data()는 널 문자로 종료되지 않을 수 있다
    // 이런 차이는 c_str()는 string이 널문자로 끝나지 않으면 널문자로 끝나는 사본을 만들어서 반환하지만, data는 내부 객체데이터를 그대로 반환하기 때문
    // 이외에도 C++ version에 따라 const를 붙이고 안붙이고 등의 차이가 있을 수 있으나, 기본적으로 동일한 작동

    // refactor : data() 대신 c_str() 사용하도록 통일
    // this->_host = inet_addr(parametr.data());
}

void ServerBlock::setRoot(std::string root)
{
	checkToken(root);
	if (getTypePath(root) == 2) // directory라면 그대로 root 설정
	{
		this->_root = root;
		return ;
	}
	// directory가 아니라면, 현재경로/인자 로 만들어준 뒤 한번 더 검사
	char dir[1024];
	getcwd(dir, 1024);
	std::string full_root = dir + root;
	if (getTypePath(full_root) != 2) // 그럼에도 directory가 아니라면 예외 throw
		throw ErrorHandler::ErrorException("Wrong syntax: root");
	this->_root = full_root;
}

void ServerBlock::setPort(std::string parametr)
{
	unsigned int port;
	
	port = 0;
	checkToken(parametr);
	// 인자가 숫자인지 검사 후 아니라면 예외처리
    if (!isNumber(parametr))
        throw ErrorHandler::ErrorException("Wrong syntax: port");

	port = ft_stoi((parametr));
	if (port < 1 || port > 65535)
		throw ErrorHandler::ErrorException("Wrong syntax: port");
	this->_port = static_cast<uint16_t>(port);
	
	// fix : max_port number is 65535. before value was 65636
	// refactor : c-style casting 대신 static_cast 사용
	// this->_port = (uint16_t) port;
}

void ServerBlock::setClientMaxBodySize(std::string parametr)
{
	unsigned long body_size;
	
	body_size = 0;
	checkToken(parametr);
    if (!isNumber(parametr))
        throw ErrorHandler::ErrorException("Wrong syntax: client_max_body_size");

	body_size = ft_stoi(parametr);
	if (!body_size)
		throw ErrorHandler::ErrorException("Wrong syntax: client_max_body_size");
	this->_client_max_body_size = body_size;
	
	// refactor : 불필요한 ft_stoi 중복호출 생략
	// if (!ft_stoi(parametr))
	// 	throw ErrorException("Wrong syntax: client_max_body_size");
	// body_size = ft_stoi(parametr);
}

void ServerBlock::setAutoindex(std::string autoindex)
{
	checkToken(autoindex);
	// on일경우 true로, off일경우 false로, 둘다 아닐경우 예외 throw
	if (autoindex == "on" || autoindex == "off")
		this->_autoindex = (autoindex == "on");
	else
		throw ErrorHandler::ErrorException("Wrong syntax: autoindex");
	
	// refactor : 
	// if (autoindex != "on" && autoindex != "off") 
	// 	throw ErrorException("Wrong syntax: autoindex");
	// if (autoindex == "on")
	// 	this->_autoindex = true;
}

/* 입력받는 <error_pages, path> pair를 map에 설정 혹은 추가함 */
void ServerBlock::setErrorPages(std::vector<std::string> &parametr)
{
	if (parametr.empty())
		return;
	// 현재 인자 vector는 에러코드 - 파일경로 - 에러코드 - 파일경로 - ... 으로 구성됨
	if (parametr.size() % 2 != 0)
		throw ErrorHandler::ErrorException ("Error page initialization faled");
	for (size_t i = 0; i < parametr.size() - 1; i++)
	{
		for (size_t j = 0; j < parametr[i].size(); j++) // 모두 숫자인지 확인
		{
			if (!std::isdigit(parametr[i][j]))
				throw ErrorHandler::ErrorException("Error code is invalid");
		}
		if (parametr[i].size() != 3) // HTTP response code는 1XX ~ 5XX
			throw ErrorHandler::ErrorException("Error code is invalid");
	
		short code_error = ft_stoi(parametr[i]);
		if (statusCodetoString(code_error)  == "Undefined" || code_error < 400) // client error : 400+, server error : 500+
			throw ErrorHandler::ErrorException ("Incorrect error code: " + parametr[i]);

		i++; // 이제 받은 code_error에 해당하는 경로로 이동
		std::string path = parametr[i];
		checkToken(path);
		if (getTypePath(path) != 2) // directory가 아닐경우, 보정
		{
			if (getTypePath(this->_root + path) != 1) // root + path 결과가 파일이 아닐경우 예외처리
				throw ErrorHandler::ErrorException ("Incorrect path for error page file: " + this->_root + path);
			// refactor : 불필요한 함수 호출 제거. R_OK는 존재여부(0)과 읽기여부(4)를 동시에 체크한다.
			// if (checkFile(this->_root + path, 0) == -1 || checkFile(this->_root + path, 4) == -1) // 해당 파일이 열수 없을경우
			if (checkFile(this->_root + path, R_OK) == -1)
				throw ErrorHandler::ErrorException ("Error page file :" + this->_root + path + " is not accessible");
		}

		// error_page map에서 code_error에 경로를 넣어준다. 없으면 추가한다.
		std::map<short, std::string>::iterator it = this->_error_pages.find(code_error);
		if (it != _error_pages.end())
			this->_error_pages[code_error] = path;
		else
			this->_error_pages.insert(std::make_pair(code_error, path));
		/* check */ // 어차피 error_page가 없으면 추가해줘야 하는데 (기존 HTTP response code에 해당되지 않는 custom code 설정하기가 가능하기 때문에) 굳이 default error_page map을 초기화해줘야 하나? -> 처음 초기값 설정할때 필요할듯
	}
}

/* 입력받은 정보 벡터를 Location으로 저장 */
void ServerBlock::setLocation(std::string path, std::vector<std::string> parametr)
{
	LocationBlock new_location;
	std::vector<std::string> methods;
	bool flag_methods = false;
	bool flag_autoindex = false;
	bool flag_max_size = false;
	int valid;

	// 인자로 받은 path 설정
	new_location.setPath(path);

	// 문자열 벡터를 순회하며 각각 확인
	for (size_t i = 0; i < parametr.size(); i++)
	{
		// 만약 파라미터가 root고 다음이 존재한다면
		if (parametr[i] == "root" && (i + 1) < parametr.size())
		{
			// 이미 root값이 들어간 적이 있다면 예외 throw
			if (!new_location.getRootLocation().empty())
				throw ErrorHandler::ErrorException("Root of location is duplicated");
			checkToken(parametr[++i]); // root 다음 파라미터 세미콜론 제거

			// 만약 파라미터가 directory라면 그대로 location의 rootlocation으로 저장, 아니라면 현재 server block의 root + 파라미터로 저장
			if (getTypePath(parametr[i]) == 2)
				new_location.setRootLocation(parametr[i]);
			else
				new_location.setRootLocation(this->_root + parametr[i]);
		} // 만약 파라미터가 allow_methods고 다음이 존재한다면
		else if ((parametr[i] == "allow_methods" || parametr[i] == "methods") && (i + 1) < parametr.size())
		{
			// 이미 allow_methods가 들어온적이 있다면 예외 throw
			if (flag_methods)
				throw ErrorHandler::ErrorException("Allow_methods of location is duplicated");

			std::vector<std::string> methods;
			while (++i < parametr.size())
			{
				if (parametr[i].find(";") != std::string::npos) // 파라미터에 세미콜론이 있다면 세미콜론 제거 후 넣기
				{
					checkToken(parametr[i]);
					methods.push_back(parametr[i]);
					break ; // 반복 종료
				}
				else // 세미콜론이 없다면 그대로 넣기
				{
					methods.push_back(parametr[i]);
					if (i + 1 >= parametr.size()) // 만약 다음이 없다면 예외 throw (세미콜론으로 끝나지 않음)
						throw ErrorHandler::ErrorException("Token is invalid");
				}
			}
			new_location.setMethods(methods); // location의 method set
			flag_methods = true; // 중복등장 방지 
		} // 만약 파라미터가 autoindex고 다음이 존재한다면
		else if (parametr[i] == "autoindex" && (i + 1) < parametr.size())
		{
			if (path == "/cgi-bin") // 경로가 cgi-bin일경우 예외 throw : CGI는 autoindex 지원안함
				throw ErrorHandler::ErrorException("Parametr autoindex not allow for CGI");
			if (flag_autoindex) // autoindex가 두번째 나오는거라면 예외 throw
				throw ErrorHandler::ErrorException("Autoindex of location is duplicated");
			// 다음 인자에서 세미콜론 제거 뒤 setAutoindex 호출로 autoindex 값 설정 (off or on)
			checkToken(parametr[++i]);
			new_location.setAutoindex(parametr[i]);
			flag_autoindex = true; // 중복등장 방지
		}
		else if (parametr[i] == "index" && (i + 1) < parametr.size())
		{
			if (!new_location.getIndexLocation().empty())
				throw ErrorHandler::ErrorException("Index of location is duplicated");
			checkToken(parametr[++i]);
			new_location.setIndexLocation(parametr[i]);
		}
		else if (parametr[i] == "return" && (i + 1) < parametr.size())
		{
			if (path == "/cgi-bin")
				throw ErrorHandler::ErrorException("Parametr return not allow for CGI");
			if (!new_location.getReturn().empty())
				throw ErrorHandler::ErrorException("Return of location is duplicated");
			checkToken(parametr[++i]);
			new_location.setReturn(parametr[i]);
		}
		else if (parametr[i] == "alias" && (i + 1) < parametr.size())
		{
			if (path == "/cgi-bin")
				throw ErrorHandler::ErrorException("Parametr alias not allow for CGI");
			if (!new_location.getAlias().empty())
				throw ErrorHandler::ErrorException("Alias of location is duplicated");
			checkToken(parametr[++i]);
			new_location.setAlias(parametr[i]);
		}
		else if (parametr[i] == "cgi_ext" && (i + 1) < parametr.size())
		{
			std::vector<std::string> extension;
			while (++i < parametr.size())
			{
				if (parametr[i].find(";") != std::string::npos)
				{
					checkToken(parametr[i]);
					extension.push_back(parametr[i]);
					break ;
				}
				else
				{
					extension.push_back(parametr[i]);
					if (i + 1 >= parametr.size())
						throw ErrorHandler::ErrorException("Token is invalid");
				}
			}
			new_location.setCgiExtension(extension);
		}
		else if (parametr[i] == "cgi_path" && (i + 1) < parametr.size())
		{
			std::vector<std::string> path;
			while (++i < parametr.size())
			{
				if (parametr[i].find(";") != std::string::npos)
				{
					checkToken(parametr[i]);
					path.push_back(parametr[i]);
					break ;
				}
				else
				{
					path.push_back(parametr[i]);
					if (i + 1 >= parametr.size())
						throw ErrorHandler::ErrorException("Token is invalid");
				}
				if (parametr[i].find("/python") == std::string::npos && parametr[i].find("/bash") == std::string::npos)
					throw ErrorHandler::ErrorException("cgi_path is invalid");
			}
			new_location.setCgiPath(path);
		}
		else if (parametr[i] == "client_max_body_size" && (i + 1) < parametr.size())
		{
			if (flag_max_size)
				throw ErrorHandler::ErrorException("Maxbody_size of location is duplicated");
			checkToken(parametr[++i]);
			new_location.setMaxBodySize(parametr[i]);
			flag_max_size = true;
		}
		else if (i < parametr.size())
			throw ErrorHandler::ErrorException("Parametr in a location is invalid");
	} // end for

	// 만약 Path가 /cgi-bin이 아니고, location의 index가 설정되지 않았다면 server block의 index로 설정 (Location 블록에서 따로 index가 명시되지 않으면 server 블록과 동일)
	if (new_location.getPath() != "/cgi-bin" && new_location.getIndexLocation().empty())
		new_location.setIndexLocation(this->_index);
	// 만약 client_max_body_size가 설정되지 않았다면, server 블록의 client_max_body_size로 설정
	if (!flag_max_size)
		new_location.setMaxBodySize(this->_client_max_body_size);
	
	// 생성한 location의 validation
	valid = isValidLocation(new_location);
	if (valid == 1)
		;
		// throw ErrorHandler::ErrorException("Failed CGI validation"); /* check */ implement later
	else if (valid == 2)
		throw ErrorHandler::ErrorException("Failed path in locaition validation");
	else if (valid == 3)
		throw ErrorHandler::ErrorException("Failed redirection file in locaition validation");
	else if (valid == 4)
		throw ErrorHandler::ErrorException("Failed alias file in locaition validation");
	// location 정보를 server_config 구조체의 _locations에 저장
	this->_locations.push_back(new_location);
}

/* --------------------------------------------------------------------------- */
/* ---------------------------- validate methods ----------------------------- */
/* --------------------------------------------------------------------------- */

/* host 주소가 valid한지 검사한다. */
bool ServerBlock::isValidHost(std::string host) const
{
	struct sockaddr_in sockaddr;
    
  	return (inet_pton(AF_INET, host.c_str(), &(sockaddr.sin_addr)) ? true : false);
    // inet_pton : human_readable text 형태의 IPv4, IPv6 주소를 binary로 변환한다.
    // 인자 1: AF_INET (IPv4) 혹은 AF_INET6 (IPv6) 둘중 하나
    // 인자 2: 문자열 형태의 IP 주소
    // 인자 3: IP주소를 binary로 변환한 결과를 저장할 메모리 주소
    // 리턴 : 성공시 1, 인자1이 잘못되면 -1, 인자2가 잘못되면 0을 반환
}

/* error pages가 valid한지 검사한다. */
bool ServerBlock::isValidErrorPages()
{
	std::map<short, std::string>::const_iterator it;
	for (it = this->_error_pages.begin(); it != this->_error_pages.end(); it++)
	{
		if (it->first < 100 || it->first > 599) // HTTP response code는 1XX ~ 5XX (custon response code도 고려)
			return (false);
		if (checkFile(getRoot() + it->second, R_OK) < 0)
			return (false);
	}
	return (true);
}

/* location이 valid한지 검사한다. 1은 CGI 오류, 2는 path 오류, 3은 redirection file 오류, 4는 alias file 오류, 0은 정상 */
int ServerBlock::isValidLocation(LocationBlock &location) const
{
	// 만약 path가 /cgi-bin 일 경우, CGI에 대한 valid를 진행한다.
	if (location.getPath() == "/cgi-bin")
	{
		// cgi_path, cgi_extension, index 셋 중 하나라도 비어있다면 에러
		if (location.getCgiPath().empty() || location.getCgiExtension().empty() || location.getIndexLocation().empty())
			return (1);

		// index 파일을 읽을 수 없으면, root/path/index 탐색, 없으면 현재디렉토리/path/index 탐색, 모두 실패시 (없거나 읽을수 없음) 에러
		if (checkFile(location.getIndexLocation(), R_OK) < 0)
		{
			std::string path = location.getRootLocation() + location.getPath() + "/" + location.getIndexLocation();
			if (getTypePath(path) != 1)
			{				
				std::string root = getcwd(NULL, 0);
				location.setRootLocation(root);
				path = root + location.getPath() + "/" + location.getIndexLocation();
			}
			if (path.empty() || getTypePath(path) != 1 || checkFile(path, R_OK) < 0)
				return (1);
		}
		// cgi_path와 cgi_extension의 수가 다를경우 에러 (둘다 std::vector<std::string>)
		if (location.getCgiPath().size() != location.getCgiExtension().size())
			return (1);
		
		// cgi_path를 탐색하며 하나라도 stat함수(파일 정보 조회) 호출에 실패할 경우 에러
		std::vector<std::string>::const_iterator it;
		for (it = location.getCgiPath().begin(); it != location.getCgiPath().end(); ++it)
		{
			if (getTypePath(*it) < 0)
				return (1);
		}
		// cgi_exteision을 탐색하며 .py 나 .sh 파일이 아니면 에러
		std::vector<std::string>::const_iterator it_path;
		for (it = location.getCgiExtension().begin(); it != location.getCgiExtension().end(); ++it)
		{
			std::string tmp = *it;
			if (tmp != ".py" && tmp != ".sh" && tmp != "*.py" && tmp != "*.sh")
				return (1);
			// cgi_path에서 cgi_extension이 .py 파일일 경우 python이 포함되어있는지 확인하고, 있으면 ext_path에 추가. .sh일 경우에 bash가 포함되어 있는지 확인후 있으면 ext_path에 추가.
			for (it_path = location.getCgiPath().begin(); it_path != location.getCgiPath().end(); ++it_path)
			{
				std::string tmp_path = *it_path;
				if (tmp == ".py" || tmp == "*.py")
				{
					if (tmp_path.find("python") != std::string::npos)
						location._ext_path.insert(std::make_pair(".py", tmp_path));
				}
				else if (tmp == ".sh" || tmp == "*.sh")
				{
					if (tmp_path.find("bash") != std::string::npos)
						location._ext_path[".sh"] = tmp_path;
				}
			}
		}
		// fix : 이미 존재하는 로직 중복
		// if (location.getCgiPath().size() != location.getExtensionPath().size())
		// 	return (1);
	}
	else // not CGI
	{
		// path경로가 / 로 시작하지 않는다면 에러
		if (location.getPath()[0] != '/')
			return (2);
		// root값이 없다면 server의 root로 설정 
		/* check */ // 이거는 여기 검사하는 항목이 아니라 location값 설정하는 setLocation() 으로 옮겨야 할것으로 보임
		if (location.getRootLocation().empty())
		{
			location.setRootLocation(this->_root);
		}
		// index 파일이 읽을수 있는지 확인한다. 먼저 index를 확인하고, 실패하면 root/path/index를 확인한다. 성공시 0, 실패시 -1 반환
		if (isFileExistAndReadable(location.getRootLocation() + location.getPath() + "/", location.getIndexLocation()))
			return (5);
		// return 이 비었지 않다면, return 및 root/return 이 readable한지 확인.
		if (!location.getReturn().empty())
		{
			if (isFileExistAndReadable(location.getRootLocation(), location.getReturn()))
				return (3);
		}
		// alias 가 비어있지 않다면, alias 및 root/alias 가 readable한지 확인.
		if (!location.getAlias().empty())
		{
			if (isFileExistAndReadable(location.getRootLocation(), location.getAlias()))
			 	return (4);
		}
	}
	// 오류없음
	return (0);
}

/* --------------------------------------------------------------------------- */
/* ------------------------------ get methods -------------------------------- */
/* --------------------------------------------------------------------------- */

const std::string &ServerBlock::getServerName()
{
	return (this->_server_name);
}

const std::string &ServerBlock::getRoot()
{
	return (this->_root);
}

const bool &ServerBlock::getAutoindex()
{
	return (this->_autoindex);
}

const in_addr_t &ServerBlock::getHost()
{
	return (this->_host);
}

const uint16_t &ServerBlock::getPort()
{
	return (this->_port);
}

const size_t &ServerBlock::getClientMaxBodySize()
{
	return (this->_client_max_body_size);
}

const std::vector<LocationBlock> &ServerBlock::getLocations()
{
	return (this->_locations);
}

const std::map<short, std::string> &ServerBlock::getErrorPages()
{
	return (this->_error_pages);
}

const std::string &ServerBlock::getIndex()
{
	return (this->_index);
}

int   	ServerBlock::getFd() 
{ 
	return (this->_listen_fd); 
}

/* the two functions below can be used later for response */
const std::string &ServerBlock::getPathErrorPage(short key)
{
	std::map<short, std::string>::iterator it = this->_error_pages.find(key);
	if (it == this->_error_pages.end())
		throw ErrorHandler::ErrorException("Error_page does not exist");
	return (it->second);
}

/* find location by a name */ //do not using in parser, created for server manager
const std::vector<LocationBlock>::iterator ServerBlock::getLocationKey(std::string key)
{
	std::vector<LocationBlock>::iterator it;
	for (it = this->_locations.begin(); it != this->_locations.end(); it++)
	{
		if (it->getPath() == key)
			return (it);
	}
	throw ErrorHandler::ErrorException("Error: path to location not found");
}

/* 세미콜론이 문자열 끝에만 위치한지 확인하고, 아니라면 예외 throw, 맞다면 세미콜론 제거 */
void ServerBlock::checkToken(std::string &parametr)
{
	size_t pos = parametr.rfind(';');
	if (pos != parametr.size() - 1)
		throw ErrorHandler::ErrorException("Token is invalid");
	parametr.erase(pos);
}

/* 각 location을 순회하며 중복된 path를 가진게 있는지 확인 */
bool ServerBlock::checkLocaitons() const
{
	if (this->_locations.size() < 2)
		return (false);
	std::vector<LocationBlock>::const_iterator it1;
	std::vector<LocationBlock>::const_iterator it2;
	for (it1 = this->_locations.begin(); it1 != this->_locations.end() - 1; it1++) {
		for (it2 = it1 + 1; it2 != this->_locations.end(); it2++) {
			if (it1->getPath() == it2->getPath())
				return (true);
		}
	}
	return (false);
}

/* socket setup and binding */
void	ServerBlock::setupServer(void)
{
	if ((_listen_fd = socket(AF_INET, SOCK_STREAM, 0) )  == -1 )
    {
		//Logger::logMsg(RED, CONSOLE_OUTPUT, "webserv: socket error %s   Closing ....", strerror(errno));
        exit(EXIT_FAILURE);
    }

    int option_value = 1;
    setsockopt(_listen_fd, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int));
    memset(&_server_address, 0, sizeof(_server_address));
    _server_address.sin_family = AF_INET;
    _server_address.sin_addr.s_addr = _host;
    _server_address.sin_port = htons(_port);
    if (bind(_listen_fd, (struct sockaddr *) &_server_address, sizeof(_server_address)) == -1)
    {
		//Logger::logMsg(RED, CONSOLE_OUTPUT, "webserv: bind error %s   Closing ....", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

const void ServerBlock::printServerBlock()
{
	struct sockaddr_in sockaddr;
	sockaddr.sin_addr.s_addr = this->_host;

	std::cout << RED << "---------- Server ----------" << CYN << std::endl;
	std::cout << "port :\t" << this->_port << std::endl;
	std::cout << "host :\t" << inet_ntoa(sockaddr.sin_addr) << std::endl;
	std::cout << "server_name :\t" << this->_server_name << std::endl;
	std::cout << "root :\t" << this->_root << std::endl;
	std::cout << "index :\t" << this->_index << std::endl;
	std::cout << "autoindex :\t" << this->_autoindex << std::endl;
	std::cout << "listen_fd :\t" << this->_listen_fd << std::endl;
	std::cout << "cmaxbsize : \t" << this->_client_max_body_size << std::endl;
	std::cout << "error_pages :\t" << std::endl;
	for (std::map<short, std::string>::iterator it = this->_error_pages.begin(); it != this->_error_pages.end(); it++)
		std::cout << it->first << " " << it->second << std::endl;
	for (std::vector<LocationBlock>::iterator it = this->_locations.begin(); it != this->_locations.end(); it++)
		it->printLocationBlock();
	std::cout << NC;
}