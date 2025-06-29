#pragma once
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include "Log.hpp"
using namespace std;
#define backlog 10
enum
{
    SocketErr = 2,
    BindErr,
    ListenErr,
};
class Socket
{
public:
    Socket()
    {
    }
    Socket(int fd) : sockfd(fd) {}
    ~Socket() {}
    void CreateSocket();
    void Bind(uint16_t port);
    void Listen();
    int Accept(string *ip = nullptr, uint16_t *port = nullptr);
    // 需要注意的是，所谓的“接收连接”并不意味着服务器调用 accept（）函数，。其实是服务器端把连接请求信息记录到等待队列。
    // 因此 connect（）函数返回后并不进行数据交换。而是要等服务器端 accept 之后才能进行数据交换（read、write）。
    bool Connect(const string &ip, uint16_t port);
    void Close()
    {
        close(sockfd);
    }
    int Send(void *buf, int len, int flag = 0);
    int NonBlockSend(void *buf, int len);
    int Recv(void *buf, int len, int flag = 0);
    int NonBlockRecv(void *buf, int len);
    int fd()
    {
        return sockfd;
    }
    void ReUseAddr(); // 设置端口复用
    void NonBlock();
    // char buf[1024];
private:
    int sockfd;
};