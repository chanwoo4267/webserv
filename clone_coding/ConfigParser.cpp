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
}