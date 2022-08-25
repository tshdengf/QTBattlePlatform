#include "rejster.h"
#include "ui_rejster.h"
#include <QDebug>
rejster::rejster(Conaunicate * com,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::rejster)
{
    ui->setupUi(this);
    m_com = com;

    connect(m_com, SIGNAL(rstResult(int)),this, SLOT(rstResult(int)));
}

rejster::~rejster()
{
    delete ui;
}

 void rejster::rstResult(int ret)
 {
     switch (ret) {
     case OK:
         QMessageBox::information(this,"注册","注册成功");
         break;
     case ERROR:
         QMessageBox::critical(this,"注册","注册失败");
         break;
     case USEREXST:
         QMessageBox::information(this,"注册","已经注册");
         break;
     default:
         break;
     }
 }

void rejster::on_rejsterpushButton_clicked()
{
    QString userName = ui->yonghulineEdit->text();
    QString password = ui->mimalineEdit->text();

    //发送注册信息
    QJsonObject josn;
    josn["cmd"] = REGISTER;
    josn["userName"] = userName;
    josn["passwd"] = password;

    //向服务器发送数据
    m_com->writeData(josn);
}

void rejster::on_brankpushButton_clicked()
{
    this->close();
}
