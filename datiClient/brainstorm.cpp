#include "brainstorm.h"
#include "ui_brainstorm.h"
#include <iostream>

BrainStorm::BrainStorm(Conaunicate *com,QJsonObject &josn,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrainStorm)
{
    ui->setupUi(this);
    m_com = com;

    m_userName = josn["userName"].toString();
    m_userRank = josn["userRank"].toString();

//  尝试接收服务器发来的json图片流
//    QByteArray img64 = josn["image"].toVariant().toByteArray();
//    QByteArray img = QByteArray::fromBase64(img64);
//    pixmap.loadFromData(img);
//    ui->guanggao->setPixmap(pixmap);

//   尝试从网络中获取图片
//    QNetworkAccessManager * manager;
//    manager = new QNetworkAccessManager(this);

//    connect(manager,SIGNAL(finished(QNetworkReply*)),
//            this,SLOT(replyFin(QNetworkReply*)));
//    manager->get(QNetworkRequest(QUrl(josn["image"].toString())));

    //从服务器中获取本地路径
//    imagefilename = josn["image"].toString();
    QPixmap pix("./adv.jpg");
    ui->guanggao->setPixmap(pix);
    ui->gaogao->setPixmap(pix);
    ui->gaogao1->setPixmap(pix);
    ui->gaogao2->setPixmap(pix);
    ui->gaogao3->setPixmap(pix);
    ui->gaogao4->setPixmap(pix);
    //在线好友列表
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"用户名");

    connect(&m_singleTimer,SIGNAL(timeout()),this,SLOT(singleTimerOut()));
    connect(&m_rankTimer,SIGNAL(timeout()),this,SLOT(rankTimerOut()));

    connect(m_com,SIGNAL(receSingle(QJsonObject)),this,SLOT(receiveSingleQuestion(QJsonObject)));
    connect(m_com,SIGNAL(rank(QJsonObject)),this,SLOT(receiveRank(QJsonObject)));
    connect(m_com,SIGNAL(getFirend(QJsonObject)),this,SLOT(getFirend(QJsonObject)));
    connect(m_com,SIGNAL(addFirend(QJsonObject)),this,SLOT(addFirend(QJsonObject)));
    connect(m_com,SIGNAL(quesDuel(QJsonObject)),this,SLOT(quesDuel(QJsonObject)));

}
void BrainStorm::replyFin(QNetworkReply*reply)
{
   if(reply->error() == QNetworkReply::NoError)
   {
       pixmap.loadFromData(reply->readAll());
//       QDateTime now;
//       QString filename = now.currentDateTime().toString("1.jpg");
//       pixmap->save(filename);
//       imagefilename = filename;
       //设置长宽
//       QPixmap tianqi_pixmap = pixmap.scaled(ui->guanggao->width(), ui.guanggao->height(), Qt::KeepAspectRatio);
//       ui->guanggao->setPixmap(tianqi_pixmap);

//       QMovie *move = new QMovie(pixmap);
//       move->start();
//       ui->guanggao->setPixmap(pixmap);
       printf("sssssss\n");
   }
}

BrainStorm::~BrainStorm()
{
    delete ui;
}

//个人训练获取
void BrainStorm::singleGetQuestion()
{
    QJsonObject json;
    json["cmd"] = SINGLE_GETQUESTION;

    m_com->writeData(json);
}
//个人训练记时器
void BrainStorm::singleTimerOut()
{
    if(--m_singleSec == 0)
    {
        ui->singlerslut->setText("回答超时");
        QString str = QString("本次得分：%1").arg(m_singleScore);
        ui->singlescore->setText(str);
        m_singleTimer.stop();
        ui->stackedWidget->setCurrentWidget(ui->single_score);
    }
    ui->lcdNumber->display(m_singleSec);
}


