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

    if(connect(socket_fd, (struct sockaddr_in *)&server_addr, sizeof(server_addr)) < 0){
        printf("connect failed %s:%d\n", strerror(errno), errno);

        exit(1);
    }

    char recv_buf[100];
    int left_len = 0;
    recv(socket_fd, recv_buf, 100, 0);

    printf("%s\n", recv_buf);
}
