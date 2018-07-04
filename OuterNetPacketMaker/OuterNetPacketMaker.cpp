/*!
*@file     OuterNetPacketMaker.cpp
*@brief    ������
*@author   maozg
*@date     2018��1��3��
*@remarks
*@version 3.0
*Copyright (c) 1998-2018 Glodon Corporation
*/
#include "OuterNetPacketMaker.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QList>
#include <QMessageBox>
#include <QLabel>
#include <QFont>
#include <QTableWidgetItem>
#include <QAbstractAnimation>
#include <QFileDialog>
#include <QSettings>

/*!
*@brief
*@author       maozg
*@time         2018��1��3��
*@param        QWidget* parent /*= nullptr*/
/*@return
* */
DragDropLineEdit::DragDropLineEdit(QWidget* parent /*= nullptr*/)
{
    setAcceptDrops(true);
}

/*!
*@brief
*@author       maozg
*@time         2018��1��3��
*@param
*@return
*/
DragDropLineEdit::~DragDropLineEdit()
{

}

/*!
*@brief
*@author       maozg
*@time         2018��1��3��
*@param        QDragEnterEvent *event
*@return       void
*/
void DragDropLineEdit::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls() || event->mimeData()->hasText())
    {
        event->acceptProposedAction();
    }
}

/*!
*@brief
*@author       maozg
*@time         2018��1��3��
*@param        QDropEvent *event
*@return       void
*/
void DragDropLineEdit::dropEvent(QDropEvent *event)
{
    
    if (event->mimeData()->hasUrls())
    {
        QList<QUrl> urlList = event->mimeData()->urls();
        if (!checkIsFile(urlList))
        {
            QMessageBox::critical(nullptr, Chinese("�����������"), Chinese("ֻ֧���Ϸ��ļ�,�����²���!"),
                QMessageBox::Ok, QMessageBox::Ok);

            return;
        }

        if (urlList.size() > 1)
        {
            QMessageBox::critical(nullptr, Chinese("�����������"), Chinese("��֧���ϷŶ���ļ�,�����²���!"),
                QMessageBox::Ok, QMessageBox::Ok);

            return;
        }

        QList<QUrl>::iterator itr = urlList.begin();
        
        QString strPath = itr->path();
        strPath = strPath.remove(0, 1);
        this->setText(strPath);
    }
    else if (event->mimeData()->hasText())
    {
        this->setText(event->mimeData()->text());
    }
}

/*!
*@brief
*@author       maozg
*@time         2018��1��3��
*@param        QList<QUrl>& olist
*@return       bool
*/
bool DragDropLineEdit::checkIsFile(QList<QUrl>& olist)
{
    for each (QUrl var in olist)
    {
        if (!var.isLocalFile())
        {
            return false;
        }
    }
    return true;
}

/*!
*@brief
*@author       maozg
*@time         2018��1��3��
*@param        const QString &text, QLineEdit *edit
*@return
*/
SelectRulesButton::SelectRulesButton(const QString &text, QLineEdit *edit)
    :QPushButton(text, edit)
{
    setText(text);
    setFixedSize(40, edit->sizeHint().height());
    setFocusPolicy(Qt::NoFocus);
    setCursor(QCursor(Qt::PointingHandCursor));

    QHBoxLayout* pHLayout = new QHBoxLayout();
    pHLayout->setContentsMargins(0, 0, 0, 0);
    pHLayout->addStretch();
    pHLayout->addWidget(this);
    edit->setLayout(pHLayout);

}

/*!
*@brief
*@author       maozg
*@time         2018��1��3��
*@param
*@return
*/
SelectRulesButton::~SelectRulesButton()
{

}

/*!
*@brief
*@author       maozg
*@time         2018��1��3��
*@param        QWidget *parent /*= Q_NULLPTR*/
/*@return
* * */
OuterNetPacketMaker::OuterNetPacketMaker(QWidget *parent /*= Q_NULLPTR*/)
    :QWidget(parent), m_pInstallEdit(nullptr), m_pRulesPathEdit(nullptr),
    m_pSelectRegionRules(nullptr), m_pOutPutPathEdit(nullptr),
    m_pDomainUserNamEdit(nullptr), m_pPassWordEdit(nullptr), m_pCoverInstall(nullptr),
    m_pUninstall(nullptr), m_pOnlineSumCheckBox(nullptr), m_pSaveUserData(nullptr),
    m_pProgressDlg(nullptr), m_pPacketMakerTask(nullptr), m_pVersionEdit(nullptr)
{
    loadUserInfo();
    iniUI();
}

