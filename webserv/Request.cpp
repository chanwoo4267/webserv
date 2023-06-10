#include "Request.hpp"

Request::Request()
{
    this->_client_fd = 0;
    this->_method = "";
    this->_path = "";
    this->_body = "";
    this->_content_length = 0;
}

Request::~Request() {}

const int& Request::getClientFd()
{
    return this->_client_fd;
}

const std::string& Request::getMethod()
{
    return this->_method;
}

const std::string& Request::getPath()
{
    return this->_path;
}

const std::map<std::string, std::string>& Request::getHeaders()
{
    return this->_headers;
}

const std::string& Request::getBody()
{
    return this->_body;
}

const size_t& Request::getContentLength()
{
    return this->_content_length;
}

const std::string& Request::getProtocolVersion()
{
    return this->_protocol_version;
}

void Request::setClientFd(int fd)
{
    this->_client_fd = fd;
}

void Request::setMethod(std::string method)
{
    this->_method = method;
}

void Request::setPath(std::string path)
{
    this->_path = path;
}

void Request::setBody(std::string body)
{
    this->_body = body;
}

void Request::setContentLength(size_t content_length)
{
    this->_content_length = content_length;
}

void Request::pushHeader(std::string str1, std::string str2)
{
    this->_headers[str1] = str2;
}

void Request::setProtocolVersion(std::string protocol_version)
{
    this->_protocol_version = protocol_version;
}

size_t Request::parse_headers_line(std::string& request, size_t index)
{
    size_t delim = request.find_first_of(":", index);
    size_t end = request.find_first_of("\r\n", delim);
    this->_headers[request.substr(index, delim - index)] = request.substr(delim + 2, end - delim - 2);
    return end;
}

std::string Request::parse_request_body_chunked(std::string& request, size_t index)
{
    size_t i, r, size;
    std::string ret, size_buf, line;

    size = 1;
    i = 0;
    line = request.substr(index + 2, request.size());
    while (1)
    {
        r = line.find_first_of("\r\n");
        size_buf = line.substr(i, r);
        size = ft_stoh(size_buf);
        i = i + 2 + size_buf.size();
        if (size == 0)
            break;
        std::string buf = line.substr(i, i + size - 2);
        ret += buf;
        i = i + size + 4;
    }
    return ret;
}

std::string Request::parse_request_body(std::string& request, size_t index)
{
    return request.substr(index + 2, request.size());
}

/*
    parse Request message

    Request Message structure:
        Request Line(method, path, protocol version)
        Request Headers(header: value)
        Empty Line
        Request Message Body
*/
int Request::parseRequest(std::string request)
{
    size_t  i, j, end;

    // request Message starts with method type (ex GET POST ..)
    i = request.find_first_of(" ", 0);
    //set method
    this->setMethod(request.substr(0, i));
    if (checkMethodType(this->_method) == INVALID)
        return 400;
    
    // if there's no other data, error
    j = request.find_first_of(" ", i + 1);
    if (j == std::string::npos)
        return 400;
    
    // set path !check! is 404 right?
    this->setPath(request.substr(i + 1, j - i - 1));
    if (getFileType(this->_path) != DIRECTORY)
        return 404;
    if (checkFilePermission(this->_path, R_OK) == false)
        return 404;
    
    // set protocol version
    this->setProtocolVersion(request.substr(j + 1, request.find_first_of("\r", i) - j - 1));
    if (this->_protocol_version != "HTTP/1.1")
        return 505;

    // parse Request Headers and Body
    i = request.find_first_of("\n", j) + 1; // first header's start letter
    while (i < request.size())
    {
        if (request[i] == '\r' && request[i + 1] == '\0') // end
			break;
		if (request[i] == '\r' && request[i + 1] == '\n') // body
		{
            // 만약 헤더 파싱중 transfer-encoding 헤더를 넣었고, 그 값이 chunked라면 chunked-body로 파싱한다.
            // map의 index 접근시 없는 key값이라면 key값을 새로 생성해주고, 대응되는 값은 null값으로 들어간다.
			if (strstr(this->_headers["Transfer-Encoding"].c_str(), "chunked") != NULL)
				this->_body = parse_request_body_chunked(request, i);
			else
				this->_body = parse_request_body(request, i);

			break;
		}
		end = parse_headers_line(request, i); // parse one header, and return its end
		if (end + 1 == '\0') // if request message ends
			break;
		i = end + 2; // current end points to /r
    }
    if (_headers["Host"] == "")
        return 400;
    return 0;
}

/*
    get path of URI

    if path is "http://test.com?query=1"
    get "http://test.com"
*/
std::string Request::getUriPath()
{
    unsigned long i = this->_path.find_first_of("?", 0);
	if (i == std::string::npos)
		return this->_path;
	return this->_path.substr(0, i);
}

/*
    get query of URI

    if path is "http://test.com?query=1"
    get "query=1"
*/
std::string Request::getUriQuery()
{
    unsigned long i = this->_path.find_first_of("?", 0);
	if (i == std::string::npos)
		return "";
	return this->_path.substr(i + 1, this->_path.size() - i);
}