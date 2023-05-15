#pragma once
#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
// # include <fstream>
# include <fcntl.h>
# include <cstring>
# include <string> 
# include <unistd.h>
# include <dirent.h>
# include <sstream>
// # include <bits/stdc++.h>
# include <cstdlib>
# include <fstream>
# include <sstream>
# include <cctype>
# include <ctime>
# include <cstdarg>

/* STL Containers */
# include <map>
# include <set>
# include <vector>
# include <algorithm>
# include <iterator>
# include <list>

/* System */
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <unistd.h>
// # include <machine/types.h>
# include <signal.h>

/* Network */
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/select.h>
# include <arpa/inet.h>

# include "ConfigParser.hpp"
# include "ConfigFile.hpp"
# include "ServerConfig.hpp"
# include "Location.hpp"
# include "HttpRequest.hpp"
# include "CgiHandler.hpp"
# include "Mime.hpp"
# include "Logger.hpp"

#define CONNECTION_TIMEOUT 60 // 아무 데이터 전송이 없으면 클라이언트가 서버로부터 추방되는데 걸리는 시간 (초 단위)

// 부하 테스트용 매크로 상수
#ifdef TESTER
    #define MESSAGE_BUFFER 40000 
#else
    #define MESSAGE_BUFFER 40000
#endif

//URI 최대길이
#define MAX_URI_LENGTH 4096
//body 최대 길이
#define MAX_CONTENT_LENGTH 30000000

// 받은 인자를 문자열로 변환
template <typename T>
std::string toString(const T val)
{
    std::stringstream stream;
    stream << val;
    return stream.str();
}

#endif