//进入个人训练
void BrainStorm::on_singleButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->singlepage);

}
//接收题目
void BrainStorm::receiveSingleQuestion(QJsonObject json)
{
    m_singlejson = json;
    m_singleSec = 10;
    m_curSingleIndex = 0;
    m_singleScore = 0;
    ui->lcdNumber->display(m_singleSec);
    singleSetQuestion();
    //开启定时器，平均一秒显示一次
    m_singleTimer.start(1000);
    ui->stackedWidget->setCurrentWidget(ui->singleruning);

}
//个人训练设置问题
void BrainStorm::singleSetQuestion()
{
    if(m_curSingleIndex == QUESTION)
    {
        ui->singlerslut->setText("答题全部正确");
        QString str = QString("本次得分：%1").arg(m_singleScore);
        ui->singlescore->setText(str);
        m_singleTimer.stop();
        ui->stackedWidget->setCurrentWidget(ui->single_score);
        return;
    }
    ui->runing_view->setText(m_singlejson["question"].toArray().at(m_curSingleIndex).toString());
    ui->pushButton_A->setText(m_singlejson["selection1"].toArray().at(m_curSingleIndex).toString());
    ui->pushButton_B->setText(m_singlejson["selection2"].toArray().at(m_curSingleIndex).toString());
    ui->pushButton_C->setText(m_singlejson["selection3"].toArray().at(m_curSingleIndex).toString());
    ui->pushButton_D->setText(m_singlejson["selection4"].toArray().at(m_curSingleIndex).toString());
}

//个人训练打开
void BrainStorm::on_brankButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
}

//开始答题
void BrainStorm::on_startButton_clicked()
{
    singleGetQuestion();
}
//返回主页面
void BrainStorm::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
}
void BrainStorm::singleAnswerQuestion(int select)
{
    if(select == m_singlejson["answer"].toArray().at(m_curSingleIndex).toString().toInt())
    {
        m_singleScore += 20*m_singleSec;
        m_curSingleIndex++;
        singleSetQuestion();
        m_singleSec=10;
        m_singleTimer.stop();
        ui->lcdNumber->display(m_singleSec);
        m_singleTimer.start(1000);
    }
    else
    {
        ui->singlerslut->setText("回答错误");
        QString str = QString("本次得分：%1").arg(m_singleScore);
        ui->singlescore->setText(str);
        m_singleTimer.stop();
        ui->stackedWidget->setCurrentWidget(ui->single_score);
    }
}


void BrainStorm::on_pushButton_A_clicked()
{
    singleAnswerQuestion(1);
}

void BrainStorm::on_pushButton_B_clicked()
{
    singleAnswerQuestion(2);
}

void BrainStorm::on_pushButton_C_clicked()
{
    singleAnswerQuestion(3);
}

void BrainStorm::on_pushButton_D_clicked()
{
    singleAnswerQuestion(4);
}


//rank开始
void BrainStorm::on_rankButton_clicked()
{
    QJsonObject json;
    json["cmd"] = RANK;

    m_com->writeData(json);
    ui->stackedWidget->setCurrentWidget(ui->rank_wait);
}

//排位接收数据
void BrainStorm::receiveRank(QJsonObject json)
{
    std::cout<<"jingrun Rank 1111111111"<<std::endl;
    int cmd = json["cmd"].toInt();
    if(cmd == RANK)
    {
        std::cout<<"jingrun Rank 1111111111"<<std::endl;
        m_rankQuestion = json["question"].toObject();
        m_enemyName = json["enemyName"].toString();
        m_enemyRank = json["enemyRank"].toString();
        m_enemyScore = json["enemyScore"].toInt();

        m_rankSec = 10;
        m_myScore = 0;
        m_currankIndex = 0;
        m_enemyrankIndex = 0;
        ui->lcdNumber_2->display(m_rankSec);

        setEnemyScore();
        setselfScore();
        rankSetQuestion();
        //开启定时器，平均一秒显示一次
        m_rankTimer.start(1000);
        ui->stackedWidget->setCurrentWidget(ui->rank_runing);
    }
    else if(cmd == ANSWER)
    {

        m_enemyScore = json["enemyscore"].toInt();
        m_enemyrankIndex = json["enemyQuestionId"].toInt();
        setEnemyScore();

        if(m_currankIndex == m_enemyrankIndex)
        {
            m_rankSec = 10;
            m_rankTimer.stop();
            ui->lcdNumber_2->display(m_rankSec);
            m_rankTimer.start(1000);
            rankSetQuestion();
        }
    }
    else if(cmd == EIXT)
    {
        json["cmd"] = RANKRESULT;
        m_com->writeData(json);
    }
    else if(cmd == RANKRESULT)
    {
        rankReslut(json);
    }
}
//设置得分
void BrainStorm::setEnemyScore()
{
    QString str = QString("%1(%2):%3").arg(m_enemyName,-5).arg(m_enemyRank).arg(m_enemyScore);
    ui->optname->setText(str);

}
void BrainStorm::setselfScore()
{
    QString str = QString("%1(%2):%3").arg(m_userName,-5).arg(m_userRank).arg(m_myScore);
    ui->selfename->setText(str);
}

