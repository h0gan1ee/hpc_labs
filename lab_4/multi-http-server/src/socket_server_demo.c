#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>

#define PORT 11451 // 服务器监听端口

void send_handle(int client_socket)
{
    char data[] = "HTTP/1.1 200 OK\nContent-Length: 13\nContent-Type: text/html; charset=utf-8\n\nHello World!";

    printf("enter sleeping...\n");
    sleep(3); // 让进程进入睡眠状态，单位是秒。
    printf("finish sleeping...\n");

    if (write(client_socket, data, sizeof(data)) != sizeof(data))
    {
        printf("[ERROR] while sending data\n");
    }
}

int main()
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0); // 初始化套接字
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // 服务地址和端口配置
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)); // 将本地地址绑定到所创建的套接字上

    listen(server_socket, 20); // 开始监听是否有客户端连接，第二个参数是最大监听数

    char buf[1024];
    int client_socket;

    while (1)
    {
        printf("[INFO] http://localhost:%d\n\n", PORT);
        printf("===== multiple threads waiting for client's request =====\n");
        if ((client_socket = accept(server_socket, (struct sockaddr *)NULL, NULL)) == -1) // 等待客户端（浏览器）连接
        {
            printf("accept socket error :%s(errno:%d)\n", strerror(errno), errno);
            continue;
        }
        printf("===== waiting for client %d read request data =====\n", client_socket);
        read(client_socket, buf, 1024); // 读取客户端内容，这里是HTTP的请求数据
        struct sockaddr_in client_addr;
        char ip_addr[INET_ADDRSTRLEN];
        socklen_t client_addr_len = sizeof(client_addr);
        getpeername(client_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        printf("client address: %s:%d\n", inet_ntop(AF_INET, &client_addr.sin_addr, ip_addr, sizeof(ip_addr)), ntohs(client_addr.sin_port));
        pthread_t thrd;
        pthread_create(&thrd, NULL, (void *)send_handle, client_socket);
        pthread_detach(thrd);
    }

    close(server_socket);
    return 0;
}
