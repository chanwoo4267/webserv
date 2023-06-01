#include "Utility.hpp"

/* 
    get file type(REGULARFILE, DIRECTORY, NONE) 
*/
enum FileType getFileType(std::string const file_path)
{
    struct stat status;
    int         result;

    result = stat(file_path.c_str(), &status);
    if (!result)
    {
        if (status.st_mode & S_IFREG)
            return REGULARFILE;
        else if (status.st_mode & S_IFDIR)
            return DIRECTORY;
        else
            return NONE;
    }
    else
        throw std::runtime_error("Error on getFileType() : stat function failed");
}

/* 
    check whether file is accessible(true) or not(false) 
*/
bool checkFilePermission(std::string const file_path, int mode)
{
    if (!access(file_path.c_str(), mode))
        return true;
    else
        return false;
}

/*
    split string into tokens by given delim characters

    find any matches of delim by find_first_of()
    split string with substr(), push it, 
    and move start point to first non-delim char by find_first_not_of()
*/
std::vector<std::string> splitString(std::string str, std::string delim)
{
    std::vector<std::string>	vec;
	std::string::size_type		start, end;

	start = end = 0;
	while (1)
	{
		end = str.find_first_of(delim, start); // <string> 기본함수, start에서부터 delim 내용중 아무거나 일치하는 제일 첫번째 글자 위치를 반환
		if (end == std::string::npos)
			break;
		std::string tmp = str.substr(start, end - start);
		vec.push_back(tmp);
		start = str.find_first_not_of(delim, end); // end 이후 delim가 아닌 첫 글자의 위치를 반환
		if (start == std::string::npos)
			break;
	}
	return vec;
}

/*
    remove semicolon at the end of string

    if there's no semicolon, return false
    else, return true
*/
bool removeSemicolon(std::string &parametr)
{
	size_t pos = parametr.rfind(';');
	if (pos != parametr.size() - 1)
		return false;
	parametr.erase(pos);
    return true;
}
