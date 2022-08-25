#include "myserver.h"

myserver::myserver()
{
#ifndef DEBUG
    auto console = spdlog::stdout_color_mt("console");
    console->info("Welcome to spdlog!");
    console->error("some error message with arg{}...",123);

//    auto rotating_logger = spdlog::rotating_logger_mt("BrainStorm","BrainStorm",1024*1024*5,3);
//    rotating_logger->flush_on(spdlog::level::info);
//    for(int i=0;i<10;i++)
//    {
//        rotating_logger->info("{}*{}equals{:>10}",i,i,i*i);
//    }
      _log = spdlog::rotating_logger_mt("BrainStorm","BrainStorm",1024*1024*5,3);
      _log->flush_on(spdlog::level::info);
      _log->info("saaaaaaa");
#endif
      m_db = new DB(NULL,"dati","1","BrainStorm");
      initRank();
      imagefile.clear();
      pthread_create(&pth_id,NULL,worker,this);
      pthread_detach(pth_id);
}
void * myserver::worker(void *arg)
{
    printf("有回调函数\n");
    myserver*p = (myserver*)arg;
    p->start_IMG(p);
}

void myserver::start_IMG(myserver *s)
{
    printf("进入调函数11111111111111\n");
    signal(SIGPIPE,SIG_IGN);
    int sokfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in s_addr;
    int opt = 1;
    int slen = sizeof(struct sockaddr_in);
    setsockopt(sokfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(9898);
    s_addr.sin_addr.s_addr = inet_addr("192.168.108.134");
#if 1
    int ret = bind(sokfd,(struct sockaddr*)&s_addr,slen);
    if(ret < 0)
    {
        perror("bind error!");
        exit(1);
    }

    listen(sokfd,2000);
#endif
#if 1
    int epfd;
    struct epoll_event event;
    struct epoll_event events[1024];

    epfd = epoll_create(1024);
    event.events = EPOLLIN;
    event.data.fd = sokfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sokfd,&event);
    socklen_t c_size;
    struct sockaddr_in c_addr;
    c_size = sizeof(struct sockaddr_in);
    while(1)
    {
       int count = epoll_wait(epfd, events, 1024, -1);
        for(int i=0; i<count; i++)
        {
            if(events[i].events == EPOLLIN)
            {
                if(events[i].data.fd == sokfd)
                {
                    int fd = accept(sokfd,(struct sockaddr*)&c_addr,&c_size);
                    if(fd<0)
                    {
                        perror("accept error!");
                        exit(1);
                    }
                    //向epoll中添加事件
//                    epoll_ctl(epfd,EPOLL_CTL_ADD,sokfd,&event);
                    char buf[1024*3];
                    int len=0;
                    int size=0;
                    FILE *fp = fopen("./adv.jpg","rb");
                    while(int temp = fread(buf,1,sizeof(buf),fp))
                    {
                        len+=temp;
                    }
                    fclose(fp);
                    fp = fopen("./adv.jpg","rb");
                    int falg = 0;
                    while(len>0)
                    {
                        _log->info("有客户端连接len = {}:::11111111",len);
                        if(len > LEN_SIZE)
                        {
                            size = LEN_SIZE;
                        }
                        else
                        {
                            size = len;
                        }
                        printf("size = %d    ::111111111111111111\n",size);
                        memset(buf,0,sizeof(buf));
//                        char *ctr = (char*)&falg;
//                        for(int i=0; i<4; i++)
//                        {
//                            buf[i] = *ctr;
//                            ctr++;
//                        }
//                        char *ch = (char*)&size;
//                        for(int i=0; i<4; i++)
//                        {
//                            buf[i] = *ch;
//                            ch++;
//                        }
                        fread(buf,1,size,fp);
                        write(fd,buf,size);
                        len-=size;
//                        falg = 1;
//                        usleep(100000);
                    }
                    fclose(fp);
                }
            }
        }
    }
#endif
}

