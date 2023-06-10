#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <map>
#include <vector>

using namespace std;

/* print msg to std::err and exit with 1 */
void exit_with_perror(const string& msg)
{
    cerr << msg << endl;
    exit(EXIT_FAILURE);
}

void add_event(vector<struct kevent>& change_list, uintptr_t ident, int16_t filter,
        uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
    struct kevent temp_event;

    EV_SET(&temp_event, ident, filter, flags, fflags, data, udata); // event 설정
    change_list.push_back(temp_event); //change_list에 event 추가
}

void disconnect_client(int client_fd, map<int, string>& clients)
{
    cout << "client disconnected: " << client_fd << endl;
    close(client_fd);
    clients.erase(client_fd);
}

int main()
{
    /* init server socket and listen */
    int server_socket;
    struct sockaddr_in server_addr;

    /* socket 할당받음 */
    if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1) // AF_INET, PF_INET 둘은 동일. SOCK_STREAM은 TCP통신 의미
        exit_with_perror("socket() error\n" + string(strerror(errno)));

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8080); // set port to 8080
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
        exit_with_perror("bind() error\n" + string(strerror(errno)));

    if (listen(server_socket, 3) == -1) // 연결 요청 대기 큐 != 연결 가능한 클라이언트, 적당한 크기면 상관없음
        exit_with_perror("listen() error\n" + string(strerror(errno)));

    /* 여기서부터 기존과 변경 */

    // non-block 모드로 설정
    fcntl(server_socket, F_SETFL, O_NONBLOCK);
    
    /* init kqueue */
    int kq;
    if ((kq = kqueue()) == -1)
        exit_with_perror("kqueue() error\n" + string(strerror(errno)));

    map<int, string> clients; // map for client socket_fd:data
    vector<struct kevent> change_list; // kevent vector for changelist
    struct kevent event_list[8]; // kevent array for eventlist

    /* add event for server socket */
    // FD를 ident로 사용, 읽어들일 data가 있을때마다 반환(EVFILT_READ) | kqueue에 이벤트 추가(EV_ADD), kevent()호출시 이벤트 반환(EV_ENABLE)
    add_event(change_list, server_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    cout << "echo server started" << endl;

    /* main loop */
    int new_events;
    struct kevent* curr_event;
    while (1)
    {
        /*  apply changes and return new events(pending events) */

        /* change_list 는 등록할 event의 list, event_list는 발생한 event의 list */

        new_events = kevent(kq, &change_list[0], change_list.size(), event_list, 8, NULL);
        if (new_events == -1)
            exit_with_perror("kevent() error\n" + string(strerror(errno)));

        change_list.clear(); // clear change_list for new changes

        for (int i = 0; i < new_events; ++i)
        {
            curr_event = &event_list[i];

            /* 만약 발생한 이벤트가 error flag라면 */
            if (curr_event->flags & EV_ERROR)
            {
                /* 에러가 발생한 fd가 server_socket 이라면 */
                if (curr_event->ident == server_socket)
                    exit_with_perror("server socket error");
                else /* server_error가 아니면 client측 에러 */
                {
                    cerr << "client socket error" << endl;
                    disconnect_client(curr_event->ident, clients);
                }
            }
            else if (curr_event->filter == EVFILT_READ) // 읽을 data가 있다면
            {
                if (curr_event->ident == server_socket) // ident는 fd, 그게 server_socket이면
                {
                    /* accept new client */
                    int client_socket;
                    if ((client_socket = accept(server_socket, NULL, NULL)) == -1)
                        exit_with_perror("accept() error\n" + string(strerror(errno)));
                    cout << "accept new client: " << client_socket << endl;
                    fcntl(client_socket, F_SETFL, O_NONBLOCK);

                    /* add event for client socket - add read && write event */
                    add_event(change_list, client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
                    add_event(change_list, client_socket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
                    clients[client_socket] = "";
                }
                else if (clients.find(curr_event->ident) != clients.end()) // ident가 client의 listen_fd라면
                {
                    /* read data from client */
                    char buf[1024];
                    int n = read(curr_event->ident, buf, sizeof(buf));

                    if (n <= 0)
                    {
                        if (n < 0)
                            cerr << "client read error!" << endl;
                        disconnect_client(curr_event->ident, clients);
                    }
                    else
                    {
                        buf[n] = '\0';
                        clients[curr_event->ident] += buf;
                        cout << "received data from " << curr_event->ident << ": " << clients[curr_event->ident] << endl;
                    }
                }
            }
            else if (curr_event->filter == EVFILT_WRITE)
            {
                /* send data to client */
                map<int, string>::iterator it = clients.find(curr_event->ident);
                if (it != clients.end())
                {
                    if (clients[curr_event->ident] != "")
                    {
                        int n;
                        if ((n = write(curr_event->ident, clients[curr_event->ident].c_str(),
                                        clients[curr_event->ident].size()) == -1))
                        {
                            cerr << "client write error!" << endl;
                            disconnect_client(curr_event->ident, clients);  
                        }
                        else
                            clients[curr_event->ident].clear();
                    }
                }
            }
        }

    }
    return (0);
}

/*
    struct sockaddr_in 구조체

    struct sockaddr_in {
        short    sin_family;          // 주소 체계: AF_INET
        u_short  sin_port;            // 16 비트 포트 번호, network byte order
        struct   in_addr  sin_addr;   // 32 비트 IP 주소
        char     sin_zero[8];         // 전체 크기를 16 비트로 맞추기 위한 dummy
    };

    struct in_addr {
        union {
                struct { UCHAR s_b1,s_b2,s_b3,s_b4; } S_un_b;
                struct { USHORT s_w1,s_w2; } S_un_w;
                ULONG S_addr;
        } S_un;
    }

    따라서 sin_addr.s_addr이 실제 IP 주소값, 이때 unsinged long 값을 가짐
    따라서 값을 집어넣을때 unsigned long 형태의, 네트워크 byte order로 넣어줘야함
    client : inet_addr()
    server : htonl()

    htonl()

    unsigned long 값을 network byte order의 unsgined long 으로 바꿔준다.

    inet_addr(ip)

    32비트 형태의 주소값(127.0.0.1) 을 unsigned long으로 변환해줌. 추가로 network byte order로 변환해줌.
    변환 실패시 (8비트값이 255가 넘는 등) INADDR_NONE을 반환한다.

    htons(port)

    The htons() function converts the unsigned short integer (port)
    from host byte order to network byte order.
    즉, 우리가 사용하는 자연수에서 네트워크가 사용하는 byte order로 변환

    INADDR_ANY
    INADDR_ANY는 자동으로 이 컴퓨터에 존재하는 랜카드 중 사용가능한 랜카드의 IP주소를 사용하라는 의미
    즉, 여러 IP주소에서 오는 데이터를 전부 수신한다는 의미

    kqueue()
    
    event를 저장할 새로운 queue를 kernal에 요청하고, 해당 queue의 descriptor를 반환받는다.
    kqueue()로 할당받은 공간을 kevent() 함수를 통해 관리한다.
*/