/*!
*@brief
*@author       maozg
*@time         2018��1��3��
*@param
*@return       void
*/
void OuterNetPacketMaker::iniUI()
{
    this->setWindowTitle(Chinese("�����������"));
    setAcceptDrops(false);
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::red);

    QVBoxLayout* pMainLayout = new QVBoxLayout(this);

    QHBoxLayout* pPublicItem = new QHBoxLayout(this);
    QVBoxLayout* pInstallLayout = new QVBoxLayout(this);

    QGroupBox* pPublicGroup = new QGroupBox(Chinese("������"), this);
    //������
    QGroupBox* pInputGroup = new QGroupBox(Chinese("·����Ϣ"), this);
    QGridLayout* pInputLayout = new QGridLayout(this);
    QLabel* pInstallLabel = new QLabel(Chinese("��װ��*:"), this);
    pInstallLabel->setPalette(pa);
    m_pInstallEdit = new DragDropLineEdit(this);
    QPushButton* pInstallBt = new QPushButton(Chinese("..."), this);
    pInstallBt->setFixedSize(40, m_pInstallEdit->sizeHint().height());
    pInputLayout->addWidget(pInstallLabel, 0, 0);
    pInputLayout->addWidget(m_pInstallEdit, 0, 1);
    pInputLayout->addWidget(pInstallBt, 0, 2);

    QLabel* pRulesLabel = new QLabel(Chinese("�����*:"), this);
    pRulesLabel->setPalette(pa);
    m_pRulesPathEdit = new DragDropLineEdit(this);
    QPushButton* pRulesBt = new QPushButton(Chinese("..."), this);
    pRulesBt->setFixedSize(40, m_pRulesPathEdit->sizeHint().height());
    pInputLayout->addWidget(pRulesLabel, 1, 0);
    pInputLayout->addWidget(m_pRulesPathEdit, 1, 1);
    pInputLayout->addWidget(pRulesBt, 1, 2);

    QLabel* pOutPutLabel = new QLabel(Chinese("���*:"), this);
    pOutPutLabel->setPalette(pa);
    m_pOutPutPathEdit = new DragDropLineEdit(this);

    pInputLayout->addWidget(pOutPutLabel);
    pInputLayout->addWidget(m_pOutPutPathEdit);

    QPushButton* pOutPutBt = new QPushButton("...", this);
    pOutPutBt->setFixedSize(40, m_pOutPutPathEdit->sizeHint().height());
    pInputLayout->addWidget(pOutPutBt);
    pInputGroup->setLayout(pInputLayout);

    QGroupBox* pInstallGroup = new QGroupBox(Chinese("��װ��λ��"), pPublicGroup);
    QHBoxLayout* pInstallByte = new QHBoxLayout(this);
    m_pX32 = new QRadioButton(Chinese("32λ"), this);
    m_pX64 = new QRadioButton(Chinese("64λ"), this);
    m_pX32->click();

    pInstallByte->addWidget(m_pX32);
    pInstallByte->addWidget(m_pX64);
    pInstallGroup->setLayout(pInstallByte);

    //�汾��Ϣ
    QGroupBox* pInstallVersion = new QGroupBox(Chinese("�汾��Ϣ"), pPublicGroup);
    QLabel* pVersion = new QLabel(Chinese("�汾��*:"), this);
    pVersion->setPalette(pa);
    m_pVersionEdit = new DragDropLineEdit(this);
    QGridLayout* pVersionLayout = new QGridLayout(this);

    pVersionLayout->addWidget(pVersion,0,0);
    pVersionLayout->addWidget(m_pVersionEdit,0,1);
    pInstallVersion->setLayout(pVersionLayout);

    pInstallLayout->addWidget(pInputGroup);
    pInstallLayout->addWidget(pInstallVersion);
    pInstallLayout->addWidget(pInstallGroup);
    
    QGroupBox* pAccountConfig = new QGroupBox(Chinese("���˻���Ϣ"), this);
    QGridLayout* pUserDataLayout = new QGridLayout(this);
    //���˻�
    QLabel* pDomainUserNamLabel = new QLabel(Chinese("���˻�*:"), this);
    pDomainUserNamLabel->setPalette(pa);
    m_pDomainUserNamEdit = new QLineEdit(this);
    if (!m_strUserName.isEmpty())
    {
        m_pDomainUserNamEdit->setText(m_strUserName);
    }
    pUserDataLayout->addWidget(pDomainUserNamLabel, 0, 1);
    pUserDataLayout->addWidget(m_pDomainUserNamEdit, 0, 2);

    //������
    QLabel* pPassWordLabel = new QLabel(Chinese("����*:"), this);
    pPassWordLabel->setPalette(pa);
    m_pPassWordEdit = new QLineEdit(this);
    if (!m_strPassWord.isEmpty())
    {
        m_pPassWordEdit->setText(m_strPassWord);
    }
    m_pPassWordEdit->setEchoMode(QLineEdit::Password);
    pUserDataLayout->addWidget(pPassWordLabel, 1, 1);
    pUserDataLayout->addWidget(m_pPassWordEdit, 1, 2);

    m_pSaveUserData = new QCheckBox(Chinese("��ס�˻�����"), this);
    m_pSaveUserData->click();
    pUserDataLayout->addWidget(m_pSaveUserData, 2, 2);
    pAccountConfig->setLayout(pUserDataLayout);


    pPublicItem->addLayout(pInstallLayout);
    pPublicItem->setStretchFactor(pInstallLayout, 2);
    pPublicItem->addWidget(pAccountConfig);

    pPublicGroup->setLayout(pPublicItem);

    QGroupBox* pRegionConfig = new QGroupBox(Chinese("����"), this);
    QHBoxLayout* pRegionConfigLayout = new QHBoxLayout(pRegionConfig);
    //������
    QGroupBox* pConfigGroup = new QGroupBox(Chinese("��װ��ʽ����"), this);
    QVBoxLayout* pConfigLayout = new QVBoxLayout(this);

    QHBoxLayout* pInstallConfig = new QHBoxLayout(this);
    m_pCoverInstall = new QRadioButton(Chinese("���ǰ�װ"), this);
    m_pUninstall = new QRadioButton(Chinese("ж�ذ�װ"), this);
    m_pUninstall->click();
    pInstallConfig->addWidget(m_pCoverInstall);
    pInstallConfig->addWidget(m_pUninstall);
    pConfigLayout->addLayout(pInstallConfig);

    m_pOnlineSumCheckBox = new QCheckBox(Chinese("Я����������(��ѡ��Я����ѡ�ǲ���)"), this);
    pConfigLayout->addWidget(m_pOnlineSumCheckBox);

    
    pConfigGroup->setLayout(pConfigLayout);

    QGroupBox* pRegion = new QGroupBox(Chinese("��������"), pRegionConfig);

    QHBoxLayout* pSelectRules = new QHBoxLayout(this);
    QLabel* pSelectRulesLabel = new QLabel(Chinese("ѡ�����*:"), this);
    pSelectRulesLabel->setPalette(pa);
    m_pSelectRegionRules = new QTextEdit(this);
    m_pSelectRegionRules->setReadOnly(true);
    QPushButton* pSelectBt = new QPushButton(Chinese("..."), this);
    pSelectBt->setFixedWidth(40);

    pSelectRules->addWidget(pSelectRulesLabel);
    pSelectRules->addWidget(m_pSelectRegionRules);
    pSelectRules->addWidget(pSelectBt);

    pRegion->setLayout(pSelectRules);

    
    pRegionConfigLayout->addWidget(pConfigGroup);
    pRegionConfigLayout->addWidget(pRegion);
    pRegionConfig->setLayout(pRegionConfigLayout);


    QVBoxLayout* pOutPutLayout = new QVBoxLayout(this);

    QHBoxLayout* pBtLayout = new QHBoxLayout(this);
    QFont font;
    font.setPointSize(18);
    m_pStartPacketMakerBt = new QPushButton(Chinese("��ʼ���"), this);
    m_pStartPacketMakerBt->setFixedSize(100, 50);
    m_pStartPacketMakerBt->setStyleSheet("background-color: rgb(255, 153, 18);");
    m_pStartPacketMakerBt->setFont(font);
    
    m_pExitBt = new QPushButton(Chinese("�˳�"), this);
    m_pExitBt->setFixedSize(100, 50);
    m_pExitBt->setStyleSheet("background-color: rgb(255, 153, 18);");
    m_pExitBt->setFont(font);
    pBtLayout->addWidget(m_pStartPacketMakerBt, Qt::AlignVCenter);
    pBtLayout->addWidget(m_pExitBt, Qt::AlignVCenter);
    pBtLayout->setSpacing(10);
    pOutPutLayout->addLayout(pBtLayout);
   
    pMainLayout->addWidget(pPublicGroup);
    pMainLayout->addWidget(pRegionConfig);
    pMainLayout->addLayout(pOutPutLayout);

    //��ʵ�ֺ���
    connect(pInstallBt, SIGNAL(clicked()), this, SLOT(installPathOpenBt()));
    connect(pRulesBt, SIGNAL(clicked()), this, SLOT(rulesPathOpenBt()));
    connect(pOutPutBt, SIGNAL(clicked()), this, SLOT(outPutOpenBt()));
    connect(m_pStartPacketMakerBt, SIGNAL(clicked()), this, SLOT(startPacketMakerBt()));
    connect(m_pExitBt, SIGNAL(clicked()), this, SLOT(exitBt()));
    connect(pSelectBt, SIGNAL(clicked()), this, SLOT(selectRegionRulesBt()));

    this->setLayout(pMainLayout);
    this->setFixedSize(600, 500);
}

