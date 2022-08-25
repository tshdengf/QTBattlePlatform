#include "tcpsoket.h"
TcpServer*Tcpsoket::m_tcpserver = NULL;
Tcpsoket::Tcpsoket(TcpServer* tcpserver,struct bufferevent*bev, char *ip, u_int16_t port)
{
    m_tcpserver = tcpserver;
    m_bev = bev;
    m_ip = ip;
    m_port = port;
    flag = 0;
}
//可读事件回调函数
void Tcpsoket::readEventCb(struct bufferevent*, void *ctx)
{
    Tcpsoket*s = (Tcpsoket*)ctx;
    m_tcpserver->readEvent(s);
}
//可写事件回调函数
void Tcpsoket::writeEventCb(struct bufferevent* ,void *ctx)
{
    Tcpsoket*s = (Tcpsoket*)ctx;
    m_tcpserver->wirteEvent(s);
}
void Tcpsoket::closeEventCb(struct bufferevent* ,short what ,void *ctx)
{
    Tcpsoket*s = (Tcpsoket*)ctx;
    m_tcpserver->closeEvent(s,what);

    delete s;
}

char * Tcpsoket::getIp()
{
    return m_ip;
}
u_int16_t Tcpsoket::getPort()
{
    return m_port;
}

int Tcpsoket::readDate(void *data, int size)
{
    return bufferevent_read(m_bev,data,size);
}
int Tcpsoket::weitDate(const void *data, int size)
{
    return bufferevent_write(m_bev,data,size);
}

void Tcpsoket::setUserName(std::string name)
{
    m_userName = name;
}

std::string Tcpsoket::getUsername()
{
    return m_userName;
}
