/*!
*@file     RegionRulesTask.cpp
*@brief    ������������
*@author   maozg
*@date     2018��1��5��
*@remarks
*@version 3.0
*Copyright (c) 1998-2018 Glodon Corporation
*/
#include "RegionRulesTask.h"
#include <QFileInfo>
#include <set>
#include "..\common\PacketMakerCommon.h"
#include "7zUnzip.h"
#include <QDir>

/*!
*@brief
*@author       maozg
*@time         2018��1��5��
*@param        const QString& strInstallPath, const QString& strRulesPath, const QString& strRegionRules
*@return
*/
RegionRulesTask::RegionRulesTask(const QString& strInstallPath, const QString& strRulesPath,
    const QString& strRegionRules)
    :m_strInstallPath(strInstallPath), m_strRulesPath(strRulesPath), m_strRegionRules(strRegionRules)
{
    m_strExtractPath = PacketMakerCommon::findFilesDirPath(strInstallPath, "Files");

    //���ѡ����ȫ��,ֱ�ӽ���
    if (-1 != strRegionRules.indexOf(Chinese("ȫ��")))
    {
        m_bIsAllRegionRules = true;
    }
    else
    {
        m_bIsAllRegionRules = false;
    }
}

/*!
*@brief
*@author       maozg
*@time         2018��1��5��
*@param
*@return
*/
RegionRulesTask::~RegionRulesTask()
{

}

/*!
*@brief
*@author       maozg
*@time         2018��1��5��
*@param        QString& strError
*@return       bool
*/
bool RegionRulesTask::onExecuteTask(QString& strError)
{
    //����ϵĹ����ļ�
    removeOldRulesFile();

    //��ѹ������ļ�
    fileType rulesPathType = PacketMakerCommon::checkFileType(m_strRulesPath);
    switch (rulesPathType)
    {
    case typeIsZip:
        //��ѹ����⣬ֱ�ӽ�ѹ����װ��
        extractRulesFile();
        break;
    case typeIsDir:
        copyRulesFile();
        break;
    default:
        //�Ƿ���·��
        break;
    }

    return true;
}

/*!
*@brief
*@author       maozg
*@time         2018��1��5��
*@param
*@return       void
*/
void RegionRulesTask::extractRulesFile()
{
    if (m_bIsAllRegionRules)
    {
        //��ѹȫ������
        Dll7zUnzipCmd::extractSpecifiedFile(m_strRulesPath, m_strExtractPath, "*.gip");
    }
    else
    {
        QString strFileName = QString("%1%2%3").arg("*").arg(m_strRegionRules).arg(
            "*.gip");

        //���չ�����һ���ϷʵĹ�����Ҫ��������һ��
        if (m_strRegionRules == Chinese("����"))
        {
            QString strTemp = QString("%1%2%3").arg("*").arg(Chinese("�Ϸ�")).arg(
                "*.gip");

            Dll7zUnzipCmd::extractSpecifiedFile(m_strRulesPath, m_strExtractPath, strTemp);
        }

        //��ѹָ����������
        Dll7zUnzipCmd::extractSpecifiedFile(m_strRulesPath, m_strExtractPath, strFileName);
    }
}

/*!
*@brief        ���������ļ�
*@author       maozg
*@time         2018��1��16��
*@param
*@return       void
*/
void RegionRulesTask::copyRulesFile()
{
    QStringList strFilters;

    if (m_bIsAllRegionRules)
    {
        strFilters.append("*.gip");
    }
    else
    {
        QString strFileName = QString("%1%2%3").arg("*").arg(m_strRegionRules).arg(
            "*.gip");

        //���չ�����һ���ϷʵĹ�����Ҫ��������һ��
        if (m_strRegionRules == Chinese("����"))
        {
            QString strTemp = QString("%1%2%3").arg("*").arg(Chinese("�Ϸ�")).arg(
                "*.gip");

            strFilters.append(strTemp);
        }

        strFilters.append(strFileName);
    }

    //���ҹ����ļ�
    findFiles(m_strRulesPath, strFilters);

    for (int i = 0; i < m_strRulesFileList.size(); ++i)
    {
        QString strFile = m_strRulesFileList.at(i);
        QFileInfo fileInfo(strFile);
        QString strCopyFile = m_strExtractPath + "/" + fileInfo.fileName();
        QFile::copy(strFile, strCopyFile);
    }
}

/*!
*@brief        ���ҹ����ļ�
*@author       maozg
*@time         2018��1��16��
*@param        QString& strDir, QStringList& strFilters
*@return       void
*/
void RegionRulesTask::findFiles(QString& strDir, QStringList& strFilters)
{
    //��ӵ�ǰĿ¼
    QDir rulesDir(strDir);
    QStringList strFileList = rulesDir.entryList(strFilters, QDir::Files | QDir::Hidden);
    addFilePathAndName(strDir, strFileList);

    QStringList strDirList = rulesDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    if (strDirList.isEmpty())
    {
        return;
    }

    for each (QString var in strDirList)
    {
        QString strSubDir = strDir + "/" + var;
        findFiles(strSubDir, strFilters);
    }
}

/*!
*@brief        ��·�����ļ�����ϲ��洢
*@author       maozg
*@time         2018��1��16��
*@param        QString& strDir, QStringList& strFilters
*@return       void
*/
void RegionRulesTask::addFilePathAndName(QString& strDir, QStringList& strFilters)
{
    for each (QString strFileName in strFilters)
    {
        QString strFile = strDir + "/" + strFileName;
        m_strRulesFileList.append(strFile);
    }
}

void RegionRulesTask::removeOldRulesFile()
{
    //��ȡ�����ļ�
    QDir rulesDir(m_strExtractPath);
    QStringList strFilters;
    strFilters << "*.gip";

    QStringList strFileList = rulesDir.entryList(strFilters, QDir::Files | QDir::Hidden, QDir::Name);

    std::set<QString> str2017GipSet;
    str2017GipSet.insert("GTJ2017.gip");
    str2017GipSet.insert("GGJ16GDataBase.gip");
    str2017GipSet.insert("CalculateOnLine.gip");
    str2017GipSet.insert("IGMSPlugin.gip");

    for (int i = 0; i < strFileList.count(); ++i)
    {
        if (str2017GipSet.find(strFileList.at(i)) != str2017GipSet.end())
        {
            continue;
        }

        QString strFileName = m_strExtractPath + "/" + strFileList.at(i);
        QDir().remove(strFileName);
    }
}

