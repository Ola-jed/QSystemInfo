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
    const QString BIOS_KEY{"HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\BIOS"};

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

    inline QString getSystemInformationAsStr()
    {
        QString infoStrBuilder{""};
        auto const t {SysInfo::getSystemInformation()};
        auto const keysList{t.keys()};
        for(auto const &temp : keysList)
        {
            infoStrBuilder += temp+" : "+t[temp]+"\n\n";
        }
        return infoStrBuilder;
    }

    inline int volumesNumber()
    {
        int size = 0;
        auto const mountedVolumes{QStorageInfo::mountedVolumes()};
        std::for_each(std::begin(mountedVolumes),std::end(mountedVolumes),[&size](QStorageInfo tmpVolume){
            if(tmpVolume.isReady() && tmpVolume.isValid())
            {
                size++;
            }
        });
        return size;
    }

    inline QList<QMap<QString,QString>> getDiskInformation()
    {
        QList<QMap<QString,QString>> diskInfo{};
        auto const mountedVolumes{QStorageInfo::mountedVolumes()};
        foreach (const QStorageInfo &storage, mountedVolumes)
        {
            if (storage.isValid()  && storage.isReady())
            {
                auto tempMap = QMap<QString,QString>{};
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

    inline QString getDiskInformationAsStr()
    {
        QString diskInfoStr{QString::number(volumesNumber())+" disks\n\n"};
        auto const v {SysInfo::getDiskInformation()};
        for(const auto &t : v)
        {
            for(auto const &temp : t.keys())
            {
                diskInfoStr.append(temp+" : "+t[temp]+"\n");
            }
            diskInfoStr.append("\n\n");
        }
        return diskInfoStr;
    }

    inline QList<QMap<QString,QString>> getNetworkInformation()
    {
        QList<QMap<QString,QString>> networkInfo{};
        foreach(const QNetworkInterface &networkInterface, QNetworkInterface::allInterfaces())
        {
            if (networkInterface.flags().testFlag(QNetworkInterface::IsUp))
            {
                foreach (QNetworkAddressEntry entry, networkInterface.addressEntries())
                {
                    if ( entry.ip().toString().contains("."))
                    {
                        const auto tempMap = QMap<QString,QString>{{"Interface",networkInterface.name()},
                                                     {"IP",entry.ip().toString()},
                                                     {"MAC",networkInterface.hardwareAddress().toLocal8Bit().constData()}};
                        networkInfo.push_back(tempMap);
                    }
                }
            }
        }
        return networkInfo;
    }

    inline QString getNetworkInformationAsStr()
    {
        QString networkInfoStr{""};
        auto const r {SysInfo::getNetworkInformation()};
        for(const auto &t : r)
        {
            for(auto const &temp : t.keys())
            {
                networkInfoStr.append(temp+" : "+t[temp]+"\n");
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
        for(auto const &p : y)
        {
            for(auto const &g  :p.keys())
            {
                cpuAndGpuInfoStr.append(p[g]+"\n");
                cpuAndGpuInfoStr.append(g+" : "+p[g]+"\n");
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
        for(auto const &tmp : biosInfo)
        {
            for(auto const &e : tmp.keys())
            {
                biosInfoStr.append(e+" : "+tmp[e]);
            }
            biosInfoStr.append("\n");
        }
        return biosInfoStr;
    }
}
#endif //QCOMINFO_SYSINFO_HPP