/*!
*@brief
*@author       maozg
*@time         2018��1��5��
*@param
*@return       void
*/
bool OuterNetPacketMaker::checkInput()
{
    if (m_pInstallEdit->text().isEmpty() || m_pRulesPathEdit->text().isEmpty() ||
        m_pSelectRegionRules->toPlainText().isEmpty() || m_pOutPutPathEdit->text().isEmpty() ||
        m_pDomainUserNamEdit->text().isEmpty() || m_pPassWordEdit->text().isEmpty()
        || m_pVersionEdit->text().isEmpty())
    {
        return false;
    }
    return true;
}

/*!
*@brief        �����û�����
*@author       maozg
*@time         2018��1��18��
*@param
*@return       void
*/
void OuterNetPacketMaker::loadUserInfo()
{
    QString strUserIni = PacketMakerCommon::binPath() + "/UserInfo.ini";
    QSettings iniFile(strUserIni, QSettings::IniFormat);

    QVariant userName = iniFile.value("/UserInfo/Account");
    m_strUserName = userName.toString();

    QVariant passWord = iniFile.value("/UserInfo/Password");
    m_strPassWord = passWord.toString();
}

/*!
*@brief        �����û�����
*@author       maozg
*@time         2018��1��18��
*@param
*@return       void
*/
void OuterNetPacketMaker::saveUserInfo()
{
    //ѡ���˼�ס�˻�����
    if (m_pSaveUserData->isChecked())
    {
        QString strUserIni = PacketMakerCommon::binPath() + "/UserInfo.ini";
        QSettings iniFile(strUserIni, QSettings::IniFormat);
        iniFile.beginGroup("UserInfo");
        iniFile.setValue("Account", m_pDomainUserNamEdit->text());
        iniFile.setValue("Password", m_pPassWordEdit->text());
        iniFile.endGroup();
    }
}

