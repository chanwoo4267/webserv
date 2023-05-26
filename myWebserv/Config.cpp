#include "Config.hpp"

Config::Config() 
{
	this->_nb_server = 0;
}

Config::~Config() 
{
	this->_servers.clear();
	this->_server_config.clear();
}

Config::Config(const Config &obj)
{
    this->_servers = obj._servers;
    this->_server_config = obj._server_config;
    this->_nb_server = obj._nb_server;
}

Config &Config::operator=(const Config &obj)
{
    if (this != &obj)
    {
        this->_servers = obj._servers;
        this->_server_config = obj._server_config;
        this->_nb_server = obj._nb_server;
    }
    return (*this);
}

/* config 파일을 열고 내용물을 읽어 해당 정보를 저장하는 _servers 구조체 설정 */
void Config::createCluster(const std::string &config_file)
{
    std::string content;

    if (getTypePath(config_file) != 1) // regular file인지 검사
        throw ErrorHandler::ErrorException("createCluster : not a file");
    else if (checkFile(config_file, R_OK) == -1) // file에 대한 권한 검사
        throw ErrorHandler::ErrorException("createCluster : not accessible file");
    
    content = readFile(config_file); // 파일을 열어 내용물을 하나의 문자열로 저장

    if (content.empty()) // 파일 내용물이 비었을경우
        throw ErrorHandler::ErrorException("createCluster : empty file");
    else if (content.find("server", 0) == std::string::npos) // 파일에 server 블록이 없을경우
        throw ErrorHandler::ErrorException("createCluster : no server block");

    preprocessContent(content);
    splitServers(content);

    for (size_t i = 0; i < this->_nb_server; i++)
    {
        ServerBlock server_block;
        createServer(this->_server_config[i], server_block);
        this->_servers.push_back(server_block);
    }

    if (this->_nb_server > 1)
        checkServers();
}

/* 주석을 전부 제거하고, 가장 앞과 뒤의 모든 공백을 제거하고, 중간에 나오는 중복된 공백은 하나로 줄인다 */
void Config::preprocessContent(std::string &content)
{
    size_t pos1, pos2;

    // 주석 전부 제거
    pos1 = content.find('#');
    while (pos1 != std::string::npos)
    {
        size_t pos2;
        pos2 = content.find('\n', pos1);
        content.erase(pos1, pos2 - pos1);
        pos1 = content.find('#');
    }

    replaceTabsWithSpaces(content); // 탭을 공백으로 치환, /* check whitespace 전부 처리? */
    remove_duplicate_spaces_and_trim(content); // 문자열의 가장 앞, 뒤의 공백 제거, 연속해서 나오는 공백 제거
}

/* 문자열을 server block 기준으로 잘라 _server_config에 저장 */
void Config::splitServers(std::string &content)
{
    size_t start = 0;
	size_t end = 1;

	while (start != end && start < content.length())
	{
		start = findStartServer(start, content);
		end = findEndServer(start, content);
		if (start == end) // findStart, findEnd 둘다 발견 실패시 start를 그대로 반환함. 따라서 start == end 일 경우 에러
			throw ErrorHandler::ErrorException("splitServers : scope error");
		this->_server_config.push_back(content.substr(start, end - start + 1)); // _server_config에 { ~ } 저장
		this->_nb_server++;
		start = end + 1;
	}
}

/* server 블록의 '{' 시작 위치를 반환 */
size_t Config::findStartServer (size_t start, std::string &content)
{
	size_t i;

	for (i = start; content[i]; i++) // 전체 내용물의 start에서부터 끝까지 탐색
	{
    // 형식은 반드시 server { ~ } 이므로, 's'가 나오면 반복종료, 공백이 아닌 다른문자가 오면 예외 throw
		if (content[i] == 's')
			break ;
		if (!isspace(content[i]))
			throw  ErrorHandler::ErrorException("findStartServer : not 's' character");
	}
    // start부터 끝까지 전부 s가 아닐경우
	if (!content[i])
	  return (start);

    // 's'를 찾은 지점에서 6글자를 비교해서 'server'인지 확인
	if (content.compare(i, 6, "server") != 0)
		throw ErrorHandler::ErrorException("findStartServer : not 'server' word" + content.substr(i));
  
    // 이후 공백 스킵
	i += 6;
	while (content[i] && isspace(content[i]))
		i++;
  
    // 다음 문자가 '{' 라면 해당 위치 반환, 아니라면 예외 throw
	if (content[i] == '{')
		return (i);
	else
		throw  ErrorHandler::ErrorException("findStartServer : not '{' character");
}

