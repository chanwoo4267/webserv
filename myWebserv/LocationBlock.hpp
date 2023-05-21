#ifndef LOCATIONBLOCK_HPP
#define LOCATIONBLOCK_HPP

#include "Webserv.hpp"

/* config 파일 Location 블럭에 들어올 수 있는 정보는 아래와 같다. */
/* path, root, autoindex, index, allow_methods, return, alias, cgi관련, client_max_body_size */
/* 생성자에서 모든 private variable을 default value로 초기화한다. */

class LocationBlock
{
	private:
		std::string					_path;
		std::string					_root;
		bool						_autoindex;
		std::string					_index;
		std::map<std::string, bool>	_methods; // 허용된 methods는 true, 아닐경우 false
		std::string					_return;
		std::string					_alias;
		std::vector<std::string>	_cgi_path;
		std::vector<std::string>	_cgi_ext;
		long						_client_max_body_size;

	public:
		std::map<std::string, std::string> _ext_path;

		LocationBlock();
		LocationBlock(const LocationBlock &obj);
		LocationBlock &operator=(const LocationBlock &obj);
		~LocationBlock();

		/* set Methods */
		void setPath(std::string parametr);
		void setRootLocation(std::string parametr);
		void setMethods(std::vector<std::string> methods);
		void setAutoindex(std::string parametr);
		void setIndexLocation(std::string parametr);
		void setReturn(std::string parametr);
		void setAlias(std::string parametr);
		void setCgiPath(std::vector<std::string> path);
		void setCgiExtension(std::vector<std::string> extension);
		void setMaxBodySize(std::string parametr);
		void setMaxBodySize(long parametr);

		/* get Methods */
		const std::string &getPath() const;
		const std::string &getRootLocation() const;
		const std::map<std::string, bool> &getMethods() const;
		const bool &getAutoindex() const;
		const std::string &getIndexLocation() const;
		const std::string &getReturn() const;
		const std::string &getAlias() const;
		const std::vector<std::string> &getCgiPath() const;
		const std::vector<std::string> &getCgiExtension() const;
		const std::map<std::string, std::string> &getExtensionPath() const;
		const long &getMaxBodySize() const;

		/* test function */
		const void printLocationBlock();
};

#endif