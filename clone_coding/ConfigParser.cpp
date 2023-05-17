#include "ConfigParser.hpp"

/* main function for parse and create config */
int ConfigParser::createCluster(const std::string &config_file)
{
  std::string content;
  ConfigFile file(config_file); // 생성자로 path = config_file이 됨

  if (file.getTypePath(file.getPath()) != 1) // path(config_file)이 regular file인지 검사
    throw ErrorException("file is invalid");
  if (file.checkFile(file.getPath(), R_OK) == -1) // path가 accessible한지 검사
    throw ErrorException("File is not accessible");

  content = file.readFile(config_file);

  if (content.empty()) // 내용물이 없다면
    throw ErrorException("File is empty");
  if (content.find("server", 0) == std::string::npos) // server 블록이 없다면
		throw ErrorException("Server did not find");
  
  removeComments(content);
  removeWhiteSpace(content);

  splitServers(content);
  // refactor : 불필요한 예외처리
  // if (this->_server_config.size() != this->_nb_server)
  //   throw ErrorException("Somthing with size");

  for (size_t i = 0; i < this->_nb_server; i++)
  {
    ServerConfig server;
    createServer(this->_server_config[i], server);
    this->_servers.push_back(server);
  }

  if (this->_nb_server > 1)
    checkServers();
  return (0);
}

