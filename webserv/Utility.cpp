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