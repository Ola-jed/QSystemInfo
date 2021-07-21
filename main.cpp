#include "InfoWindow.hpp"
#include <QApplication>

int main(int argc , char **argv)
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QCoreApplication::setOrganizationName("QSystemInfo");
    QCoreApplication::setApplicationName("QSystemInfo");
    QCoreApplication::setApplicationVersion("1.0");
    InfoWindow e{};
    e.show();
    return QApplication::exec();
}