void myserver::connectionEvent(Tcpsoket*s)
{
    _log->info("有一个新连接[{}:{}]",s->getIp(),s->getPort());
    printf("client connection:%s   %d\n",s->getIp(),s->getPort());

    //给新连接端发广告
    sendCIMG(s);
}

//有事件可读
void myserver::readEvent(Tcpsoket*s)
{
//    char buf[1024]={0};
//    s->readDate(buf,1024);
//    printf("sned:%s\n",buf);
//    if(s->flag == 1)
//    {
//     c++文件系统接收
//        int len = 0;
//        char buf[1024*1024] = {0};
//        //主动调用读
//        s->readDate(&len,sizeof(len));
//        if(len <=0)
//        {
//        }
//        _log->info("准备可读");
//        s->readDate(buf,len);
//        _log->info("读时长度{}",len);

//        std::ofstream outfile;
//        outfile.open("phto.jpg");
//        int fileSize = 0;
//        printf("len = :%d\n",len);
//        outfile << buf << std::endl;
//        fileSize += len;
//        outfile.close();
        //c语言文件系统接受
//        FILE *fp;
//        fp = fopen("phto.txt","wb");
//        int temp;
//        int len;
//        char buf[15*1024] = {0};
//        temp = s->readDate(buf,sizeof(buf));
//        len = fwrite(buf,1,temp,fp);
//        printf("len = :%d\n",len);

        //xjy版本
//        FILE *dest;

//            int len;   // 接受的图片大小
//            int jpg_name = 1;           // 最终存储的.jpg图片文件名
//            int result = OK;            // 最终结果
//            char name_buf[100];         // 检查文件是否存在缓冲区
//            char recv_buf[15*1024];        // 接收文件缓冲区

//            while (1)
//            {
//                memset(name_buf, 0, sizeof(name_buf));
//                sprintf(name_buf, "%d.jpg", jpg_name);
//                if (access(name_buf, 0) != 0)    // 文件不存在
//                {
//                    break;
//                }
//                jpg_name++;
//            }
//            _log->info("create {}.jpg", jpg_name);

//            // 创建相应图片
//            dest = fopen(name_buf, "wb");

//            if (dest == NULL)
//            {
//                _log->error("admin upload jpg error");
//                result = ERROR;
//            }
//            else
//            {
//                _log->info("recieve {}.jpg", jpg_name);
//                // 开始接收图片
//                s->readDate(&len, sizeof(len));
//                _log->info("len is {}", len);
//                while (len != 0)
//                {
//                    memset(recv_buf, 0, sizeof(recv_buf));
//                    int tmp = len > sizeof(recv_buf) ? sizeof(recv_buf) : len;
//                    _log->info("{}", tmp);
//                    s->readDate(recv_buf, tmp);
//                    fwrite(recv_buf, 1, tmp, dest);
//                    len -= tmp;
//                    fflush(dest);
//                }
//                // 关闭文件描述符
//                fclose(dest);
//            }

//        s->flag = 0;
//    }
    char buf[15*1024] = {0};
    while(1)
    {
        int len = 0;
        //主动调用读
        s->readDate(&len,sizeof(len));
        if(len <=0)
        {
            break;
        }
        _log->info("准备可读");
        s->readDate(buf,len);
        _log->info("读时长度{}",len);
        //数据解析
        Json::Value root;
        Json::Reader reader; //json解析器
        if(!reader.parse(buf,root))
        {
            _log->error("json 数据解析失败");
            return;
        }
        _log->info("json 数据解析成功");
//        if(s->flag == 1)
//        {
//            createImage(s,root);
//            break;
//        }
        int cmd = root["cmd"].asInt();
        _log->info("走到这一步cmd = {}\n",cmd);
        switch (cmd) {
        case REGISTER:
            Register(s,root);
            break;
        case LOGIN:
            Login(s,root);
            break;
        case SINGLE_GETQUESTION:
            singleGetQuestion(s);
            break;
        case RANK:
            Rank(s);
            break;
        case ANSWER:
            rankAnswer(s,root);
            break;
        case RANKRESULT:
            rankResult(s,root);
            break;
        case GUANLI:
            guanliup(s,root);
            break;
        case GETTABLE:
            getTable(s,root);
            break;
        case SIMAGE:
            saveimage(s,root);
            break;
        case SENDHEAD:
            rideRecv(s,root);
            break;
        case GETFRIEND:
            chatfriend(s,root);
            break;
        case ADDFIREND:
            addFirend(s,root);
            break;
        case AGREE:
            setFirend(s,root);
            break;
        case DULE:
        {
            std::string toname = root["toname"].asString();
            user*auser = m_users[toname];
            if(auser == nullptr)
            {
                break;
            }
            root["myname"] = s->getUsername();
            writeData(auser->getTcpsoket(),root);
            break;
        }
        case YDULE:
            quesDule(s,root);
            break;
        case IMG:
        {
            _log->info("解析成功，准备进入图片2122121212121");
            sendIMG(s,root);
            break;
        }
        case CIMAG:
            startCIMG(s,root);
            break;
        default:
            break;

        }

    }
}
//不被动的等待可写事件，而是每次结束后主动写
void myserver::wirteEvent(Tcpsoket*)
{

}

