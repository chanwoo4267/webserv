#include "server.hpp"

int main(int argc, char* argv[])
{
    try
    {
        int server_sock;
        int client_sock;

        struct sockaddr_in server_addr;
        struct sockaddr_in client_addr;
        socklen_t client_addr_size;

        // int socket(int domain, int type, int protocol)
        // domain : 어떤 영역에서 통신할건지 지정한다. AF_UNIX, AF_INET, AF_INET6 등
        // type : 어떤 서비스 타입의 소켓을 생성할건지 지정한다. SOCK_STREAM, SOCK_DGRAM, SOCK_ROW 등
        // protocol : 소켓에서 사용할 프로토콜을 지정한다. IPPROTO_TCP, IPPROTO_UDP, 0(type에서 이미 지정함) 등

        server_sock = socket(PF_INET, SOCK_STREAM, 0);
        if (server_sock == -1)
            throw std::runtime_error("socket error");
        
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr.sin_port = htons(atoi(argv[1]));

        // int bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen)
        // sockfd : 참조할 소켓기술자(descriptor)
        // myaddr : 서버의 IP주소
        // addrlen : 주소의 길이

        if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
            throw std::runtime_error("bind error");

        // int listen(int sockfd, int backlog)
        // sockfd : 참조할 소켓기술자
        // backlog : 연결 대기열의 크기를 지정한다.
        
        if (listen(server_sock, 5) == -1)
            throw std::runtime_error("listen error");
        
        client_addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_size);
        if (client_sock == -1)
            throw std::runtime_error("accept error");
        
        char msg[] = "Hello This is Server!";
        write(client_sock, msg, sizeof(msg));

        close(client_sock);
        close(server_sock);

        return 0;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}