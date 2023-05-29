#ifndef UTILITY_HPP
#define UTILITY_HPP

# include "Webserv.hpp"

enum FileType getFileType(std::string const file_path);
bool checkFilePermission(std::string const file_path, int mode);

#endif