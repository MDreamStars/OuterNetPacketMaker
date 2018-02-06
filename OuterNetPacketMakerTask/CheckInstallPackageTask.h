/*!
*@file     CheckInstallPackageTask.h
*@brief    ��鰲װ��
*@author   maozg
*@date     2018��1��16��
*@remarks
*@version 3.0
*Copyright (c) 1998-2018 Glodon Corporation
*/
#pragma once
#include "..\common\PacketMakerCommon.h"

/*
* @brief  ��鰲װ��������Ϣ
*
* @author maozg  2018��1��16��
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
    QString m_strInstallPath;           //��װ����ѹ���·��
};