//有退出信号时调用
void myserver::closeEvent(Tcpsoket*s,short )
{
    //有用户退出
    if(m_users[s->getUsername()] != nullptr)
    {
        {
//          std::unique_lock<std::mutex> lock(m_rankLock);
            int rank = m_users[s->getUsername()]->getrank();
            auto it = m_rankQueue.find(rank);
            if(it != m_rankQueue.end())
            {
                m_rankQueue.erase(it);
            }
        }

//    _log->info("有一个用户退出[{}:{}]",s->getIp(),s->getPort());
    _log->info("client eixt:%s   %d\n",s->getIp(),s->getPort());
    std::unique_lock<std::mutex> lock(m_userLock);
    std::map<std::string, user*>::iterator it = m_users.begin();
    while(it != m_users.end())
    {
        if(it->second->getTcpsoket() == s)
        {
            m_users.erase(it);
            _log->info("有一个用户退出{}[{}:{}]",it->second->getName(), s->getIp(),s->getPort());

            //释放user
            delete it->second;
            return;
        }
        it++;
    }
    }
    _log->info("有一个用户退出[{}:{}]", s->getIp(),s->getPort());
}
//封装主动写只处理需要写的事件
void myserver::writeData(Tcpsoket*s,const Json::Value &inJson)
{
    std::string data = inJson.toStyledString();
    s->weitDate(data.c_str(),data.length());
}
//用户注册
void myserver::Register(Tcpsoket*s,Json::Value &inJson)
{
    std::string userName = inJson["userName"].asString();
    std::string passwd = inJson["passwd"].asString();
    printf("Name:%s; passwd:%s\n",userName.c_str(),passwd.c_str());
    //查询用户是否已经存在
    char sql[100] = {0};
    sprintf(sql,"select *from user where name = '%s' and passwd = '%s'",userName.c_str(),passwd.c_str());

    int result = OK;
    Json::Value outJson;
    //进行查询，将数据保存到json
    bool ret = m_db->db_select(sql,outJson);
    if(!ret)
    {
        printf("没有sql字符串数据\n");
        _log->error("Register select");
        result = ERROR;
    }

    if(outJson.isMember("name"))
    {
        printf("有重复的名字\n");
        result = USEREXST;
    }
    else
    {
        _log->info("可以注册了result:%d\n",result);
        sprintf(sql,"insert into user(name,passwd,rank) values('%s','%s',0)",userName.c_str(),passwd.c_str());
        bool ret = m_db->db_exec(sql);
        if(!ret)
        {
            result = ERROR;
            _log->error("register insert");
        }
        else
        {
            _log->info("Register user = {} succed",userName);
        }
    }
    _log->info("处理完了result:%d\n",result);
    Json::Value json;
    json["cmd"] = REGISTER;
    json["resulst"] =result;
    _log->info("writeData:%s; passwd:%s, result:%d\n",userName.c_str(),passwd.c_str(),json["resulst"].isInt());
    std::cout<<json["resulst"].asString()<<std::endl;
    writeData(s,json);
}
//用户登陆
void myserver::Login(Tcpsoket*s,Json::Value &inJson)
{
    std::string userName = inJson["userName"].asString();
    std::string passwd = inJson["passwd"].asString();

    //查询用户是否已经注册
    char sql[100] = {0};
    sprintf(sql,"select *from user where name = '%s' and passwd = '%s'",userName.c_str(),passwd.c_str());

    int result = OK;
    int rank;
    Json::Value outJson;
    bool ret = m_db->db_select(sql,outJson);
    if(!ret)
    {
        _log->error("Register select");
        result = ERROR;
    }
    //用户已经登陆
    if(outJson.isMember("name"))//可以找到，可以登陆
    {
//        std::unique_lock<std::mutex> lock(m_userLock);
        if(m_users.find(userName) != m_users.end())//用户在线列表有相同的则返回
            result = USEREXSTLOGIN;
        else//否则登陆成功插入用户类
        {
            int i=0;
            rank = atoi(outJson["rank"][i].asString().c_str());

            user *auser = new user(userName,passwd,rank,s);

            m_users.insert(make_pair(userName,auser));
            _log->info("用户{}[{}:{}]login",userName,s->getIp(),s->getPort());
            s->setUserName(userName);
        }
    }
    else//没有注册
    {
        result = NAMEORPASSWD;
    }

    Json::Value json;
    json["cmd"] = LOGIN;
    json["resulst"] =result;
    json["userName"] = userName;
    json["rank"] = m_rankMap[rank];
    json["image"] = imagefile;
    writeData(s,json);
}
//个人训练获取问题
void myserver::singleGetQuestion(Tcpsoket *s)
{
    char sql[100] = {0};
    sprintf(sql,"select * from question order by rand() limit %d",QUESTION);

    int result = OK;
    Json::Value outJson;
    bool ret = m_db->db_select(sql,outJson);
    if(!ret || outJson["question"].size() != QUESTION)
    {
        result = ERROR;
        _log->error("singleGetQuestion select user error");
    }
    Json::Value json;
    json["cmd"] = SINGLE_GETQUESTION;
    json["result"] = result;
    json["question"] = outJson;

    _log->info("用户{}[{}:{}]获取题目:{}\n",s->getUsername(),s->getIp(),s->getPort(),json.toStyledString());
    writeData(s,json);
}
//初始化排位分
void myserver::initRank()
{
    char buf[100] = {0};
    int rank =0;
    int num = 0;
    for(int i=0; i<100; i++)
    {
        if(i<9)
        {
            rank = i/3;
            num = i%3;
            sprintf(buf,"青铜%d %d颗星",3-rank,num+1);
        }
        else if(i >= 9 && i<= 18)
        {
            rank = (i - 9)/3;
            num = (i-9)%3;
            sprintf(buf,"白银%d %d颗星",3-rank,num+1);
        }
        else if(i >= 18 && i<= 34)
        {
            rank = (i - 18)/3;
            num = (i-18)%3;
            sprintf(buf,"黄金%d %d颗星",3-rank,num+1);
        }
        else if(i >= 34 && i<= 50)
        {
            rank = (i - 34)/3;
            num = (i-34)%3;
            sprintf(buf,"铂金%d %d颗星",3-rank,num+1);
        }
        else if(i >= 50 && i<= 75)
        {
            rank = (i - 50)/3;
            num = (i-50)%3;
            sprintf(buf,"星耀%d %d颗星",3-rank,num+1);
        }
        else if(i >= 75 && i<= 100)
        {
            rank = (i - 75)/3;
            num = (i-75)%3;
            sprintf(buf,"王者%d %d颗星",3-rank,num+1);
        }
        m_rankMap.insert(std::make_pair(i,buf));
    }
}
//处理排位
void myserver::Rank(Tcpsoket*s)
{
    _log->info("{}:=进入选人111111111111111",s->getUsername());
    Tcpsoket *other = NULL;
    int rank = m_users[s->getUsername()]->getrank();
    _log->info("{}:=进入选人222222222222222",s->getUsername());
//    std::unique_lock<std::mutex> lock(m_rankLock);
    //查找同一队列对手
    std::map<int,Tcpsoket*>::iterator it = m_rankQueue.find(rank);
    if(it != m_rankQueue.end())
    {
        _log->info("{}:=进入选人33333333333",s->getUsername());
        other = it->second;
        m_rankQueue.erase(it);
    }
    else
    {
        _log->info("{}:=进入选人4444444444",s->getUsername());
        //查找其他段位选手，上下五个段位
        for(int i=1; i<=5; i++)
        {
            it == m_rankQueue.find(rank+i);
            if(it != m_rankQueue.end())
            {
                _log->info("{}:=进入选人1111::{}",s->getUsername(),i);
                other = it->second;
                _log->info("it.first = {}:it.second",it->first,it->second->getUsername());
                _log->info("{}:=进入选人2222::{}",s->getUsername(),i);
                m_rankQueue.erase(it);
                _log->info("{}:=进入选人3333::{}",s->getUsername(),i);
                break;
            }
            it == m_rankQueue.find(rank-i);
            if(it != m_rankQueue.end())
            {
                _log->info("{}:=进入选人4444::{}",s->getUsername(),i);
                other = it->second;
                _log->info("it.first = {}:it.second",it->first,it->second->getUsername());
                _log->info("{}:=进入选人5555::{}",s->getUsername(),i);
                m_rankQueue.erase(it);
                _log->info("{}:=进入选人6666::{}",s->getUsername(),i);
                break;
            }
        }
        _log->info("{}:=进入选人5555555555",s->getUsername());
    }
    if(other == NULL)
    {
        m_rankQueue.insert(std::make_pair(rank,s));
        _log->info("当前等候rank人数:{}",m_rankQueue.size());
    }
    else
    {
        //开始对决
        startRank(s,other);
        _log->info("开始对决1111");

    }
}
//开始决斗
void myserver::startRank(Tcpsoket*first,Tcpsoket*second)
{
    _log->info("开始对决2222");
    char sql[100] = {0};
    sprintf(sql,"select * from question order by rand() limit %d",QUESTION);
    _log->info("开始对决33333");
    int result = OK;
    Json::Value outJson;
    _log->info("开始对决4444");
    bool ret = m_db->db_select(sql,outJson);
//    std::cout<< outJson.asString()<<std::endl;
    _log->info("开始对决4444");
//    if((ret==false) || (outJson["question"].size() != QUESTION));
//    {
//        result = ERROR;
//        _log->error("rank select:{}",ret);
//        _log->error("rank select:{}",outJson["question"].size());
//        return;
//    }

    Json::Value json;
    json["cmd"] = RANK;
    json["result"] = result;
    json["question"] = outJson;

    json["enemyName"] = first->getUsername();
    json["enemyRank"] = m_rankMap[m_users[first->getUsername()]->getrank()];
    json["enemyScore"] = 0;
    writeData(second,json);

    json["enemyName"] = second->getUsername();
    json["enemyRank"] = m_rankMap[m_users[second->getUsername()]->getrank()];
    json["enemyScore"] = 0;
    writeData(first,json);

    _log->info("决斗开始");
}

