#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "conaunicate.h"
#include "guanli.h"


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    //图片传输
//    void recvAdv();
    ~Widget();

private slots:
    void on_denlupushButton_clicked();

    void on_zhuchepushButton_clicked();
    //用户登录
    void login(QJsonObject json);
    //管理登录
    void guanli1(QJsonObject json);

    void on_pushButton_clicked();

//    void recvIMG(QJsonObject json);

private:
    Ui::Widget *ui;
    QTcpSocket *fs;
    Conaunicate *m_com; //通信类指针，负责和服务器通信
};

#endif // WIDGET_H
