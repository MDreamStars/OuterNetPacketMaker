#include "OuterNetPacketMaker.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OuterNetPacketMaker w;
    w.show();
    return a.exec();
}