//rank设置问题
void BrainStorm::rankSetQuestion()
{
    if(m_currankIndex == QUESTION)
    {
        printf("111111111111111111111111111111\n");
        m_rankTimer.stop();
        //发送给服务器
        QJsonObject json;
        json["cmd"] = RANKRESULT;
        json["score"] = m_myScore;
        json["enemyName"] = m_enemyName;
        json["enemyScore"] = m_enemyScore;

        m_com->writeData(json);

    }
    ui->rankview->setText(m_rankQuestion["question"].toArray().at(m_currankIndex).toString());
    ui->pushButtonA->setText(m_rankQuestion["selection1"].toArray().at(m_currankIndex).toString());
    ui->pushButtonB->setText(m_rankQuestion["selection2"].toArray().at(m_currankIndex).toString());
    ui->pushButtonC->setText(m_rankQuestion["selection3"].toArray().at(m_currankIndex).toString());
    ui->pushButtonD->setText(m_rankQuestion["selection4"].toArray().at(m_currankIndex).toString());
    ui->pushButtonA->setStyleSheet("");
    ui->pushButtonB->setStyleSheet("");
    ui->pushButtonC->setStyleSheet("");
    ui->pushButtonD->setStyleSheet("");
    ui->pushButtonA->setEnabled(true);
    ui->pushButtonB->setEnabled(true);
    ui->pushButtonC->setEnabled(true);
    ui->pushButtonD->setEnabled(true);

}

//排位定时器
void BrainStorm::rankTimerOut()
{
    if(--m_rankSec == 0)
    {
        rankAnser(0);
    }
    ui->lcdNumber_2->display(m_rankSec);
    return;
}

//排位赛回答问题
void BrainStorm::rankAnser(int select)
{
    if(select == m_rankQuestion["answer"].toArray().at(m_currankIndex).toString().toInt())
    {
        m_myScore += 20*m_rankSec;
    }
    setselfScore();
    printf("m_cur = %d\n",m_currankIndex);
    //判断是否跳到下一题
    m_currankIndex++;
    if(m_currankIndex == m_enemyrankIndex)
    {
        m_rankSec = 10;
        m_rankTimer.stop();
        ui->lcdNumber_2->display(m_rankSec);
        m_rankTimer.start(1000);
        rankSetQuestion();
    }
    //准备数据
    QJsonObject json;
    json["cmd"] = ANSWER;
    json["enemyName"] = m_enemyName;
    json["score"] = m_myScore;
    json["questionId"] = m_currankIndex;

    m_com->writeData(json);

}

void BrainStorm::on_pushButtonA_clicked()
{
    ui->pushButtonA->setStyleSheet("color: rgb(85, 255, 255)");
    ui->pushButtonA->setEnabled(false);
    ui->pushButtonB->setEnabled(false);
    ui->pushButtonC->setEnabled(false);
    ui->pushButtonD->setEnabled(false);
    rankAnser(1);
}

void BrainStorm::on_pushButtonB_clicked()
{
    ui->pushButtonB->setStyleSheet("color: rgb(85, 255, 255)");
    ui->pushButtonA->setEnabled(false);
    ui->pushButtonB->setEnabled(false);
    ui->pushButtonC->setEnabled(false);
    ui->pushButtonD->setEnabled(false);
    rankAnser(2);
}

void BrainStorm::on_pushButtonC_clicked()
{
    ui->pushButtonC->setStyleSheet("color: rgb(85, 255, 255)");
    ui->pushButtonA->setEnabled(false);
    ui->pushButtonB->setEnabled(false);
    ui->pushButtonC->setEnabled(false);
    ui->pushButtonD->setEnabled(false);
    rankAnser(3);
}

