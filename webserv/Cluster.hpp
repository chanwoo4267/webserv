#ifndef CLUSTER_HPP
#define CLUSTER_HPP

# include "Webserv.hpp"
# include "Server.hpp"

class Cluster
{
    private:
        std::vector<Server> _cluster_servers;
    
    public:
        Cluster();
	    ~Cluster();
	    Cluster(const Cluster& obj);
	    Cluster& operator=(const Cluster& obj);

        /* get methods */
        const std::vector<Server>& getClusterServers();
};

#endif