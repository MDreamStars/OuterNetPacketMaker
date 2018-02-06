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
    m_bIsAllRegionRules = false;
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
    //解析地区规则
    parsRegionRules();

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
        //当前都是单个地区，后期会有多个地区的，此处优先适配
        for (int i = 0; i < m_strRulesList.size(); ++i)
        {
            QString strFileName = QString("%1%2%3").arg("*").arg(m_strRulesList.at(i)).arg(
                                                   "*.gip");
            //解压指定地区规则
            Dll7zUnzipCmd::extractSpecifiedFile(m_strRulesPath, m_strExtractPath, strFileName);
        }
    }
}

/*!
*@brief
*@author       maozg
*@time         2018年1月9日
*@param
*@return       void
*/
void RegionRulesTask::parsRegionRules()
{
    //规则库文件名字有带省的也有没有带的，所以解析一下
    QStringList strRulesList = m_strRegionRules.split(',');

    for (int i = 0; i < strRulesList.size(); ++i)
    {
        QString strTemp = strRulesList.at(i);
        //如果选择了全国,直接结束
        if (-1 != strTemp.indexOf(Chinese("全国")))
        {
            m_bIsAllRegionRules = true;
            break;
        }
        
        int nIndex1 = strTemp.indexOf(Chinese("市"));
        int nIndex2 = strTemp.indexOf(Chinese("省"));

        if (-1 != nIndex1)
        {
            m_strRulesList.append(strTemp.mid(0, nIndex1));
        }
        else if (-1 != nIndex2)
        {
            m_strRulesList.append(strTemp.mid(0, nIndex2));
        }
        else
        {
            //取前两个字 比如宁夏、新疆、香港等
            m_strRulesList.append(strTemp.mid(0, 2));
        }
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
        //当前都是单个地区，后期会有多个地区的，此处优先适配
        for (int i = 0; i < m_strRulesList.size(); ++i)
        {
            QString strFileName = QString("%1%2%3").arg("*").arg(m_strRulesList.at(i)).arg(
                "*.gip");
            strFilters.append(strFileName);
        }
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

