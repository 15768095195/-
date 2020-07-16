#ifndef P1WIDGET_H
#define P1WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QUdpSocket>
#include <QHostAddress>

namespace Ui {
class P1Widget;
}

class P1Widget : public QWidget
{
    Q_OBJECT

public:
    explicit P1Widget(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void dealPressPoint();
    bool isVictory();
    enum chess{ black,white};
    bool isVictory(int,int,chess);
    void iswin();
    void islow();

    enum mode{ alone,online};
    void initchessboard();
    mode mod;
    QString uName;
    QString oName;
    bool way;

    ~P1Widget();
signals:
    win();
public slots:
    void geam();
    void geamStart(int);

private:
    Ui::P1Widget *ui;
    QPoint P_cli;
    int chessboard[14][13];
    int w;
    int h;
    QUdpSocket *readySocket;
    chess who;
    chess onlineWho;

    QHostAddress cliAddr; //对方地址
    quint16 port;//对方端口


    bool go;

};

#endif // P1WIDGET_H