/* server 블록의 '}' 위치를 반환 */
size_t Config::findEndServer (size_t start, std::string &content)
{
	size_t	i;
	size_t	scope;
	
	scope = 0;
	for (i = start; content[i]; i++)
	{
		if (content[i] == '{')
			scope++;
		if (content[i] == '}')
		{
            scope--;
			if (!scope) // 마지막 scope, 즉 server 블록의 '}' 발견
				return (i);
		}
	}
  //'{' 없을경우
	return (start);
}

/* 서버들 목록에서 port/host/server_name이 동일한게 있는지 체크 후 예외 throw */
void Config::checkServers()
{
  std::vector<ServerBlock>::iterator it1;
  std::vector<ServerBlock>::iterator it2;

  for (it1 = this->_servers.begin(); it1 != this->_servers.end() - 1; it1++)
  {
    for (it2 = it1 + 1; it2 != this->_servers.end(); it2++)
    {
      if (it1->getPort() == it2->getPort() && it1->getHost() == it2->getHost() && it1->getServerName() == it2->getServerName())
        throw ErrorHandler::ErrorException("checkServers : duplicated server found");
    }
  }
}

/* creating Server from string and fill the value */
void Config::createServer(std::string &config, ServerBlock &server)
{
	std::vector<std::string>	parametrs;
	std::vector<std::string>	error_codes;

	int		flag_loc = 1;
	bool	flag_autoindex = false;
	bool	flag_max_size = false;

  	// 서버 블록을 공백, 줄바꿈, 탭 기준으로 잘라서 벡터화. 원할한 분리를 위해 config 끝에 공백을 넣어줌
	parametrs = splitParametrs(config += ' ', std::string(" \n\t"));

  	// 분리된 각 파라미터들에 대해
	for (size_t i = 0; i < parametrs.size(); i++)
	{
    	// listen이고, 다음 파라미터가 있고, flag_loc 이 set 이라면 - /* check */ 왜 flag_set 을 통해서 location 블록 위치를 끝으로 강제?
		if (parametrs[i] == "listen" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (server.getPort()) // 중복된 포트 입력 예외처리
				throw  ErrorHandler::ErrorException("createServer : duplicated port");
			removeSemicolon(parametrs[++i]);
			server.setPort(parametrs[i]);
		} 	// location이고 다음 파라미터가 있다면
		else if (parametrs[i] == "location" && (i + 1) < parametrs.size())
		{
			std::string	path;

			path = parametrs[++i]; // 경로 path에 저장
  
			std::vector<std::string> loc_info;
			if (parametrs[++i] != "{")
				throw  ErrorHandler::ErrorException("createServer : not '{' character");
			i++;
  
      		// location / { ~ } 에서 각 내용물들을 loc_info에 벡터 형태로 저장
			while (i < parametrs.size() && parametrs[i] != "}")
				loc_info.push_back(parametrs[i++]);
  
      		// location으로 저장
			server.setLocation(path, loc_info);
			if (i < parametrs.size() && parametrs[i] != "}")
				throw  ErrorHandler::ErrorException("createServer : not '}' character");
			flag_loc = 0;
		} 	// host이면서 파라미터가 더 존재
		else if (parametrs[i] == "host" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (server.getHost())
				throw  ErrorHandler::ErrorException("createServer : duplicated host");
			removeSemicolon(parametrs[++i]);
			server.setHost(parametrs[i]);
		} 	// root이면서 파라미터가 더 존재
		else if (parametrs[i] == "root" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (!server.getRoot().empty())
				throw  ErrorHandler::ErrorException("createServer : duplicated root");
			removeSemicolon(parametrs[++i]);
			server.setRoot(parametrs[i]);
		} 	// error_page 이면서 파라미터가 더 존재
		else if (parametrs[i] == "error_page" && (i + 1) < parametrs.size() && flag_loc)
		{
			while (++i < parametrs.size())
			{
				error_codes.push_back(parametrs[i]);
				if (parametrs[i].find(';') != std::string::npos)
					break ;
				if (i + 1 >= parametrs.size())
					throw ErrorHandler::ErrorException("not ';' character at error_pages");
			} // [ error_page 404 405 406 .... /error.html; ] 와 같은 형식이므로, error_codes에 [404] [405] ... [/error.html;] 형태로 저장
		} 	// client_max_body_size 및 다음 파라미터 존재시
		else if (parametrs[i] == "client_max_body_size" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (flag_max_size) /* check */ // 이거뿐만 아니라, 실제 nginx에서 duplciated value에 대한 예외처리가 어떻게 되는지 확인해보기
				throw  ErrorHandler::ErrorException("createServer : duplicated client_max_body_size");
			removeSemicolon(parametrs[++i]);
			server.setClientMaxBodySize(parametrs[i]);
			flag_max_size = true;
		} 	// server_name 및 다음 파라미터 존재시
		else if (parametrs[i] == "server_name" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (!server.getServerName().empty())
				throw  ErrorHandler::ErrorException("createServer : duplicated server_name");
			removeSemicolon(parametrs[++i]);
			server.setServerName(parametrs[i]);
		} 	// index 및 다음 파라미터 존재시
		else if (parametrs[i] == "index" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (!server.getIndex().empty())
				throw  ErrorHandler::ErrorException("createServer : duplicated index");
			removeSemicolon(parametrs[++i]);
			server.setIndex(parametrs[i]);
		} 	// autoindex 및 다음 파라미터 존재시
		else if (parametrs[i] == "autoindex" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (flag_autoindex)
				throw ErrorHandler::ErrorException("createServer : duplicated autoindex");
			removeSemicolon(parametrs[++i]);
			server.setAutoindex(parametrs[i]);
			flag_autoindex = true;
		} 	// 위 옵션들이 아니면서, { } 가 아닐시 에러
		else if (parametrs[i] != "}" && parametrs[i] != "{")
		{
			if (!flag_loc)
				throw  ErrorHandler::ErrorException("createServer : parametre after location block");
			else
				throw  ErrorHandler::ErrorException("createServer : wrong syntax");
		}
	} // end for

	if (server.getRoot().empty())
		server.setRoot("/");
	if (server.getHost() == 0)
		server.setHost("localhost");
	if (server.getIndex().empty())
		server.setIndex("index.html");

  	// index에 대한 검사 (존재 및 accessible)
	if (isFileExistAndReadable(server.getRoot(), server.getIndex()))
		throw ErrorHandler::ErrorException("createServer : index file invalid");

  	// location 중복path 확인
	if (server.checkLocaitons())
		throw ErrorHandler::ErrorException("createServer : duplicated location");
  
  	// port 있는지 확인
	if (!server.getPort())
		throw ErrorHandler::ErrorException("createServer : invalid port");
  
  	// 에러 페이지 세팅
	server.setErrorPages(error_codes);

  	// 에러 페이지 validation
	if (!server.isValidErrorPages())
		throw ErrorHandler::ErrorException("createServer : invalid error code or path");
}

const void Config::printConfig()
{
	std::cout << RED << "---------- Config ----------" << CYN << std::endl;
	std::cout << "nb_servers :\t" << this->_nb_server << std::endl;
	for (std::vector<ServerBlock>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
		it->printServerBlock();
	std::cout << NC;
}