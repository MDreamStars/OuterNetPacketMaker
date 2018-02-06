/*!
*@file     OuterNetPakcetMakerTask.h
*@brief    组外网包任务类
*@author   maozg
*@date     2018年1月5日
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
* @brief 组包任务
*
* @author maozg  2018年1月5日
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
* @brief 组包任务线程     传入的任务类内存，调用者释放
*
* @author maozg  2018年1月5日
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
* @brief 命令处理类
*
* @author maozg  2018年1月24日
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