/* creating Server from string and fill the value */
void ConfigParser::createServer(std::string &config, ServerConfig &server)
{
	std::vector<std::string>	parametrs;
	std::vector<std::string>	error_codes;

	int		flag_loc = 1;
	bool	flag_autoindex = false;
	bool	flag_max_size = false;

  // 서버 블록을 공백, 줄바꿈, 탭 기준으로 잘라서 벡터화. 원할한 분리를 위해 config 끝에 공백을 넣어줌
	parametrs = splitParametrs(config += ' ', std::string(" \n\t"));

  /* check */ // 필수는 port 하나 아닌가?
	if (parametrs.size() < 3)
		throw  ErrorException("Failed server validation");

  // 분리된 각 파라미터들에 대해
	for (size_t i = 0; i < parametrs.size(); i++)
	{
    // listen이고, 다음 파라미터가 있고, flag_loc 이 set 이라면 - /* check */ 왜 flag_set 을 통해서 location 블록 위치를 끝으로 강제?
		if (parametrs[i] == "listen" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (server.getPort()) // 중복된 포트 입력 예외처리
				throw  ErrorException("Port is duplicated");
			server.setPort(parametrs[++i]); // 포트 입력
		} // location이고 다음 파라미터가 있다면
		else if (parametrs[i] == "location" && (i + 1) < parametrs.size())
		{
			std::string	path;
			i++;
      // location 뒤에 경로가 오고, 그 뒤에 { ~ } 가 와야 함
			if (parametrs[i] == "{" || parametrs[i] == "}")
				throw  ErrorException("Wrong character in server scope{}");

			path = parametrs[i]; // 경로 path에 저장
  
			std::vector<std::string> codes;
			if (parametrs[++i] != "{")
				throw  ErrorException("Wrong character in server scope{}");
			i++;
  
      // location / { ~ } 에서 각 내용물들을 codes에 벡터 형태로 저장
			while (i < parametrs.size() && parametrs[i] != "}")
				codes.push_back(parametrs[i++]);
  
      // location으로 저장
			server.setLocation(path, codes);
			if (i < parametrs.size() && parametrs[i] != "}")
				throw  ErrorException("Wrong character in server scope{}");
			flag_loc = 0;
		} // host이면서 파라미터가 더 존재
		else if (parametrs[i] == "host" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (server.getHost())
				throw  ErrorException("Host is duplicated");
			server.setHost(parametrs[++i]);
		} // root이면서 파라미터가 더 존재
		else if (parametrs[i] == "root" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (!server.getRoot().empty())
				throw  ErrorException("Root is duplicated");
			server.setRoot(parametrs[++i]);
		} // error_page 이면서 파라미터가 더 존재
		else if (parametrs[i] == "error_page" && (i + 1) < parametrs.size() && flag_loc)
		{
			while (++i < parametrs.size())
			{
				error_codes.push_back(parametrs[i]);
				if (parametrs[i].find(';') != std::string::npos)
					break ;
				if (i + 1 >= parametrs.size())
					throw ErrorException("Wrong character out of server scope{}");
			} // [ error_page 404 405 406 .... /error.html; ] 와 같은 형식이므로, error_codes에 [404] [405] ... [/error.html;] 형태로 저장
		} // client_max_body_size 및 다음 파라미터 존재시
		else if (parametrs[i] == "client_max_body_size" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (flag_max_size) /* check */ // 이거뿐만 아니라, 실제 nginx에서 duplciated value에 대한 예외처리가 어떻게 되는지 확인해보기
				throw  ErrorException("Client_max_body_size is duplicated");
			server.setClientMaxBodySize(parametrs[++i]);
			flag_max_size = true;
		} // server_name 및 다음 파라미터 존재시
		else if (parametrs[i] == "server_name" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (!server.getServerName().empty())
				throw  ErrorException("Server_name is duplicated");
			server.setServerName(parametrs[++i]);
		} // index 및 다음 파라미터 존재시
		else if (parametrs[i] == "index" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (!server.getIndex().empty())
				throw  ErrorException("Index is duplicated");
			server.setIndex(parametrs[++i]);
		} // autoindex 및 다음 파라미터 존재시
		else if (parametrs[i] == "autoindex" && (i + 1) < parametrs.size() && flag_loc)
		{
			if (flag_autoindex)
				throw ErrorException("Autoindex of server is duplicated");
			server.setAutoindex(parametrs[++i]);
			flag_autoindex = true;
		} // 위 옵션들이 아니면서, { } 가 아닐시 에러
		else if (parametrs[i] != "}" && parametrs[i] != "{")
		{
			if (!flag_loc)
				throw  ErrorException("Parametrs after location");
			else
				throw  ErrorException("Unsupported directive");
		}
	} // end for

	if (server.getRoot().empty())
		server.setRoot("/;");
	if (server.getHost() == 0)
		server.setHost("localhost;");
	if (server.getIndex().empty())
		server.setIndex("index.html;");

  // index에 대한 검사 (존재 및 accessible)
	if (ConfigFile::isFileExistAndReadable(server.getRoot(), server.getIndex()))
		throw ErrorException("Index from config file not found or unreadable");

  // location 중복path 확인
	if (server.checkLocaitons())
		throw ErrorException("Locaition is duplicated");
  
  // port 있는지 확인
	if (!server.getPort())
		throw ErrorException("Port not found");
  
  // 에러 페이지 세팅
	server.setErrorPages(error_codes);

  // 에러 페이지 validation
	if (!server.isValidErrorPages())
		throw ErrorException("Incorrect path for error page or number of error");
}

/* sep 기준으로 line을 분리하여 vector 형태로 반환 */
std::vector<std::string> splitParametrs(std::string line, std::string sep)
{
	std::vector<std::string>	str;
	std::string::size_type		start, end;

	start = end = 0;
	while (1)
	{
		end = line.find_first_of(sep, start); // <string> 기본함수, start에서부터 sep 내용중 아무거나 일치하는 제일 첫번째 글자 위치를 반환
		if (end == std::string::npos)
			break;
		std::string tmp = line.substr(start, end - start);
		str.push_back(tmp);
		start = line.find_first_not_of(sep, end); // end 이후 sep가 아닌 첫 글자의 위치를 반환
		if (start == std::string::npos)
			break;
	}
	return (str);
}

