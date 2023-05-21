#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "Webserv.hpp"

class ServerBlock;

class Config {
    private:
        std::vector<ServerBlock> _servers;
        std::vector<std::string> _server_config;
        size_t                   _nb_server;

    public:
        Config();
        ~Config();
        Config(const Config &obj);
        Config &operator=(const Config &obj);

        void createCluster(const std::string &config_file);
		void splitServers(std::string &content);
		void preprocessContent(std::string &content);
		size_t findStartServer(size_t start, std::string &content);
		size_t findEndServer(size_t start, std::string &content);
		void createServer(std::string &config, ServerBlock &server);
		void checkServers();
		std::vector<ServerBlock> getServers();
		int	stringCompare(std::string str1, std::string str2, size_t pos);
		const void printConfig();
};

#endif