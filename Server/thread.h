#ifndef THREAD_H
#define THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>

#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/event_compat.h>

//
class Thread
{
public:
    Thread();

    void start();//

    struct event_base * getBase();
protected:
    static void *worker(void *);
    static void pipeRead(evutil_socket_t,short,void *);
    void run();
private:
    struct event_base *m_base;
    pthread_t m_threadId;
    int m_pipeReadFd;  //管道读端
    int m_pipeWriteFd;  //管道写端
    struct event m_pipeEvent; //管道事件集合
};

#endif // THREAD_H
