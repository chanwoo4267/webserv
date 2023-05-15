#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "Webserv.hpp"

static std::string	serverParametrs[] = {"server_name", "listen", "root", "index", "allow_methods", "client_body_buffer_size"};

class Location;

class ServerConfig
{
	private:
		uint16_t						_port; // 포트 번호
		in_addr_t						_host; // 호스트 주소
		std::string						_server_name; // 서버 이름
		std::string						_root; // 루트 경로
		unsigned long					_client_max_body_size; // 최대 전송 사이즈
		std::string						_index; // 인덱스
		bool							_autoindex; // 오토인덱싱 여부
		std::map<short, std::string>	_error_pages; // 에러 페이지 연결 <에러번호, 경로>
		std::vector<Location> 			_locations; // Location 각각의 정보
        struct sockaddr_in 				_server_address; //
        int     						_listen_fd; // 

	public:
		ServerConfig();
		~ServerConfig();
		ServerConfig(const ServerConfig &other);
		ServerConfig &operator=(const ServerConfig & rhs);

		void initErrorPages(void);

		void setServerName(std::string server_name);
		void setHost(std::string parametr);
		void setRoot(std::string root);
		void setFd(int);
		void setPort(std::string parametr);
		void setClientMaxBodySize(std::string parametr);
		void setErrorPages(std::vector<std::string> &parametr);
		void setIndex(std::string index);
		void setLocation(std::string nameLocation, std::vector<std::string> parametr);
		void setAutoindex(std::string autoindex);

		bool isValidHost(std::string host) const;
		bool isValidErrorPages();
		int isValidLocation(Location &location) const;

		const std::string &getServerName();
		const uint16_t &getPort();
		const in_addr_t &getHost();
		const size_t &getClientMaxBodySize();
		const std::vector<Location> &getLocations();
		const std::string &getRoot();
		const std::map<short, std::string> &getErrorPages();
		const std::string &getIndex();
		const bool &getAutoindex();
		const std::string &getPathErrorPage(short key);
		const std::vector<Location>::iterator getLocationKey(std::string key);

		static void checkToken(std::string &parametr);
		bool checkLocaitons() const;

		public:
		class ErrorException : public std::exception
		{
			private:
				std::string _message;
			public:
				ErrorException(std::string message) throw()
				{
					_message = "SERVER CONFIG ERROR: " + message;
				}
				virtual const char* what() const throw()
				{
					return (_message.c_str());
				}
				virtual ~ErrorException() throw() {}
		};

		void	setupServer();
        int     getFd();

};

#endif
