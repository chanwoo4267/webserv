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