/*!
*@file     PacketMkaerCommon.h
*@brief    公共类
*@author   maozg
*@date     2017年12月29日
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
    "全国",
    "北京市(京)","天津市(津)","上海市(沪、申)","重庆市(渝)", "深圳市(深)", "河北省(冀)","山西省(晋)","辽宁省(辽)","吉林省(吉)",
    "黑龙江省(黑)","江苏省(苏)","浙江省(浙)","安徽省(皖)","福建省(闽)","江西省(赣)","山东省(鲁)","河南省(豫)",
    "湖北省(鄂)","湖南省(湘)","广东省(粤)","海南省(琼)","四川省(川、蜀)","贵州省(贵、黔)","云南省(滇、云)",
    "陕西省(陕、秦)","甘肃省(甘、陇)","青海省(青)","台湾省(台)","内蒙古自治区(蒙)","广西壮族自治区(桂)",
    "西藏自治区(藏)","宁夏回族自治区(宁)","新疆维吾尔自治区(新)","香港特别行政区(港)","澳门特别行政区(澳)"
};

//地区规则枚举
enum RegionRulesEnum
{
    AllRegionRules = 0,                //全国
    BeiJingRegionRules,                //北京地区规则
    TianJinRegionRules,                //天津地区规则
    ShangHaiRegionRules,               //上海地区规则
    ChongQingRegionRules,              //重庆地区规则
    HeBeiRegionRules,                  //河北地区规则
    ShanXiRegionRules,                 //山西地区规则
    LiaoNingRegionRules,               //辽宁地区规则
    JiLinRegionRules,                  //吉林地区规则
    HeiLongJiangRegionRules,           //黑龙江地区规则
    JiangSuRegionRules,                //江苏地区规则
    ZheJiangRegionRules,               //浙江地区规则
    AanHuiRegionRules,                 //安徽地区规则
    FuJianRegionRules,                 //福建地区规则
    JiangXiRegionRules,                //江西地区规则
    ShanDongRegionRules,               //山东地区规则
    HeNanRegionRules,                  //河南地区规则
    HuBeiRegionRules,                  //湖北地区规则
    HuNanRegionRules,                  //湖南地区规则
    GuangDongRegionRules,              //广东地区规则
    HaiNanRegionRules,                 //海南地区规则
    SiChuanRegionRules,                //四川地区规则
    GuiZhouRegionRules,                //贵州地区规则
    YunNanRegionRules,                 //云南地区规则
    ChangAnRegionRules,                //陕西地区规则
    GanSuRegionRules,                  //甘肃地区规则
    QingHaiRegionRules,                //青海地区规则
    TaiWanRegionRules,                 //台湾地区规则
    NeiMengGuRegionRules,              //内蒙古地区规则
    GuangXiRegionRules,                //广西地区规则
    XiZangRegionRules,                 //西藏地区规则
    NingXiaRegionRules,                //宁夏地区规则
    XinJiangRegionRules,               //新疆地区规则
    HongKongRegionRules,               //香港地区规则
    MacauRegionRules                   //澳门地区规则
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

//外网包位数
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
    //获取当前exe的路径
    static QString binPath() { return QCoreApplication::applicationDirPath(); };
    static fileType checkFileType(const QString &strFile);
    static QString findFilesDirPath(const QString& strDir, const QString& strDirName);

    //组外网包
//     static bool mergerMakerCmd(const QString& strWorkPath, const QString& strSrcInstall, const QString& strNewInstall,
//         const QString& strUserName, const QString& strPassWard);
    //签名安装包
    static bool signInstallPacket(const QString& strInstallPath);
    
    //删除文件目录
    static bool removeDir(const QString& strDir);
    //检查目录是否存在，如果不存在就创建一个
    static void checkDirExists(const QString& strDir);
private:
    static bool isZipFile(const QString &file);
    static bool isZipStream(QFile *stream);
    static void removeDirAndFiles(const QString& strFileDir);
};

bool mergerMakerCmd(const QString& strSrcInstall, const QString& strNewInstall,
	const QString& strUserName, const QString& strPassWard);