//回答的问题
void myserver::rankAnswer(Tcpsoket*s,const Json::Value &inJson)
{
    std::string enemyName = inJson["enemyName"].asString();
    user* auser = m_users[enemyName];
    if(auser == nullptr)
    {
        Json::Value json;
        json["cmd"] = EIXT;
        json["enemyscore"] = inJson["score"].asInt();
        json["enemyQuestionId"] = inJson["questionId"].asInt();
        writeData(s,json);
        return;
    }
    Json::Value json;
    json["cmd"] = ANSWER;
    json["enemyscore"] = inJson["score"].asInt();
    json["enemyQuestionId"] = inJson["questionId"].asInt();
    writeData(auser->getTcpsoket(),json);
}

//rank结果
void myserver::rankResult(Tcpsoket*s,const Json::Value &inJson)
{
//    std::unique_lock<std::mutex> lock(m_userLock);
    user*auser = m_users[s->getUsername()];
    int score = inJson["score"].asInt();
    int enemyScore = inJson["enemyScore"].asInt();

    if(score < enemyScore)
    {
        auser->setRank(auser->getrank()-1);
    }
    else if(score > enemyScore)
    {
        auser->setRank(auser->getrank()+1);
    }
    _log->info("取得分数");
    Json::Value json;
    json["cmd"] = RANKRESULT;
    json["newRank"] = m_rankMap[auser->getrank()];
    _log->info("当前段位：{}",m_rankMap[auser->getrank()]);
    writeData(s,json);
}

