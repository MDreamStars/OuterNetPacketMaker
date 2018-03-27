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
#include <iostream>
#include <map>
#include "PacketMakerCmdTask.h"

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
        AutomaticPacketMaker(setLog).start();
    }

    return 0;
}