/*!
*@brief        ��������
*@author       maozg
*@time         2018��6��5��
*@param
*@return       void
*/
void OuterNetPacketMaker::createTask()
{
    m_oTaskList.clear();
    //��ȡ������Ҫ����ĵ���
    QString strRegion = m_pSelectRegionRules->toPlainText();
    QStringList strRegionList = strRegion.split(',');

    QString strByte = "";
    if (m_pX32->isChecked())
    {
        strByte = Chinese("32λ");
    }
    else if (m_pX64->isChecked())
    {
        strByte = Chinese("64λ");
    }

    for (int i = 0; i < strRegionList.count(); ++i)
    {
        PacketMakerConfigInfo oPacketParam;
        oPacketParam.strInstallFilePath = m_pInstallEdit->text();
        oPacketParam.strRulesFilePath = m_pRulesPathEdit->text();
        oPacketParam.bIsCoverInstall = m_pCoverInstall->isChecked();
        oPacketParam.bIsUninstall = m_pUninstall->isChecked();
        oPacketParam.bIsOnlineSum = m_pOnlineSumCheckBox->isChecked();
        oPacketParam.bIsSaveUserData = m_pSaveUserData->isChecked();
        oPacketParam.strRegionRules = parsRegionRules(strRegionList.at(i));
        oPacketParam.strUserName = m_pDomainUserNamEdit->text();
        oPacketParam.strUserPassWord = m_pPassWordEdit->text();
       
        if (oPacketParam.bIsOnlineSum)
        {
            //����������
            oPacketParam.strOuterNetPacketName = QString("%1%2%3%4%5%6").arg(Chinese("GTJ2018��װ����")).arg(m_pVersionEdit->text()).arg(
                "_").arg(oPacketParam.strRegionRules).arg(Chinese("����")).arg(Chinese("_��������"));
        }
        else
        {
            oPacketParam.strOuterNetPacketName = QString("%1%2%3%4%5").arg(Chinese("GTJ2018��װ����")).arg(m_pVersionEdit->text()).arg(
                "_").arg(oPacketParam.strRegionRules).arg(Chinese("����"));
        }

        oPacketParam.strOutPutPath = m_pOutPutPathEdit->text() + "/" + strByte;

        m_oTaskList.push_back(oPacketParam);
    }
}