//管理员登陆
void myserver::guanliup(Tcpsoket*s,Json::Value &inJson)
{
    std::string userName = inJson["userName"].asString();
    std::string passwd = inJson["passwd"].asString();

    //查询是否用户与密码是否正确
    char sql[100] = {0};
    sprintf(sql,"select * from guanli where name = '%s' and passwd = '%s'",userName.c_str(),passwd.c_str());
    int result = OK;
    int rank;
    Json::Value outJson;
    bool ret = m_db->db_select(sql,outJson);
    if(!ret)
    {
        _log->error("Register select");
        result = ERROR;
    }
    if(outJson.isMember("name"))//可以找到
    {
        std::unique_lock<std::mutex> lock(m_userLock);
        if(m_users.find(userName) != m_users.end())
            result = USEREXSTLOGIN;
        else
        {
            int i=0;
            rank = atoi(outJson["rank"][i].asString().c_str());
            user*auser = new user(userName,passwd,rank,s);
            m_users.insert(make_pair(userName,auser));
            s->setUserName(userName);
        }
    }
    else
    {
        result = NAMEORPASSWD;
    }
    Json::Value json;
    json["cmd"] = GUANLI;
    json["resulst"] = result;
    json["userName"] = userName;
    json["rank"] = m_rankMap[rank];

    writeData(s,json);

}

