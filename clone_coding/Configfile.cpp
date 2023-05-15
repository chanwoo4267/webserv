#include "Configfile.hpp"

ConfigFile::ConfigFile() : _size(0) { }

ConfigFile::ConfigFile(std::string const path) : _path(path), _size(0) { }

ConfigFile::~ConfigFile() { }

/* file일경우 1, directory일경우 2,  그 외에는 3을 반환. stat 함수 실패시 -1을 반환 */
int ConfigFile::getTypePath(std::string const path)
{
	struct stat	buffer;
	int			result;
	
	result = stat(path.c_str(), &buffer); // stat : 파일의 정보를 조회, 성공시 0을 반환. 실패시 -1 반환
	if (result == 0)
	{
		if (buffer.st_mode & S_IFREG) // S_IFREG : regular file
			return (1);
		else if (buffer.st_mode & S_IFDIR) // S_IFDIR : directory
			return (2);
		else
			return (3);
	}
	else
		return (-1);
}

/* 파일을 access할 수 있는지 확인 후 있으면 0, 아니면 -1 반환 */
int	ConfigFile::checkFile(std::string const path, int mode)
{
	return (access(path.c_str(), mode));
	/* mode에 4(R_OK)는 읽기, 2(W_OK)는 쓰기, 1(X_OK)은 실행, 0은 존재여부만 검사한다. */
}

/* 파일을 read 할 수 있는지 확인 후 있으면 0, 아니면 -1 반환 */
int ConfigFile::isFileExistAndReadable(std::string const path, std::string const index)
{
	if (getTypePath(index) == 1 && checkFile(index, R_OK) == 0) // regular file이고, 읽을 수 있음
		return (0);
	if (getTypePath(path + index) == 1 && checkFile(path + index, R_OK) == 0)
		return (0);
	return (-1);
}

/* 파일을 읽어 하나의 문자열로 저장 */
std::string	ConfigFile::readFile(std::string path)
{
	if (path.empty() || path.length() == 0)
		return (NULL);

	std::ifstream config_file(path.c_str());
	if (!config_file || !config_file.is_open()) // is_open : 파일이 열려있음
		return (NULL);

	/* config 파일은 인자로 받은 경로를 통해 연 파일의 스트림이다. 따라서 config_file.rdbuf() 를 통해 해당 파일 스트림의 스트림 버퍼를 얻을 수 있다. */
    /* 스트림 버퍼의 모든 내용물을 stringstream 으로 넣어준다. 이후 stringstream.str() 을 통해 스트림 내용물을 문자열로 변환하여 반환한다. */
	std::stringstream stream_binding;
	stream_binding << config_file.rdbuf(); //rdbuf() : 스트림 버퍼를 연관시키거나 얻는다.
	return (stream_binding.str());
}

/* --------------------------------------------------------------------------- */
/* ------------------------------- get methods ------------------------------- */
/* --------------------------------------------------------------------------- */

std::string ConfigFile::getPath()
{
	return (this->_path);
}

int ConfigFile::getSize()
{
	return (this->_size);
}