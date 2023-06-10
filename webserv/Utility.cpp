#include "Utility.hpp"

/* 
    get file type (REGULARFILE, DIRECTORY, NONE) 
*/
enum FileType getFileType(std::string const file_path)
{
    struct stat status;
    int         result;

    result = stat(file_path.c_str(), &status);
    if (!result)
    {
        if (status.st_mode & S_IFREG)
            return REGULARFILE;
        else if (status.st_mode & S_IFDIR)
            return DIRECTORY;
        else
            return NONE;
    }
    else
        return NONE;
}

/* 
    check whether file is accessible(true) or not(false) 
*/
bool checkFilePermission(std::string const file_path, int mode)
{
    if (!access(file_path.c_str(), mode))
        return true;
    else
        return false;
}

/*
    split string into tokens by given delim characters

    find any matches of delim by find_first_of()
    split string with substr(), push it, 
    and move start point to first non-delim char by find_first_not_of()
*/
std::vector<std::string> splitString(std::string str, std::string delim)
{
    std::vector<std::string>	vec;
	std::string::size_type		start, end;

	start = end = 0;
	while (1)
	{
		end = str.find_first_of(delim, start); // <string> 기본함수, start에서부터 delim 내용중 아무거나 일치하는 제일 첫번째 글자 위치를 반환
		if (end == std::string::npos)
			break;
		std::string tmp = str.substr(start, end - start);
		vec.push_back(tmp);
		start = str.find_first_not_of(delim, end); // end 이후 delim가 아닌 첫 글자의 위치를 반환
		if (start == std::string::npos)
			break;
	}
	return vec;
}

/*
    remove semicolon at the end of string

    if there's no semicolon, return false
    else, return true
*/
bool removeSemicolon(std::string &parametr)
{
	size_t pos = parametr.rfind(';');
	if (pos != parametr.size() - 1)
		return false;
	parametr.erase(pos);
    return true;
}

/*
    check whether given string is decimal number, or not

    if each character is non-digit, not '-' and '+' then return false
*/
bool ft_isnum(std::string str)
{
    for (size_t i = 0; i < str.length(); i++)
        if (!isdigit(str[i]) && str[i] != '-' && str[i] != '+')
            return false;
    return true;
}

/*
    convert string to short

    first convert to int by ft_stoi, then check bound
    if out of bound throw error
*/
short ft_stos(std::string str)
{
    int temp = ft_stoi(str);

    if (temp >= std::numeric_limits<short>::max() + 1.0)
        throw std::runtime_error("Error on ft_stos : out of bound");
    else if (temp <= std::numeric_limits<short>::min() - 1.0)
        throw std::runtime_error("Error on ft_stos : out of bound");
    
    return static_cast<short>(temp);
}

/*
    convert string to int

    check bound for int, and if out of bound throw error
*/
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
                throw std::runtime_error("Error on ft_stoi : out of bound");
            else if (sign == -1 && result >= std::numeric_limits<int>::max() + 2.0)
                throw std::runtime_error("Error on ft_stoi : out of bound");
        } 
        else 
            break;
    }
    return result * sign;
}

/*
    input string, convert it to correspond method type

    if not GET, POST, DELETE then return INVALID
*/
MethodType checkMethodType(std::string method)
{
    if (method == "GET")
        return GET;
    else if (method == "POST")
        return POST;
    else if (method == "DELETE")
        return DELETE;
    else if (method == "HEAD")
        return HEAD;
    else if (method == "PUT")
        return PUT;
    else if (method == "CONNECT")
        return CONNECT;
    else if (method == "OPTIONS")
        return OPTIONS; 
    else if (method == "TRACE")
        return TRACE;
    else if (method == "PATCH")
        return PATCH;  
    else
        return INVALID;
}

/*
    convert string to hex number
*/
size_t ft_stoh(std::string input)
{
	std::stringstream ss;
	size_t ret = 0;
	ss << std::hex << input;
	ss >> ret;

	return ret;
}

/*
    return status string in case of http status code
*/
std::string statusCodetoString(short status_code)
{
    switch(status_code)
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