/*!
*@brief
*@author       maozg
*@time         2018��1��9��
*@param
*@return       void
*/
QString OuterNetPacketMaker::parsRegionRules(QString strRegion)
{
    //������ļ������д�ʡ��Ҳ��û�д��ģ����Խ���һ��
    QString strResult = "";

    //���ѡ����ȫ��,ֱ�ӽ���
    if (-1 != strRegion.indexOf(Chinese("ȫ��")))
    {
        strResult = Chinese("ȫ��");
        return strResult;
    }

    int nIndex1 = strRegion.indexOf(Chinese("��"));
    int nIndex2 = strRegion.indexOf(Chinese("ʡ"));

    if (-1 != nIndex1)
    {
        strResult.append(strRegion.mid(0, nIndex1));
    }
    else if (-1 != nIndex2)
    {
        strResult.append(strRegion.mid(0, nIndex2));
    }
    else
    {
        //ȡǰ������ �������ġ��½�����۵�
        strResult.append(strRegion.mid(0, 2));
    }

    return strResult;
}

/*!
*@brief
*@author       maozg
*@time         2018��1��4��
*@param
*@return       void
*/
void OuterNetPacketMaker::installPathOpenBt()
{
    //�򿪰�װ���ļ�·��
    QString strFilePath = QFileDialog::getOpenFileName(this, Chinese("�򿪰�װ���ļ�"), ".", 0);
    if (!strFilePath.isNull())
    {
        m_pInstallEdit->setText(strFilePath);
    }
}

/*!
*@brief
*@author       maozg
*@time         2018��1��4��
*@param
*@return       void
*/
void OuterNetPacketMaker::rulesPathOpenBt()
{
    //�򿪹�����ļ�·��
    QString strFilePath = QFileDialog::getOpenFileName(this, Chinese("�򿪹�����ļ�"), ".", 0);
    if (!strFilePath.isNull())
    {
        m_pRulesPathEdit->setText(strFilePath);
    }
}

/*!
*@brief
*@author       maozg
*@time         2018��1��4��
*@param
*@return       void
*/
void OuterNetPacketMaker::outPutOpenBt()
{
    QString strFilePath = QFileDialog::getExistingDirectory(this, Chinese("���ļ���"), ".", 0);
    if (!strFilePath.isNull())
    {
        m_pOutPutPathEdit->setText(strFilePath);
    }
}

