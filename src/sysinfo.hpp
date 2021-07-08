#ifndef QCOMINFO_SYSINFO_HPP
#define QCOMINFO_SYSINFO_HPP

#include "qglobal.h"
#include <QMap>
#include <QList>
#include <QString>
#include <QProcess>
#include <QSysInfo>
#include <QSettings>
#include <QStorageInfo>
#include <QNetworkInterface>

namespace SysInfo
{
    // Constants
    const qint64 GIGABYTES_IN_BYTES{1000000000};
    const QString CPU_NAME{"wmic cpu get name"};
    const QString GPU_NAME{"wmic PATH Win32_videocontroller get VideoProcessor "};
    const QString VRAM{"wmic PATH Win32_VideoController get AdapterRAM"};
    const QString LINUX_CPU_NAME {"cat /proc/cpuinfo | grep 'model name' | uniq"};
    const QString BIOS_KEY{R"(HKEY_LOCAL_MACHINE\HARDWARE\DESCRIPTION\System\BIOS)"};

    // Get information about system, kernel
    inline QMap<QString,QString> getSystemInformation()
    {
        QMap<QString,QString> systemInfo{};
        systemInfo.insert("CPU Architecture",QSysInfo::currentCpuArchitecture());
        systemInfo.insert("Product type",QSysInfo::prettyProductName());
        systemInfo.insert("Kernel type",QSysInfo::kernelType());
        systemInfo.insert("Kernel version",QSysInfo::kernelVersion());
        systemInfo.insert("Machine id",QSysInfo::machineUniqueId());
        return systemInfo;
    }

    // Call system informations and format in a string
    inline QString getSystemInformationAsStr()
    {
        QString infoStrBuilder{""};
        auto const t {SysInfo::getSystemInformation()};
        for(auto const &temp : t)
        {
            infoStrBuilder += t.key(temp) + " : " + temp +"\n\n";
        }
        return infoStrBuilder;
    }

    // Get all the mounted volumes number
    inline int volumesNumber()
    {
        int size = 0;
        auto const mountedVolumes{QStorageInfo::mountedVolumes()};
        std::for_each(std::begin(mountedVolumes),std::end(mountedVolumes),[&size](const QStorageInfo& tmpVolume){
            if(tmpVolume.isReady() && tmpVolume.isValid())
            {
                size++;
            }
        });
        return size;
    }

    // Get information about all the mounted volumes
    inline QList<QMap<QString,QString>> getDiskInformation()
    {
        QList<QMap<QString,QString>> diskInfo{};
        auto const mountedVolumes{QStorageInfo::mountedVolumes()};
        foreach (const QStorageInfo &storage, mountedVolumes)
        {
            if (storage.isValid()  && storage.isReady())
            {
                QMap<QString,QString> tempMap{};
                if(!storage.name().isEmpty())
                {
                    tempMap.insert({{"Name",storage.name()}});
                }
                tempMap.insert({{"Root",storage.rootPath()}});
                tempMap.insert({{"File system",storage.fileSystemType()}});
                tempMap.insert({{"Space available",QString::number(storage.bytesAvailable()/GIGABYTES_IN_BYTES)+" GB"}});
                tempMap.insert({{"Total space",QString::number(storage.bytesTotal()/GIGABYTES_IN_BYTES)+" GB"}});
                tempMap.insert({{"Readonly",storage.isReadOnly() ? "Yes" : "No"}});
                diskInfo.push_back(tempMap);
            }
        }
        return diskInfo;
    }

    // Call volumes informations and append in a string
    inline QString getDiskInformationAsStr()
    {
        QString diskInfoStr{QString::number(volumesNumber())+" disks\n\n"};
        auto const v {SysInfo::getDiskInformation()};
        for(const auto &t : v)
        {
            for(auto const &tempValue : t)
            {
                diskInfoStr.append(t.key(tempValue) + " : " + tempValue + "\n");
            }
            diskInfoStr.append("\n\n");
        }
        return diskInfoStr;
    }

