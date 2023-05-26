#include "Webserv.hpp"

/* 문자열에서 숫자가 아닌 글자가 하나라도 있는지 확인 */
bool isNumber(std::string input)
{
    for(size_t i = 0; i < input.length(); i++)
    {
        if (!std::isdigit(input[i]))
            return false;
    }
    return true;
}

/* string을 int로 변환, 실패시 음수 반환 */
int ft_stoi(std::string input)
{
    long result = 0;
    int sign = 1;
    std::string::const_iterator it = input.begin();
    if (*it == '-') 
    {
        sign = -1;
        ++it;
    }
    for (; it != input.end(); ++it) 
    {
        if (isdigit(*it)) 
        {
            result = result * 10 + (*it - '0');
            if (sign == 1 && result >= std::numeric_limits<int>::max() + 1.0)
                return INT_MAX;
            else if (sign == -1 && result >= std::numeric_limits<int>::max() + 1.0)
                return INT_MIN;
        } 
        else 
            break;
    }
    return result * sign;
}

/* file일경우 1, directory일경우 2,  그 외에는 3을 반환. stat 함수 실패시 -1을 반환 */
int getTypePath(std::string const path)
{
	struct stat	buffer;
	int			result;
	
	result = stat(path.c_str(), &buffer); // stat : 파일의 정보를 조회, 성공시 0을 반환. 실패시 -1 반환
	if (result == 0)
	{
		if (buffer.st_mode & S_IFREG) // S_IFREG : regular file
			return (1);
		else if (buffer.st_mode & S_IFDIR) // S_IFDIR : directory
			return (2);
		else
			return (3);
	}
	else
		return (-1);
}

/* 파일을 access할 수 있는지 확인 후 있으면 0, 아니면 -1 반환 */
int	checkFile(std::string const path, int mode)
{
	return (access(path.c_str(), mode));
	/* mode에 4(R_OK)는 읽기, 2(W_OK)는 쓰기, 1(X_OK)은 실행, 0은 존재여부만 검사한다. */
}

/* 파일을 read 할 수 있는지 확인 후 있으면 0, 아니면 -1 반환 */
int isFileExistAndReadable(std::string const path, std::string const index)
{
	if (getTypePath(index) == 1 && checkFile(index, R_OK) == 0) // regular file이고, 읽을 수 있음
		return (0);
	if (getTypePath(path + index) == 1 && checkFile(path + index, R_OK) == 0)
		return (0);
	return (-1);
}

/* 파일을 읽어 하나의 문자열로 저장 */
std::string	readFile(std::string path)
{
	if (path.empty())
		return (NULL);

	std::ifstream config_file(path.c_str());
	if (!config_file || !config_file.is_open()) // is_open : 파일이 열려있음
		return (NULL);

	/* config 파일은 인자로 받은 경로를 통해 연 파일의 스트림이다. 따라서 config_file.rdbuf() 를 통해 해당 파일 스트림의 스트림 버퍼를 얻을 수 있다. */
    /* 스트림 버퍼의 모든 내용물을 stringstream 으로 넣어준다. 이후 stringstream.str() 을 통해 스트림 내용물을 문자열로 변환하여 반환한다. */
	std::stringstream stream_binding;
	stream_binding << config_file.rdbuf(); //rdbuf() : 스트림 버퍼를 연관시키거나 얻는다.
	return (stream_binding.str()); // ifstream은 소멸자 호출시 자동으로 close됨
}