//获取数据库信息/在线用户信息
void myserver::getTable(Tcpsoket*s,Json::Value &inJson)
{
//    char sql[100] = {0};
//    sprintf(sql,"select name rank from user");
//    int result = OK;
//    Json::Value outJson;
//    bool ret = m_db->db_select(sql,outJson);
//    if(!ret)
//    {
//        _log->error("Register select");
//        result = ERROR;
//    }
    Json::Value json;
    json["cmd"] = GETTABLE;
    int row = 0;
    std::map<std::string,user*>::iterator it;
    it = m_users.begin();
    _log->info("getTable:11111111111111111");
    while(it != m_users.end())
    {
        json["name"].append(it->second->getName());
        json["rank"].append(m_rankMap[it->second->getrank()]);
        _log->info("getTable:{}",row);
        row++;
        it++;
    }
    json["row"] = row;

    writeData(s,json);
}

//保存图片数据
void myserver::saveimage(Tcpsoket*s,Json::Value &inJson)
{
    _log->info("saveimage11111111111111111");
    jsonImage.clear();
    imagefile.clear();
//    jsonImage["image"] = inJson["image"];
    imagefile = inJson["image"].asString();
    _log->info("saveimage22222222222222222");
}

void myserver::rideRecv(Tcpsoket*s,Json::Value &inJson)
{
    m_fileName = inJson["head"].asString();
    m_fileSize = inJson["filesize"].asInt();
    printf("m_fileSize = %d\n",m_fileSize);
    //准备读文件
    s->flag = 1;
}

