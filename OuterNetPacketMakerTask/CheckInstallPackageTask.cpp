/*!
*@file     CheckInstallPackageTask.cpp
*@brief    ��鰲װ��
*@author   maozg
*@date     2018��1��16��
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
*@time         2018��1��16��
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
*@time         2018��1��16��
*@param
*@return
*/
CheckConfigTask::~CheckConfigTask()
{

}

/*!
*@brief        ִ�м������
*@author       maozg
*@time         2018��1��16��
*@param        QString& strError
*@return       bool
*/
bool CheckConfigTask::onExecuteCheckTask(QString& strError)
{
    //��鰲װ����
    if (!checkInstallIni())
    {
        strError = Chinese("Install.ini�ļ���\"/Install/CheckXP\"XPϵͳ�����ò���ȷ!");
        return false;
    }

    //��鰲װ��ʽ
    checkInstallType();

    //����Ƿ�Я����������
    checkOnlineSum();

    //�޸�Files.ini��Ϣ
    modifyFilesConfig();

    return true;
}

/*!
*@brief        ��鰲װ��ʽ
*@author       maozg
*@time         2018��1��16��
*@param
*@return       bool
*/
bool CheckConfigTask::checkInstallType()
{
    //�����ж�ذ�װ������Ҫ�޸������ļ�
    if (m_packetInfo.bIsUninstall)
    {
        return true;
    }
    else if (m_packetInfo.bIsCoverInstall)//���ǰ�װ��Ҫ�޸������ļ�
    {
        QString str2017Gip = QString("%1%2%3").arg(m_strInstallPath).arg(
            "/Files").arg("/GTJ2017.gip");

        QString str2017XmlPath = PacketMakerCommon::binPath() + "/GTJ2017.xml";
        //��ѹGTJ2017xml�ļ�
        Dll7zUnzipCmd::extractSpecifiedFile(str2017Gip, PacketMakerCommon::binPath(), "GTJ2017.xml");

        //�޸�xml�ļ�
        if (!modifyGTJ2017xml())
        {
            return false;
        }

        //���޸ĺ��xml�ļ���ӵ�ѹ����
        Dll7zUnzipCmd::updateFileToZip(str2017Gip, str2017XmlPath);

        //ɾ����ʱ�ļ�
        QFile::remove(str2017XmlPath);
    }

    return true;
}

/*!
*@brief        
*@author       maozg
*@time         2018��1��16��
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
*@brief        ����Ƿ������������
*@author       maozg
*@time         2018��1��17��
*@param
*@return       void
*/
void CheckConfigTask::checkOnlineSum()
{
    if (!m_packetInfo.bIsOnlineSum)
    {
        //��Я����������
        QString strOnlineGip = QString("%1%2%3").arg(m_strInstallPath).arg(
            "/Files").arg("/CalculateOnLine.gip");

        //ɾ���������ܰ�
        QFile::remove(strOnlineGip);
    }
}

/*!
*@brief        �޸�Files.ini�ļ�����
*@author       maozg
*@time         2018��1��17��
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

    //��ȡ�����ļ�
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

    //д�ļ�
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
*@brief        ���Install.ini
*@author       maozg
*@time         2018��1��18��
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
        //����������ʾ
        return false;
    }

    return true;
}
