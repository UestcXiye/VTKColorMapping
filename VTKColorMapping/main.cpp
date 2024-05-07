#include "VTKColorMapping.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VTKColorMapping w;
    w.show();
    return a.exec();
}
