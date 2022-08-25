#ifndef TCPSOKET_H
#define TCPSOKET_H

#include "tcpserver.h"
#include <string>
class TcpServer;
class Tcpsoket
{
public:
    Tcpsoket(TcpServer* tcpserver,struct bufferevent* bev, char *ip, u_int16_t port);
    //可读事件回调函数
    static void readEventCb(struct bufferevent*bev, void *ctx);
    //可写事件回调函数
    static void writeEventCb(struct bufferevent* bev,void *ctx);
    //异常事件回调函数
    static void closeEventCb(struct bufferevent* bev,short what,void *ctx);

    char * getIp();
    u_int16_t getPort();
    //从客户端读数据
    int readDate(void *data, int size);
    //从客户端写数据
    int weitDate(const void *data, int size);

    void setUserName(std::string name);
    std::string getUsername();

    int flag;
private:
    static TcpServer*m_tcpserver;
    struct bufferevent*m_bev;
    char *m_ip;
    u_int16_t m_port;


    std::string m_userName;
};

#endif // TCPSOKET_H
