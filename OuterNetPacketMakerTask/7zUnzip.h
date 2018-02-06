/*!
*@file     7zUnzip.h
*@brief    7z��ѹ����
*@author   maozg
*@date     2017��12��29��
*@remarks
*@version 3.0
*Copyright (c) 1998-2017 Glodon Corporation
*/
#pragma once
#include <QString>

/*
* @brief ʹ��7z�����ѹ���ļ�
*
* @author maozg  2017��12��29��
*
* @class Dll7zUnzipCmd
*/
class Dll7zUnzipCmd
{
public:
    Dll7zUnzipCmd();
    ~Dll7zUnzipCmd();

    //��ѹ�����ļ�
    static bool extractAllFile(const QString& strZipPath, const QString& strOutPath);
    //��ѹָ���ļ�
    static bool extractSpecifiedFile(const QString& strZipPath, const QString& strOutPath, const QString& strFileName);
    static bool updateFileToZip(const QString& strZipPath, const QString& strFilePath);
};
