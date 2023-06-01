#include "ConfigParser.hpp"

/* -------------------------------------------------- */
/* ------------------static methods------------------ */
/* -------------------------------------------------- */

/*
    input string, convert it to correspond method type

    if not GET, POST, DELETE then return INVALID
*/
static MethodType checkMethodType(std::string method)
{
    if (method == "GET")
        return GET;
    else if (method == "POST")
        return POST;
    else if (method == "DELETE")
        return DELETE;
    else
        return INVALID;
}

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

    if (getFileType(config_file_path) != REGULARFILE)
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
        parseServerBlock(server_strings[i], server_info);
        server_vector.push_back(server_info);
    }
}

/*
    parse error_pages info and push to Server

    input : "server { ~ }", current index : "error_page"
*/
void parseErrorPages(std::vector<std::string>& tokens, Server& server_info, size_t& i)
{
    std::vector<short>  vec;
    size_t              size = tokens.size();

    while (++i < size)
    {
        removeSemicolon(tokens[i]);
        if (getFileType(tokens[i]) == REGULARFILE)
        {
            for(size_t t = 0; t < vec.size(); t++)
                server_info.pushServerErrorPage(vec[t], tokens[i]);
            break;
        }
        else
        {
            vec.push_back(ft_stos(tokens[i]));
        }
    }
}

/*
    parse one location_block info and push to Server

    input : "server { ~ }", current index : "location"
    last_idx directs to the last token, "}"
*/
void parseLocationBlock(std::vector<std::string>& tokens, Server& server_info, size_t& i)
{
    Location    loc;
    bool        autoindex_flag = false;
    bool        end_flag;
    size_t      last_idx = i + 1; // last_idx는 directory 가리키는중

    if (getFileType(tokens[++i]) != DIRECTORY)
        throw std::runtime_error("Error on parseLocationBlock : not a directory");
    loc.setLocationPath(tokens[i++]);

    while (tokens[last_idx] != "}")
        last_idx++;

    while (++i < last_idx)
    {
        if (tokens[i] == "root" && i + 1 < last_idx)
        {
            if (loc.getLocationRoot() != "")
                throw std::runtime_error("Error on parseLocationBlock : duplicated root");
            if (removeSemicolon(tokens[++i]) == false)
                throw std::runtime_error("Error on parseLocationBlock : no ';' in root");
            loc.setLocationRoot(tokens[i]);
        }
        else if (tokens[i] == "path" && i + 1 < last_idx)
        {
            if (loc.getLocationPath() != "")
                throw std::runtime_error("Error on parseLocationBlock : duplicated path");
            if (removeSemicolon(tokens[++i]) == false)
                throw std::runtime_error("Error on parseLocationBlock : no ';' in path");
            loc.setLocationPath(tokens[i]);
        }
        else if (tokens[i] == "index" && i + 1 < last_idx)
        {
            if (loc.getLocationIndex() != "")
                throw std::runtime_error("Error on parseLocationBlock : duplicated index");
            if (removeSemicolon(tokens[++i]) == false)
                throw std::runtime_error("Error on parseLocationBlock : no ';' in index");
            loc.setLocationIndex(tokens[i]);
        }
        else if (tokens[i] == "return" && i + 2 < last_idx)
        {
            if (loc.getLocationReturn() != "")
                throw std::runtime_error("Error on parseLocationBlock : duplicated return");
            loc.setLocationReturnStatus(ft_stoi(tokens[++i]));
            if (removeSemicolon(tokens[++i]) == false)
                throw std::runtime_error("Error on parseLocationBlock : no ';' in return");
            loc.setLocationReturn(tokens[i]);
        }
        else if (tokens[i] == "client_max_body_size" && i + 1 < last_idx)
        {
            if (loc.getLocationClientMaxBodySize() != 0)
                throw std::runtime_error("Error on parseLocationBlock : duplicated client_max_body_size");
            if (removeSemicolon(tokens[++i]) == false)
                throw std::runtime_error("Error on parseLocationBlock : no ';' in client_max_body_size");
            loc.setLocationClientMaxBodySize(strtoul(tokens[i].c_str(), NULL, 10));
        }
        else if (tokens[i] == "autoindex" && i + 1 < last_idx)
        {
            if (autoindex_flag)
                throw std::runtime_error("Error on parseLocationBlock : duplicated autoindex");
            if (removeSemicolon(tokens[++i]) == false)
                throw std::runtime_error("Error on parseLocationBlock : no ';' in autoindex");
            if (tokens[i] == "on" || tokens[i] == "off")
                loc.setLocationAutoindex((tokens[i] == "on" ? true : false));
            else
                throw std::runtime_error("Error on parseLocationBlock : invalid value for autoindex");
            autoindex_flag = true;
        }
        else if (tokens[i] == "allow_methods")
        {
            if (loc.getLocationAllowMethods().empty() != true)
                throw std::runtime_error("Error on parseLocationBlock : duplicated allow_methods");

            std::vector<MethodType> method_vec;
            while (++i < last_idx)
            {
                end_flag = removeSemicolon(tokens[i]);
                method_vec.push_back(checkMethodType(tokens[i]));
                if (end_flag)
                    break;
            }
            loc.setLocationAllowMethods(method_vec);
            method_vec.clear();
        }
        else if (tokens[i] == "cgi_info" && i + 2 < last_idx)
        {  
            if (loc.getLocationCgiPath() != "" || loc.getLocationCgiExtension() != "")
                throw std::runtime_error("Error on parseLocationBlock : duplicated cgi_info");
            loc.setLocationCgiExtension(tokens[++i]);
            if (removeSemicolon(tokens[++i]) == false)
                throw std::runtime_error("Error on parseLocationBlock : no ';' in cgi_path");
            loc.setLocationCgiPath(tokens[i]);
        }
        else
            throw std::runtime_error("Error on parseLocationBlock : invalid token : " + tokens[i]);
    }        
    server_info.pushServerLocation(loc);
}

