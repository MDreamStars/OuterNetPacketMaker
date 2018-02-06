/*!
*@file     CheckInstallPackageTask.cpp
*@brief    检查安装包
*@author   maozg
*@date     2018年1月16日
*@remarks
*@version 3.0
*Copyright (c) 1998-2018 Glodon Corporation
*/
#include "CheckInstallPackageTask.h"
#include "7zUnzip.h"
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStringList>
#include <set>
#include <QSettings>
#include <QLocale>
#include <QCollator>


/*!
*@brief
*@author       maozg
*@time         2018年1月16日
*@param        const QString& strInstallPath, PacketMakerConfigInfo& packetInfo
*@return
*/
CheckConfigTask::CheckConfigTask(const QString& strInstallPath, PacketMakerConfigInfo& packetInfo)
    :m_packetInfo(packetInfo), m_strInstallPath(strInstallPath)
{

}

/*!
*@brief
*@author       maozg
*@time         2018年1月16日
*@param
*@return
*/
CheckConfigTask::~CheckConfigTask()
{

}

/*!
*@brief        执行检查任务
*@author       maozg
*@time         2018年1月16日
*@param        QString& strError
*@return       bool
*/
bool CheckConfigTask::onExecuteCheckTask(QString& strError)
{
    //检查安装配置
    if (!checkInstallIni())
    {
        strError = Chinese("Install.ini文件中\"/Install/CheckXP\"XP系统的配置不正确!");
        return false;
    }

    //检查安装方式
    checkInstallType();

    //检查是否携带联机汇总
    checkOnlineSum();

    //修改Files.ini信息
    modifyFilesConfig();

    return true;
}

/*!
*@brief        检查安装方式
*@author       maozg
*@time         2018年1月16日
*@param
*@return       bool
*/
bool CheckConfigTask::checkInstallType()
{
    //如果是卸载安装，不需要修改配置文件
    if (m_packetInfo.bIsUninstall)
    {
        return true;
    }
    else if (m_packetInfo.bIsCoverInstall)//覆盖安装需要修改配置文件
    {
        QString str2017Gip = QString("%1%2%3").arg(m_strInstallPath).arg(
            "/Files").arg("/GTJ2017.gip");

        QString str2017XmlPath = PacketMakerCommon::binPath() + "/GTJ2017.xml";
        //解压GTJ2017xml文件
        Dll7zUnzipCmd::extractSpecifiedFile(str2017Gip, PacketMakerCommon::binPath(), "GTJ2017.xml");

        //修改xml文件
        if (!modifyGTJ2017xml())
        {
            return false;
        }

        //把修改后的xml文件添加到压缩包
        Dll7zUnzipCmd::updateFileToZip(str2017Gip, str2017XmlPath);

        //删除临时文件
        QFile::remove(str2017XmlPath);
    }

    return true;
}

/*!
*@brief        
*@author       maozg
*@time         2018年1月16日
*@param
*@return       bool
*/
bool CheckConfigTask::modifyGTJ2017xml()
{
    QString strXml = PacketMakerCommon::binPath() + "/GTJ2017.xml";
    QFile xmlFile(strXml);
    if (!xmlFile.open(QFile::ReadOnly))
    {
        return false;
    }

    QDomDocument doc;
    if (!doc.setContent(&xmlFile))
    {
        xmlFile.close();
        return false;
    }

    xmlFile.close();

    QDomElement root = doc.documentElement();
    QDomNodeList nodeList = root.elementsByTagName("ProductInfo");

    for (int i = 0; i < nodeList.count(); ++i)
    {
        QDomNodeList childNodeList = nodeList.at(i).childNodes();
        for (int j = 0; j < childNodeList.count(); ++j)
        {
            QDomNode node = childNodeList.at(j);
            if (node.nodeName() == "OldVerKey")
            {
                QDomNode oldNode = node.firstChild();
                node.firstChild().setNodeValue("");
                QDomNode newNode = node.firstChild();

                node.replaceChild(newNode, oldNode);

                if (!xmlFile.open(QFile::WriteOnly | QFile::Truncate))
                {
                    return false;
                }

                QTextStream out(&xmlFile);
                doc.save(out, 4);
                xmlFile.close();

                break;
            }
        }
    }

    return true;
}

