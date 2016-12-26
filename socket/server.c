#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

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

    printf("The server is now ready to accept connections\n");
    for(;;)
    {
        struct sockaddr_in client_addr;
        int client_size = sizeof(client_addr);
        //accpet
        int client_fd = accept(socket_fd, &client_addr, &client_size);
        printf("new client accept %d\n", client_fd);

        //send msg
        char* msg = "connect success\n";
        send(client_fd, msg, strlen(msg) + sizeof(char), 0);

        close(client_fd);
    }
}
