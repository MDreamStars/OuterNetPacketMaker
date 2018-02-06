/*!
*@file     
*@brief    ��������������
*@author   maozg
*@date     2018��1��24��
*@remarks
*@version 3.0
*Copyright (c) 1998-2018 Glodon Corporation
*/
#include <QtCore/QCoreApplication>
#include <..\OuterNetPacketMakerTask\OuterNetPakcetMakerTask.h>
#include "..\common\PacketMakerCommon.h"
#include <iostream>
#include <QObject>
#include <map>
#include <QTextStream>
#include <QSettings>

#if defined(_MSC_VER) && (_MSC_VER >= 1600)    
# pragma execution_character_set("utf-8")    
#endif  

using namespace std;

bool loadPacketMakerInfo(QStringList& strParameterList, PacketMakerConfigInfo& outInfo)
{
    if (strParameterList.size() < 10)
    {
        return false;
    }

    outInfo.strInstallFilePath = strParameterList.at(0);
    outInfo.strRulesFilePath = strParameterList.at(1);
    outInfo.strOuterNetPacketName = strParameterList.at(2);
    outInfo.bIsCoverInstall = strParameterList.at(3).toLower().compare("true") == 0 ? true : false;
    outInfo.bIsUninstall = strParameterList.at(4).toLower().compare("true") == 0 ? true : false;
    outInfo.bIsOnlineSum = strParameterList.at(5).toLower().compare("true") == 0 ? true : false;
    outInfo.strRegionRules = strParameterList.at(6);
    outInfo.strUserName = strParameterList.at(7);
    outInfo.strUserPassWord = strParameterList.at(8);
    outInfo.strOutPutPath = strParameterList.at(9);

    return true;
}

//����ʹ��
void outPutParameter(const PacketMakerConfigInfo& outInfo)
{
     wcout << QString("��װ��·��:").toStdWString() << outInfo.strInstallFilePath.toStdWString() << endl;
     wcout << QString("��������·��:").toStdWString() << outInfo.strRulesFilePath.toStdWString() << endl;
     wcout << QString("����������:").toStdWString() << outInfo.strOuterNetPacketName.toStdWString() << endl;
     wcout << QString("���ǰ�װ:").toStdWString() << outInfo.bIsCoverInstall << endl;
     wcout << QString("ж�ذ�װ:").toStdWString() << outInfo.bIsUninstall << endl;
     wcout << QString("�Ƿ�Я����������:").toStdWString() << outInfo.bIsOnlineSum << endl;
     wcout << QString("��������:").toStdWString() << outInfo.strRegionRules.toStdWString() << endl;
     wcout << QString("���˻�:").toStdWString() << outInfo.strUserName.toStdWString() << endl;
     wcout << QString("������:").toStdWString() << outInfo.strUserPassWord.toStdWString() << endl;
     wcout << QString("���������·��:").toStdWString() << outInfo.strOutPutPath.toStdWString() << endl;
}

//���ִ����־
static void setLog(const QString& strLog)
{
    QString strTemp = QString("==========> %1 <=========").arg(strLog);
    wcout << strTemp.toStdWString() << endl;
}

/*
* @brief �Զ�ִ�����
*
* @author maozg  2018��1��25��
*
* @class AutomaticPacketMaker
*/
class AutomaticPacketMaker
{
public:
    AutomaticPacketMaker();
    ~AutomaticPacketMaker();
public:
    void start();
private:
    void readConfigFile();
    void executeTask();
    bool mergePacketMakerParame(QStringList& strParameterList, PacketMakerConfigInfo& outInfo);
private:
    vector<QStringList> m_vecParameList;
    AutomaticPacketMakerPublicParame m_oPacketMakerPublicParame;
};

/*!
*@brief
*@author       maozg
*@time         2018��1��25��
*@param
*@return
*/
AutomaticPacketMaker::AutomaticPacketMaker()
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

    //ִ���������
    executeTask();
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
        if (strTask.at(0).compare("InstallPath") == 0)
        {
            m_oPacketMakerPublicParame.strInstallFilePath = strTask.at(1);
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
        else if (strTask.at(0).compare("Byte") == 0)
        {
            m_oPacketMakerPublicParame.strByte = strTask.at(1);
            continue;
        }

        QStringList strTaskParame = strTask.at(1).split(';');

        m_vecParameList.push_back(strTaskParame);
    }

    file.close();
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
    for (int i =0; i < m_vecParameList.size(); ++i)
    {
        QStringList strTaskParame = m_vecParameList.at(i);

        PacketMakerConfigInfo packetMakerInfo;
        //��ϲ����б�
        if (!mergePacketMakerParame(strTaskParame, packetMakerInfo))
        {
            continue;
        }

        wcout << QString("=====>��ʼִ��%1�����������...").arg(packetMakerInfo.strRegionRules).toStdWString() << endl;
        wcout << QString("=====>��ʼ��������װ��...").toStdWString() << endl;
        OutNetPacketMakerCMD cmdExe(packetMakerInfo, setLog);
        cmdExe.startPacketMaker();
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

    outInfo.strInstallFilePath = m_oPacketMakerPublicParame.strInstallFilePath;
    outInfo.strRulesFilePath = m_oPacketMakerPublicParame.strRulesFilePath;
    outInfo.strOuterNetPacketName = strParameterList.at(0) + m_oPacketMakerPublicParame.strByte;
    outInfo.bIsCoverInstall = strParameterList.at(1).toLower().compare("true") == 0 ? true : false;
    outInfo.bIsUninstall = strParameterList.at(2).toLower().compare("true") == 0 ? true : false;
    outInfo.bIsOnlineSum = strParameterList.at(3).toLower().compare("true") == 0 ? true : false;
    outInfo.strRegionRules = strParameterList.at(4);
    outInfo.strUserName = m_oPacketMakerPublicParame.strUserName;
    outInfo.strUserPassWord = m_oPacketMakerPublicParame.strUserPassWord;
    outInfo.strOutPutPath = m_oPacketMakerPublicParame.strOutPutPath;

    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setlocale(LC_CTYPE, "chs");

    //���ȼ���Ƿ��������ʽִ��,���Я����������1������ΪĬ�ϻᴫ��һ��������exe·����
    QStringList strList;
    for (int i = 1; i < argc; ++i)
    {
        QString strParam = Chinese(argv[i]);
        strList.append(strParam);
    }

    //����ʹ�ô���
//     strList.append(QString("D:/Install.exe"));
//     strList.append(QString("D:/GTJ2018-ȫ�������-1.0.12.0-1.0.0.15-260��.rar"));
//     strList.append(QString("GTJ2018ȫ������ж�ذ�װ��������������"));
//     strList.append(QString("false"));
//     strList.append(QString("true"));
//     strList.append(QString("false"));
//     strList.append(QString("�Ĵ�"));
//     strList.append(QString("maozg"));
//     strList.append(QString("mzg@*963."));
//     strList.append(QString("D:/������"));

    if (!strList.isEmpty())
    {
        PacketMakerConfigInfo packetMakerInfo;
        //���в���
        if (!loadPacketMakerInfo(strList, packetMakerInfo))
        {
            return -1;
        }

        outPutParameter(packetMakerInfo);

        wcout << QString("=======>��ʼ��������װ��...").toStdWString() << endl;
        OutNetPacketMakerCMD cmdExe(packetMakerInfo, setLog);
        cmdExe.startPacketMaker();

    }
    else
    {
        //���������ļ������ã��Զ�ִ�����
        AutomaticPacketMaker().start();
    }

    return 0;
}

