#pragma once
#include "..\common\PacketMakerCommon.h"
#include <vector>
#include <QTextStream>
#include <QSettings>
#include <QThread>
#include <QObject>
#include <..\OuterNetPacketMakerTask\OuterNetPakcetMakerTask.h>

using namespace std;

class PacketMakerConfigInfoIterator
{
public:
	PacketMakerConfigInfoIterator(vector<PacketMakerConfigInfo>& taskList);
	~PacketMakerConfigInfoIterator();

	PacketMakerConfigInfo& getCurrentTask();
	bool isDone();
	void next();
	void first();
private:
	int m_nCount;
	int m_nPost;
	vector<PacketMakerConfigInfo> m_oTaskList;
};

/*
* @brief 自动执行组包
*
* @author maozg  2018年1月25日
*
* @class AutomaticPacketMaker
*/
class AutomaticPacketMaker
{
public:
	AutomaticPacketMaker(logFun pFun);
	~AutomaticPacketMaker();
public:
	void start();
	PacketMakerConfigInfo* getNextTask();
private:
	void readConfigFile();
	void readThreadConfig();
	void executeTask();
	bool mergePacketMakerParame(QStringList& strParameterList, PacketMakerConfigInfo& outInfo);
	void createTask(outPackByte packByte);
	void executeThread();
public:
	logFun m_pLogFun;
private:
	vector<QStringList> m_vecParameList;
	AutomaticPacketMakerPublicParame m_oPacketMakerPublicParame;
	vector<PacketMakerConfigInfo> m_oTaskList;
	PacketMakerConfigInfoIterator* m_pTaskIterator;
	PackeThreadMutex m_oMutex;
	bool m_isUseThread;
};

class PackMakeTask : public QObject
{
public:
	PackMakeTask(AutomaticPacketMaker* pTask);
	~PackMakeTask();

	void onRunTask();

private:
	AutomaticPacketMaker * m_pTask;
};

class PackMakeThread : public QThread
{
public:
	PackMakeThread(PackMakeTask* pPackTask);
	~PackMakeThread();

	void run();

private:
	PackMakeTask * m_pPackTask;
};
