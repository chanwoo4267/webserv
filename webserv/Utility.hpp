#ifndef UTILITY_HPP
#define UTILITY_HPP

# include "Webserv.hpp"

enum FileType getFileType(std::string const file_path);
bool checkFilePermission(std::string const file_path, int mode);
std::vector<std::string> splitString(std::string str, std::string delim);
short ft_stos(std::string str);
int ft_stoi(std::string str);
bool ft_isnum(std::string str);

/* function that changes param */
bool removeSemicolon(std::string &parametr);

#endif