std::string statusCodetoString(int error_code)
{
    switch(error_code)
    {
        case 100:
            return "Continue";
        case 101:
            return "Switching Protocol";
        case 102:
            return "Processing";
        case 103:
            return "Early Hints";
        case 200:
            return "0K";
        case 201:
            return "Created";
        case 202:
            return "Accepted";
        case 203:
            return "Non-Authoritative Information";
        case 204:
            return "No Content";
        case 205:
            return "Reset Content";
        case 207:
            return "Multi-Status";
        case 208:
            return "Multi-Status";
        case 226:
            return "IM Used";
        case 300:
            return "Multiple Choice";
        case 301:
            return "Moved Permanently";
        case 302:
            return "Found";
        case 303:
            return "See Other";
        case 304:
            return "Not Modified";
        case 305:
            return "Use Proxy";
        case 306:
            return "unused";
        case 307:
            return "Temporary Redirect";
        case 308:
            return "Permanent Redirect";
        case 400:
            return "Bad Request";
        case 401:
            return "Unauthorized";
        case 402:
            return "Payment Required";
        case 403:
            return "Forbidden";
        case 404:
            return "Not Found";
        case 405:
            return "Method Not Allowed";
        case 406:
            return "Not Acceptable";
        case 407:
            return "Proxy Authentication Required";
        case 408:
            return "Request Timeout";
        case 409:
            return "Conflict";
        case 410:
            return "Gone";
        case 411:
            return "Length Required";
        case 412:
            return "Precondition Failed";
        case 413:
            return "Payload Too Large";
        case 414:
            return "URI Too Long";
        case 415:
            return "Unsupported Media Type";
        case 416:
            return "Requested Range Not Satisfiable";
        case 417:
            return "Expectation Failed";
        case 418:
            return "I'm a teapot";
        case 421:
            return "Misdirected Request";
        case 422:
            return "Unprocessable Entity";
        case 423:
            return "Locked";
        case 424:
            return "Failed Dependency";
        case 425:
            return "Too Early";
        case 426:
            return "Upgrade Required";
        case 428:
            return "Precondition Required";
        case 429:
            return "Too Many Requests";
        case 431:
            return "Request Header Fields Too Large";
        case 451:
            return "Unavailable For Legal Reasons";
        case 500:
            return "Internal Server Error";
        case 501:
            return "Not Implemented";
        case 502:
            return "Bad Gateway";
        case 503:
            return "Service Unavailable";
        case 504:
            return "Gateway Timeout";
        case 505:
            return "HTTP Version Not Supported";
        case 506:
            return "Variant Also Negotiates";
        case 507:
            return "Insufficient Storage";
        case 508:
            return "Loop Detected";
        case 510:
            return "Not Extended";
        case 511:
            return "Network Authentication Required";
        default:
            return "Undefined";
    }
}

/* string utilities */
bool both_are_spaces(char lhs, char rhs)
{
    return std::isspace(lhs) && std::isspace(rhs);
}

/* 연속해서 나오는 공백을 하나만 남기고 제거, 시작과 끝 지점의 공백, 줄바꿈 전부 제거 */
void remove_duplicate_spaces_and_trim(std::string& str)
{
    // Remove duplicate spaces
    std::string::iterator new_end = std::unique(str.begin(), str.end(), both_are_spaces);
    str.erase(new_end, str.end());

    // Trim leading spaces
    std::string::size_type first_non_space = str.find_first_not_of(' ');
    if (first_non_space != std::string::npos) {
        str = str.substr(first_non_space);
    } else {
        str.clear();
        return;
    }

    // Trim trailing spaces
    std::string::size_type last_non_space = str.find_last_not_of(' ');
    if (last_non_space != std::string::npos) {
        str.erase(last_non_space + 1);
    }

    // Trim leading newlines
    first_non_space = str.find_first_not_of('\n');
    if (first_non_space != std::string::npos) {
        str = str.substr(first_non_space);
    } else {
        str.clear();
        return;
    }

    // Trim trailing newlines
   last_non_space = str.find_last_not_of('\n');
    if (last_non_space != std::string::npos) {
        str.erase(last_non_space + 1);
    }
}

/* 탭을 공백으로 치환 */
void replaceTabsWithSpaces(std::string& str)
{
    const char tab = '\t';
    const char space = ' ';
    size_t pos = str.find(tab);

    while (pos != std::string::npos) {
        str.replace(pos, 1, 1, space);
        pos = str.find(tab, pos + 1);
    }
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

/* 세미콜론이 문자열 끝에만 위치한지 확인하고, 아니라면 예외 throw, 맞다면 세미콜론 제거 */
void removeSemicolon(std::string &parametr)
{
	size_t pos = parametr.rfind(';');
	if (pos != parametr.size() - 1)
		throw ErrorHandler::ErrorException("Token is invalid");
	parametr.erase(pos);
}