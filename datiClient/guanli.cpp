#include "guanli.h"
#include "ui_guanli.h"
#include <QDebug>
#include <iostream>
#include <unistd.h>
guanli::guanli(Conaunicate *com,QJsonObject &sjosn,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::guanli)
{
    ui->setupUi(this);
    m_com = com;
    totalBytes = 0;
    connect(m_com,SIGNAL(getTable(QJsonObject)),this,SLOT(getTable(QJsonObject)));
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(sendData()));
    connect(m_com,SIGNAL(sendData(QJsonObject)),this,SLOT(sendData(QJsonObject)));

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"用户名"<<"当前积分");

    QJsonObject json;
    json["cmd"] = GETTABLE;
    m_com->writeData(json);
}

guanli::~guanli()
{
    delete ui;
}

void guanli::on_pushButton_4_clicked()
{
//    /* 创建表格视图 */
//    QTableView *tableView = new QTableView;

//    /* 设置表格视图大小 */
//    tableView->resize(850, 400);

//    /* 显示 */
//    tableView->show();
    QJsonObject json;
    json["cmd"] = GETTABLE;
    m_com->writeData(json);
}
void guanli::getTable(QJsonObject json)
{
    int row = json["row"].toInt();
    ui->tableWidget->setRowCount(row);
    for(int i=0; i<row; i++)
    {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(json["name"].toArray().at(i).toString()));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(json["rank"].toArray().at(i).toString()));
    }
}

