#ifndef MYSERVER_H
#define MYSERVER_H

#include "tcpserver.h"
#include <spdlog/spdlog.h>
#include "db.h"
#include <jsoncpp/json/json.h>
#include "../commo.h"
#include <string>
#include <map>
#include "user.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <pthread.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <sys/epoll.h>
#include <signal.h>



//#define DEBUG 1
class myserver:public TcpServer
{
public:
    myserver();

    void connectionEvent(Tcpsoket*);

    void readEvent(Tcpsoket*);

    void wirteEvent(Tcpsoket*);

    void closeEvent(Tcpsoket*,short );

private:
    //发送数据
    void writeData(Tcpsoket*s,const Json::Value &inJson);
    //用户注册
    void Register(Tcpsoket*s,Json::Value &inJson);
    //用户登陆
    void Login(Tcpsoket*s,Json::Value &inJson);
    //个人训练获取题目
    void singleGetQuestion(Tcpsoket*s);

    //进行排位
    void Rank(Tcpsoket*s);
    //开始对决
    void startRank(Tcpsoket*first,Tcpsoket*second);
    //初始化rank积分对照表
    void initRank();
    //回答的问题
    void rankAnswer(Tcpsoket*s,const Json::Value &inJson);
    //rank结果
    void rankResult(Tcpsoket*s,const Json::Value &inJson);
    //管理员登陆
    void guanliup(Tcpsoket*s,Json::Value &inJson);
    //获取数据库信息
    void getTable(Tcpsoket*s,Json::Value &inJson);
    //保存图片数据
    void saveimage(Tcpsoket*s,Json::Value &inJson);
    //准备接收图片数据
    void rideRecv(Tcpsoket*s,Json::Value &inJson);
    //构造图片数据
    void createImage(Tcpsoket*s,Json::Value &inJson);
    //查找在线好友
    void chatfriend(Tcpsoket*s,Json::Value &inJson);
    //添加好友
    void addFirend(Tcpsoket*s,Json::Value &inJson);
    //添加好友
    void setFirend(Tcpsoket*s,Json::Value &inJson);
    //邀请对战
    void quesDule(Tcpsoket*s,Json::Value &inJson);
    //传图片
    void sendIMG(Tcpsoket*s,Json::Value &inJson);
    //给客户端发图片
    void sendCIMG(Tcpsoket*s);
    //开始发文件
    void startCIMG(Tcpsoket*s,Json::Value &inJson);
    //开启第二个服务器线程
    static void * worker(void *arg);
    //执行服务器线程内容
    void start_IMG(myserver*);
private:
    std::shared_ptr<spdlog::logger> _log;
    DB* m_db;
    pthread_t pth_id;          //第二个服务器线程id
    //建是用户名，值是用户指针
    std::mutex m_userLock;
    std::map<std::string,user*> m_users; //在线用户列表
    //key是rank积分，值是段位说明
    std::map<int,std::string> m_rankMap; //rank积分对比表
    //key是积分 vlue是用户句柄
    std::mutex m_rankLock;
    std::map<int,Tcpsoket*> m_rankQueue;

    Json::Value jsonImage;               //图片json对象
    std::string imagefile;                   //图片路径
    std::string m_fileName;              //文件名
    int m_fileSize;              //文件长度
    FILE *m_fd;
    int flag;

};

#endif // MYSERVER_H
