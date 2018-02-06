/*!
*@file     OuterNetPakcetMakerTask.h
*@brief    ��������������
*@author   maozg
*@date     2018��1��5��
*@remarks
*@version 3.0
*Copyright (c) 1998-2018 Glodon Corporation
*/
#pragma once
#include "..\common\PacketMakerCommon.h"
#include <QObject>
#include <QThread>
#include "outnetpacketmakertask_global.h"

/*
* @brief �������
*
* @author maozg  2018��1��5��
*
* @class OuterNetPakcetMakerTask
*/
class OUTNETPACKETMAKERTASK_EXPORT OuterNetPakcetMakerTask : public QObject
{
    Q_OBJECT
public:
    OuterNetPakcetMakerTask(PacketMakerConfigInfo& packetInfo);
    ~OuterNetPakcetMakerTask();

public:
    void onExecuteTask();

signals:
    void onTaskFinished();
    void setLog(const QString& strLog, int nProgress);

private:
    void runRulesTask();
    void runCheckTask();
    void runPacketMakerSignatureTask();
    bool extractInstallPakect();
    void removeAllTempFiles();
private:
    PacketMakerConfigInfo m_oPacketMakerInfo;
    QString m_strInstallFilePath;
};

/*
* @brief ��������߳�     ������������ڴ棬�������ͷ�
*
* @author maozg  2018��1��5��
*
* @class PacketMakerTaskThread
*/
class OUTNETPACKETMAKERTASK_EXPORT PacketMakerTaskThread : public QThread
{
    Q_OBJECT
public:
    PacketMakerTaskThread(OuterNetPakcetMakerTask* pTask, QObject* parent = nullptr);
    ~PacketMakerTaskThread();

    void run();

private:
    OuterNetPakcetMakerTask * m_pTask;
};

/*
* @brief �������
*
* @author maozg  2018��1��24��
*
* @class OutNetPacketMakerCMD
*/
class OUTNETPACKETMAKERTASK_EXPORT OutNetPacketMakerCMD : public QObject
{
    Q_OBJECT
public:
    OutNetPacketMakerCMD(PacketMakerConfigInfo& packetInfo, logFun pFun);
    virtual ~OutNetPacketMakerCMD();

public:
    void startPacketMaker();
private slots:
    void onSetCMDLog(const QString& strLog, int nValue);
private:
    PacketMakerConfigInfo m_oPacketMakerInfo;
    OuterNetPakcetMakerTask *m_pPacketMakerTask;
    logFun m_pSetLogFun;
};