/* # 문자가 등장하면 줄바꿈(\n) 까지 전부 삭제 */
void ConfigParser::removeComments(std::string &content)
{
  size_t pos;

  pos = content.find('#');
  while (pos != std::string::npos)
  {
    size_t pos_end;
    pos_end = content.find('\n', pos);
    content.erase(pos, pos_end - pos);
    pos = content.find('#');
  }
}

/* deleting whitespaces in the start, end and in the content if more than one */
void ConfigParser::removeWhiteSpace(std::string &content)
{
  size_t i, j = 0;

  while (content[i] && isspace(content[i]))
    i++;
  j = content.length() - 1;
  while (j > 0 && isspace(content[j]))
    j--;
  content = content.substr(i, j + 1);

  // refactor : 불필요한 substr 호출 생략
  // while (content[i] && isspace(content[i]))
  // 	i++;
  // content = content.substr(i);
  // i = content.length() - 1;
  // while (i > 0 && isspace(content[i]))
  // 	i--;
  // content = content.substr(0, i + 1);
}

/* spliting servers on separetly strings in vector */
void ConfigParser::splitServers(std::string &content)
{
	size_t start = 0;
	size_t end = 1;

	while (start != end && start < content.length())
	{
		start = findStartServer(start, content);
		end = findEndServer(start, content);
		if (start == end) // findStart, findEnd 둘다 발견 실패시 start를 그대로 반환함. 따라서 start == end 일 경우 에러
			throw ErrorException("problem with scope");
		this->_server_config.push_back(content.substr(start, end - start + 1)); // _server_config에 { ~ } 저장
		this->_nb_server++;
		start = end + 1;
	}
}

/* server 블록의 '{' 시작 위치를 반환 */
size_t ConfigParser::findStartServer (size_t start, std::string &content)
{
	size_t i;

	for (i = start; content[i]; i++) // 전체 내용물의 start에서부터 끝까지 탐색
	{
    // 형식은 반드시 server { ~ } 이므로, 's'가 나오면 반복종료, 공백이 아닌 다른문자가 오면 예외 throw
		if (content[i] == 's')
			break ;
		if (!isspace(content[i]))
			throw  ErrorException("Wrong character out of server scope{}");
	}
  // start부터 끝까지 전부 다 공백일경우 ( '{' 없을경우 )
	if (!content[i])
	  return (start);
  
  // 's'를 찾은 지점에서 6글자를 비교해서 'server'인지 확인
	if (content.compare(i, 6, "server") != 0)
		throw ErrorException("Wrong character out of server scope{}");
  
  // 이후 공백 스킵
	i += 6;
	while (content[i] && isspace(content[i]))
		i++;
  
  // 다음 문자가 '{' 라면 해당 위치 반환, 아니라면 예외 throw
	if (content[i] == '{')
		return (i);
	else
		throw  ErrorException("Wrong character out of server scope{}");
}

/* server 블록의 '}' 위치를 반환 */
size_t ConfigParser::findEndServer (size_t start, std::string &content)
{
	size_t	i;
	size_t	scope;
	
	scope = 0;
	for (i = start + 1; content[i]; i++)
	{
		if (content[i] == '{')
			scope++;
		if (content[i] == '}')
		{
			if (!scope) // 마지막 scope, 즉 server 블록의 '}' 발견
				return (i);
			scope--;
		}
	}
  //'{' 없을경우
	return (start);
}

/* 서버들 목록에서 port/host/server_name이 동일한게 있는지 체크 후 예외 throw */
void ConfigParser::checkServers()
{
  std::vector<ServerConfig>::iterator it1;
  std::vector<ServerConfig>::iterator it2;

  for (it1 = this->_servers.begin(); it1 != this->_servers.end() - 1; it1++)
  {
    for (it2 = it1 + 1; it2 != this->_servers.end(); it2++)
    {
      if (it1->getPort() == it2->getPort() && it1->getHost() == it2->getHost() && it1->getServerName() == it2->getServerName())
        throw ErrorException("Failed server validation");
    }
  }
}