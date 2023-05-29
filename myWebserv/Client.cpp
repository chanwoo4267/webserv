#include "Client.hpp"

Client::Client()
{
    this->_socket = 0;
    this->_last_commu_time = time(NULL);
}

