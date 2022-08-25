#include <iostream>
#include "thread.h"
#include "tcpserver.h"
#include "myserver.h"
using namespace std;

int main2()
{
    TcpServer s;
    s.listen1(9999);
    s.start();

    return 0;
}

int main()
{
    myserver s;
    s.listen1(9999);
    s.start();
    return 0;
}
