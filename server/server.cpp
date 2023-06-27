#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

int port = 0;
int server_fd = -1;
int new_socket = -1;

struct timeval start_time, end_time;
double time_used = 0;

int main(int argc, char const *argv[]) {
    int val_read = 0;
    struct sockaddr_in address;
    int opt = 1;
    int addr_len = sizeof(address);
    char buffer[1024] = { 0 };
 
    // 创建socket文件描述符
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed!\n");
        return -1;
    }
    // 设置socket选项
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("set sockopt error!\n");
        close(server_fd);
        return -1;
    }
    printf("bind port:");
    scanf("%d", &port);
    // 设置服务器地址结构体
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
 
    // 绑定socket到指定地址和端口
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed!\n");
        close(server_fd);
        return -1;
    }

    // 监听socket
    if (listen(server_fd, 3) < 0) {
        perror("listen error!\n");
        close(server_fd);
        return -1;
    }
 
    // 接受客户端连接
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addr_len)) < 0) {
        perror("accept error!\n");
        close(server_fd);
        return -1;
    }
    // 读取客户端发送的消息
    gettimeofday(&start_time, NULL);
    while (true) {
        val_read = recv(new_socket, buffer, sizeof(buffer), 0);
        if (val_read <= 0) {
            break;
        }
    }
    gettimeofday(&end_time, NULL);
    time_used = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    printf("time used:%lf seconds\n", time_used);
    close(server_fd);
    return 0;
}