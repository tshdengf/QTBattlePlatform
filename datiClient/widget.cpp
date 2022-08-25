#include "widget.h"
#include "ui_widget.h"
#include "rejster.h"
#include "brainstorm.h"
#include <QMessageBox>
#include <iostream>
#include <QJsonArray>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //负责和服务器通信
    m_com = new Conaunicate();

    connect(m_com, SIGNAL(login(QJsonObject)),this, SLOT(login(QJsonObject)));

    connect(m_com,SIGNAL(guangli1(QJsonObject)),this,SLOT(guanli1(QJsonObject)));
//    connect(m_com,SIGNAL(recvIMG(QJsonObject)),this,SLOT(recvIMG(QJsonObject)));

//    recvAdv();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::login(QJsonObject json)
{
    int result = json["resulst"].toInt();
    switch (result) {
    case OK:
    {
        QMessageBox::information(this,"登录","登录成功");
        this->hide();

        BrainStorm*dlg = new BrainStorm(m_com,json);
        dlg->show();
        /*所以说调用close()并不一定就会将窗口对象销毁。
         * 而只有设置了 Qt::WA_DeleteOnClose属性才会删除销毁。
         * 如果这个属性没有设置，close()的作用和hide（），setvisible（false）一样，
         * 只会隐藏窗口对象而已，并不会销毁该对象。*/
        //把dlg指定为某种类型
        dlg->setAttribute(Qt::WA_DeleteOnClose);//
        break;
    }
    case ERROR:
    {
        QMessageBox::critical(this,"登录","登录失败未知错误");
        break;
    }
    case USEREXSTLOGIN:
    {
        QMessageBox::information(this,"登录","登录失败，不允许重复登录");
        break;
    }
    case NAMEORPASSWD:
    {
        QMessageBox::information(this,"登录","用户名或密码错误");
        break;
    }
    default:
        break;
    }
}

void Widget::on_denlupushButton_clicked()
{
    QString userName = ui->userlineEdit->text();
    QString password = ui->passwordlineEdit->text();

    //将登录信息发送给服务器
    QJsonObject josn;
    josn["cmd"] = LOGIN;
    josn["userName"] = userName;
    josn["passwd"] = password;


    //给服务器发消息
    m_com->writeData(josn);
}

void Widget::on_zhuchepushButton_clicked()
{
    this->hide();
    rejster reg(m_com);
    reg.exec();
    this->show();
}

void Widget::on_pushButton_clicked()
{
    QString userName = ui->userlineEdit->text();
    QString passwd = ui->passwordlineEdit->text();

    //发送给服务器
    QJsonObject json;
    json["cmd"] = GUANLI;
    json["userName"] = userName;
    json["passwd"] = passwd;

    //发给服务器
    m_com->writeData(json);
}

//管理登录
void Widget::guanli1(QJsonObject json)
{
    int result = json["resulst"].toInt();
    switch (result) {
    case OK:
    {
        QMessageBox::information(this,"管理员登录","登录成功");
        this->hide();
        guanli*gua = new guanli(m_com,json);
        gua->show();
        gua->setAttribute(Qt::WA_DeleteOnClose);
        break;
    }
    case ERROR:
    {
        QMessageBox::critical(this,"管理员登录","登录失败未知错误");
        break;
    }
    case USEREXSTLOGIN:
    {
        QMessageBox::information(this,"管理员登录","登录失败，不允许重复登录");
        break;
    }
    case NAMEORPASSWD:
    {
        QMessageBox::information(this,"管理员登录","用户名或密码错误");
        break;
    }
    default:
        break;
    }
}
#if 0
void recvAdv()
{
    fs = new QTcpSocket();
    fs->connectToHost(QHostAddress(SERVER_IP),9998);
    connect(fs,SIGNAL(readyRead()),this,SLOT(recvIMG(QJsonObject)));//当有数据可读时，就调用readData;

}

void Widget::recvIMG(QJsonObject injson)
{
    std::cout <<"11111111111111111"<<std::endl;
    int bf = injson["BF"].toInt();
    static FILE*fd = NULL;
    if(bf == 1)
    {
        fd = fopen("./adv.jpg","w");
        if(NULL == fd)
        {
            return;
        }
    }
    else if(bf != 0)
    {
        return;
    }
    std::cout <<"2222222222222"<<std::endl;
    int len = injson["len"].toInt();
    char buff[FILE_BLOCK_MAX];
//    QJsonArray array = injson["data"].toArray();
//    std::cout <<"3333333333331"<<std::endl;
//    std::cout <<"ssssssssssss"<<array.at(20).isString()<<std::endl;
//    for(int i=0; i<len; i++)
//    {
//        buff[i] = array.at(i).isString();
//        std::cout <<array.at(i).isString()<<std::endl;
//    }
//    std::cout <<"44444444444441"<<std::endl;

    fwrite(buff,len,1,fd);
    int mf = injson["MF"].toInt();
    if(mf == 0)
    {
        QJsonObject json;
        json["cmd"] = CIMAG;
        json["FM"] = 0;
        m_com->writeData(json);
        fclose(fd);
    }
    else if(mf != 1)
    {
        return;
    }
    else
    {
        QJsonObject json;
        json["cmd"] = CIMAG;
        json["FM"] = 1;
        m_com->writeData(json);
        return;
    }

}
#endif
