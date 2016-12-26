#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/epoll.h>


int main()
{
    int socket_fd = 0;
    //create socket
    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("create socket failed %s:%d\n", strerror(errno), errno);

        exit(1);
    }

    //init sockaddr
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2323);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //bind
    if(bind(socket_fd, &server_addr, sizeof(server_addr)) < 0) {
        printf("bind failed %s:%d\n", strerror(errno), errno);

        exit(1);
    }

    //listen
    if(listen(socket_fd, 10) < 0) {
        printf("listen failed %s:%d\n", strerror(errno), errno);

        exit(1);
    }

    //ctl epoll event
    struct epoll_event events[100], event;
    int epoll_fd = epoll_create(100);
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = socket_fd;

    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) < 0) {
        printf("epoll_ctl failed %s:%d\n", strerror(errno), errno);

        exit(1);
    }


    printf("The server is now ready to accept connections\n");
    for(;;)
    {
        int ret = epoll_wait(epoll_fd, events, 100, 3000);
        int i = 0;
        for(i = 0; i < ret; i++) {
            //new connection
            if (events[i].data.fd == socket_fd) {
                struct sockaddr_in client_addr;
                int client_size = sizeof(client_addr);
                int client_fd = accept(socket_fd, &client_addr, &client_size);
                if (client_fd < 0) {
                    continue;
                }
                
                struct epoll_event client_event;
                client_event.data.fd = client_fd;
                client_event.events = EPOLLIN;
                printf("new client accept %d\n", client_fd);

                //ctl client_fd can read event
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &client_event);
            } else {
                if (events[i].events == EPOLLIN) { //receive msg from client_fd
                    char recv_buf[100];
                    int left_len = 0;
                    read(events[i].data.fd, recv_buf, 100);
                    printf("%s\n", recv_buf);
                }
            }
        }
    }
}
