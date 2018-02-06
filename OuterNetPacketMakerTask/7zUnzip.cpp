/*!
*@file     7zUnzip.cpp
*@brief    7z��ѹ����
*@author   maozg
*@date     2017��12��29��
*@remarks
*@version 3.0
*Copyright (c) 1998-2017 Glodon Corporation
*/
#include "7zUnzip.h"
#include <QProcess>
#include <..\common\PacketMakerCommon.h>
#include <windows.h>

static inline QString cmd7zExe()
{
    return "7z.exe";
}

/*!
*@brief
*@author       maozg
*@time         2017��12��29��
*@param
*@return
*/
Dll7zUnzipCmd::Dll7zUnzipCmd()
{

}

/*!
*@brief
*@author       maozg
*@time         2017��12��29��
*@param
*@return
*/
Dll7zUnzipCmd::~Dll7zUnzipCmd()
{

}

/*!
*@brief        ����·����ѹ�����ļ�
*@author       maozg
*@time         2017��12��29��
*@param        const QString& strZipPath, const QString& strOutPath
*@return       bool
*/
bool Dll7zUnzipCmd::extractAllFile(const QString& strZipPath, const QString& strOutPath)
{
    QProcess pExe;
    QString strExe;
    strExe.append("\"");
    strExe.append(cmd7zExe());
    strExe.append("\" x \"");
    strExe.append(strZipPath);
    strExe.append("\" -y -o\"");
    strExe.append(strOutPath);
    strExe.append("\"");

    pExe.start(strExe);

    if (!pExe.waitForStarted())
    {
        pExe.close();
        return false;
    }

    if (!pExe.waitForFinished())
    {
        pExe.close();
        return false;
    }
    
    pExe.close();
    return true;
}

/*!
*@brief
*@author       maozg
*@time         2018��1��9��
*@param        const QString& strZipPath, const QString& strOutPath, const QString& strFileName
*@return       bool
*/
bool Dll7zUnzipCmd::extractSpecifiedFile(const QString& strZipPath, const QString& strOutPath, const QString& strFileName)
{
    //windows�����ⲿ����ķ���
//     QStringList strCMD;
//     strCMD.append(cmd7zExe());
//     strCMD.append(" e ");
// 
//     strCMD.append("\"");
//     strCMD.append(strZipPath);
//     strCMD.append("\"");
// 
// 
//     strCMD.append(" -y ");
//     strCMD.append(" -o");
// 
//     strCMD.append("\"");
//     strCMD.append(strOutPath);
//     strCMD.append("\"");
// 
//     strCMD.append(" ");
//     strCMD.append("\"");
//     strCMD.append(strFileName);
//     strCMD.append("\"");
// 
//     strCMD.append(" -r");
// 
// 
//     std::wstring wstrCMD = strCMD.join("").toStdWString();
// 
//     STARTUPINFO si = { sizeof(STARTUPINFO) };
//     si.dwFlags = STARTF_USESHOWWINDOW;
//     si.wShowWindow = SW_HIDE;
//     PROCESS_INFORMATION pi = { 0 };
//     WCHAR wszExe[MAX_PATH] = { 0 };
// 
//     CreateProcessW(nullptr, &wstrCMD[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
//     DWORD nProce = WaitForSingleObject(pi.hProcess, (DWORD)-1);
//     CloseHandle(pi.hProcess);
//     CloseHandle(pi.hThread);
// 
//     if (0 == nProce)
//     {
//         return true;
//     }
//     else
//     {
//         return false;
//     }

    QProcess pExe;
    //�����������
    QStringList strCmd;
    strCmd.append("e");
    strCmd.append(strZipPath);

    //�������ò���  //��Ҫע��ո� ����Ĳ����ᱻ�ָ� ����·��Ҫ�����Ű���
    QString strArg;
    strArg.append("-y");
    strArg.append(" -o");
    strArg.append("\"");
    strArg.append(strOutPath);
    strArg.append("\" \"");
    strArg.append(strFileName);
    strArg.append("\" -r");
    pExe.setNativeArguments(strArg);

    //ִ������
    pExe.start(cmd7zExe(), strCmd);

    if (!pExe.waitForStarted())
    {
        pExe.close();
        return false;
    }

    if (!pExe.waitForFinished())
    {
        pExe.close();
        return false;
    }

    pExe.close();

    //QString strTemp = Chinese(pExe.readAllStandardOutput());

    return true;
}

/*!
*@brief        ����ѹ�������ļ�
*@author       maozg
*@time         2018��1��17��
*@param        const QString& strZipPath, const QString& strFilePath
*@return       bool
*/
bool Dll7zUnzipCmd::updateFileToZip(const QString& strZipPath, const QString& strFilePath)
{
    QProcess pExe;
    QString strExe;
    strExe.append("\"");
    strExe.append(cmd7zExe());
    strExe.append("\"");

    strExe.append(" u \"");
    strExe.append(strZipPath);
    strExe.append("\" \"");
    strExe.append(strFilePath);
    strExe.append("\"");

    //ִ������
    pExe.start(strExe);

    if (!pExe.waitForStarted())
    {
        pExe.close();
        return false;
    }

    if (!pExe.waitForFinished())
    {
        pExe.close();
        return false;
    }

    pExe.close();

    return true;
}
