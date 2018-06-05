/*!
*@file     RegionRulesTask.h
*@brief    ������������
*@author   maozg
*@date     2018��1��5��
*@remarks
*@version 3.0
*Copyright (c) 1998-2018 Glodon Corporation
*/
#pragma once
#include <QString>
#include <QStringList>

/*
* @brief ���������
*
* @author maozg  2018��1��5��
*
* @class RegionRulesTask
*/
class RegionRulesTask
{
public:
    RegionRulesTask(const QString& strInstallPath, const QString& strRulesPath, 
        const QString& strRegionRules);
    ~RegionRulesTask();

    bool onExecuteTask(QString& strError);
private:
    void extractRulesFile();
    void copyRulesFile();
    void findFiles(QString& strDir, QStringList& strFilters);
    void addFilePathAndName(QString& strDir, QStringList& strFilters);
private:
    QString m_strInstallPath;
    QString m_strRulesPath;
    QString m_strRegionRules;
    QString m_strExtractPath;
    bool m_bIsAllRegionRules;   //�Ƿ���ȫ������
    QStringList m_strRulesFileList;
};
