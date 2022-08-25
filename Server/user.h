#ifndef USER_H
#define USER_H
#include <string>
#include "tcpsoket.h"
class user
{
public:
    user(std::string n,std::string m, int rank, Tcpsoket *s);

    Tcpsoket * getTcpsoket();
    const char* getName();
    int getrank();

    void setRank(int rank);
private:
    std::string m_userName;
    std::string m_passwd;
    int m_rank;

    Tcpsoket *m_s;
};

#endif // USER_H
