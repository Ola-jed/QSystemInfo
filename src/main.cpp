#include "InfoWindow.hpp"
#include <QApplication>

int main(int argc , char **argv)
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("QSystemInfo");
    QCoreApplication::setApplicationName("QSystemInfo");
    QCoreApplication::setApplicationVersion("1.0");
    InfoWindow e{};
    e.show();
    return QApplication::exec();
}
