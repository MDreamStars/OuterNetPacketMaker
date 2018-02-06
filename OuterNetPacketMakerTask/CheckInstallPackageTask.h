/*!
*@file     CheckInstallPackageTask.h
*@brief    检查安装包
*@author   maozg
*@date     2018年1月16日
*@remarks
*@version 3.0
*Copyright (c) 1998-2018 Glodon Corporation
*/
#pragma once
#include "..\common\PacketMakerCommon.h"

/*
* @brief  检查安装包配置信息
*
* @author maozg  2018年1月16日
*
* @class CheckConfigTask
*/
class CheckConfigTask
{
public:
    CheckConfigTask(const QString& strInstallPath, PacketMakerConfigInfo& packetInfo);
    ~CheckConfigTask();
public:
    bool onExecuteCheckTask(QString& strError);

private:
    bool checkInstallType();
    bool modifyGTJ2017xml();
    void checkOnlineSum();
    void modifyFilesConfig();
    bool checkInstallIni();
private:
    PacketMakerConfigInfo m_packetInfo;
    QString m_strInstallPath;           //安装包解压后的路径
};
