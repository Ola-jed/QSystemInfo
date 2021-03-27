// You may need to build the project (run Qt uic code generator) to get "ui_InfoWindow.h" resolved
#include "InfoWindow.hpp"
#include "ui_InfoWindow.h"

InfoWindow::InfoWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::InfoWindow)
{
    ui->setupUi(this);
    setLayout();
    buildTabs();
    loadAllInfo();
    makeConnections();
}

InfoWindow::~InfoWindow()
{
    delete ui;
}

// Window resizing and position
void InfoWindow::setLayout()
{
    setFixedSize(900,600);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,size(),QGuiApplication::primaryScreen()->availableGeometry()));
}

// Creating all the tabs
void InfoWindow::buildTabs()
{
    ui->tabWidget->addTab(new QTextEdit(),QIcon(":assets/system.ico"),tr("System"));
    ui->tabWidget->addTab(new QTextEdit(),QIcon(":assets/disk.ico"),tr("Disk"));
    ui->tabWidget->addTab(new QTextEdit(),QIcon(":assets/cpu.ico"),tr("Cpu"));
    ui->tabWidget->addTab(new QTextEdit(),QIcon(":assets/network.ico"),tr("Network"));
    #ifdef Q_OS_WIN // If we are on windows, we can call bios information with regedit
        ui->tabWidget->addTab(new QLabel(),QIcon(":assets/bios.ico"),tr("Bios"));
    #endif
    for(auto i = 0;i < ui->tabWidget->count();i++)
    {
        getTextEditAt(i)->setReadOnly(true);
    }
}

void InfoWindow::makeConnections()
{
    connect(ui->saveInFile,&QAction::triggered,this,&InfoWindow::saveInfoIntoFile);
    connect(ui->aboutQSystemInfo,&QAction::triggered,this,&InfoWindow::onAbout);
    connect(ui->help,&QAction::triggered,this,&InfoWindow::onAbout);
}

// Loading all the information
void InfoWindow::loadSystemInfo()
{
    getTextEditAt(SYSTEM_INDEX_IN_TAB)->setText(SysInfo::getSystemInformationAsStr());
}

void InfoWindow::loadDiskInfo()
{
    getTextEditAt(DISK_INDEX_IN_TAB)->setText(SysInfo::getDiskInformationAsStr());
}

void InfoWindow::loadCpuInfo()
{
    getTextEditAt(CPU_INDEX_IN_TAB)->setText(SysInfo::getCpuAndGpuInfoAsStr());
}

void InfoWindow::loadNetworkInfo()
{
    getTextEditAt(NETWORK_INDEX_IN_TAB)->setText(SysInfo::getNetworkInformationAsStr());
}

void InfoWindow::loadBiosInfo()
{
    #ifdef Q_OS_WIN // If we are on windows, we need this value
        getTextEditAt(BIOS_INDEX_IN_TAB)->setText(SysInfo::getBiosInfoAsStr());
    #endif
}

void InfoWindow::loadAllInfo()
{
    loadSystemInfo();
    loadDiskInfo();
    loadCpuInfo();
    loadNetworkInfo();
    #ifdef Q_OS_WIN // If we are on windows, we can load this function
        loadBiosInfo();
    #endif
}

// Get the item at the given index
QTextEdit *InfoWindow::getTextEditAt(const int index)
{
    return qobject_cast<QTextEdit*>(ui->tabWidget->widget(index));
}

// Save all system information into a file
void InfoWindow::saveInfoIntoFile()
{
    auto const filename {QFileDialog::getSaveFileName(this)};
    if(filename.isEmpty())
    {
        QMessageBox::warning(this,"Save system info","Enter a valid name");
    }
    else
    {
        QFile fileToSave{filename};
        if((!fileToSave.open(QIODevice::ReadWrite)))
        {
            QMessageBox::critical(this,"Save system info","Could not save");
            return;
        }
        else
        {
            QTextStream out{&fileToSave};
            for(auto i = 0;i < ui->tabWidget->count();i++)
            {
                out << getTextEditAt(i)->toPlainText();
            }
            QMessageBox::information(this,"Save system info","File saved successfully");
        }
        fileToSave.close();
    }
}

// Show the about-dialog
void InfoWindow::onAbout()
{
    auto dialog = new AboutDialog(this);
    dialog->show();
}
