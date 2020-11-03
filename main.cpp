#include "trail.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    trail w;
    w.show();

    return a.exec();
}