void guanli::on_pushButton_3_clicked()
{
    //第一次尝试json传输文件失败
//    sendPhot();
    //第二次尝试打开文件传输
    //先发送文件头信息，文件名和文件大小
//    QJsonObject json;
//    json["cmd"] = SENDHEAD;
//    json["head"] = m_ImageName;
//    json["filesize"] = m_fileSize;

//    m_com->writeData(json);
//    m_timer.start(20);
    QJsonObject json;
    json["FM"] = 1;
    flag = 0;
    sendData(json);
}
void guanli::sendData(QJsonObject injson)
{
#if 0
    //定时器任务完成，关闭定时器
    m_timer.stop();
    //用QFile传
//    int sendSize = 0;
    int len = m_fileSize;
    std::cout<<"len "<<len<<std::endl;
    unsigned char buff[51];
    QJsonObject json;
    json["cmd"] = IMG;
    json["MF"] = 1;
//    do{
//        char buf[1024*1024] = {0};
//        len = 0;
//        len = m_file.read(buf,m_fileSize);
//        qDebug("%s\n",buf);
//        m_com->writeFile(buf);
//        qDebug("len = %d\n",len);
//        sendSize += len;
//    }while(len>0);//len == 0结束

//    if(sendSize == m_fileSize)
//    {
//        m_file.close();
//    }

    while(len>0)
    {
        memset(buff,0,51);
        int size;
        if(len > 50)
        {
            size = 50;
        }
        else
        {
            size = len;
            json["MF"] = 0;
        }
        json["len"] = size;
        m_file.read((char *)buff,size);
        QJsonArray array;
        for(int i=0; i<size; i++)
        {
            array.push_back(buff[i]);
        }
        json["data"] = array;
        len -= 50;
        m_com->writeData(json);
        usleep(100000);
    }
    m_file.close();
#endif
    //转base64传
//    QImage image(m_fileName);
//    QByteArray imageData = getImageData(image);
//    QString qstr = QString::fromLatin1(imageData, imageData.size());

//    m_com->writeFile(imageData);

    //用FILE文件系统传
//    FILE *fp;
//    fp = fopen(m_fileName.toStdString().c_str(),"rb");
//    int len = 0;
//    int temp;
//    char buf[1024];
//    while((temp = fread(buf,1,sizeof(buf),fp)) > 0)
//    {
//        len+= temp;
//        printf("len = %d",len);
//    }
//    fclose(fp);
//    fp = fopen(m_fileName.toStdString().c_str(),"rb");
//    std::cout<<len<<std::endl;
//    temp = fread(buf,1,len,fp);
//    std::cout<<temp<<std::endl;
//    while ((tmp = fread(buf, 1, sizeof(buf), fp)) > 0)
//    {
//        m_com->writeFile(buf,len);
//        std::cout<<buf<<std::endl;
//    }
//    FILE *fp1 = fopen("oo.txt","wb");
//    fwrite(buf,1,len,fp1);


    //xjy版本
    // 仅限打开.jpg格式图片
//           QString filename = QFileDialog::getOpenFileName(this, tr("打开文件"), QCoreApplication::applicationFilePath(), "*");
//           FILE *fp;

//           fp = fopen(m_fileName.toStdString().c_str(),"rb");
//           if (fp == NULL)
//           {
//               QMessageBox::critical(this, "提示", "打开文件失败");
//               return;
//           }
//           else
//           {
//               QMessageBox::information(this, "提示", "正在上传文件");
//           }
//           // 先向服务器发送即将上传广告的提示

//           // 开始传送文件
//           // 先发送图片的大小 int
//           int len = 0;
//           int tmp;
//           char buf[1024];
//           // 计算图片的大小
//           while ((tmp = fread(buf, 1, sizeof(buf), fp)) > 0)// 计算文件大小
//           {
//               len += tmp;
//           }
//           // 传送图片大小
//   #if 1
//           m_com->writeFile(&len, sizeof(len));
//   #endif
//           printf("len = %d\n", len);
//           //  将文件指针便宜到文件开头
//           fclose(fp);
//           fp = fopen(m_fileName.toStdString().c_str(), "rb");
//   #if 1
//           // 开始传送图片
//           while ((tmp = fread(buf, 1, sizeof(buf), fp)) > 0)
//           {
//               m_com->writeFile(buf, tmp);
//           }
//   #endif
//           // test
//   //        FILE *test = fopen("./test.jpg", "wb");
//   //        while ((tmp = fread(buf, 1, sizeof(buf), fp)) > 0)
//   //        {
//   //            fwrite(buf, 1, tmp, test);
//   //        }
//   //        fclose(test);

//           // 发送完毕
//           // 关闭文件描述符
//           fclose(fp);
#if 0
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle(QStringLiteral("选中文件"));
    //设置默认文件路径
    fileDialog->setDirectory(".");
    //设置文件过滤器
    fileDialog->setNameFilter(tr("File(*.jpg)"));
    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    QStringList fileNames;
    fileDialog->exec();
    fileNames = fileDialog->selectedFiles();
    QFile file(fileNames[0]);
//    QFile file("/home/zt/tmp/2.jpg");
    if( file.open(QIODevice::ReadOnly))
    {
        qInfo() << "can not open file";
    }
#endif

#if 0
    QFileInfo info(m_file);
    int length = info.size();//以字节为单位
    //发送信息
    QJsonObject json;
    json["cmd"] = IMG;
    json["BF"] = 1;//表明是开头
    json["MF"] = 1;//后续还有文件块
    int fm = injson["FM"].toInt();
    if(fm == 0)
    {
       m_file.close();
       QMessageBox::information(this,"文件传输","传输成功");
       return;
    }
    unsigned char buff[FILE_BLOCK_MAX+1];
//    while(length > 0)
    {
        memset(buff,0,FILE_BLOCK_MAX);
        int size;
        if(length > FILE_BLOCK_MAX)
            size = FILE_BLOCK_MAX;
        else
        {
            std::cout<<"传完了，最后一块了，改接收了....."<<std::endl;
            size = length;
            json["MF"] = 0;//后续没有文件块了
        }
        json["len"] = size;
        m_file.read((char*)buff,size);
        QJsonArray array;
        for(int i=0; i< size; i++)
        {
            array.push_back(buff[i]);
            std::cout<<"zhifu: "<<buff[i]<<std::endl;
        }
        json["data"] = array;
        length-=FILE_BLOCK_MAX;
        m_com->writeData(json);
        json["BF"] = 0;//表明不是开头
        usleep(10000000);
    }



#endif

#if 1
    //再次尝试
    QJsonObject json;
    json["cmd"] = IMG;
    if(flag == 0)
    {
        json["BF"] = 1;//表明是开头
    }
    else
    {
        json["BF"] = 0;//表明不是开头
    }
    json["MF"] = 1;//后续还有文件块
    int fm = injson["FM"].toInt();
    if(fm == 0)
    {
//       m_file.close();
       QMessageBox::information(this,"文件传输","传输成功");
       flag = 0;
       fclose(m_fp);
       return;
    }
    unsigned char buff[FILE_BLOCK_MAX+1];
    memset(buff,0,FILE_BLOCK_MAX);
    int size;
    if(m_fileSize > FILE_BLOCK_MAX)
        size = FILE_BLOCK_MAX;
    else
    {
        std::cout<<"传完了，最后一块了，改接收了...111111111111111111111111111111111111111111111.."<<std::endl;
        size = m_fileSize;
        json["MF"] = 0;//后续没有文件块了
    }
    json["len"] = size;
    fread((char*)buff,1,size,m_fp);
    QJsonArray array;
    for(int i=0; i< size; i++)
    {
        array.push_back(buff[i]);
        std::cout<<"zhifu: "<<buff[i]<<std::endl;
    }
    json["data"] = array;
    m_fileSize-=FILE_BLOCK_MAX;
    m_com->writeData(json);

    flag = 1;
 #endif
}

