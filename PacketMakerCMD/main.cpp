/*!
*@file     
*@brief    外网组包命令入口
*@author   maozg
*@date     2018年1月24日
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

//测试使用
void outPutParameter(const PacketMakerConfigInfo& outInfo)
{
     wcout << QString("安装包路径:").toStdWString() << outInfo.strInstallFilePath.toStdWString() << endl;
     wcout << QString("地区规则路径:").toStdWString() << outInfo.strRulesFilePath.toStdWString() << endl;
     wcout << QString("外网包名称:").toStdWString() << outInfo.strOuterNetPacketName.toStdWString() << endl;
     wcout << QString("覆盖安装:").toStdWString() << outInfo.bIsCoverInstall << endl;
     wcout << QString("卸载安装:").toStdWString() << outInfo.bIsUninstall << endl;
     wcout << QString("是否携带联机汇总:").toStdWString() << outInfo.bIsOnlineSum << endl;
     wcout << QString("地区规则:").toStdWString() << outInfo.strRegionRules.toStdWString() << endl;
     wcout << QString("域账户:").toStdWString() << outInfo.strUserName.toStdWString() << endl;
     wcout << QString("域密码:").toStdWString() << outInfo.strUserPassWord.toStdWString() << endl;
     wcout << QString("外网包输出路径:").toStdWString() << outInfo.strOutPutPath.toStdWString() << endl;
}

//输出执行日志
static void setLog(const QString& strLog)
{
    QString strTemp = QString("==========> %1 <=========").arg(strLog);
    wcout << strTemp.toStdWString() << endl;
}

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
*@time         2018年1月25日
*@param
*@return
*/
AutomaticPacketMaker::AutomaticPacketMaker()
{

}

/*!
*@brief
*@author       maozg
*@time         2018年1月25日
*@param
*@return
*/
AutomaticPacketMaker::~AutomaticPacketMaker()
{

}

/*!
*@brief
*@author       maozg
*@time         2018年1月25日
*@param
*@return       void
*/
void AutomaticPacketMaker::start()
{
    //读取配置文件
    readConfigFile();

    //执行组包任务
    executeTask();
}

/*!
*@brief
*@author       maozg
*@time         2018年1月25日
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
        //判断是否是注释
        if (strInfo.mid(0, 1).compare(";") == 0)
        {
            continue;
        }

        QStringList strTask = strInfo.split('=');
        if (strTask.size() < 2)
        {
            continue;
        }

        //读取公共配置
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
*@brief        执行任务
*@author       maozg
*@time         2018年1月25日
*@param
*@return       void
*/
void AutomaticPacketMaker::executeTask()
{
    for (int i =0; i < m_vecParameList.size(); ++i)
    {
        QStringList strTaskParame = m_vecParameList.at(i);

        PacketMakerConfigInfo packetMakerInfo;
        //组合参数列表
        if (!mergePacketMakerParame(strTaskParame, packetMakerInfo))
        {
            continue;
        }

        wcout << QString("=====>开始执行%1规则组包任务...").arg(packetMakerInfo.strRegionRules).toStdWString() << endl;
        wcout << QString("=====>开始组外网安装包...").toStdWString() << endl;
        OutNetPacketMakerCMD cmdExe(packetMakerInfo, setLog);
        cmdExe.startPacketMaker();
    }
}

/*!
*@brief        组合组包参数参数
*@author       maozg
*@time         2018年1月29日
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

    //首先检测是否是以命令方式执行,入参携带参数大于1个（因为默认会传入一个参数是exe路径）
    QStringList strList;
    for (int i = 1; i < argc; ++i)
    {
        QString strParam = Chinese(argv[i]);
        strList.append(strParam);
    }

    //测试使用代码
//     strList.append(QString("D:/Install.exe"));
//     strList.append(QString("D:/GTJ2018-全国规则库-1.0.12.0-1.0.0.15-260个.rar"));
//     strList.append(QString("GTJ2018全国规则卸载安装不包含联机汇总"));
//     strList.append(QString("false"));
//     strList.append(QString("true"));
//     strList.append(QString("false"));
//     strList.append(QString("四川"));
//     strList.append(QString("maozg"));
//     strList.append(QString("mzg@*963."));
//     strList.append(QString("D:/外网包"));

    if (!strList.isEmpty())
    {
        PacketMakerConfigInfo packetMakerInfo;
        //带有参数
        if (!loadPacketMakerInfo(strList, packetMakerInfo))
        {
            return -1;
        }

        outPutParameter(packetMakerInfo);

        wcout << QString("=======>开始组外网安装包...").toStdWString() << endl;
        OutNetPacketMakerCMD cmdExe(packetMakerInfo, setLog);
        cmdExe.startPacketMaker();

    }
    else
    {
        //按照配置文件的配置，自动执行组包
        AutomaticPacketMaker().start();
    }

    return 0;
}

