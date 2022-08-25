#include "thread.h"

Thread::Thread()
{
    m_base = event_base_new();
    if(!m_base)
    {
        fprintf(stderr,"create error");
        exit(-1);
    }
    //创建管道
    int fd[2];  //读段和写段
    if (pipe(fd) == -1)
    {
      perror("pipe");
      exit(EXIT_FAILURE);
    }
    m_pipeReadFd=fd[0];
    m_pipeWriteFd=fd[1];

    //让管道事件监听管道的读段
    //如果监听到管道读段有数据可读，则会执行相应的函数  EV_PERSIST使事件永久有效 EV_READ：监听管道是否有可读事件
    event_set(&m_pipeEvent,m_pipeReadFd,EV_READ|EV_PERSIST,pipeRead,this);
    //将事件添加到m_base集合中
    event_base_set(m_base,&m_pipeEvent);
    //开启可读事件
    event_add(&m_pipeEvent,0);

}
void Thread::pipeRead(evutil_socket_t,short,void *)
{

}
void Thread::start()
{
    //create thread
    int i =  pthread_create(&m_threadId,NULL,worker,this);
    if(i)
    {
        exit(-1);
    }
    pthread_detach(m_threadId);
}

void* Thread::worker(void *arg)
{
    Thread *p = (Thread *)arg;
    p->run();
    return NULL;
}

void Thread::run()
{
    //监听base事件集合,如果集合是空的，event_base_dispatch会立即返回
    //初始化的时候需要给m_base添加一个事件,让他不变
    event_base_dispatch(m_base);
//    event_base_loop(m_base,0);
    event_base_free(m_base);
}
struct event_base * Thread::getBase()
{
    return m_base;
}