/*
    parse one server_block info string to Server instance

    input : "server { ~ }"
*/
void parseServerBlock(std::string& content, Server& server_info)
{
    std::vector<std::string>    tokens;
    bool                        autoindex_flag = false;
    size_t                      i = 0;

    tokens = splitString(content += ' ', std::string(" \n\t"));
    if (tokens[i] != "{")
        throw std::runtime_error("Error on parseServerBlock: no server block");
    i += 1;
    for (; i < tokens.size(); i++)
    {
        if (tokens[i] == "listen" && i + 1 < tokens.size())
        {
            if (server_info.getServerPort() != "")
                throw std::runtime_error("Error on parseServerBlock : duplicated port");
            if (removeSemicolon(tokens[++i]) == false)
                throw std::runtime_error("Error on parseServerBlock : no ';' in port");
            /* if listen 0.0.0.0:8002 */
            size_t pos = tokens[i].find(':');
            if (pos == std::string::npos)
                server_info.setServerPort(tokens[i]);
            else
            {
                server_info.setServerHost(tokens[i].substr(0, pos));
                server_info.setServerPort(tokens[i].substr(pos + 1));
            }
        }
        else if (tokens[i] == "client_max_body_size" && i + 1 < tokens.size())
        {
            if (server_info.getServerClientMaxBodySize() != 0)
                throw std::runtime_error("Error on parseServerBlock : duplicated client_max_body_size");
            if (removeSemicolon(tokens[++i]) == false)
                throw std::runtime_error("Error on parseServerBlock : no ';' in client_max_body_size");
            server_info.setServerClientMaxBodySize(strtoul(tokens[i].c_str(), NULL, 10));
        }
        else if (tokens[i] == "server_name" && i + 1 < tokens.size())
        {
            if (server_info.getServerName() != "")
                throw std::runtime_error("Error on parseServerBlock : duplicated server_name");
            if (removeSemicolon(tokens[++i]) == false)
                throw std::runtime_error("Erro on parseServerBlock : no ';' in server_name");
            server_info.setServerName(tokens[i]);
        }
        else if (tokens[i] == "root" && i + 1 < tokens.size())
        {
            if (server_info.getServerRoot() != "")
                throw std::runtime_error("Error on parseServerBlock : duplicated root");
            if (removeSemicolon(tokens[++i]) == false)
                throw std::runtime_error("Erro on parseServerBlock : no ';' in root");
            server_info.setServerRoot(tokens[i]);
        }
        else if (tokens[i] == "index" && i + 1 < tokens.size())
        {
            if (server_info.getServerIndex() != "")
                throw std::runtime_error("Error on parseServerBlock : duplicated index");
            if (removeSemicolon(tokens[++i]) == false)
                throw std::runtime_error("Erro on parseServerBlock : no ';' in index");
            server_info.setServerIndex(tokens[i]);
        }
        else if (tokens[i] == "return" && i + 2 < tokens.size())
        {
            if (server_info.getServerReturn() != "")
                throw std::runtime_error("Error on parseServerBlock : duplicated return");
            server_info.setServerReturnStatus(ft_stoi(tokens[++i]));
            if (removeSemicolon(tokens[++i]) == false)
                throw std::runtime_error("Erro on parseServerBlock : no ';' in return");
            server_info.setServerReturn(tokens[i]);
        }
        else if (tokens[i] == "autoindex" && i + 1 < tokens.size())
        {
            if (autoindex_flag)
                throw std::runtime_error("Error on parseServerBlock : duplicated autoindex");
            if (removeSemicolon(tokens[++i]) == false)
                throw std::runtime_error("Error on parseServerBlock : no ';' in autoindex");
            if (tokens[i] == "on" || tokens[i] == "off")
                server_info.setServerAutoindex((tokens[i] == "on" ? true : false));
            else
                throw std::runtime_error("Error on parseServerBlock : invalid value for autoindex");
            autoindex_flag = true;
        }
        else if (tokens[i] == "host" && i + 1 < tokens.size())
        {
            if (server_info.getServerHost() != "")
                throw std::runtime_error("Error on parseServerBlock : duplicated host");
            if (removeSemicolon(tokens[++i]) == false)
                throw std::runtime_error("Erro on parseServerBlock : no ';' in host");
            server_info.setServerHost(tokens[i]);
        }
        else if (tokens[i] == "location" && i + 1 < tokens.size())
        {
            parseLocationBlock(tokens, server_info, i);
        }
        else if (tokens[i] == "error_page" && i + 1 < tokens.size())
        {
            parseErrorPages(tokens, server_info, i);
        }
        else if (tokens[i] == "}")
            continue;
        else
            throw std::runtime_error("Error on parseServerBlock : invalid token " + tokens[i]);
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