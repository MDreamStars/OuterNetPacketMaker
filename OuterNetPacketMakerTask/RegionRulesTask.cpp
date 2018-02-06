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
    m_bIsAllRegionRules = false;
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
    //������������
    parsRegionRules();

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
        //��ǰ���ǵ������������ڻ��ж�������ģ��˴���������
        for (int i = 0; i < m_strRulesList.size(); ++i)
        {
            QString strFileName = QString("%1%2%3").arg("*").arg(m_strRulesList.at(i)).arg(
                                                   "*.gip");
            //��ѹָ����������
            Dll7zUnzipCmd::extractSpecifiedFile(m_strRulesPath, m_strExtractPath, strFileName);
        }
    }
}

/*!
*@brief
*@author       maozg
*@time         2018��1��9��
*@param
*@return       void
*/
void RegionRulesTask::parsRegionRules()
{
    //������ļ������д�ʡ��Ҳ��û�д��ģ����Խ���һ��
    QStringList strRulesList = m_strRegionRules.split(',');

    for (int i = 0; i < strRulesList.size(); ++i)
    {
        QString strTemp = strRulesList.at(i);
        //���ѡ����ȫ��,ֱ�ӽ���
        if (-1 != strTemp.indexOf(Chinese("ȫ��")))
        {
            m_bIsAllRegionRules = true;
            break;
        }
        
        int nIndex1 = strTemp.indexOf(Chinese("��"));
        int nIndex2 = strTemp.indexOf(Chinese("ʡ"));

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
            //ȡǰ������ �������ġ��½�����۵�
            m_strRulesList.append(strTemp.mid(0, 2));
        }
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
        //��ǰ���ǵ������������ڻ��ж�������ģ��˴���������
        for (int i = 0; i < m_strRulesList.size(); ++i)
        {
            QString strFileName = QString("%1%2%3").arg("*").arg(m_strRulesList.at(i)).arg(
                "*.gip");
            strFilters.append(strFileName);
        }
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

