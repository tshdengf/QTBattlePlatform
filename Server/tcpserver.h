#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "thread.h"

#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <event2/listener.h>

#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include "tcpsoket.h"

class Tcpsoket;
class TcpServer
{
    friend class Tcpsoket;
public:
    TcpServer(int threadNum = 8);

    int listen1(int port, const char *ip= NULL);

    //
    void start();
protected:
    //监听回调函数，有客户端连接会调用这个函数
//    static void listenCb(struct evconnlistener*,evutil_socket_t,struct sockaddr*,int socklen, void *);
    static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sock,int, void *arg);
    //监听处理函数
    void listenEvent(evutil_socket_t fd, struct sockaddr_in*);


    //----------------------------
    //客户端连接事件
    virtual void connectionEvent(Tcpsoket*){}
    //客户端可读事件
    virtual void readEvent(Tcpsoket*){}
    //客户端可写事件
    virtual void wirteEvent(Tcpsoket*){}
    //客户端关闭事件
    virtual void closeEvent(Tcpsoket*,short ){}



private:
    Thread * m_threadpool;
    int m_threadNum;
    struct event_base *m_base;
    struct evconnlistener *m_listener;//

    int m_nextThread;
};

#endif // TCPSERVER_H
