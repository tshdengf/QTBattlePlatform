#include "conaunicate.h"
#include <QHostAddress>
#include <QDebug>
#include <iostream>
Conaunicate::Conaunicate(QObject *parent) : QObject(parent)
{
    //连接服务器
    s.connectToHost(QHostAddress("192.168.108.134"),9999);
    qDebug()<<"连接服务器\n";
    connect(&s, SIGNAL(readyRead()),this,SLOT(readData()));
std::cout<<"11111111110"<<std::endl;
//    fs = new QTcpSocket();
    fs.connectToHost(QHostAddress("192.168.108.134"),9898);
std::cout<<"11111111112"<<std::endl;
    //当有数据可读时，就调用readData
    connect(&fs,SIGNAL(readyRead()),this,SLOT(timeil1()));
std::cout<<"11111111113"<<std::endl;
    connect(&til,SIGNAL(timeout()),this,SLOT(timeil2()));
std::cout<<"11111111114"<<std::endl;
}
void Conaunicate::timeil1()
{
std::cout<<"11111111115"<<std::endl;
    til.start(20);
    fp = fopen("./adv.jpg","wb");
}

void Conaunicate::timeil2()
{
    csRead();
std::cout<<"11111111116"<<std::endl;
    if(size == 0)
    {
        til.stop();
std::cout<<"11111111117"<<std::endl;
    }
}

void Conaunicate::csRead()
{
    QByteArray data;

    int len = 0;
//    int falg = 0;
    size = LEN_SIZE;
    //判断有没有字节
        //先读前4个字节，转换为长度
//    while(size == LEN_SIZE)
    {
//        fs.read((char*)&falg,sizeof(falg));
//        if(falg == 0)
//        {
//            fp = fopen("./adv.jpg","wb");
//            std::cout<<"1111111111"<<std::endl;
//        }
//        std::cout<<"falg="<<falg<<std::endl;
//        fs.read((char*)&len,sizeof(len));
//        std::cout<<"len="<<len<<std::endl;

//        data = fs.read(len);
        data = fs.readAll();
        std::cout<<data.size()<<std::endl;
        len = data.size();

        char * buf = data.data();
        size = fwrite(buf,1,len,fp);
        if(size == 0)
        {
            fclose(fp);
        }
    }

}

void Conaunicate::readData()
{
    QByteArray data;
    //判断服务消息有没有读完
    while(s.bytesAvailable())
    {
        data += s.readAll();
    }
    qDebug()<<"进行数据解析\n";
    //数据解析
    QJsonDocument dt = QJsonDocument::fromJson(data);
    //没有得到数据
    if(dt.isNull())
    {
        return;
    }

    //转化位json对象
    QJsonObject root = dt.object();

    //具体的逻辑处理
    const int cmd = root["cmd"].toInt();
    std::cout<<root["cmd"].toInt()<<std::endl;
    switch (cmd){
    case REGISTER:
        emit rstResult(root["resulst"].toInt());
        break;
    case LOGIN:
        emit login(root);
        break;
    case SINGLE_GETQUESTION:
        emit receSingle(root["question"].toObject());
        break;
    case RANK:
        emit rank(root);
        break;
    case ANSWER:
        emit rank(root);
        break;
    case RANKRESULT:
        emit rank(root);
        break;
    case GUANLI:
        emit guangli1(root);
        break;
    case GETTABLE:
        emit getTable(root);
        break;
    case EIXT:
        emit rank(root);
        break;
    case GETFRIEND:
        emit getFirend(root);
        break;
    case ADDFIREND:
        emit addFirend(root);
        break;
    case DULE:
        emit quesDuel(root);
        break;
    case IMG:
        emit sendData(root);
        break;
    case CIMAG:
        emit recvIMG(root);
        break;
    default:
        break;
    }
}

void Conaunicate::writeData(const QJsonObject &json)
{
    QJsonDocument d(json);
    QByteArray sendData = d.toJson();
    int len = sendData.size();
    std::cout<<len<<std::endl;
    s.write((char*)&len,sizeof(int));//发送数据长度
    s.write(sendData); //发送数据
}
//void Conaunicate::writeDou(QByteArray tmep)
//{
//    s.write(tmep);
//}

void Conaunicate::writeFile(void *buf,int len)
{
    s.write((char*)buf,len);
}
