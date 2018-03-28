/*!
*@file     OuterNetPakcetMakerTask.cpp
*@brief    ��������������
*@author   maozg
*@date     2018��1��5��
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
*@time         2018��1��5��
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
*@time         2018��1��5��
*@param
*@return
*/
OuterNetPakcetMakerTask::~OuterNetPakcetMakerTask()
{

}

/*!
*@brief        ִ������
*@author       maozg
*@time         2018��1��5��
*@param
*@return       void
*/
void OuterNetPakcetMakerTask::onExecuteTask()
{
    //��ѹ��װ��
    extractInstallPakect();

    //�������
    runRulesTask();

    //ִ�м������
    runCheckTask();

    //�����ǩ��
    runPacketMakerSignatureTask();

    //ɾ����ʱĿ¼
    removeAllTempFiles();

    emit setLog(Chinese("������"), 100);
    //���ͽ�����Ϣ
    emit onTaskFinished();
}

/*!
*@brief        ��ʼ�������
*@author       maozg
*@time         2018��1��5��
*@param
*@return       void
*/
void OuterNetPakcetMakerTask::runRulesTask()
{
    emit setLog(Chinese("������������ļ�"), 20);
    //ִ�й��������
    RegionRulesTask rulesTask(m_strInstallFilePath,
        m_oPacketMakerInfo.strRulesFilePath, m_oPacketMakerInfo.strRegionRules);
    QString strError = "";
    if (!rulesTask.onExecuteTask(strError))
    {
        return;
    }
}

/*!
*@brief        ִ�м������
*@author       maozg
*@time         2018��1��5��
*@param
*@return       void
*/
void OuterNetPakcetMakerTask::runCheckTask()
{
    emit setLog(Chinese("��鰲װ�������ļ�"), 40);
    CheckConfigTask checkTask(m_strInstallFilePath, m_oPacketMakerInfo);
    QString strError = "";
    if (!checkTask.onExecuteCheckTask(strError))
    {
        return;
    }
}

/*!
*@brief        �����ǩ��
*@author       maozg
*@time         2018��1��5��
*@param
*@return       void
*/
void OuterNetPakcetMakerTask::runPacketMakerSignatureTask()
{
    QString strExeName;

    //������·���������ھʹ���һ��
    PacketMakerCommon::checkDirExists(m_oPacketMakerInfo.strOutPutPath);
    if (m_oPacketMakerInfo.strOuterNetPacketName.isEmpty())
    {
        strExeName = m_oPacketMakerInfo.strOutPutPath + "/Install.exe";
    }
    else
    {
        strExeName = m_oPacketMakerInfo.strOutPutPath + "/" + m_oPacketMakerInfo.strOuterNetPacketName + ".exe";
    }

    emit setLog(Chinese("�ϲ������İ�װ��"), 60);
    //ִ�����
    if (!mergerMakerCmd(m_strInstallFilePath, strExeName, m_oPacketMakerInfo.strUserName,
        m_oPacketMakerInfo.strUserPassWord))
    {
        return;
    }

    emit setLog(Chinese("��װ��ǩ��"), 80);
    //ִ��ǩ��
    if (!PacketMakerCommon::signInstallPacket(strExeName))
    {
        return;
    }
}

/*!
*@brief        ��ѹ��װ��
*@author       maozg
*@time         2018��1��12��
*@param
*@return       bool
*/
bool OuterNetPakcetMakerTask::extractInstallPakect()
{
    emit setLog(Chinese("��ѹ��װ��ѹ���ļ�"), 10);
    if (typeIsExe == PacketMakerCommon::checkFileType(m_oPacketMakerInfo.strInstallFilePath))
    {
        return Dll7zUnzipCmd::extractAllFile(m_oPacketMakerInfo.strInstallFilePath, m_strInstallFilePath);
    }

    return false;
}

/*!
*@brief
*@author       maozg
*@time         2018��1��18��
*@param
*@return       void
*/
void OuterNetPakcetMakerTask::removeAllTempFiles()
{
    emit setLog(Chinese("������ʱ�ļ�"), 95);
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
*@time         2018��1��5��
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
*@time         2018��1��5��
*@param
*@return
*/
PacketMakerTaskThread::~PacketMakerTaskThread()
{

}

/*!
*@brief
*@author       maozg
*@time         2018��1��5��
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
*@time         2018��1��24��
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
*@time         2018��1��24��
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
*@time         2018��1��24��
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
*@time         2018��1��24��
*@param        const QString& strLog, int nValue 
/*@return       void
* */
void OutNetPacketMakerCMD::onSetCMDLog(const QString& strLog, int nValue)
{
    (*m_pSetLogFun)(strLog);
}

