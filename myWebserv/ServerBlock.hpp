#ifndef SERVERBLOCK_HPP
# define SERVERBLOCK_HPP

# include "Webserv.hpp"
# include "LocationBlock.hpp"

class ServerBlock
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
		std::vector<LocationBlock> 		_locations; // Location 각각의 정보
        struct sockaddr_in 				_server_address; // socket address
        int     						_listen_fd; // 할당받은 fd

	public:
		ServerBlock();
		~ServerBlock();
		ServerBlock(const ServerBlock &obj);
		ServerBlock &operator=(const ServerBlock & obj);

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
		int isValidLocation(LocationBlock &location) const;

		const std::string &getServerName();
		const uint16_t &getPort();
		const in_addr_t &getHost();
		const size_t &getClientMaxBodySize();
		const std::vector<LocationBlock> &getLocations();
		const std::string &getRoot();
		const std::map<short, std::string> &getErrorPages();
		const std::string &getIndex();
		const bool &getAutoindex();
		const std::string &getPathErrorPage(short key);
		const std::vector<LocationBlock>::iterator getLocationKey(std::string key);

		bool checkLocaitons() const;

		void	setupServer();
        int     getFd();

		const void printServerBlock();
};

#endif