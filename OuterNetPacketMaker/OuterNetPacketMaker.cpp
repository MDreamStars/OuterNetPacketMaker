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
    m_pPacketNameEdit(nullptr), m_pSelectRegionRules(nullptr), m_pOutPutPathEdit(nullptr),
    m_pDomainUserNamEdit(nullptr), m_pPassWordEdit(nullptr), m_pCoverInstall(nullptr),
    m_pUninstall(nullptr), m_pOnlineSumCheckBox(nullptr), m_pSaveUserData(nullptr),
    m_pPacketMakerTask(nullptr)
{
    m_pProgressDlg = nullptr;
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
    //������
    QGroupBox* pInputGroup = new QGroupBox(Chinese("����"), this);
    QGridLayout* pInputLayout = new QGridLayout(this);
    QLabel* pInstallLabel = new QLabel(Chinese("��װ��·��*:"), this);
    pInstallLabel->setPalette(pa);
    m_pInstallEdit = new DragDropLineEdit(this);
    QPushButton* pInstallBt = new QPushButton(Chinese("..."), this);
    pInstallBt->setFixedSize(40, m_pInstallEdit->sizeHint().height());
    pInputLayout->addWidget(pInstallLabel, 0, 0);
    pInputLayout->addWidget(m_pInstallEdit, 0, 1);
    pInputLayout->addWidget(pInstallBt, 0, 2);

    QLabel* pRulesLabel = new QLabel(Chinese("�����·��*:"), this);
    pRulesLabel->setPalette(pa);
    m_pRulesPathEdit = new DragDropLineEdit(this);
    QPushButton* pRulesBt = new QPushButton(Chinese("..."), this);
    pRulesBt->setFixedSize(40, m_pRulesPathEdit->sizeHint().height());
    pInputLayout->addWidget(pRulesLabel, 1, 0);
    pInputLayout->addWidget(m_pRulesPathEdit, 1, 1);
    pInputLayout->addWidget(pRulesBt, 1, 2);

    QLabel* pOuterNetNameLabel = new QLabel(Chinese("������װ������:"), this);
    m_pPacketNameEdit = new DragDropLineEdit(this);
    pInputLayout->addWidget(pOuterNetNameLabel, 2, 0);
    pInputLayout->addWidget(m_pPacketNameEdit, 2, 1);
    pInputGroup->setLayout(pInputLayout);

    //������
    QGroupBox* pConfigGroup = new QGroupBox(Chinese("����"), this);
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

    QHBoxLayout* pSelectRules = new QHBoxLayout(this);
    QLabel* pSelectRulesLabel = new QLabel(Chinese("ѡ���������*:"), this);
    pSelectRulesLabel->setPalette(pa);
    m_pSelectRegionRules = new QLineEdit(this);
    m_pSelectRegionRules->setReadOnly(true);
    SelectRulesButton* pSelectBt = new SelectRulesButton(Chinese("..."), m_pSelectRegionRules);
    pSelectRules->addWidget(pSelectRulesLabel);
    pSelectRules->addWidget(m_pSelectRegionRules);
    pConfigLayout->addLayout(pSelectRules);
    pConfigGroup->setLayout(pConfigLayout);

    //�����
    QGroupBox* pOutPutGroup = new QGroupBox(Chinese("���"), this);
    QVBoxLayout* pOutPutLayout = new QVBoxLayout(this);

    QGridLayout* pUserDataLayout = new QGridLayout(this);
    //���˻�
    QLabel* pDomainUserNamLabel = new QLabel(Chinese("���˻�*:"), this);
    pDomainUserNamLabel->setPalette(pa);
    m_pDomainUserNamEdit = new QLineEdit(this);
    if (!m_strUserName.isEmpty())
    {
        m_pDomainUserNamEdit->setText(m_strUserName);
    }
    m_pDomainUserNamEdit->setFixedWidth(150);
    pUserDataLayout->addWidget(pDomainUserNamLabel, 0, 1/*,1,1,Qt::AlignRight | Qt::AlignVCenter*/);
    pUserDataLayout->addWidget(m_pDomainUserNamEdit, 0, 2/*, 1, 1, Qt::AlignLeft | Qt::AlignVCenter*/);
    QSpacerItem* pSpace = new QSpacerItem(700, m_pDomainUserNamEdit->sizeHint().height());
    pUserDataLayout->addItem(pSpace, 0, 3);
    QSpacerItem* pSpace2 = new QSpacerItem(80, m_pDomainUserNamEdit->sizeHint().height());
    pUserDataLayout->addItem(pSpace2, 0, 0);

    //������
    QLabel* pPassWordLabel = new QLabel(Chinese("����*:"), this);
    pPassWordLabel->setPalette(pa);
    m_pPassWordEdit = new QLineEdit(this);
    if (!m_strPassWord.isEmpty())
    {
        m_pPassWordEdit->setText(m_strPassWord);
    }
    m_pPassWordEdit->setFixedWidth(150);
    m_pPassWordEdit->setEchoMode(QLineEdit::Password);
    pUserDataLayout->addWidget(pPassWordLabel, 1, 1/*,1,1,Qt::AlignRight | Qt::AlignVCenter*/);
    pUserDataLayout->addWidget(m_pPassWordEdit, 1, 2/*, 1, 1, Qt::AlignLeft | Qt::AlignVCenter*/);

    m_pSaveUserData = new QCheckBox(Chinese("��ס�˻�����"), this);
    m_pSaveUserData->click();
    pUserDataLayout->addWidget(m_pSaveUserData, 2, 2);
    pOutPutLayout->addLayout(pUserDataLayout);

    QHBoxLayout* pOutPutPathLayout = new QHBoxLayout(this);
    QLabel* pOutPutLabel = new QLabel(Chinese("������װ������·��*:"), this);
    pOutPutLabel->setPalette(pa);
    m_pOutPutPathEdit = new DragDropLineEdit(this);
    QPushButton* pOutPutBt = new QPushButton("...", this);
    pOutPutBt->setFixedSize(40, m_pOutPutPathEdit->sizeHint().height());
    pOutPutPathLayout->addWidget(pOutPutLabel);
    pOutPutPathLayout->addWidget(m_pOutPutPathEdit);
    pOutPutPathLayout->addWidget(pOutPutBt);
    pOutPutLayout->addLayout(pOutPutPathLayout);

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
    pOutPutGroup->setLayout(pOutPutLayout);

    pMainLayout->addWidget(pInputGroup);
    pMainLayout->addWidget(pConfigGroup);
    pMainLayout->addWidget(pOutPutGroup);

    //��ʵ�ֺ���
    connect(pInstallBt, SIGNAL(clicked()), this, SLOT(installPathOpenBt()));
    connect(pRulesBt, SIGNAL(clicked()), this, SLOT(rulesPathOpenBt()));
    connect(pOutPutBt, SIGNAL(clicked()), this, SLOT(outPutOpenBt()));
    connect(m_pStartPacketMakerBt, SIGNAL(clicked()), this, SLOT(startPacketMakerBt()));
    connect(m_pExitBt, SIGNAL(clicked()), this, SLOT(exitBt()));
    connect(pSelectBt, SIGNAL(clicked()), this, SLOT(selectRegionRulesBt()));

    this->setLayout(pMainLayout);
    this->setFixedSize(550, 600);
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
        m_pSelectRegionRules->text().isEmpty() || m_pOutPutPathEdit->text().isEmpty() || 
        m_pDomainUserNamEdit->text().isEmpty() || m_pPassWordEdit->text().isEmpty())
    {
        return false;
    }
    return true;
}

