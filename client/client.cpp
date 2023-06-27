#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
 
int port = 0;
char server_ip[64] = { 0 };
int sock_fd = -1;

char file_path[128] = { 0 };
int file_size = 0;
char *file_content = nullptr;

struct timeval start_time, end_time;
double time_used = 0;
bool read_file() {
    printf("file path:");
    scanf("%s", file_path);
    FILE *fp = fopen(file_path, "rb");
    if (!fp) {
        printf("%s open failed!\n", file_path);
        return false;
    }
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    file_content = (char*)malloc(sizeof(char) * file_size);
    assert(file_content);
    printf("read:%zd\n", fread(file_content, 1, file_size, fp));
    fclose(fp);
    printf("read file size:%d\n", file_size);
    return true;
}
void close() {
    if (file_content != nullptr) {
        free(file_content);
        file_content = nullptr;
    }
    if (sock_fd != -1) {
        close(sock_fd);
        sock_fd = -1;
    }
}
bool connect_server() {
    struct sockaddr_in serv_addr = { 0 };
    // 创建socket文件描述符
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket creation error!\n");
        return false;
    }
    printf("server ip:");
    scanf("%s", server_ip);
    printf("server port:");
    scanf("%d", &port);
    // 设置服务器地址结构体
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    // 将IP地址从文本转换为二进制格式
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        printf("%s invalid address/address not supported!\n", server_ip);
        return false;
    }
    // 连接服务器
    if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("connection %s failed!\n", server_ip);
        return false;
    }
    return true;
}
int main(int argc, char const *argv[]) {
    if (false == read_file()) {
        return -1;
    }
    if (false == connect_server()) {
        close();
    }
    int send_count = 0;
    printf("send count:");
    scanf("%d", &send_count);
    // 发送消息给服务器
    gettimeofday(&start_time, NULL);
    for (int i = 0;i < send_count;i++) {
        if (file_size != send(sock_fd, file_content, file_size, 0)) {
            printf("send failed!\n");
        }
    }
    gettimeofday(&end_time, NULL);
    time_used = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    printf("time used:%lf seconds\n", time_used);
    printf("average time used:%lf seconds\n", time_used / send_count);
    close();

    return 0;
}
