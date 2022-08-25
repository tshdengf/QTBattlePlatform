#include "user.h"

user::user(std::string n,std::string m, int rank, Tcpsoket *s)
    :m_userName(n),
     m_passwd(m),
     m_rank(rank),
     m_s(s)
{

}

Tcpsoket * user::getTcpsoket()
{
    return m_s;
}

const char* user::getName()
{
    return m_userName.c_str();
}

int user::getrank()
{
    return m_rank;
}
void user::setRank(int rank)
{
    if(rank <= 0)
    {
        rank = 0;
    }
    m_rank = rank;
}