/*!
*@brief
*@author       maozg
*@time         2018��1��5��
*@param        PacketMakerConfigInfo& packetInfo
*@return       void
*/
void OuterNetPacketMaker::initializeInputData(PacketMakerConfigInfo& packetInfo)
{
    packetInfo.strInstallFilePath = m_pInstallEdit->text();
    packetInfo.strRulesFilePath = m_pRulesPathEdit->text();
    packetInfo.strOuterNetPacketName = m_pPacketNameEdit->text();
    packetInfo.bIsCoverInstall = m_pCoverInstall->isChecked();
    packetInfo.bIsUninstall = m_pUninstall->isChecked();
    packetInfo.bIsOnlineSum = m_pOnlineSumCheckBox->isChecked();
    packetInfo.bIsSaveUserData = m_pSaveUserData->isChecked();
    packetInfo.strRegionRules = m_pSelectRegionRules->text();
    packetInfo.strUserName = m_pDomainUserNamEdit->text();
    packetInfo.strUserPassWord = m_pPassWordEdit->text();
    packetInfo.strOutPutPath = m_pOutPutPathEdit->text();
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
*@brief
*@author       maozg
*@time         2018��1��23��
*@param        const QString& strLog, int nProgress
*@return       void
*/
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

    //��֯������Ϣ
    PacketMakerConfigInfo packetInfo;
    initializeInputData(packetInfo);

    m_pPacketMakerTask = new OuterNetPakcetMakerTask(packetInfo);
    PacketMakerTaskThread* pTaskThread = new PacketMakerTaskThread(m_pPacketMakerTask, this);

    connect(pTaskThread, &PacketMakerTaskThread::finished, pTaskThread, &QObject::deleteLater);
    connect(m_pPacketMakerTask, SIGNAL(onTaskFinished()), this, SLOT(onPacketMakerFinished()));
    connect(m_pPacketMakerTask, SIGNAL(setLog(const QString&,int)), this, SLOT(onSetProgressValue(const QString&, int)));

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

    m_pRulesTableWidget->setRowCount(nLength);
    m_pRulesTableWidget->setColumnCount(1);
    m_pRulesTableWidget->setHorizontalHeaderLabels(strHead);
    m_pRulesTableWidget->horizontalHeader()->setStretchLastSection(true);

    for (int i = 0; i < nLength; ++i)
    {
        QTableWidgetItem * check = new QTableWidgetItem();
        m_pRulesTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        check->setCheckState(Qt::Unchecked);
        check->setText(Chinese(RegionRules[i]));

        m_pRulesTableWidget->setItem(i, 0, check);
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
                m_pRulesTableWidget->item(i, col)->setFlags(Qt::NoItemFlags);
                m_pRulesTableWidget->item(i, col)->setCheckState(Qt::Unchecked);
            }
        }
        else
        {
            for (int i = 1; i < m_pRulesTableWidget->rowCount(); ++i)
            {
                m_pRulesTableWidget->item(i, col)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
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
    if (m_pRulesTableWidget->item(0, 0)->checkState() == Qt::Checked)
    {
        m_oRulesContainer.addItem(RegionRulesEnum(0));
    }
    else
    {
        for (int i = 1; i < m_pRulesTableWidget->rowCount(); ++i)
        {
            if (m_pRulesTableWidget->item(i, 0)->checkState() == Qt::Checked)
            {
                m_oRulesContainer.addItem(RegionRulesEnum(i));
            }
        }
    }

    this->hide();
    emit selectRulesComplete();
}

