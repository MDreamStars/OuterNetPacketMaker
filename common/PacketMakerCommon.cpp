/*!
*@file     PacketMkaerCommon.h
*@brief    ������нӿ�
*@author   maozg
*@date     2018��1��12��
*@remarks
*@version 3.0
*Copyright (c) 1998-2018 Glodon Corporation
*/
#include "PacketMakerCommon.h"
#include <QFileInfo>
#include <set>
#include <QDir>
#include <QFileInfoList>
#include <QByteArray>
#include <windows.h>
#include <QProcess>

/*!
*@brief        �����ļ�����
*@author       maozg
*@time         2018��1��12��
*@param        const QString &strFile
*@return       fileType
*/
fileType PacketMakerCommon::checkFileType(const QString &strFile)
{
    QFileInfo fileInfo(strFile);
    std::set<QString> fileSuffix;

    fileSuffix.insert("zip");
    fileSuffix.insert("rar");
    fileSuffix.insert("7z");

    if (fileInfo.isFile())
    {
        QString strFileSuffix = fileInfo.suffix();
        if (fileSuffix.find(strFileSuffix) != fileSuffix.end())
        {
            return typeIsZip;
        }
        else if (fileInfo.isExecutable())
        {
            return typeIsExe;
        }
    }
    else if (fileInfo.isDir())
    {
        return typeIsDir;
    }

    return typeIsIllegal;
}

/*!
*@brief        ����ָ��Ŀ¼�����ļ���·��
*@author       maozg
*@time         2018��1��16��
*@param        const QString& strDir, const QString& strDirName
*@return       QT_NAMESPACE::QString
*/
QString PacketMakerCommon::findFilesDirPath(const QString& strDir, const QString& strDirName)
{
    QString strResult = "";
    QDir strDirPath(strDir);

    if (!strDirPath.exists())
    {
        return strResult;
    }

    strDirPath.setFilter(QDir::Dirs | QDir::Files);
    strDirPath.setSorting(QDir::DirsFirst);
    QStringList strList;
    strList.append(strDirName);

    QFileInfoList strDirFileList = strDirPath.entryInfoList(strList);

    if (!strDirFileList.isEmpty())
    {
        strResult = strDirFileList.at(0).filePath();
    }

    return strResult;
}


/*!
*@brief        �ϲ���װ��
*@author       maozg
*@time         2018��1��18��
*@param        const QString& strSrcInstall, const QString& strNewInstall, const QString& strUserName, const QString& strPassWard
*@return       bool
*/
bool PacketMakerCommon::mergerMakerCmd(const QString& strSrcInstall, const QString& strNewInstall, 
    const QString& strUserName, const QString& strPassWard)
{
    QProcess pExe;

    QString strExe;
    strExe.append("\"");
    strExe.append("MergerMaker.exe");
    strExe.append("\" \"");
    strExe.append(strSrcInstall);
    strExe.append("\" \"");
    strExe.append(strNewInstall);
    strExe.append("\" \"");
    strExe.append(strUserName);
    strExe.append("\" \"");
    strExe.append(strPassWard);
    strExe.append("\"");

    //ִ������
    pExe.start(strExe);

    if (!pExe.waitForStarted())
    {
        pExe.close();
        return false;
    }

    if (!pExe.waitForFinished(40000))
    {
        pExe.close();
        return false;
    }

    pExe.close();
    return true;
}

