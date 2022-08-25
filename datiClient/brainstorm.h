#ifndef BRAINSTORM_H
#define BRAINSTORM_H

#include <QDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include "conaunicate.h"
#include <QTimer>
#include <QFileInfo>
#include <QByteArray>
#include <QBitArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMovie>
#include <QMessageBox>

namespace Ui {
class BrainStorm;
}

class BrainStorm : public QDialog
{
    Q_OBJECT

public:
    explicit BrainStorm(Conaunicate *com,QJsonObject &josn,QWidget *parent = 0);
    ~BrainStorm();
private:
    //单人训练
    void singleGetQuestion();
    //单人训练设置问题
    void singleSetQuestion();

private slots:
    void on_singleButton_clicked();

    void on_brankButton_clicked();

    void on_startButton_clicked();
    //单人训练接收数据
    void receiveSingleQuestion(QJsonObject json);
    //定时器
    void singleTimerOut();

    //个人训练答题
    void singleAnswerQuestion(int select);

    //设置得分
    void setEnemyScore();
    void setselfScore();
    //rank设置问题
    void rankSetQuestion();
    //rank答题
    void rankAnser(int a);
    //rank结果
    void rankReslut(QJsonObject json);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_A_clicked();

    void on_pushButton_B_clicked();

    void on_pushButton_C_clicked();

    void on_pushButton_D_clicked();

    void on_rankButton_clicked();

    //排位接收数据
    void receiveRank(QJsonObject json);
    //排位定时器
    void rankTimerOut();

    void on_pushButtonA_clicked();

    void on_pushButtonB_clicked();

    void on_pushButtonC_clicked();

    void on_pushButtonD_clicked();

    void on_rank_brank_clicked();

    void replyFin(QNetworkReply*);

    void on_pushButton_2_clicked();

    void getFirend(QJsonObject json);

    void on_pushButton_5_clicked();

    void addFirend(QJsonObject json);

    void on_pushButton_3_clicked();

    void quesDuel(QJsonObject json);

    void on_pushButton_4_clicked();

private:
    Ui::BrainStorm *ui;
    Conaunicate *m_com;        //通信类指针，负责和服务器通信

    QJsonObject m_singlejson;  //当前问题集合
    int m_curSingleIndex;      //当前问题下标

    QTimer m_singleTimer;      //个人训练定时器
    int m_singleSec;           //个人训练答题时间
    int m_singleScore;         //个人训练得分

    //rank_______________------------
    QJsonObject m_rankQuestion;//rank的题
    QString m_enemyName;       //对手名字
    QString m_enemyRank;       //对手段位
    int m_enemyScore;          //对手的得分

    int m_rankSec;             //rank 答题计时
    int m_myScore;             //自己得分
    QString m_userName;        //用户名称
    QString m_userRank;        //用户段位

    int m_currankIndex;        //当前问题下标
    int m_enemyrankIndex;      //对手问题下标
    QTimer m_rankTimer;        //排位定时器

    QFileInfo fileInfo;        //文件类数据

    QPixmap  pixmap;          //得到的图片
    QString imagefilename;     //图片文件名
};

#endif // BRAINSTORM_H
