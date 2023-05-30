#include "ConfigParser.hpp"

/* -------------------------------------------------- */
/* ------------------static methods------------------ */
/* -------------------------------------------------- */

/*
    search "server {" and return position of '{'

    if the first letter is not 's'
    if the first word is not "server"
    if letter after "server" is not '{' throw error
    if there's no server block, return start
*/
static size_t findStartServer(size_t start, std::string &content)
{
	size_t i;

	for (i = start; content[i]; i++)
	{
		if (content[i] == 's')
			break ;
		if (!isspace(content[i]))
			throw std::runtime_error("Error on findStartServer() : not starting with 's'");
	}
	if (!content[i])
	  return (start);

	if (content.compare(i, 6, "server") != 0)
		throw std::runtime_error("Error on findStartServer() : not starting with 'server'");
  
	i += 6;
    if (isspace(content[i]))
        i++;
  
	if (content[i] == '{')
		return (i);
	else
		throw std::runtime_error("Error on findStartServer() : not trailing with '{'");
}

/*
    find position of "}" which indicates end of server block

    find server block's end by checking parenthesis count
    if there's no server block end, return start
*/
static size_t findEndServer(size_t start, std::string &content)
{
	size_t	i;
	size_t	scope;
	
	scope = 0;
	for (i = start; content[i]; i++)
	{
		if (content[i] == '{')
			scope++;
		if (content[i] == '}')
		{
            scope--;
			if (!scope)
				return (i);
		}
	}
	return (start);
}

/*
    split content by server blocks

    split content by server blocks, and store each server block in vector
    the result is stored in second parameter, to avoid inefficient copy assignment
*/
static void splitContentByServers(std::string& content, std::vector<std::string>& ret_vec)
{
    size_t start = 0;
	size_t end = 1;

	while (start != end && start < content.length())
	{
		start = findStartServer(start, content);
		end = findEndServer(start, content);
		if (start == end)
			throw std::runtime_error("Error on splitContentByServers() : scope error");
		ret_vec.push_back(content.substr(start, end - start + 1));
		start = end + 1;
	}
}

/* 
    remove comments in string
*/
static void removeComments(std::string& str)
{
    size_t pos1, pos2;

    pos1 = str.find('#');
    while (pos1 != std::string::npos)
    {
        pos2 = str.find('\n', pos1);
        str.erase(pos1, pos2 - pos1);
        pos1 = str.find('#');
    }
}

/* 
    replaces tabs with spaces in string 
*/
static void replaceTabsWithSpaces(std::string& str)
{
    const char tab = '\t';
    const char space = ' ';
    size_t pos = str.find(tab);

    while (pos != std::string::npos) {
        str.replace(pos, 1, 1, space);
        pos = str.find(tab, pos + 1);
    }
}

/* 
    check whether both characters are spaces(true) or not(false) 
*/
static bool checkBothCharSpaces(char lhs, char rhs)
{
    return std::isspace(lhs) && std::isspace(rhs);
}

/* 
    check whether both characters are newlines(true) or not(false)
*/
static bool checkBothNewlines(char lhs, char rhs)
{
    return (lhs == '\n') && (rhs == '\n');
}

/* 
    remove duplciated, frontmost and rearmost, spaces and newliens 
*/
static void removeDupAndMostSpacesAndNewlines(std::string& str)
{
    std::string::iterator strit;
    std::string::size_type strtype;

    strit = std::unique(str.begin(), str.end(), checkBothCharSpaces);
    str.erase(strit, str.end());

    strit = std::unique(str.begin(), str.end(), checkBothNewlines);
    str.erase(strit, str.end());

    strtype = str.find_first_not_of(' ');
    if (strtype != std::string::npos) {
        str = str.substr(strtype);
    } else {
        str.clear();
        return;
    }

    strtype = str.find_last_not_of(' ');
    if (strtype != std::string::npos) {
        str.erase(strtype + 1);
    }

    strtype = str.find_first_not_of('\n');
    if (strtype != std::string::npos) {
        str = str.substr(strtype);
    } else {
        str.clear();
        return;
    }

    strtype = str.find_last_not_of('\n');
    if (strtype != std::string::npos) {
        str.erase(strtype + 1);
    }
}

/* -------------------------------------------------- */
/* ------------------normal methods------------------ */
/* -------------------------------------------------- */

/*
    open file and parse into several strings, per server block

    validate file, open file and read its content to string
    then, split string per server block strings.
*/
void parseConfigFile(std::string& config_file_path, Cluster& cluster)
{
    std::vector<Server> server_vector;
    std::vector<std::string> server_strings;
    std::string content;

    if (getFileType(config_file_path) != 1)
        throw std::runtime_error("Error on parseConfigFileToCluster() : not a regular file");
    if (checkFilePermission(config_file_path, R_OK) == false)
        throw std::runtime_error("Error on parseConfigFileToCluster() : no permission to file_read");
    
    content = readFile(config_file_path);

    if (content.empty())
        throw std::runtime_error("Error on parseConfigFileToCluster() : empty file content");
    else if (content.find("server", 0) == std::string::npos)
        throw std::runtime_error("Error on parseConfigFileToCluster() : no server block");
    
    preprocessFileContent(content);
    splitContentByServers(content, server_strings);
    parseStringToServers(server_strings, server_vector);
    cluster.setClusterServers(server_vector);
}

/*
    parse several server_block info strings to Servers vector.

    input : "server { ~ }", "server { ~ }", ...
*/
void parseStringToServers(std::vector<std::string>& server_strings, std::vector<Server>& server_vector)
{
    for (size_t i = 0; i < server_strings.size(); i++)
    {
        Server server_info;
        parseServerInfo(server_strings[i], server_info);
    }
}

/*
    parse one server_block info string to Server instance

    input : "server { ~ }"
*/
void parseServerInfo(std::string& content, Server& server_info)
{
    std::vector<std::string> tokens;

    tokens = splitString(content += ' ', std::string(" \n\t"));
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i] == "listen" && i + 1 < tokens.size())
        {
            if (server_info.getServerPort() != "")
                throw std::runtime_error("Error on parseServerInfo : duplicated port");
        }
    }
}

/*  
    open file and return file content by string

    ifstream.rdbuf() returns stream buffer. 
    push all contents in stream buffer to stringstream, then
    convent contents to string by stringstream.str()
*/
std::string readFile(std::string& config_file_path)
{
    std::ifstream       ifs;
    std::stringstream   ss_bind;

    if (config_file_path.empty())
        return (NULL);
    
    ifs.open(config_file_path.c_str());
    if (!ifs || ifs.fail())
        return (NULL);
    
    ss_bind << ifs.rdbuf();
    return (ss_bind.str());
}

/* 
    preprocess File content to be clear

    delete comments, replace tabs with spaces
    remove duplciated or frontmost/rearmost spaces and newliens 
*/ 
void preprocessFileContent(std::string &content)
{
    removeComments(content);
    replaceTabsWithSpaces(content);
    removeDupAndMostSpacesAndNewlines(content);
}