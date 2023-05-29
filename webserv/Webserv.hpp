#ifndef WEBSERV_HPP
#define WEBSERV_HPP

/* includes */
# include <iostream>
# include <fstream>
# include <sstream>
# include <string>
# include <vector>
# include <map>
# include <ctime>
# include <algorithm>
# include <iterator>
# include <exception>
# include <stdexcept>

# include <fcntl.h>
# include <unistd.h>
# include <dirent.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <sys/stat.h>
# include <netinet/in.h>
# include <arpa/inet.h>

/* colors */
# define RED "\e[0;31m"
# define GRN "\e[0;32m"
# define YLW "\e[0;33m"
# define BLU "\e[0;34m"
# define CYN "\e[0;36m"

/* enum */
enum MethodType
{
    GET,
    POST,
    DELETE,
    NONE
};

enum FileType
{
    REGULARFILE,
    DIRECTORY,
    NONE
};

#endif