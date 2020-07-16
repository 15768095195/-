#include "onlineconnect.h"
#include "first.h"
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QMessageBox>
#include <QWidget>
onlineconnect::onlineconnect(QWidget *parent) : QWidget(parent)
{

    setWindowTitle("设置游戏名字和端口号");
//    setWindowFlags(Qt::Dialog|windowFlags());
    //设置为模态对话框
    setWindowModality(Qt::ApplicationModal);
    resize(400, 300);
    nameLable =  new QLabel;
    nameLable->setParent(this);
    nameLineEdit = new QLineEdit;
    nameLineEdit->setParent(this);
    portLable = new QLabel;
    portLable->setParent(this);
    portLineEdit = new QLineEdit;
    portLineEdit->setParent(this);
    suer = new QPushButton;
    suer->setParent(this);
    clo = new QPushButton;
    clo->setParent(this);

    nameLable->setText(QString("游戏名字："));
    portLable->setText(QString("通讯的端口号："));
    portLineEdit->setText(QString("8888"));

    suer->setText(QString("确认"));
    clo->setText(QString("关闭"));


    gLayout = new QGridLayout;

    gLayout->addWidget(nameLable,0,0);
    gLayout->addWidget(nameLineEdit,0,1);
    gLayout->addWidget(portLable,1,0);
    gLayout->addWidget(portLineEdit,1,1);

    gLayout->addWidget(suer,3,0);
    gLayout->addWidget(clo,3,2);

    gLayout->setMargin(10);
    gLayout->setSpacing(5);

    this->setLayout(gLayout);
    connect(suer,SIGNAL(clicked()),this,SLOT(newConnect()));
    connect(suer,SIGNAL(clicked()),this,SLOT(close()));
    connect(clo,SIGNAL(clicked()),this,SLOT(close()));

}

void onlineconnect::dealConnect()
{

}

void onlineconnect::newConnect()
{
//    First::userName = nameLineEdit->text();
//    First::userport = portLineEdit->text().toInt();
}

onlineconnect::~onlineconnect()
{

}

