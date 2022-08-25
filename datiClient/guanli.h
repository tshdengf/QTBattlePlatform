#ifndef GUANLI_H
#define GUANLI_H

#include <QDialog>
#include "conaunicate.h"

#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QDataStream>
#include <QImage>
#include <QBuffer>
#include <QFile>
#include <QTimer>
#include <stdio.h>
#include <QMessageBox>

namespace Ui {
class guanli;
}

class guanli : public QDialog
{
    Q_OBJECT

public:
    explicit guanli(Conaunicate *com,QJsonObject &josn,QWidget *parent = 0);
    ~guanli();

private slots:
    //刷新在线用户消息
    void on_pushButton_4_clicked();
    //获取数据
    void getTable(QJsonObject json);
    //确认更新广告
    void on_pushButton_3_clicked();
    //打开要发送的图片文件
    void openFile();
    //开始发送文件数据
    void sendPhot();
    //图片转base64字符串
    QByteArray getImageData(const QImage &);
    void on_openPhotButton_clicked();

    void sendData(QJsonObject json);

    void on_pushButton_5_clicked();

private:
    Ui::guanli *ui;
    Conaunicate *m_com;

    //-------------文件操作--------
    QString m_fileName;        //文件名
    QString m_ImageName;       //图片名
    int m_fileSize;            //文件大小
    QFile m_file;              //文件类

    QByteArray outBlock;       //数据缓冲区，存放每次要发送的数据块
    QImage m_image;            //图片
    qint64 totalBytes;         //发送数据总大小

    QTimer m_timer;            //定时器(防止粘包)
    FILE *m_fp;
    int flag;
};

#endif // GUANLI_H
