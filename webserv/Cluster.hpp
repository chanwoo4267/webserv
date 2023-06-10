#ifndef CLUSTER_HPP
#define CLUSTER_HPP

# include "Webserv.hpp"
# include "Server.hpp"

class Cluster
{
    private:
        std::vector<Server> _cluster_servers;

        int                 _max_fd;
    
    public:
        Cluster();
	    ~Cluster();
	    Cluster(const Cluster& obj);
	    Cluster& operator=(const Cluster& obj);

        /* get methods */
        const std::vector<Server>& getClusterServers();

        /* set methods */
        void setClusterServers(std::vector<Server> servers);

        void printCluster();
        bool checkCluster();
        void executeServer();

        /* server execute */
        void waitForClient();
        void acceptSocket();
        void treatRequest();
};

#endif