/*!
*@brief        检查是否带有联机汇总
*@author       maozg
*@time         2018年1月17日
*@param
*@return       void
*/
void CheckConfigTask::checkOnlineSum()
{
    if (!m_packetInfo.bIsOnlineSum)
    {
        //不携带联机汇总
        QString strOnlineGip = QString("%1%2%3").arg(m_strInstallPath).arg(
            "/Files").arg("/CalculateOnLine.gip");

        //删除联机汇总包
        QFile::remove(strOnlineGip);
    }
}

/*!
*@brief        修改Files.ini文件内容
*@author       maozg
*@time         2018年1月17日
*@param
*@return       void
*/
void CheckConfigTask::modifyFilesConfig()
{
    QString strFilesPath = QString("%1%2").arg(m_strInstallPath).arg(
        "/Files");
    QString strIniFilePath = QString("%1%2").arg(m_strInstallPath).arg(
        "/Files.ini");

    QLocale cn(QLocale::Chinese);
    QCollator collator(cn);

    //获取所有文件
    QDir rulesDir(strFilesPath);
    QStringList strFilters;
    strFilters << "*.gip";

    QStringList strFileList = rulesDir.entryList(strFilters, QDir::Files | QDir::Hidden, QDir::Name);
    
    strFileList.sort();
    std::sort(strFileList.begin(), strFileList.end(), collator);

    std::set<QString> str2017GipSet;
    str2017GipSet.insert("GTJ2017.gip");
    str2017GipSet.insert("GGJ16GDataBase.gip");
    str2017GipSet.insert("CalculateOnLine.gip");

    QStringList str2017Gip;
    str2017Gip << "GTJ2017.gip" << "GGJ16GDataBase.gip" << "CalculateOnLine.gip";

    //写文件
    int nFlag = 0;
    QString strTemp = "[Files]";

    QFile iniFile(strIniFilePath);
    if (!iniFile.open(QFile::WriteOnly | QFile::Truncate | QFile::Text))
    {
        return;
    }
    QTextStream writeIni(&iniFile);

    writeIni << strTemp << endl;
    writeIni << QString("p%1=").arg(nFlag) << QString("Files\\%1").arg(str2017Gip.at(0)) << endl;
    nFlag++;
    writeIni << QString("p%1=").arg(nFlag) << QString("Files\\%1").arg(str2017Gip.at(1)) << endl;
    if (m_packetInfo.bIsOnlineSum)
    {
        nFlag++;
        writeIni << QString("p%1=").arg(nFlag) << QString("Files\\%1").arg(str2017Gip.at(2)) << endl;
    }

    for (int i = 0; i < strFileList.count(); ++i)
    {
        QString strFileName = strFileList.at(i);

        if (str2017GipSet.find(strFileName) != str2017GipSet.end())
        {
            continue;
        }

        nFlag++;
        strFileName = QString("Files\\%1").arg(strFileName);
        writeIni << QString("p%1=").arg(nFlag) << strFileName << endl;
    }

    iniFile.close();
}

/*!
*@brief        检查Install.ini
*@author       maozg
*@time         2018年1月18日
*@param
*@return       bool
*/
bool CheckConfigTask::checkInstallIni()
{
    QString strIniFilePath = QString("%1%2").arg(m_strInstallPath).arg(
        "/Install.ini");

    QSettings iniInfo(strIniFilePath, QSettings::IniFormat);

    QVariant XpConfig = iniInfo.value("/Install/CheckXP");
    QString strConfig = XpConfig.toString();

    if (strConfig.compare("2") != 0)
    {
        //弹出错误提示
        return false;
    }

    return true;
}