void BrainStorm::on_pushButtonD_clicked()
{
    ui->pushButtonD->setStyleSheet("color: rgb(85, 255, 255)");
    ui->pushButtonA->setEnabled(false);
    ui->pushButtonB->setEnabled(false);
    ui->pushButtonC->setEnabled(false);
    ui->pushButtonD->setEnabled(false);
    rankAnser(4);
}

//rank结果
void BrainStorm::rankReslut(QJsonObject json)
{
    QString newRank = json["newRank"].toString();
    if(m_myScore == m_enemyScore)
    {
        ui->reslut_new->setText("平局");
    }
    else if(m_myScore < m_enemyScore)
    {
        ui->reslut_new->setText("失败");
    }
    else
    {
        ui->reslut_new->setText("成功");
    }
    QString str = QString("%1-->%2").arg(m_userRank).arg(newRank);
    ui->newRank->setText(str);
    m_userRank = newRank;
    ui->stackedWidget->setCurrentWidget(ui->rank_result);
}

void BrainStorm::on_rank_brank_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
}

//查找在线好友按钮
void BrainStorm::on_pushButton_2_clicked()
{
    QJsonObject json;
    json["cmd"] = GETFRIEND;
    json["name"] = m_userName;
    m_com->writeData(json);
}
void BrainStorm::getFirend(QJsonObject json)
{
    int row = json["row"].toInt();
    ui->tableWidget->setRowCount(row);
    for(int i=0; i<row; i++)
    {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(json["name"].toArray().at(i).toString()));
//        ui->tableWidget->setItem(i,1,new QTableWidgetItem(json["rank"].toArray().at(i).toString()));
    }
}

//结束页面添加好友按钮
void BrainStorm::on_pushButton_5_clicked()
{
    QJsonObject json;
    json["cmd"] = ADDFIREND;
    json["name"] = m_enemyName;
    m_com->writeData(json);
}
void BrainStorm::addFirend(QJsonObject json)
{
    QJsonObject tojson;
    int fg = json["FG"].toInt();
    QString name = json["name"].toString();
    if(fg == 0)
    {
        QMessageBox::information(this,"好友","已拒绝,添加失败");
    }
    else if(fg == 1)
    {
        int agss = QMessageBox::question(this,"添加好友","是否接受？",QMessageBox::Yes,QMessageBox::No);
        if(QMessageBox::Yes == agss)
        {
            std::cout<<"1111111111"<<std::endl;
            tojson["cmd"] = AGREE;
            tojson["myname"] = m_userName;
            tojson["toname"] = name;
            m_com->writeData(tojson);
        }
        else
        {
            std::cout<<"222222222222"<<std::endl;
            tojson["cmd"] = ADDFIREND;
            tojson["FG"] = 3;
            m_com->writeData(tojson);
        }
    }
    else if(fg == 3)
    {
        QMessageBox::information(this,"好友","已同意,添加成功");

    }
}

//好友对战按钮
void BrainStorm::on_pushButton_3_clicked()
{
    QString toname = ui->lineEdit->text();
    QJsonObject json;
    json["cmd"] = DULE;
    json["toname"] = toname;
    m_com->writeData(json);
}
//回应是否参加
void BrainStorm::quesDuel(QJsonObject json)
{
    QJsonObject tojson;
    QString name = json["toname"].toString();
    int agss = QMessageBox::question(this,"对战邀请","是否接受？",QMessageBox::Yes,QMessageBox::No);
    if(QMessageBox::Yes == agss)
    {
        std::cout<<"1111111111"<<std::endl;
        tojson["cmd"] = YDULE;
        tojson["myname"] = json["myname"].toString();
        tojson["toname"] = name;
        m_com->writeData(tojson);
    }
    else
    {
        std::cout<<"222222222222"<<std::endl;
//        tojson["cmd"] = ADDFIREND;
//        tojson["FG"] = 3;
//        m_com->writeData(tojson);
    }
}
//通过名字加好友
void BrainStorm::on_pushButton_4_clicked()
{
    QString toname = ui->lineEdit_2->text();
    QJsonObject json;
    json["cmd"] = ADDFIREND;
    json["name"] = toname;
    m_com->writeData(json);
}
