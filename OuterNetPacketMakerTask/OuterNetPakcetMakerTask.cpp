/*!
*@file     OuterNetPakcetMakerTask.cpp
*@brief    组外网包任务类
*@author   maozg
*@date     2018年1月5日
*@remarks
*@version 3.0
*Copyright (c) 1998-2018 Glodon Corporation
*/
#include "OuterNetPakcetMakerTask.h"
#include "RegionRulesTask.h"
#include "7zUnzip.h"
#include "CheckInstallPackageTask.h"
#include <QDir>
#include <windows.h>
#include <QProcess>

/*!
*@brief
*@author       maozg
*@time         2018年1月5日
*@param        PacketMakerConfigInfo& packetInfo
*@return
*/
OuterNetPakcetMakerTask::OuterNetPakcetMakerTask(PacketMakerConfigInfo& packetInfo)
    :m_oPacketMakerInfo(packetInfo)
{
    if (m_oPacketMakerInfo.strOuterNetPacketName.isEmpty())
    {
        m_strInstallFilePath = PacketMakerCommon::binPath() + "/Install";
    }
    else
    {
        m_strInstallFilePath = PacketMakerCommon::binPath() + "/" + m_oPacketMakerInfo.strOuterNetPacketName;
    }
}

/*!
*@brief
*@author       maozg
*@time         2018年1月5日
*@param
*@return
*/
OuterNetPakcetMakerTask::~OuterNetPakcetMakerTask()
{

}

/*!
*@brief        执行任务
*@author       maozg
*@time         2018年1月5日
*@param
*@return       void
*/
void OuterNetPakcetMakerTask::onExecuteTask()
{
    //解压安装包
    extractInstallPakect();

    //处理规则
    runRulesTask();

    //执行检查任务
    runCheckTask();

    //组包并签名
    runPacketMakerSignatureTask();

    //删除临时目录
    removeAllTempFiles();

    emit setLog(Chinese("组包完成"), 100);
    //发送结束消息
    emit onTaskFinished();
}

/*!
*@brief        开始组包任务
*@author       maozg
*@time         2018年1月5日
*@param
*@return       void
*/
void OuterNetPakcetMakerTask::runRulesTask()
{
    emit setLog(Chinese("处理地区规则文件"), 20);
    //执行规则库任务
    RegionRulesTask rulesTask(m_strInstallFilePath,
        m_oPacketMakerInfo.strRulesFilePath, m_oPacketMakerInfo.strRegionRules);
    QString strError = "";
    if (!rulesTask.onExecuteTask(strError))
    {
        return;
    }
}

/*!
*@brief        执行检查任务
*@author       maozg
*@time         2018年1月5日
*@param
*@return       void
*/
void OuterNetPakcetMakerTask::runCheckTask()
{
    emit setLog(Chinese("检查安装包配置文件"), 40);
    CheckConfigTask checkTask(m_strInstallFilePath, m_oPacketMakerInfo);
    QString strError = "";
    if (!checkTask.onExecuteCheckTask(strError))
    {
        return;
    }
}

/*!
*@brief        组包并签名
*@author       maozg
*@time         2018年1月5日
*@param
*@return       void
*/
void OuterNetPakcetMakerTask::runPacketMakerSignatureTask()
{
    QString strExeName;

    //检查输出路径，不存在就创建一个
    PacketMakerCommon::checkDirExists(m_oPacketMakerInfo.strOutPutPath);
    if (m_oPacketMakerInfo.strOuterNetPacketName.isEmpty())
    {
        strExeName = m_oPacketMakerInfo.strOutPutPath + "/Install.exe";
    }
    else
    {
        strExeName = m_oPacketMakerInfo.strOutPutPath + "/" + m_oPacketMakerInfo.strOuterNetPacketName + ".exe";
    }

    emit setLog(Chinese("合并处理后的安装包"), 60);
    //执行组包
    if (!mergerMakerCmd(m_strInstallFilePath, strExeName, m_oPacketMakerInfo.strUserName,
        m_oPacketMakerInfo.strUserPassWord))
    {
        return;
    }

    emit setLog(Chinese("安装包签名"), 80);
    //执行签名
    if (!PacketMakerCommon::signInstallPacket(strExeName))
    {
        return;
    }
}

/*!
*@brief        解压安装包
*@author       maozg
*@time         2018年1月12日
*@param
*@return       bool
*/
bool OuterNetPakcetMakerTask::extractInstallPakect()
{
    emit setLog(Chinese("解压安装包压缩文件"), 10);
    if (typeIsExe == PacketMakerCommon::checkFileType(m_oPacketMakerInfo.strInstallFilePath))
    {
        return Dll7zUnzipCmd::extractAllFile(m_oPacketMakerInfo.strInstallFilePath, m_strInstallFilePath);
    }

    return false;
}

/*!
*@brief
*@author       maozg
*@time         2018年1月18日
*@param
*@return       void
*/
void OuterNetPakcetMakerTask::removeAllTempFiles()
{
    emit setLog(Chinese("清理临时文件"), 95);
    if (!PacketMakerCommon::removeDir(m_strInstallFilePath))
    {
        return;
    }

    if (!PacketMakerCommon::removeDir(PacketMakerCommon::binPath() + "/Update"))
    {
        return;
    }
}

/*!
*@brief
*@author       maozg
*@time         2018年1月5日
*@param        OuterNetPakcetMakerTask* pTask, QObject* parent /*= nullptr*/
/*@return
* */
PacketMakerTaskThread::PacketMakerTaskThread(OuterNetPakcetMakerTask* pTask, QObject* parent /*= nullptr*/)
    :QThread(parent), m_pTask(pTask)
{
}

/*!
*@brief
*@author       maozg
*@time         2018年1月5日
*@param
*@return
*/
PacketMakerTaskThread::~PacketMakerTaskThread()
{

}

/*!
*@brief
*@author       maozg
*@time         2018年1月5日
*@param
*@return       void
*/
void PacketMakerTaskThread::run()
{
    m_pTask->onExecuteTask();
}


/*!
*@brief
*@author       maozg
*@time         2018年1月24日
*@param        PacketMakerConfigInfo& packetInfo
*@return
*/
OutNetPacketMakerCMD::OutNetPacketMakerCMD(PacketMakerConfigInfo& packetInfo, logFun pFun)
    :m_oPacketMakerInfo(packetInfo), m_pPacketMakerTask(nullptr), m_pSetLogFun(pFun)
{

}

/*!
*@brief
*@author       maozg
*@time         2018年1月24日
*@param
*@return
*/
OutNetPacketMakerCMD::~OutNetPacketMakerCMD()
{
    if (nullptr != m_pPacketMakerTask)
    {
        delete m_pPacketMakerTask;
        m_pPacketMakerTask = nullptr;
    }
}

/*!
*@brief
*@author       maozg
*@time         2018年1月24日
*@param
*@return       void
*/
void OutNetPacketMakerCMD::startPacketMaker()
{
    m_pPacketMakerTask = new OuterNetPakcetMakerTask(m_oPacketMakerInfo);

    connect(m_pPacketMakerTask, SIGNAL(setLog(const QString&, int)), this, SLOT(onSetCMDLog(const QString&, int)));

    m_pPacketMakerTask->onExecuteTask();
}

/*!
*@brief
*@author       maozg
*@time         2018年1月24日
*@param        const QString& strLog, int nValue 
/*@return       void
* */
void OutNetPacketMakerCMD::onSetCMDLog(const QString& strLog, int nValue)
{
    (*m_pSetLogFun)(strLog);
}