/*!
*@brief        ǩ����װ��
*@author       maozg
*@time         2018��1��18��
*@param        const QString& strInstallPath
*@return       bool
*/
bool PacketMakerCommon::signInstallPacket(const QString& strInstallPath)
{
    if (typeIsExe != PacketMakerCommon::checkFileType(strInstallPath))
    {
        //���ǿ�ִ�г���
        return false;
    }

    QProcess pExe;

    QString strExe;
    strExe.append("\"");
    strExe.append("signfile.exe");
    strExe.append("\" \"");
    strExe.append(strInstallPath);
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


/*!
*@brief        ɾ��Ŀ¼
*@author       maozg
*@time         2018��1��18��
*@param        const QString& strDir
*@return       bool
*/
bool PacketMakerCommon::removeDir(const QString& strDir)
{
    QDir remDir(strDir);
    if (!remDir.exists())
    {
        return false;
    }

    removeDirAndFiles(strDir);

    return true;
}

/*!
*@brief
*@author       maozg
*@time         2018��1��25��
*@param        const QString& strDir
*@return       void
*/
void PacketMakerCommon::checkDirExists(const QString& strDir)
{
    QDir dir(strDir);

    if (!dir.exists())
    {
        dir.mkdir(strDir);
    }
}

/*!
*@brief
*@author       maozg
*@time         2018��1��17��
*@param        const QString &file
*@return       bool
*/
bool PacketMakerCommon::isZipFile(const QString &file)
{
    bool result = false;

    QFile fileStream(file);
    try
    {
        if (!fileStream.open(QFile::ReadOnly))
        {
            fileStream.close();
            return false;
        }
        fileStream.seek(0);
        result = isZipStream(&fileStream);
    }
    catch (...)
    {
        fileStream.close();
        throw;
    }
    fileStream.close();
    return result;
}

int posData(int data, int len, const QByteArray &s)
{
    QByteArray strData = QByteArray();
    strData = strData.append((char *)&data, len);
    QString res = QString(s);
    QString des = QString(strData);
    return res.indexOf(des);
}

/*!
*@brief
*@author       maozg
*@time         2018��1��17��
*@param        QString *stream
*@return       bool
*/
bool PacketMakerCommon::isZipStream(QFile *stream)
{
    bool bresult = false;
    unsigned long nLen(0);
    INT64 nPos(0);
    INT64 nPosition(0);
    unsigned long uSignature;
    try
    {
        stream->read((char *)&uSignature, sizeof(uSignature));
        if (uSignature == (unsigned long)c_LocalFileHeaderSignature)
            bresult = true;
        else if (uSignature == (unsigned long)c_FileHeaderSignature)
        {
            stream->read((char *)&nLen, sizeof(unsigned long));
            nPosition = stream->pos();
            stream->seek(nPosition + nLen);
            stream->read((char *)&uSignature, sizeof(unsigned long));
            bresult = (uSignature == (unsigned long)c_LocalFileHeaderSignature);
        }
        else
        {
            QByteArray strBuffer;
            strBuffer.resize(c_defMaxFileHeaderSize);
            strBuffer.append((char *)&uSignature, sizeof(unsigned long));
            nLen = stream->read(strBuffer.data() + sizeof(unsigned long), strBuffer.length() - sizeof(unsigned long));
            nLen += sizeof(unsigned long);
            strBuffer.resize(nLen);
            nPos = posData(c_LocalFileHeaderSignature, sizeof(unsigned long), strBuffer);
            bresult = nPos > 0;
            if (bresult)
            {
                nPosition = stream->pos();
                stream->seek(nPosition + nPos - nLen + sizeof(unsigned long) - 1);
            }
        }
    }
    catch (...)
    {
        bresult = false;
    }
    return bresult;
}

/*!
*@brief
*@author       maozg
*@time         2018��1��18��
*@param        const QString& strFileDir
*@return
*/
void PacketMakerCommon::removeDirAndFiles(const QString& strFileDir)
{
    QDir             dir(strFileDir);
    QFileInfoList    fileList;
    QFileInfo        curFile;
    QFileInfoList    fileListTemp;
    int            infoNum;
    int            i;
    int            j;
    /* ���Ȼ�ȡĿ���ļ����������ļ����ļ�����Ϣ */
    fileList = dir.entryInfoList(QDir::Dirs | QDir::Files
        | QDir::Readable | QDir::Writable
        | QDir::Hidden | QDir::NoDotAndDotDot
        , QDir::Name);

    while (fileList.size() > 0)
    {
        infoNum = fileList.size();

        for (i = infoNum - 1; i >= 0; i--)
        {
            curFile = fileList[i];
            if (curFile.isFile()) /* ������ļ���ɾ���ļ� */
            {
                QFile fileTemp(curFile.filePath());
                fileTemp.remove();
                fileList.removeAt(i);
            }

            if (curFile.isDir()) /* ������ļ��� */
            {
                QDir dirTemp(curFile.filePath());
                fileListTemp = dirTemp.entryInfoList(QDir::Dirs | QDir::Files
                    | QDir::Readable | QDir::Writable
                    | QDir::Hidden | QDir::NoDotAndDotDot
                    , QDir::Name);
                if (fileListTemp.size() == 0) /* �²�û���ļ����ļ��� ��ֱ��ɾ��*/
                {
                    dirTemp.rmdir(".");
                    fileList.removeAt(i);
                }
                else /* �²����ļ��л��ļ� ����Ϣ��ӵ��б�*/
                {
                    for (j = 0; j < fileListTemp.size(); j++)
                    {
                        if (!(fileList.contains(fileListTemp[j])))
                        {
                            fileList.append(fileListTemp[j]);
                        }
                    }
                }
            }
        }
    }

    dir.rmdir(".");/*ɾ��Ŀ���ļ���,���ֻ������ļ���folderFullPath�����ݶ���ɾ��folderFullPath����,��ɾ�����м���*/
}

PackeThreadMutex::PackeThreadMutex()
	:QMutex()
{

}

PackeThreadMutex::~PackeThreadMutex()
{

}

PackeThreadMutexLocker::PackeThreadMutexLocker(PackeThreadMutex* pMutex)
	:QMutexLocker(pMutex)
{

}

PackeThreadMutexLocker::~PackeThreadMutexLocker()
{

}
