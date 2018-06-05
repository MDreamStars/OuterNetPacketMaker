/*!
*@file     PacketMkaerCommon.h
*@brief    ������
*@author   maozg
*@date     2017��12��29��
*@remarks
*@version 3.0
*Copyright (c) 1998-2017 Glodon Corporation
*/
#pragma once

#include <QString>
#include <QCoreApplication>
#include <QFile>
#include <QMutex>

typedef void(*logFun)(const QString&);

#define Chinese(str) QString::fromLocal8Bit(str)

const long c_LocalFileHeaderSignature = 0x04034B50;
const long c_FileHeaderSignature = 0x20444C47;
const int c_defMaxFileHeaderSize = 64;

static const char* RegionRules[] =
{
    "ȫ��",
    "������(��)","�����(��)","�Ϻ���(������)","������(��)", "������(��)", "�ӱ�ʡ(��)","ɽ��ʡ(��)","����ʡ(��)","����ʡ(��)",
    "������ʡ(��)","����ʡ(��)","�㽭ʡ(��)","����ʡ(��)","����ʡ(��)","����ʡ(��)","ɽ��ʡ(³)","����ʡ(ԥ)",
    "����ʡ(��)","����ʡ(��)","�㶫ʡ(��)","����ʡ(��)","�Ĵ�ʡ(������)","����ʡ(��ǭ)","����ʡ(�ᡢ��)",
    "����ʡ(�¡���)","����ʡ(�ʡ�¤)","�ຣʡ(��)","̨��ʡ(̨)","���ɹ�������(��)","����׳��������(��)",
    "����������(��)","���Ļ���������(��)","�½�ά���������(��)","����ر�������(��)","�����ر�������(��)"
};

//��������ö��
enum RegionRulesEnum
{
    AllRegionRules = 0,                //ȫ��
    BeiJingRegionRules,                //������������
    TianJinRegionRules,                //����������
    ShangHaiRegionRules,               //�Ϻ���������
    ChongQingRegionRules,              //�����������
    HeBeiRegionRules,                  //�ӱ���������
    ShanXiRegionRules,                 //ɽ����������
    LiaoNingRegionRules,               //������������
    JiLinRegionRules,                  //���ֵ�������
    HeiLongJiangRegionRules,           //��������������
    JiangSuRegionRules,                //���յ�������
    ZheJiangRegionRules,               //�㽭��������
    AanHuiRegionRules,                 //���յ�������
    FuJianRegionRules,                 //������������
    JiangXiRegionRules,                //������������
    ShanDongRegionRules,               //ɽ����������
    HeNanRegionRules,                  //���ϵ�������
    HuBeiRegionRules,                  //������������
    HuNanRegionRules,                  //���ϵ�������
    GuangDongRegionRules,              //�㶫��������
    HaiNanRegionRules,                 //���ϵ�������
    SiChuanRegionRules,                //�Ĵ���������
    GuiZhouRegionRules,                //���ݵ�������
    YunNanRegionRules,                 //���ϵ�������
    ChangAnRegionRules,                //������������
    GanSuRegionRules,                  //�����������
    QingHaiRegionRules,                //�ຣ��������
    TaiWanRegionRules,                 //̨���������
    NeiMengGuRegionRules,              //���ɹŵ�������
    GuangXiRegionRules,                //������������
    XiZangRegionRules,                 //���ص�������
    NingXiaRegionRules,                //���ĵ�������
    XinJiangRegionRules,               //�½���������
    HongKongRegionRules,               //��۵�������
    MacauRegionRules                   //���ŵ�������
};

struct PacketMakerConfigInfo
{
    QString strInstallFilePath;
    QString strRulesFilePath;
    QString strOuterNetPacketName;
    bool bIsCoverInstall;
    bool bIsUninstall;
    bool bIsOnlineSum;
    bool bIsSaveUserData;
    QString strUserName;
    QString strUserPassWord;
    QString strRegionRules;
    QString strOutPutPath;

    PacketMakerConfigInfo()
    {
        strInstallFilePath = "";
        strRulesFilePath = "";
        strOuterNetPacketName = "";
        bIsCoverInstall = false;
        bIsUninstall = false;
        bIsOnlineSum = false;
        bIsSaveUserData = true;
        strUserName = "";
        strUserPassWord = "";
        strRegionRules = "";
        strOutPutPath = "";
    }
};

struct AutomaticPacketMakerPublicParame
{
    QString strx86InstallFilePath;
	QString strx64InstallFilePath;
    QString strInstallPath;
    QString strRulesFilePath;
    QString strUserName;
    QString strUserPassWord;
    QString strOutPutPath;
    QString strByte;

    AutomaticPacketMakerPublicParame()
    {
		strx86InstallFilePath = "";
		strx64InstallFilePath = "";
        strInstallPath = "";
        strRulesFilePath = "";
        strUserName = "";
        strUserPassWord = "";
        strOutPutPath = "";
        strByte = "";
    }
};

enum fileType
{
    typeIsIllegal = -1,
    typeIsZip,
    typeIsDir,
    typeIsExe
};

//������λ��
enum outPackByte
{
	X86_Pack,
	X64_Pack
};

class Q_DECL_EXPORT PackeThreadMutex : public QMutex
{
public:
	PackeThreadMutex();
	~PackeThreadMutex();
};

class Q_DECL_EXPORT PackeThreadMutexLocker : public QMutexLocker
{
public:
	PackeThreadMutexLocker(PackeThreadMutex* pMutex);
	~PackeThreadMutexLocker();
};

class Q_DECL_EXPORT PacketMakerCommon
{
public:
    //��ȡ��ǰexe��·��
    static QString binPath() { return QCoreApplication::applicationDirPath(); };
    static fileType checkFileType(const QString &strFile);
    static QString findFilesDirPath(const QString& strDir, const QString& strDirName);

    //��������
//     static bool mergerMakerCmd(const QString& strWorkPath, const QString& strSrcInstall, const QString& strNewInstall,
//         const QString& strUserName, const QString& strPassWard);
    //ǩ����װ��
    static bool signInstallPacket(const QString& strInstallPath);
    
    //ɾ���ļ�Ŀ¼
    static bool removeDir(const QString& strDir);
    //���Ŀ¼�Ƿ���ڣ���������ھʹ���һ��
    static void checkDirExists(const QString& strDir);
private:
    static bool isZipFile(const QString &file);
    static bool isZipStream(QFile *stream);
    static void removeDirAndFiles(const QString& strFileDir);
};

bool mergerMakerCmd(const QString& strSrcInstall, const QString& strNewInstall,
	const QString& strUserName, const QString& strPassWard);