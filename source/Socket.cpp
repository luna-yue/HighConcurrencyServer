#include "Socket.hpp"


void Socket::CreateSocket()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        lg(Fatal, "socket error, %s: %d", strerror(errno), errno);
        exit(SocketErr);
    }

    // setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, buf, sizeof(buf));
}
void Socket::Bind(uint16_t port)
{
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    local.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr *)&local, sizeof(local)) < 0)
    {
        lg(Fatal, "bind error, %s: %d", strerror(errno), errno);
        exit(BindErr);
    }
}
void Socket::Listen()
{
    if (listen(sockfd, backlog) < 0)
    {
        lg(Fatal, "listen error, %s: %d", strerror(errno), errno);
        exit(ListenErr);
    }
}
int Socket::Accept(string *ip , uint16_t *port )
{ // 输出型参数
    struct sockaddr_in peer;
    socklen_t len = sizeof(peer);
    int newfd = accept(sockfd, (struct sockaddr *)&peer, &len);
    if (newfd < 0)
    {
        lg(Warning, "accept error, %s: %d", strerror(errno), errno);
        // sleep(1000);
        return -1;
    }
    char ipstr[64];
    inet_ntop(AF_INET, &peer.sin_addr, ipstr, sizeof(ipstr));
    if (ip != nullptr && port != nullptr)
    {
        *ip = ipstr;
        *port = ntohs(peer.sin_port);
    }
    return newfd;
}
// 需要注意的是，所谓的“接收连接”并不意味着服务器调用 accept（）函数，。其实是服务器端把连接请求信息记录到等待队列。
// 因此 connect（）函数返回后并不进行数据交换。而是要等服务器端 accept 之后才能进行数据交换（read、write）。
bool Socket::Connect(const string &ip, uint16_t port)
{
    struct sockaddr_in peer;
    memset(&peer, 0, sizeof(peer));
    peer.sin_family = AF_INET;
    peer.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &(peer.sin_addr));
    if (connect(sockfd, (struct sockaddr *)&peer, sizeof(peer)))
    {
        lg(Error, "Connect Error %s", strerror(errno));
        return false;
    }
    return true;
}

int Socket::Send(void *buf, int len, int flag )
{
    int ret = send(sockfd, buf, len, flag);
    if (ret < 0)
    {
        if (errno == EAGAIN || errno == EINTR)
            return 0;
        else
        {
            lg(Error, "%s SOCKET SEND ERROR::%s", strerror(errno));
            return -1;
        }
    }
    return ret;
}
int Socket::NonBlockSend(void *buf, int len)
{
    return Send(buf, len, MSG_DONTWAIT);
}
int Socket::Recv(void *buf, int len, int flag)
{
    int ret = recv(sockfd, buf, len, flag);
   // lg(Warning,"ret!!!%d",ret);
    if (ret <= 0)
    {
        if (errno == EAGAIN || errno == EINTR)
            {lg(Warning,"return 0");return 0;}
        else if (ret == 0)
            return -1;
        else
        {
            lg(Error, "SOCKET RECV ERROR::%s", strerror(errno));
            return -1;
        }
    }
    return ret;
}
int Socket::NonBlockRecv(void *buf, int len)
{
    return Recv(buf, len, MSG_DONTWAIT);
}
void Socket::ReUseAddr()
{
    int val = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void *)&val, sizeof(int));
    val = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (void *)&val, sizeof(int));
} // 设置端口复用
void Socket::NonBlock()
{
    int oldSocketFlag = fcntl(sockfd, F_GETFL, 0);
    int newSocketFlag = oldSocketFlag | O_NONBLOCK;

    int flag = fcntl(sockfd, F_SETFL, newSocketFlag);
    if (flag == -1)
        lg(Error, "Nonblock error");
}