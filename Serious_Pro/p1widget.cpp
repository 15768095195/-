#include "p1widget.h"
#include "ui_p1widget.h"
#include "first.h"
#include <QPoint>
#include <QBrush>
#include <QPen>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QWidget>

P1Widget::P1Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::P1Widget)
{
    ui->setupUi(this);
    qDebug() << "进入";
    who = black;
    initchessboard();
    setWindowTitle("五子棋");
    go = true;
    qDebug() << 15;
    setGeometry(300,100,1000,800);
}

void P1Widget::geamStart(int k)
{
    qDebug() << "进入geam";
    if(k == 0 )
    {
        onlineWho = black;
        way = true;
    }
    else if(k == 1)
    {
        onlineWho = white;
        way = false;
    }

    readySocket = new QUdpSocket(this);
    readySocket->bind(QHostAddress::AnyIPv4,8888);
    qDebug() << 49;
    connect(readySocket,SIGNAL(readyRead()),this,SLOT(geam()));
    qDebug() << 51;
    go =false;
    update();

}

void P1Widget::geam()
{
    char buf[1024] = {0};
    int x = -1,y = -1;
    qint64 len = readySocket->readDatagram(buf,sizeof(buf));
    if(len > 0 && (QString(buf).section("##",0,0) == uName) && !way)
    {
          x = QString(buf).section("##",1,1).toInt();
          y = QString(buf).section("##",2,2).toInt();
          if(chessboard[x][y] == -1)
          {
              chess ch;
              if(onlineWho == black) ch = white;
              else ch = black;
              chessboard[x][y] = ch;
              update();
              way = (!way);
              if(isVictory(x,y,ch))
                  islow();
          }

    }
}

void P1Widget::initchessboard()
{
    for(int i=0;i<14;i++)
    {
        for(int j=0;j<13;j++)
        {
            chessboard[i][j]=-1;
        }
    }
}

void P1Widget::paintEvent(QPaintEvent *)
{
    QPainter Qpain(this);
    QPixmap Qpix(":/Image/board1.jpg");

    Qpain.drawPixmap(0,0,frameGeometry().width(),frameGeometry().height(),Qpix);

    w = frameGeometry().width()/14;
    h = frameGeometry().height()/14;

    QPoint x(w/2,h/2);
    QPoint y(w/2+13*w,h/2);
    Qpain.setPen(QPen(Qt::SolidPattern,5,Qt::SolidLine));
    for(int i=0;i<13;i++)
    {
        Qpain.drawLine(x,y);
        x.ry()+=h;
        y.ry()+=h;
    }
    x.setX(w/2);     x.setY(h/2);
    y.setX(w/2);     y.setY(h/2+h*12);
    for(int i=0;i<14;i++)
    {
        Qpain.drawLine(x,y);
        x.rx()+=w;
        y.rx()+=w;
    }

    for(int i=0;i<14;i++)
    {
        for(int j=0;j<13;j++)
        {
            if(chessboard[i][j]==black)
            {
                Qpain.drawPixmap(w/4+i*w,h/4+j*h,w/2,h/2,QPixmap(":/Image/black.png"));
            }
            else if(chessboard[i][j]==white)
            {
                Qpain.drawPixmap(w/4+i*w,h/4+j*h,w/2,h/2,QPixmap(":/Image/white.png"));
            }
        }
    }
}

void P1Widget::mousePressEvent(QMouseEvent *e)
{
    P_cli=QPoint(e->globalPos()-frameGeometry().topLeft());

    if(mod == alone)//单人模式
    {
        qDebug() << "alone";
        if(e->button()==Qt::LeftButton)
        {
            if(who == black &&chessboard[(P_cli.rx()-w/2)/w][(P_cli.ry()-h/2)/h] == -1)
            {
                chessboard[(P_cli.rx()-w/2)/w][(P_cli.ry()-h/2)/h] = black;
                qDebug() << e->globalPos().rx() << ":" << e->globalPos().ry();
                if(isVictory())
                {
                    iswin();
                }
                who = white;
             }

            else if(who == white && chessboard[(P_cli.rx()-w/2)/w][(P_cli.ry()-h/2)/h] == -1)
            {
                chessboard[(P_cli.rx()-w/2)/w][(P_cli.ry()-h/2)/h] = white;
                if(isVictory())
                {
                    iswin();
                }
                who = black;
            }
            update();
         }
        else if(e->button()==Qt::RightButton)//悔棋
        {
            P_cli=QPoint(e->globalPos()-frameGeometry().topLeft());

            chessboard[(P_cli.rx()-w/2)/w][(P_cli.ry()-h/2)/h]=-1;
            update();
        }
    }
    else if (mod == online)//联机
    {
        qDebug() << "go";

        qDebug() << "online";
        if(e->button()==Qt::LeftButton)
        {
            if(way&&chessboard[(P_cli.rx()-w/2)/w][(P_cli.ry()-h/2)/h] == -1)
            {
                int x,y;
                x = (P_cli.rx()-w/2)/w;
                y = (P_cli.ry()-h/2)/h;
                chessboard[x][y] = onlineWho;
                qDebug() << e->globalPos().rx() << ":" << e->globalPos().ry();
                if(isVictory())
                {
                    iswin();
                }
                qDebug() << "hahah";
                QString str = oName + "##" + x + "##" + y;
                qDebug() << str;
                if(readySocket->writeDatagram(str.toUtf8(),QHostAddress::Broadcast,8888)!=str.length())
                {   return ;    }
                qDebug() << "shinile";
                way = (!way);
                update();
            }
         }
    }

}