void guanli::openFile()
{

    m_fileName = QFileDialog::getOpenFileName(this,tr("选择文件"),"/home",tr("image(*.jpg);;TXT(*.txt);;(*.png)"));
    if(!m_fileName.isEmpty())
    {
//        m_ImageName = m_fileName.right(m_fileName.size()-m_fileName.lastIndexOf('/')-1);
        m_ImageName.clear();
        m_fileSize = 0;

        QFileInfo info(m_fileName);
        m_ImageName = info.fileName();
        m_fileSize = info.size();//获得文件长度


//        //打开QFile
//        m_file.setFileName(m_fileName);

//        bool isOK = m_file.open(QIODevice::ReadOnly);
//        if(!isOK)
//        {
//            qDebug()<<"只读方式打开失败";
//        }



        //打开open

        m_fp = fopen(m_fileName.toStdString().c_str(),"rb");

    }
    QImage image(m_fileName);
    ui->lineEdit->setText(m_fileName);
    ui->label->setPixmap(QPixmap::fromImage(image));

}

void guanli::on_openPhotButton_clicked()
{
    openFile();
}
void guanli::sendPhot()
{
    //将二进制文件写入流
//    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
//    sendOut.setVersion(QDataStream::Qt_5_6);

//    QFile file(m_fileName);
//    file.open(QIODevice::ReadOnly);
//    QByteArray arr = file.readAll();
//    file.close();
//    char *p = arr.data();

//    //获得图片数据
//    QImage image(m_fileName);
//    QByteArray imageData = getImageData(image);

//    ui->label->setPixmap(QPixmap::fromImage(image));
//    QString str = "http://www.baidu.com/img/bdlogo.png";
    QJsonObject json;
    json["cmd"] = SIMAGE;
    json["image"] = m_fileName;
//    json.insert("image",QJsonValue::fromVariant(imageData));

    m_com->writeData(json);
}

//png/jpg编码为base64数据
QByteArray guanli::getImageData(const QImage &image)
{
    QByteArray imagData;
    QBuffer buffer(&imagData);

    image.save(&buffer,"PNG");
    imagData = imagData.toBase64();

    return imagData;

//    QByteArray byte_array;
//    QDataStream data_stream(&byte_array,QIODevice::WriteOnly);
//    data_stream<<pixmap;
//    QString res = QString::fromLocal8Bit(byte_array.toBase64());
//    return res;
}

void guanli::on_pushButton_5_clicked()
{
    sendPhot();
}