/*!
*@brief        ��ʼִ�����
*@author       maozg
*@time         2018��1��4��
*@param
*@return       void
*/
void OuterNetPacketMaker::startPacketMakerBt()
{
    //���������Ϣ�Ƿ�����
    if (!checkInput())
    {
        QMessageBox::critical(this, Chinese("�����������"), Chinese("��װ��·���������·����ѡ�����\n����⡢���˻������롢������װ��\n����·���Ȳ���Ϊ��,����!"),
            QMessageBox::Ok, QMessageBox::Ok);

        return;
    }

    //�����û�����
    saveUserInfo();

    //��������
    createTask();

    m_pPacketMakerTask = new OuterNetPakcetMakerTask(m_oTaskList);
    PacketMakerTaskThread* pTaskThread = new PacketMakerTaskThread(m_pPacketMakerTask, this);

    connect(pTaskThread, &PacketMakerTaskThread::finished, pTaskThread, &QObject::deleteLater);
    connect(m_pPacketMakerTask, SIGNAL(onTaskFinished()), this, SLOT(onPacketMakerFinished()));
    connect(m_pPacketMakerTask, SIGNAL(setLog(const QString&, int)), this, SLOT(onSetProgressValue(const QString&, int)));

    m_pStartPacketMakerBt->setEnabled(false);
    m_pExitBt->setEnabled(false);

    if (nullptr != m_pProgressDlg)
    {
        m_pProgressDlg->close();
        delete m_pProgressDlg;
        m_pProgressDlg = nullptr;
    }

    m_pProgressDlg = new QProgressDialog(this);
    m_pProgressDlg->setWindowTitle(Chinese("�������"));
    m_pProgressDlg->setLabelText(Chinese("��ʼ��������"));
    m_pProgressDlg->setRange(0, 100);
    m_pProgressDlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Dialog);
    m_pProgressDlg->setCancelButton(0);
    m_pProgressDlg->setFixedSize(260, 80);
    m_pProgressDlg->setWindowModality(Qt::WindowModal);
    m_pProgressDlg->setValue(5);

    m_pProgressDlg->show();
    pTaskThread->start();
}

/*!
*@brief
*@author       maozg
*@time         2018��1��5��
*@param
*@return       void
*/
void OuterNetPacketMaker::onPacketMakerFinished()
{
    m_pStartPacketMakerBt->setEnabled(true);
    m_pExitBt->setEnabled(true);
    m_pProgressDlg->hide();

    QMessageBox::information(this, Chinese("�����������"), Chinese("������װ��������!"),
        QMessageBox::Ok, QMessageBox::Ok);

    if (nullptr != m_pPacketMakerTask)
    {
        delete m_pPacketMakerTask;
        m_pPacketMakerTask = nullptr;
    }
}

void OuterNetPacketMaker::onSetProgressValue(const QString& strLog, int nProgress)
{
    if (nullptr != m_pProgressDlg)
    {
        m_pProgressDlg->setLabelText(strLog);
        m_pProgressDlg->setValue(nProgress);
    }
}

/*!
*@brief
*@author       maozg
*@time         2018��1��4��
*@param
*@return       void
*/
void OuterNetPacketMaker::exitBt()
{
    close();
}

/*!
*@brief
*@author       maozg
*@time         2018��1��4��
*@param
*@return       void
*/
void OuterNetPacketMaker::selectRegionRulesBt()
{
    m_pRulesWidget = new RegionRulesSelectWidget(this);
    connect(m_pRulesWidget, SIGNAL(selectRulesComplete()), this, SLOT(onSelectRulesComplete()));
    m_pRulesWidget->show();
}

/*!
*@brief
*@author       maozg
*@time         2018��1��5��
*@param
*@return       void
*/
void OuterNetPacketMaker::onSelectRulesComplete()
{
    RegionRulesContainer &rulesList = m_pRulesWidget->getRulesContainer();
    if (rulesList.isEmpty())
    {
        return;
    }

    QString strRegionRules = Chinese(RegionRules[rulesList.item(0)]);
    for (int i = 1; i < rulesList.getCount(); ++i)
    {
        strRegionRules.append(",");
        strRegionRules.append(Chinese(RegionRules[rulesList.item(i)]));
    }

    m_pSelectRegionRules->setText(strRegionRules);

    if (nullptr != m_pRulesWidget)
    {
        m_pRulesWidget->close();
        delete m_pRulesWidget;
        m_pRulesWidget = nullptr;
    }
}

