#include "Cluster.hpp"

Cluster::Cluster() {}
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

/* -------------------------------------------------- */
/* ------------------other methods------------------- */
/* -------------------------------------------------- */

void Cluster::printCluster()
{
    std::vector<Server>::iterator it;

    for (it = _cluster_servers.begin(); it != _cluster_servers.end(); it++)
    {
        it->printServer();
    }
}