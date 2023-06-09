#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

# include "Webserv.hpp"
# include "Cluster.hpp"
# include "Utility.hpp"

void parseConfigFile(std::string& config_file_path, Cluster& cluster);
std::string readFile(std::string& config_file_path);
void preprocessFileContent(std::string& content);
void parseStringToServers(std::vector<std::string>& server_strings, std::vector<Server>& server_vector);
void parseServerBlock(std::string& content, Server& server_info);

#endif