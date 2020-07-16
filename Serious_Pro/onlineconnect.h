#ifndef ONLINECONNECT_H
#define ONLINECONNECT_H

#include <QWidget>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "first.h"
class onlineconnect : public QWidget
{
    Q_OBJECT
public:
    explicit onlineconnect(QWidget *parent = 0);
    ~onlineconnect();

    void dealConnect();
signals:

public slots:
    void newConnect();

private:
    QLabel *nameLable;
    QLineEdit *nameLineEdit;
    QLineEdit *portLineEdit;
    QLabel *portLable;
    QPushButton *suer;
    QPushButton *clo;
    QGridLayout *gLayout;
};

#endif // ONLINECONNECT_H
