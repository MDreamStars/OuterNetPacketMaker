/*!
*@file     RegionRulesTask.cpp
*@brief    处理规则库任务
*@author   maozg
*@date     2018年1月5日
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
*@time         2018年1月5日
*@param        const QString& strInstallPath, const QString& strRulesPath, const QString& strRegionRules
*@return
*/
RegionRulesTask::RegionRulesTask(const QString& strInstallPath, const QString& strRulesPath,
    const QString& strRegionRules)
    :m_strInstallPath(strInstallPath), m_strRulesPath(strRulesPath), m_strRegionRules(strRegionRules)
{
    m_strExtractPath = PacketMakerCommon::findFilesDirPath(strInstallPath, "Files");

    //如果选择了全国,直接结束
    if (-1 != strRegionRules.indexOf(Chinese("全国")))
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
*@time         2018年1月5日
*@param
*@return
*/
RegionRulesTask::~RegionRulesTask()
{

}

/*!
*@brief
*@author       maozg
*@time         2018年1月5日
*@param        QString& strError
*@return       bool
*/
bool RegionRulesTask::onExecuteTask(QString& strError)
{
    //清空老的规则文件
    removeOldRulesFile();

    //解压规则库文件
    fileType rulesPathType = PacketMakerCommon::checkFileType(m_strRulesPath);
    switch (rulesPathType)
    {
    case typeIsZip:
        //解压规则库，直接解压到安装包
        extractRulesFile();
        break;
    case typeIsDir:
        copyRulesFile();
        break;
    default:
        //非法的路径
        break;
    }

    return true;
}

/*!
*@brief
*@author       maozg
*@time         2018年1月5日
*@param
*@return       void
*/
void RegionRulesTask::extractRulesFile()
{
    if (m_bIsAllRegionRules)
    {
        //解压全国规则
        Dll7zUnzipCmd::extractSpecifiedFile(m_strRulesPath, m_strExtractPath, "*.gip");
    }
    else
    {
        QString strFileName = QString("%1%2%3").arg("*").arg(m_strRegionRules).arg(
            "*.gip");

        //安徽规则有一个合肥的规则，需要单独处理一下
        if (m_strRegionRules == Chinese("安徽"))
        {
            QString strTemp = QString("%1%2%3").arg("*").arg(Chinese("合肥")).arg(
                "*.gip");

            Dll7zUnzipCmd::extractSpecifiedFile(m_strRulesPath, m_strExtractPath, strTemp);
        }

        //解压指定地区规则
        Dll7zUnzipCmd::extractSpecifiedFile(m_strRulesPath, m_strExtractPath, strFileName);
    }
}

/*!
*@brief        拷贝规则文件
*@author       maozg
*@time         2018年1月16日
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

        //安徽规则有一个合肥的规则，需要单独处理一下
        if (m_strRegionRules == Chinese("安徽"))
        {
            QString strTemp = QString("%1%2%3").arg("*").arg(Chinese("合肥")).arg(
                "*.gip");

            strFilters.append(strTemp);
        }

        strFilters.append(strFileName);
    }

    //查找规则文件
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
*@brief        查找规则文件
*@author       maozg
*@time         2018年1月16日
*@param        QString& strDir, QStringList& strFilters
*@return       void
*/
void RegionRulesTask::findFiles(QString& strDir, QStringList& strFilters)
{
    //添加当前目录
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
*@brief        把路径和文件名组合并存储
*@author       maozg
*@time         2018年1月16日
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
    //获取所有文件
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