bool P1Widget::isVictory()
{
    int x=(P_cli.rx()-w/2)/w;
    int y=(P_cli.ry()-h/2)/h;
    //判断横
    int k=0;
    int x1=x;
    int y1=y;
    chess now =who;
    while(x1>=0)
    {
        if(chessboard[x1][y1]==now)
        {
            qDebug() << QString("(%1,%2)").arg(x1).arg(y1);
            x1--;
        }
        else
        {
            break;
        }
    }
    x1++;
    while(x1>=0&&y1>=0&&x1<=13&&y1<=12)
    {
        if(chessboard[x1][y1]==now)
        {
            k++;
            qDebug() << "k=" << k;
        }
        else break;
        x1++;
    }
    if(k>=5) return true;

    //判断竖
    x1=x;
    y1=y;
    k=0;
    while(x1>=0&&y1>=0&&x1<=13&&y1<=12)
    {
        if(chessboard[x1][y1]==now)
        {
            y1--;
        }
        else
        {
            break;
        }
    }
    y1++;
    while(x1>=0&&y1>=0&&x1<=13&&y1<=12)
    {
        if(chessboard[x1][y1]==now)
        {   k++;    }
        else break;
        y1++;
    }
    if(k>=5) return true;

    //判断斜
    x1=x;
    y1=y;
    k=0;
    while(x1>=0&&y1>=0&&x1<=13&&y1<=12)
    {
        if(chessboard[x1][y1]==now)
        {
            y1--;
            x1--;
        }
        else
        {
            break;
        }
    }
    x1++;
    y1++;
    while(x1>=0&&y1>=0&&x1<=13&&y1<=12)
    {
        if(chessboard[x1][y1]==now)
        {   k++;    }
        else break;
        y1++;
        x1++;
    }
    if(k>=5) return true;

    //判断反斜
    x1=x;
    y1=y;
    k=0;
    while(x1>=0&&y1>=0&&x1<=13&&y1<=12)
    {
        if(chessboard[x1][y1]==now)
        {
            y1++;
            x1--;
        }
        else
        {
            break;
        }
    }
    x1++;
    y1--;
    while(x1>=0&&y1>=0&&x1<=13&&y1<=12)
    {
        if(chessboard[x1][y1]==now)
        {   k++;    }
        else break;
        y1--;
        x1++;
    }
    if(k>=5) return true;

    return  false;
}



bool P1Widget::isVictory(int x,int y,chess th)
{
    //判断横
    int k=0;
    int x1=x;
    int y1=y;
    chess now =th;
    while(x1>=0)
    {
        if(chessboard[x1][y1]==now)
        {
            qDebug() << QString("(%1,%2)").arg(x1).arg(y1);
            x1--;
        }
        else
        {
            break;
        }
    }
    x1++;
    while(x1>=0&&y1>=0&&x1<=13&&y1<=12)
    {
        if(chessboard[x1][y1]==now)
        {
            k++;
            qDebug() << "k=" << k;
        }
        else break;
        x1++;
    }
    if(k>=5) return true;

    //判断竖
    x1=x;
    y1=y;
    k=0;
    while(x1>=0&&y1>=0&&x1<=13&&y1<=12)
    {
        if(chessboard[x1][y1]==now)
        {
            y1--;
        }
        else
        {
            break;
        }
    }
    y1++;
    while(x1>=0&&y1>=0&&x1<=13&&y1<=12)
    {
        if(chessboard[x1][y1]==now)
        {   k++;    }
        else break;
        y1++;
    }
    if(k>=5) return true;

    //判断斜
    x1=x;
    y1=y;
    k=0;
    while(x1>=0&&y1>=0&&x1<=13&&y1<=12)
    {
        if(chessboard[x1][y1]==now)
        {
            y1--;
            x1--;
        }
        else
        {
            break;
        }
    }
    x1++;
    y1++;
    while(x1>=0&&y1>=0&&x1<=13&&y1<=12)
    {
        if(chessboard[x1][y1]==now)
        {   k++;    }
        else break;
        y1++;
        x1++;
    }
    if(k>=5) return true;

    //判断反斜
    x1=x;
    y1=y;
    k=0;
    while(x1>=0&&y1>=0&&x1<=13&&y1<=12)
    {
        if(chessboard[x1][y1]==now)
        {
            y1++;
            x1--;
        }
        else
        {
            break;
        }
    }
    x1++;
    y1--;
    while(x1>=0&&y1>=0&&x1<=13&&y1<=12)
    {
        if(chessboard[x1][y1]==now)
        {   k++;    }
        else break;
        y1--;
        x1++;
    }
    if(k>=5) return true;

    return  false;
}


void P1Widget::iswin()
{
    QMessageBox::information(this,tr("提示"),tr("恭喜你，你赢了！！"));

    switch (QMessageBox::question(this,tr("提示"),tr("是否再来一盘？"),
                                  QMessageBox::Ok , QMessageBox::Cancel)) {
    case QMessageBox::Ok:
        initchessboard();

        break;
    case QMessageBox::Cancel:
        close();
        break;
    default:
        break;
    }
}

void P1Widget::islow()
{
    QMessageBox::information(this,tr("提示"),tr("很遗憾，你输了！！"));

    switch (QMessageBox::question(this,tr("提示"),tr("是否再来一盘？"),
                                  QMessageBox::Ok , QMessageBox::Cancel)) {
    case QMessageBox::Ok:
        initchessboard();

        break;
    case QMessageBox::Cancel:
        readySocket->disconnect();
        readySocket->close();
        close();
        break;
    default:
        break;
    }
}


P1Widget::~P1Widget()
{
    delete ui;
}
