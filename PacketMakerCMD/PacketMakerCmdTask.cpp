#include "PacketMakerCmdTask.h"
#include <windows.h>
#include <QSettings>

PacketMakerConfigInfoIterator::PacketMakerConfigInfoIterator(vector<PacketMakerConfigInfo>& taskList)
	:m_oTaskList(taskList), m_nPost(0)
{
	m_nCount = m_oTaskList.size();
}

PacketMakerConfigInfoIterator::~PacketMakerConfigInfoIterator()
{

}

PacketMakerConfigInfo& PacketMakerConfigInfoIterator::getCurrentTask()
{
	if (m_nPost >= 0 && m_nPost < m_nCount)
	{
		return m_oTaskList[m_nPost];
	}

	return PacketMakerConfigInfo();
}

bool PacketMakerConfigInfoIterator::isDone()
{
	return (m_nPost >= m_nCount);
}

void PacketMakerConfigInfoIterator::next()
{
	++m_nPost;
}

void PacketMakerConfigInfoIterator::first()
{
	m_nPost = 0;
}

/*!
*@brief
*@author       maozg
*@time         2018��1��25��
*@param
*@return
*/
AutomaticPacketMaker::AutomaticPacketMaker(logFun pFun)
	:m_pTaskIterator(nullptr), m_pLogFun(pFun), m_isUseThread(true)
{

}

/*!
*@brief
*@author       maozg
*@time         2018��1��25��
*@param
*@return
*/
AutomaticPacketMaker::~AutomaticPacketMaker()
{
	if (nullptr != m_pTaskIterator)
	{
		delete m_pTaskIterator;
		m_pTaskIterator = nullptr;
	}
}

/*!
*@brief
*@author       maozg
*@time         2018��1��25��
*@param
*@return       void
*/
void AutomaticPacketMaker::start()
{
	//��ȡ�����ļ�
	readConfigFile();

	readThreadConfig();

	//ִ���������
	executeTask();
}

PacketMakerConfigInfo* AutomaticPacketMaker::getNextTask()
{
	PackeThreadMutexLocker oLoker(&m_oMutex);

	if (m_pTaskIterator->isDone())
	{
		return nullptr;
	}

	PacketMakerConfigInfo* pResult = &m_pTaskIterator->getCurrentTask();

	m_pTaskIterator->next();

	return pResult;
}

/*!
*@brief
*@author       maozg
*@time         2018��1��25��
*@param
*@return       void
*/
void AutomaticPacketMaker::readConfigFile()
{
	m_vecParameList.clear();
	QString strIniPath = PacketMakerCommon::binPath() + "/CMDTaskConfig.ini";

	QFile file(strIniPath);
	if (!file.open(QFile::ReadOnly))
	{
		return;
	}
	QTextStream read(&file);

	while (!read.atEnd())
	{
		QString strInfo = read.readLine();
		//�ж��Ƿ���ע��
		if (strInfo.mid(0, 1).compare(";") == 0)
		{
			continue;
		}

		QStringList strTask = strInfo.split('=');
		if (strTask.size() < 2)
		{
			continue;
		}

		//��ȡ��������
		if (strTask.at(0).compare("x86InstallPath") == 0)
		{
			m_oPacketMakerPublicParame.strx86InstallFilePath = strTask.at(1);
			continue;
		}
		else if (strTask.at(0).compare("x64InstallPath") == 0)
		{
			m_oPacketMakerPublicParame.strx64InstallFilePath = strTask.at(1);
			continue;
		}
		else if (strTask.at(0).compare("RulesPath") == 0)
		{
			m_oPacketMakerPublicParame.strRulesFilePath = strTask.at(1);
			continue;
		}
		else if (strTask.at(0).compare("Account") == 0)
		{
			m_oPacketMakerPublicParame.strUserName = strTask.at(1);
			continue;
		}
		else if (strTask.at(0).compare("Password") == 0)
		{
			m_oPacketMakerPublicParame.strUserPassWord = strTask.at(1);
			continue;
		}
		else if (strTask.at(0).compare("OutPutPath") == 0)
		{
			m_oPacketMakerPublicParame.strOutPutPath = strTask.at(1);
			continue;
		}

		QStringList strTaskParame = strTask.at(1).split(';');

		m_vecParameList.push_back(strTaskParame);
	}

	file.close();
}

void AutomaticPacketMaker::readThreadConfig()
{
	QString strThreadini = PacketMakerCommon::binPath() + "/thread.ini";
	QSettings config(strThreadini, QSettings::IniFormat);
	config.beginGroup("Thread");

	QString strIsThread = config.value("useThread").toString();
	if (strIsThread.toLower() == "true")
	{
		m_isUseThread = true;
	}
	else if (strIsThread.toLower() == "false")
	{
		m_isUseThread = false;
	}

	config.endGroup();
}