    // Get the information about network interfaces
    inline QList<QMap<QString,QString>> getNetworkInformation()
    {
        QList<QMap<QString,QString>> networkInfo{};
        foreach(const QNetworkInterface &networkInterface, QNetworkInterface::allInterfaces())
        {
            if (networkInterface.flags().testFlag(QNetworkInterface::IsUp))
            {
                for(const auto &entry: networkInterface.addressEntries())
                {
                    if ( entry.ip().toString().contains("."))
                    {
                        const QMap<QString,QString> tempMap{{"Interface",networkInterface.name()},
                                                     {"IP",entry.ip().toString()},
                                                     {"MAC",networkInterface.hardwareAddress().toLocal8Bit().constData()}};
                        networkInfo.push_back(tempMap);
                    }
                }
            }
        }
        return networkInfo;
    }

    // Format network information in a string
    inline QString getNetworkInformationAsStr()
    {
        QString networkInfoStr{""};
        auto const r {SysInfo::getNetworkInformation()};
        for(const auto &t : r)
        {
            for(auto const &tempValue : t)
            {
                networkInfoStr.append(t.key(tempValue) + " : " + tempValue + "\n");
            }
            networkInfoStr.append("\n");
        }
        return networkInfoStr;
    }

    inline QList<QMap<QString,QString>> getCpuAndGpuInfo()
    {
        QList<QMap<QString,QString>> cpuAndGpuInfo{};
        QProcess process_system;
        #if defined(Q_OS_LINUX)
            process_system.start(LINUX_CPU_NAME);
            process_system.waitForFinished();
            cpuAndGpuInfo.push_back({{"CPU",process_system.readAllStandardOutput()}});
        #elif defined(Q_OS_WIN)
            process_system.start(CPU_NAME);
            process_system.waitForFinished();
            cpuAndGpuInfo.push_back({{"CPU",process_system.readAllStandardOutput().toUpper()}});

            process_system.start(GPU_NAME);
            process_system.waitForFinished();
            cpuAndGpuInfo.push_back({{"GPU",process_system.readAllStandardOutput()}});

            process_system.start(VRAM);
            process_system.waitForFinished();
            cpuAndGpuInfo.push_back({{"VRAM",process_system.readAllStandardOutput()}});
        #endif
        return cpuAndGpuInfo;
    }

    inline QString getCpuAndGpuInfoAsStr()
    {
        QString cpuAndGpuInfoStr{""};
        auto const y{SysInfo::getCpuAndGpuInfo()};
        for(auto const &tempInfoPair : y)
        {
            for(auto const &tempValue : tempInfoPair)
            {
                cpuAndGpuInfoStr.append(tempValue + "\n");
                cpuAndGpuInfoStr.append(tempInfoPair.key(tempValue) + " : " + tempValue + "\n");
            }
            cpuAndGpuInfoStr.append("\n");
        }
        return cpuAndGpuInfoStr;
    }

    inline QList<QMap<QString,QString>> getBiosInfo()
    {
        QList<QMap<QString,QString>> biosInfo{};
        QSettings settings(BIOS_KEY, QSettings::NativeFormat);
        biosInfo.push_back({{"BaseBoard Manufacturer",settings.value("BaseBoardManufacturer", "0").toString()}});
        biosInfo.push_back({{"BaseBoard Product",settings.value("BaseBoardProduct", "0").toString()}}) ;
        biosInfo.push_back({{"BIOS Vendor",settings.value("BIOSVendor", "0").toString()}});
        biosInfo.push_back({{"BIOS Release Date",settings.value("BIOSReleaseDate", "0").toString()}});
        biosInfo.push_back({{"System Manufacturer",settings.value("SystemManufacturer", "0").toString()}});
        biosInfo.push_back({{"Product Name",settings.value("SystemProductName", "0").toString()}});
        return biosInfo;
    }

    inline QString getBiosInfoAsStr()
    {
        QString biosInfoStr{""};
        auto const biosInfo{getBiosInfo()};
        for(auto const &tmpBiosInfoPair : biosInfo)
        {
            for(auto const &tempInfo : tmpBiosInfoPair)
            {
                biosInfoStr.append(tmpBiosInfoPair.key(tempInfo) + " : " + tempInfo);
            }
            biosInfoStr.append("\n");
        }
        return biosInfoStr;
    }
}
#endif //QCOMINFO_SYSINFO_HPP