#include "client.hpp"

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
            throw std::runtime_error("Usage : ./client [IP] [port]");
        
        int sock = socket(PF_INET, SOCK_STREAM, 0);
        if (sock == -1)
            throw std::runtime_error("socket error");
        
        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(argv[1]);
        server_addr.sin_port = htons(atoi(argv[2]));

        if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
            throw std::runtime_error("connect error");

        char buffer[1024];
        size_t str_size;
        while (1)
        {
            std::cout << "Enter Message : ";
            std::cin.getline(buffer, sizeof(buffer));

            write(sock, buffer, sizeof(buffer));
            
            str_size = read(sock, buffer, sizeof(buffer));
            if (str_size == -1)
                throw std::runtime_error("read error");
            
            std::cout << "Received Message : " << buffer << std::endl;
        }

        close(sock);
        return 0;
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
