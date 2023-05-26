#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "Webserv.hpp"

bool isNumber(std::string input);
int ft_stoi(std::string input);

int getTypePath(std::string const path);
int	checkFile(std::string const path, int mode);
int isFileExistAndReadable(std::string const path, std::string const index);
std::string	readFile(std::string path);

std::string statusCodetoString(int error_code);

/* string utilities */
bool both_are_spaces(char lhs, char rhs);
void remove_duplicate_spaces_and_trim(std::string& str);
void replaceTabsWithSpaces(std::string& str);
std::vector<std::string> splitParametrs(std::string line, std::string sep);
void removeSemicolon(std::string &parametr);

#endif