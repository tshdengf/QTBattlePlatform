#ifndef CONAUNICATE_H
#define CONAUNICATE_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "../commo.h"
#include <QTimer>
//#include "rejster.h"

class Conaunicate : public QObject
{
    Q_OBJECT
public:
    explicit Conaunicate(QObject *parent = nullptr);

    void writeData(const QJsonObject &json);
    void writeFile(void *buf,int len);
    //准备接收广告
    void recvaddr();
//    void writeDou(QByteArray);
signals:
    void rstResult(int a);
    void login(QJsonObject json);
    void receSingle(QJsonObject json);
    void rank(QJsonObject json);
    void guangli1(QJsonObject json);
    void getTable(QJsonObject json);
    void getFirend(QJsonObject json);
    void addFirend(QJsonObject json);
    void quesDuel(QJsonObject json);
    void sendData(QJsonObject json);
    void recvIMG(QJsonObject json);

public slots:
    void readData();
    void csRead();
    void timeil1();
    void timeil2();


private:
    QTcpSocket s;
    QTcpSocket fs;
    FILE *fp;
    QTimer til;
    int size;

};

#endif // CONAUNICATE_H