void myserver::createImage(Tcpsoket*s,Json::Value &inJson)
{
    int fg = inJson["MF"].asInt();
    int len = inJson["len"].asInt();
    std::string buf[len];

    for(int i=0; i<len; i++)
    {
        std::cout<<inJson["data"].asString()<<std::endl;
    }

    if(fg == 0)
    {
        s->flag = 0;

    }
}
void myserver::chatfriend(Tcpsoket*s,Json::Value &inJson)
{
    std::string name = inJson["name"].asString();
    //查找所有好友用户
    char sql[100] = {0};
    sprintf(sql,"select toname from firend where myname = '%s'",name.c_str());
    int result = OK;
    Json::Value outJson;
    bool ret = m_db->db_select(sql,outJson);
    if(!ret)
    {
        _log->error("Register select");
        result = ERROR;
    }
    int len = outJson["toname"].size();

    //在线好友json
    Json::Value fjson;
    int row=0;
    //将元素数组元素输出
    Json::Reader Reader;
    Json::Value dest;
    std::string nameall =  outJson["toname"].toStyledString();
    Reader.parse(nameall, dest);
    for(int i=0; i<len; i++)
    {
        auto it = m_users.begin();
        while(it != m_users.end())
        {
            if(0==strcmp(it->second->getName(),dest[i].asString().c_str()))
            {
                std::cout<<"itname  "<<it->second->getName()<<std::endl;
                std::cout<<"name  "<<dest[i].asString().c_str()<<std::endl;
                fjson["name"].append(it->second->getName());
                row++;
            }
            it++;
        }
    }
    fjson["row"] = row;
    fjson["cmd"] = GETFRIEND;
    writeData(s,fjson);
//    std::cout << dest[0]<<std::endl;
//    for(int i=; i<len; i++)
//    {
//        auto it = m_users.begin();
//        while(it != m_users.end())
//        {
//            if(strcmp(it->second->getName(),nameall))
//            it++;
//        }
//    }
}

