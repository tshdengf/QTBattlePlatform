#include "tcpserver.h"

TcpServer::TcpServer(int threadNum):m_nextThread(0)
{
    if(threadNum <=0)
    {
        exit(-1);
    }
    //创建线程集合
    m_threadNum = threadNum;
    m_threadpool = new Thread[m_threadNum];
    if(m_threadpool == NULL)
    {
        exit(-1);
    }
    //第一步初始化 event_base()
    m_base = event_base_new();
    if(!m_base)
    {
        fprintf(stderr,"create error\n");
        exit(-1);
    }
}

void TcpServer::listener_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sock,int, void *arg)
{
    if(listener == NULL)
    {
        printf("asdasdsasd   error\n");
    }
    TcpServer *p = (TcpServer*)arg;
    p->listenEvent(fd,(struct sockaddr_in *)sock);
}
void TcpServer::listenEvent(evutil_socket_t fd, struct sockaddr_in*clientAddr)
{
    char *ip = inet_ntoa(clientAddr->sin_addr);
    u_int16_t port =  ntohs(clientAddr->sin_port);

    struct event_base* base = m_threadpool[m_nextThread].getBase();
    this->m_nextThread = (m_nextThread+1)%m_threadNum;
    //创建一个用于socket的bufferevent，用来缓存套接字读或写的数据，然后调用对应的回调函数，是的用户可以仅仅关注生成业务逻辑功能。
    struct bufferevent * bev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
    if (!bev)
    {
        fprintf(stderr, "Error constructing bufferevent!");
        event_base_loopbreak(base);
        return;
    }
    //把自身传给tcpsoket,有多少客户端就有多少tcpsoket，但都只有一个tcpserver
    Tcpsoket *s = new Tcpsoket(this, bev, ip, port);

    bufferevent_setcb(bev,s->readEventCb,s->writeEventCb,s->closeEventCb,s);
    bufferevent_enable(bev,EV_WRITE);//打开写缓冲区
    bufferevent_enable(bev,EV_READ);
    bufferevent_enable(bev,EV_SIGNAL);
    connectionEvent(s);
}

int TcpServer::listen1(int port, const char *ip)
{
    struct sockaddr_in sin;
    memset(&sin,0,sizeof (sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    if(ip != NULL){
        inet_aton(ip,&sin.sin_addr);
    }
    //创建监听器成功
    m_listener = evconnlistener_new_bind(m_base,listener_cb,this,
               LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,-1,
               (struct sockaddr*)&sin,sizeof(sin));
    if(!m_listener)
    {
        printf("sadasda error\n");
        return -1;
    }
    printf("创建监听器成功\n");

    //开启线程
    for(int i=0; i<m_threadNum; i++)
    {
        m_threadpool[i].start();
        printf("thred %d start\n",i+1);
    }

        return 0;
}

void TcpServer::start()
{
    event_base_dispatch(m_base);
//    event_base_loop(m_base,0);
    evconnlistener_free(m_listener);
    event_base_free(m_base);
    printf("don\n");
}
