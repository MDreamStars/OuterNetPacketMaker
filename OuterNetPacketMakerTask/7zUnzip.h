/*!
*@file     7zUnzip.h
*@brief    7z解压缩类
*@author   maozg
*@date     2017年12月29日
*@remarks
*@version 3.0
*Copyright (c) 1998-2017 Glodon Corporation
*/
#pragma once
#include <QString>

/*
* @brief 使用7z命令解压缩文件
*
* @author maozg  2017年12月29日
*
* @class Dll7zUnzipCmd
*/
class Dll7zUnzipCmd
{
public:
    Dll7zUnzipCmd();
    ~Dll7zUnzipCmd();

    //解压所有文件
    static bool extractAllFile(const QString& strZipPath, const QString& strOutPath);
    //解压指定文件
    static bool extractSpecifiedFile(const QString& strZipPath, const QString& strOutPath, const QString& strFileName);
    static bool updateFileToZip(const QString& strZipPath, const QString& strFilePath);
};