/*!
*@brief
*@author       maozg
*@time         2018��1��4��
*@param        
/*@return
* */
RegionRulesSelectWidget::RegionRulesSelectWidget(QWidget* parent /*= nullptr*/)
    :QWidget(parent)
{
    m_oRulesContainer.clear();
    iniUI();
}

/*!
*@brief
*@author       maozg
*@time         2018��1��4��
*@param
*@return
*/
RegionRulesSelectWidget::~RegionRulesSelectWidget()
{

}

/*!
*@brief
*@author       maozg
*@time         2018��1��4��
*@param
*@return       void
*/
void RegionRulesSelectWidget::iniUI()
{
    this->setWindowTitle(Chinese("ѡ���������"));
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
    this->setWindowModality(Qt::WindowModal);
    this->setFixedSize(300, 400);
    QVBoxLayout* pMainLayout = new QVBoxLayout();
    m_pRulesTableWidget = new QTableWidget(this);
    initializeRulesInfo();
    pMainLayout->addWidget(m_pRulesTableWidget);
    QHBoxLayout* pButtonLayout = new QHBoxLayout();
    QPushButton* pDetermineBt = new QPushButton(Chinese("ȷ��"));
    pDetermineBt->setFixedWidth(100);
    pButtonLayout->addStretch();
    pButtonLayout->addWidget(pDetermineBt, Qt::AlignRight);
    pMainLayout->addLayout(pButtonLayout);

    connect(pDetermineBt, SIGNAL(clicked()), this, SLOT(determineBtClick()));
    this->setLayout(pMainLayout);
}

/*!
*@brief
*@author       maozg
*@time         2018��1��4��
*@param
*@return       void
*/
void RegionRulesSelectWidget::initializeRulesInfo()
{
    QStringList strHead;
    strHead << Chinese("����");

    int nLength = sizeof(RegionRules) / sizeof(RegionRules[0]);

    //����һ��ȫѡ
    m_pRulesTableWidget->setRowCount(nLength + 1);
    m_pRulesTableWidget->setColumnCount(1);
    m_pRulesTableWidget->setHorizontalHeaderLabels(strHead);
    m_pRulesTableWidget->horizontalHeader()->setStretchLastSection(true);

    QTableWidgetItem * checkAll = new QTableWidgetItem();
    m_pRulesTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    checkAll->setCheckState(Qt::Unchecked);
    checkAll->setText(Chinese("ȫѡ"));
    m_pRulesTableWidget->setItem(0, 0, checkAll);

    for (int i = 0; i < nLength; ++i)
    {
        QTableWidgetItem * check = new QTableWidgetItem();
        m_pRulesTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        check->setCheckState(Qt::Unchecked);
        check->setText(Chinese(RegionRules[i]));

        m_pRulesTableWidget->setItem(i + 1, 0, check);
    }

    connect(m_pRulesTableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(updataCheckState(int, int)));
}

/*!
*@brief        ����checkboxѡ��״̬
*@author       maozg
*@time         2018��1��4��
*@param
*@return       void
*/
void RegionRulesSelectWidget::updataCheckState(int row, int col)
{
    if (row == 0 && col == 0)
    {
        if (m_pRulesTableWidget->item(row, col)->checkState() == Qt::Checked)
        {
            for (int i = 1; i < m_pRulesTableWidget->rowCount(); ++i)
            {
                m_pRulesTableWidget->item(i, col)->setCheckState(Qt::Checked);
            }
        }
        else
        {
            for (int i = 1; i < m_pRulesTableWidget->rowCount(); ++i)
            {
                m_pRulesTableWidget->item(i, col)->setCheckState(Qt::Unchecked);
            }
        }
    }
}

/*!
*@brief
*@author       maozg
*@time         2018��1��5��
*@param
*@return       void
*/
void RegionRulesSelectWidget::determineBtClick()
{
    m_oRulesContainer.clear();
    
    for (int i = 1; i < m_pRulesTableWidget->rowCount(); ++i)
    {
        if (m_pRulesTableWidget->item(i, 0)->checkState() == Qt::Checked)
        {
            m_oRulesContainer.addItem(RegionRulesEnum(i - 1));
        }
    }

    this->hide();
    emit selectRulesComplete();
}