void myserver::addFirend(Tcpsoket*s,Json::Value &inJson)
{
    std::string name = inJson["name"].asString();
    user* auser = m_users[name];
    if(auser == nullptr)
    {
        return;
    }
    Json::Value json;
    json["cmd"] = ADDFIREND;
    if(auser == nullptr)
    {
        json["FG"] = 0;
        writeData(s,json);
        return;
    }
    json["name"] = s->getUsername();
    json["FG"] = 1;
    writeData(auser->getTcpsoket(),json);
}
void myserver::setFirend(Tcpsoket*s,Json::Value &inJson)
{
    _log->info("准备插入好友11111111111");
    std::string toname = inJson["toname"].asString();
    std::string myname = inJson["myname"].asString();
    char sql[100] = {0};
    _log->info("准备插入好友2222222222222");
    sprintf(sql,"insert into firend values('%s','%s')",myname.c_str(),toname.c_str());
    bool ret = m_db->db_exec(sql);
    _log->info("准备插入好友33333333333");
    if(!ret)
    {
        _log->error("register insert");
    }
    else
    {
        _log->info("Register user = {} succed",myname);
    }
    char sql1[100] = {0};
    sprintf(sql1,"insert into firend values('%s','%s')",toname.c_str(),myname.c_str());
    bool ret1 = m_db->db_exec(sql1);
    if(!ret1)
    {
        _log->error("register insert");
    }
    else
    {
        _log->info("Register user = {} succed",toname);
    }

    user* auser = m_users[toname];
    Json::Value json;
    json["cmd"] = ADDFIREND;
    if(auser == nullptr)
    {
        return;
    }
    json["name"] = s->getUsername();
    json["FG"] = 3;
    writeData(auser->getTcpsoket(),json);
}
void myserver::quesDule(Tcpsoket*s,Json::Value &inJson)
{
    std::string name = inJson["myname"].asString();
    user*auser = m_users[name];
    Tcpsoket* other = auser->getTcpsoket();
    std::cout<<"s:="<<s->getUsername()<<"  other:="<<other->getUsername()<<std::endl;
    startRank(s,other);
}
void myserver::sendIMG(Tcpsoket*s,Json::Value &inJson)
{
#if 0
    int bf = inJson["BF"].asInt();
    FILE * fp = NULL;
    if(bf == 1)
    {
        fp = fopen("s.jpg","w");
        if(NULL == fp)
        {
            _log->error("change adv, can not open file");
        }
    }

    int len = inJson["len"].asInt();
    char buff[50+1];
    Json::Value array = inJson["data"];
    for(int i=0; i<len; i++)
    {
        buff[i] = (char)array[i].asInt();
    }
    fwrite(buff,len,1,fp);

    int mf = inJson["MF"].asInt();
    if(mf == 0)
    {
        fclose(fp);
    }
    else if(mf != 1)
    {
        _log->error("change adv, json mf error");
        return;
    }
#endif
    _log->info("进入传图片1111111111111111111111");
    int bf = inJson["BF"].asInt();
        static FILE* fd = NULL;
        if(bf == 1)
        {
            fd = fopen("./adv.jpg","w");//覆盖
            if(NULL == fd)
            {
                _log->error("change adv, can not open file");
                return;
            }
        }
        else if(bf != 0)
        {
            _log->error("change adv, json bf error");
            return;
        }

        int len = inJson["len"].asInt();
        char buff[FILE_BLOCK_MAX];
        Json::Value array = inJson["data"];
        for(int i=0; i<len; i++)
        {
            buff[i] = (char)array[i].asInt();
            _log->info("字符：{}",buff[i]);
        }
        _log->info("len = {}",len);
        fwrite(buff,len,1,fd);

        int mf = inJson["MF"].asInt();
        if(mf == 0)
        {
            _log->info("传输结束1111111111111");
            Json::Value json;
            json["cmd"] = IMG;
            json["FM"] = 0;
            writeData(s,json);
            fclose(fd);
        }
        else if(mf != 1)
        {
            _log->error("change adv, json mf error");

            return;
        }
        else {
            _log->info("继续传.......");
            Json::Value json;
            json["cmd"] = IMG;
            json["FM"] = 1;
            writeData(s,json);
            return;
        }
}
void myserver::sendCIMG(Tcpsoket*s)
{
    //先发json
    m_fd = fopen("./adv.jpg","rb");//只读
    int temp;
    char buf[1024];
    while((temp = fread(buf,1,sizeof (buf),m_fd))>0)
    {
        m_fileSize+=temp;
    }

    Json::Value json;
    json["BF"] = 1;
    json["MF"] = 1;
    flag = 0;
    startCIMG(s,json);
}
void myserver::startCIMG(Tcpsoket*s,Json::Value &inJson)
{
#if 0
    Json::Value json;
    json["cmd"] = CIMAG;
    if(flag == 0)
    {
        json["BF"] = 1;//开头
    }
    else
    {
        json["BF"] = 0;
    }
    json["MF"]=1;
    int mf = inJson["MF"].asInt();

    if(mf == 0)
    {
        fclose(m_fd);
        flag=0;
        return;
    }

    unsigned char buff[FILE_BLOCK_MAX+1];
    memset(buff,0,FILE_BLOCK_MAX);
    int size;
    if(m_fileSize>FILE_BLOCK_MAX)
    {
        size = FILE_BLOCK_MAX;
    }
    else {
        size = m_fileSize;
        json["MF"]=0;
    }
    json["len"] = size;
    fread((char*)buff,1,size,m_fd);
    Json::Value array;
    for(int i=0; i<size; i++)
    {
        array["data"].append(buff[i]);
    }
    writeData(s,json);
    flag = 1;
#endif
    Json::Value json;
    json["cmd"] = CIMAG;
    if(flag == 0)
    {
        json["BF"] = 1;//开头
    }
    else
    {
        json["BF"] = 0;
    }
    json["MF"]=1;
    int mf = inJson["MF"].asInt();

    if(mf == 0)
    {
        fclose(m_fd);
        flag=0;
        return;
    }

    unsigned char buff[FILE_BLOCK_MAX+1];
    memset(buff,0,FILE_BLOCK_MAX);
    int size;
    if(m_fileSize>FILE_BLOCK_MAX)
    {
        size = FILE_BLOCK_MAX;
    }
    else {
        size = m_fileSize;
        json["MF"]=0;
    }
    json["len"] = size;
    //先发json通之要接收多少
    writeData(s,json);

    fread((char*)buff,1,size,m_fd);
    s->weitDate(buff,size);
    flag = 1;
}
