#include "first.h"
#include "ui_first.h"
#include "p1widget.h"
#include <QDesktopWidget>
#include <QRect>
#include <QPainter>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QMessageBox>
#include <QWidget>
#include <QDebug>
#include <QStringList>
#include <QInputDialog>
#include <QTimer>
#include <QUdpSocket>
#include <QHostAddress>


First::First(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::First)
{
    ui->setupUi(this);


    OK = false;
    setWindowTitle("五子棋");

    //获取屏幕大小，并将窗口放置中央
    QDesktopWidget w;
    QRect rec =w.availableGeometry();
    int x = (rec.width()-300)/2;
    int y = (rec.height()-400)/2;
    setGeometry(x,y,300,400);
    connect(this,SIGNAL(isOK(int)),&P1,SLOT(geamStart(int)));
}


void First::geam()
{
 //   qint64 len = udpSocket->readDatagram(buf,sizeof(buf),&cliAddr,&port);
}


void First::getMsg()
{
    char buf[1024] = {0};
    time->stop();
    qDebug() << "getmsg";
    qint64 len = udpSocket->readDatagram(buf,sizeof(buf),&cliAddr,&port);
    if(len > 0)
    {
        qDebug() << buf;
        if(QString(buf).section("##",0,0)==userName)
        {
            OK = true;
            otherName = QString(buf).section("##",1,1);
            udpSocket->disconnect(this);
            udpSocket->close();
            this->hide();
//            P1.way = true;//当用户使用创建房间进入游戏时
            P1.uName = userName;
            P1.oName = otherName;
            P1.show();
            emit isOK(0);
        }
    }

}

void First::timeout()
{

    QString str = userName;
    qDebug() << "timeout" << userName << 3333;
    if(str=="")
    {   return ;    }

    if((udpSocket->writeDatagram(str.toUtf8(),QHostAddress::Broadcast,3333))!=str.length())
    {   return ;    }

}


void First::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0,0,frameGeometry().width(),frameGeometry().height(),QPixmap(":/Image/board4.jpg"));
}

First::~First()
{
    delete ui;
}

void First::on_aloneButton_clicked()
{
    this->hide();
    qDebug() << "开始" << P1Widget::alone;
    P1.mod = P1Widget::alone;
    P1.show();
}

void First::on_onlineButton_clicked()
{
    P1.mod = P1Widget::online;
    dealConnect();
}

void First::dealConnect()
{
    QDialog d;
    d.setWindowTitle("设置游戏名字和端口号");
    d.resize(400, 300);
    QLabel nameLabel;
    nameLabel.setParent(&d);
    nameLineEdit = new QLineEdit;
    nameLineEdit->setParent(&d);
    QLabel portLable;
    portLable.setParent(&d);
    portLineEdit = new QLineEdit;
    portLineEdit->setParent(&d);
    QPushButton suer;
    suer.setParent(&d);
    QPushButton clo;
    clo.setParent(&d);

    nameLabel.setText(QString("游戏名字："));
    portLable.setText(QString("通讯的端口号："));
    portLineEdit->setText(QString("8888"));

    suer.setText(QString("确认"));
    clo.setText(QString("关闭"));


    QGridLayout gLayout;

    gLayout.addWidget(&nameLabel,0,0);
    gLayout.addWidget(nameLineEdit,0,1);
    gLayout.addWidget(&portLable,1,0);
    gLayout.addWidget(portLineEdit,1,1);

    gLayout.addWidget(&suer,3,0);
    gLayout.addWidget(&clo,3,2);

    gLayout.setMargin(10);
    gLayout.setSpacing(5);

    d.setLayout(&gLayout);

    connect(&suer,&QPushButton::clicked,this,&First::newConnect);
    connect(&suer,SIGNAL(clicked()),&d,SLOT(close()));
    connect(&clo,SIGNAL(clicked()),&d,SLOT(close()));
    d.exec();
}
void First::newConnect()
{
    userName = nameLineEdit->text();
    userport = portLineEdit->text().toInt();
    qDebug() << userName << userport;
    QStringList list;
    list << tr("创建房间") << tr("加入房间");
    bool ok;
    QString item = QInputDialog::getItem(this,tr("请选择"),
                   tr("请选择方式："),list,0,false,&ok);

    if(ok && !item.isEmpty() )
    {
        mode = item;
        qDebug() << mode;
        udpSocket = new QUdpSocket(this);

        time = new QTimer;
        if(mode == QString("创建房间"))
        {
            qDebug() << mode << 123;
            udpSocket->bind(userport);
            newSocket = new QUdpSocket(this);
            newSocket->bind(userport);

            connect(udpSocket,SIGNAL(readyRead()),this,SLOT(getMsg()));

            connect(time,SIGNAL(timeout()),this,SLOT(timeout()));
            time->start(1000);
        }
        else if(mode == QString("加入房间"))
        {
            qDebug() << mode << 123;
            udpSocket->bind(3333);
            connect(udpSocket,SIGNAL(readyRead()),this,SLOT(getStart()));
        }
    }
}

void First::getStart()
{
    char buf[1024] = {0};
    qDebug() << "getStart";
    qint64 len = udpSocket->readDatagram(buf,1024,&cliAddr,&port);
    if(len > 0)
    {
          otherName = QString(buf);
    }

    QString str = otherName + "##" + userName;
    newSocket = new QUdpSocket(this);
    newSocket->bind(userport);
    if((newSocket->writeDatagram(str.toUtf8(),QHostAddress::Broadcast,userport))!=str.length())
    {   return ;    }
    OK = true;
    this->hide();

    P1.show();
//    P1.way = false;// 加入房间方式
    P1.uName = userName;
    P1.oName = otherName;
    emit isOK(1);
    udpSocket->disconnect(this);
    udpSocket->close();
}

quint16 First::getuserport()
{
    return this->userport;
}











