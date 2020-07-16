#ifndef FIRST_H
#define FIRST_H

#include <QWidget>
#include "p1widget.h"
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include <QTimer>
#include <QUdpSocket>
namespace Ui {
class First;
}

class First : public QWidget
{
    Q_OBJECT

public:
    explicit First(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);
    void dealConnect();
    QString getUsername();
    quint16 getuserport();
    QString setUsername(QString);
    quint16 setuserport(quint16);

    ~First();

signals:
    void setConnect();
    void isOK(int);


private slots:
    void on_aloneButton_clicked();
    void on_onlineButton_clicked();
    void newConnect();
    void timeout();
    void getMsg();
    void getStart();
//    void geamStart(int);
    void geam();

private:
    Ui::First *ui;
    QLineEdit *nameLineEdit;
    QLineEdit *portLineEdit;
    quint16 userport;
    QString mode;
    P1Widget P1;
    QTimer *time;
    QUdpSocket *udpSocket;
    QUdpSocket *newSocket;
    bool OK;

public:
    QHostAddress cliAddr; //对方地址
    quint16 port;//对方端口
    QString userName;
    QString otherName;
};

#endif // FIRST_H