/*!
*@brief        ִ������
*@author       maozg
*@time         2018��1��25��
*@param
*@return       void
*/
void AutomaticPacketMaker::executeTask()
{
	m_oTaskList.clear();
	createTask(X86_Pack);
	createTask(X64_Pack);

	if (m_isUseThread)
	{
		//ʹ�ö��߳�
		executeThread();
	}
	else
	{
		for (size_t i = 0; i < m_oTaskList.size(); ++i)
		{
			//����ʽ���
			QString strLog = Chinese("=====>��ʼִ��%1�����������...").arg(m_oTaskList[i].strRegionRules);
			m_pLogFun(strLog);

			OutNetPacketMakerCMD cmdExe(m_oTaskList[i], m_pLogFun);
			cmdExe.startPacketMaker();
		}
	}
}

/*!
*@brief        ��������������
*@author       maozg
*@time         2018��1��29��
*@param        QStringList& strParameterList, PacketMakerConfigInfo& outInfo
*@return       bool
*/
bool AutomaticPacketMaker::mergePacketMakerParame(QStringList& strParameterList, PacketMakerConfigInfo& outInfo)
{
	if (strParameterList.size() < 5)
	{
		return false;
	}

	outInfo.strRulesFilePath = m_oPacketMakerPublicParame.strRulesFilePath;
	outInfo.strOuterNetPacketName = strParameterList.at(0);
	outInfo.bIsCoverInstall = strParameterList.at(1).toLower().compare("true") == 0 ? true : false;
	outInfo.bIsUninstall = strParameterList.at(2).toLower().compare("true") == 0 ? true : false;
	outInfo.bIsOnlineSum = strParameterList.at(3).toLower().compare("true") == 0 ? true : false;
	outInfo.strRegionRules = strParameterList.at(4);
	outInfo.strUserName = m_oPacketMakerPublicParame.strUserName;
	outInfo.strUserPassWord = m_oPacketMakerPublicParame.strUserPassWord;

	return true;
}

void AutomaticPacketMaker::createTask(outPackByte packByte)
{
	QString strInstallPath = "";
	QString strByte = "";

	switch (packByte)
	{
	case X86_Pack:
		strInstallPath = m_oPacketMakerPublicParame.strx86InstallFilePath;
		strByte = Chinese("32λ");
		break;
	case X64_Pack:
		strInstallPath = m_oPacketMakerPublicParame.strx64InstallFilePath;
		strByte = Chinese("64λ");
		break;
	default:

		break;
	}

    if (strInstallPath.isEmpty())
    {
        return;
    }

	for (int i = 0; i < m_vecParameList.size(); ++i)
	{
		QStringList strTaskParame = m_vecParameList.at(i);

		PacketMakerConfigInfo packetMakerInfo;
		packetMakerInfo.strInstallFilePath = strInstallPath;
		packetMakerInfo.strOutPutPath = m_oPacketMakerPublicParame.strOutPutPath + QString("/%1").arg(strByte);

		//��ϲ����б�
		if (!mergePacketMakerParame(strTaskParame, packetMakerInfo))
		{
			continue;
		}

		m_oTaskList.push_back(packetMakerInfo);
	}
}

void AutomaticPacketMaker::executeThread()
{
	m_pTaskIterator = new PacketMakerConfigInfoIterator(m_oTaskList);

	vector<PackMakeThread*> vecThread;

	//����8���߳�
	int nThreadCount = 8;
	for (int i = 0; i < nThreadCount; ++i)
	{
		PackMakeTask * pTask = new PackMakeTask(this);
		PackMakeThread* pThread = new PackMakeThread(pTask);

		vecThread.push_back(pThread);
	}

	m_pTaskIterator->first();
	//ִ���߳�
	for (int nThread = 0; nThread < nThreadCount; ++nThread)
	{
		vecThread[nThread]->start();
		Sleep(100);
	}

	//�ȴ��߳̽���
	for (int nWait = 0; nWait < nThreadCount; ++nWait)
	{
		vecThread[nWait]->wait();
	}

	for each (PackMakeThread* var in vecThread)
	{
		if (nullptr != var)
		{
			delete var;
			var = nullptr;
		}
	}

	vecThread.clear();
}

PackMakeTask::PackMakeTask(AutomaticPacketMaker* pTask)
	:m_pTask(pTask)
{

}

PackMakeTask::~PackMakeTask()
{

}

void PackMakeTask::onRunTask()
{
	while (true)
	{
		PacketMakerConfigInfo* pInfo = m_pTask->getNextTask();
		if (nullptr == pInfo)
		{
			break;
		}

		QString strLog = Chinese("=====>��ʼִ��%1�����������...").arg(pInfo->strRegionRules);
		m_pTask->m_pLogFun(strLog);

		OutNetPacketMakerCMD cmdExe(*pInfo, m_pTask->m_pLogFun);
		cmdExe.startPacketMaker();
	}
}

PackMakeThread::PackMakeThread(PackMakeTask* pPackTask)
	:m_pPackTask(pPackTask)
{

}

PackMakeThread::~PackMakeThread()
{
	if (nullptr != m_pPackTask)
	{
		delete m_pPackTask;
		m_pPackTask = nullptr;
	}
}

void PackMakeThread::run()
{
	try
	{
		m_pPackTask->onRunTask();
	}
	catch (const std::exception&)
	{
		//todo
	}
}