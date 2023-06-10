#include "Cluster.hpp"

Cluster::Cluster()
{
    this->_max_fd = -1;
}

Cluster::~Cluster() {}
Cluster::Cluster(const Cluster& obj)
{
    this->_cluster_servers = obj._cluster_servers;
}

Cluster& Cluster::operator=(const Cluster& obj)
{
    if (this != &obj)
    {
        this->_cluster_servers = obj._cluster_servers;
    }
    return *this;
}

/* -------------------------------------------------- */
/* -------------------get methods-------------------- */
/* -------------------------------------------------- */

const std::vector<Server>& Cluster::getClusterServers()
{
    return this->_cluster_servers;
}

/* -------------------------------------------------- */
/* -------------------get methods-------------------- */
/* -------------------------------------------------- */

void Cluster::setClusterServers(std::vector<Server> servers)
{
    this->_cluster_servers = servers;
}

/*
    print all server, location info
*/
void Cluster::printCluster()
{
    std::vector<Server>::iterator it;

    for (it = _cluster_servers.begin(); it != _cluster_servers.end(); it++)
    {
        it->printServer();
    }
}

/*
    check server-dimension duplication

    such as, duplicated host + port
*/
bool Cluster::checkCluster()
{
    for (std::vector<Server>::iterator it = this->_cluster_servers.begin(); it != this->_cluster_servers.end(); it++)
    {
        for (std::vector<Server>::iterator itt = it + 1; itt != this->_cluster_servers.end(); itt++)
        {
            if (it->getServerHost() == itt->getServerHost() && it->getServerPort() == itt->getServerHost())
                throw std::runtime_error("Error on checkCluster : duplicated server host&port");
            if (it->getServerName() == itt->getServerName())
                throw std::runtime_error("Error on checkCluster : duplicated server name");
        }
    }
}

void Cluster::executeServer()
{
    checkCluster();
    for (std::vector<Server>::iterator it = this->_cluster_servers.begin(); it != this->_cluster_servers.end(); it++)
        it->setupServerSocket();
    
    while (1)
    {
        1;
    }
}