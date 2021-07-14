#ifndef QCOMINFO_INFOWINDOW_HPP
#define QCOMINFO_INFOWINDOW_HPP

#include "sysinfo.hpp"
#include "AboutDialog.hpp"
#include <QFile>
#include <QStyle>
#include <QLabel>
#include <QScreen>
#include <QTextEdit>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QGuiApplication>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class InfoWindow;
}
QT_END_NAMESPACE

class InfoWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit InfoWindow(QWidget *parent = nullptr);
        ~InfoWindow() override;

    private:
        Ui::InfoWindow *ui;
        void setLayout();
        void buildTabs();
        QTextEdit* getTextEditAt(int index);

        void loadAllInfo();
        void loadSystemInfo();
        void loadDiskInfo();
        void loadCpuInfo();
        void loadNetworkInfo();
        void loadBiosInfo();
        void makeConnections();

        // Constants
        static constexpr int SYSTEM_INDEX_IN_TAB{0};
        static constexpr int DISK_INDEX_IN_TAB{1};
        static constexpr int CPU_INDEX_IN_TAB{2};
        static constexpr int NETWORK_INDEX_IN_TAB{3};
        #ifdef Q_OS_WIN // If we are on windows, we need this value
            static constexpr int BIOS_INDEX_IN_TAB{4};
        #endif

    private slots:
        void saveInfoIntoFile();
        void onAbout();
};

#endif //QCOMINFO_INFOWINDOW_HPP
