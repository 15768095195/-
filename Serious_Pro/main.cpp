#include "p1widget.h"
#include <QApplication>
#include "first.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    First f;
    f.show();

    return a.exec();
}
