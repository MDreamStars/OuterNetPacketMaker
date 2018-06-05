#pragma once

#include <QtWidgets/QWidget>
#include "ui_OuterNetPacketMaker.h"
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QStringList>
#include <QTableWidget>
#include "RegionRulesContainer.h"
#include <vector>
#include "..\OuterNetPacketMakerTask\OuterNetPakcetMakerTask.h"
#include <QProgressDialog>
#include <QTextEdit>

using namespace std;


/*
* @brief 重写lineedit 实现拖放文件功能
*
* @author maozg  2017年12月29日
*
* @class DragDropLineEdit
*/
class DragDropLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    DragDropLineEdit(QWidget* parent = nullptr);
    ~DragDropLineEdit();

private:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    bool checkIsFile(QList<QUrl>& olist);
};

/*
* @brief 地区规则选择界面
*
* @author maozg  2018年1月4日
*
* @class RegionRulesSelectWidget
*/
class RegionRulesSelectWidget : public QWidget
{
    Q_OBJECT
public:
    RegionRulesSelectWidget(QWidget* parent = nullptr);
    ~RegionRulesSelectWidget();

    RegionRulesContainer& getRulesContainer() { return m_oRulesContainer; };
private:
    void iniUI();
    void initializeRulesInfo();
private slots:
    void updataCheckState(int row, int col);
    void determineBtClick();
signals:
    void selectRulesComplete();
private:
    RegionRulesContainer m_oRulesContainer;
    QTableWidget* m_pRulesTableWidget;
};

/*
* @brief 挂载lineedit的按钮
*
* @author maozg  2017年12月29日
*
* @class SelectRulesButton
*/
class SelectRulesButton : public QPushButton
{
    Q_OBJECT
public:
    SelectRulesButton(const QString &text, QLineEdit *edit);
    ~SelectRulesButton();
};

class OuterNetPacketMaker : public QWidget
{
    Q_OBJECT

public:
    OuterNetPacketMaker(QWidget *parent = Q_NULLPTR);

private:
    Ui::OuterNetPacketMakerClass ui;
private:
    void iniUI();
    bool checkInput();
    void initializeInputData(PacketMakerConfigInfo& packetInfo);
    void loadUserInfo();
    void saveUserInfo();
private slots:
    void installPathOpenBt();
    void rulesPathOpenBt();
    void outPutOpenBt();
    void startPacketMakerBt();
    void exitBt();
    void selectRegionRulesBt();
    void onSelectRulesComplete();
    void onPacketMakerFinished();
    void onSetProgressValue(const QString& strLog, int nProgress);

private:
    DragDropLineEdit * m_pInstallEdit;
    DragDropLineEdit* m_pRulesPathEdit;
    QTextEdit * m_pSelectRegionRules;
    DragDropLineEdit* m_pOutPutPathEdit;
    QLineEdit* m_pDomainUserNamEdit;
    QLineEdit* m_pPassWordEdit;
    QRadioButton* m_pCoverInstall;
    QRadioButton* m_pUninstall;
    QCheckBox* m_pOnlineSumCheckBox;
    QCheckBox* m_pSaveUserData;
    RegionRulesSelectWidget* m_pRulesWidget;
    OuterNetPakcetMakerTask* m_pPacketMakerTask;
    QPushButton* m_pStartPacketMakerBt;
    QPushButton* m_pExitBt;
    QString m_strUserName;
    QString m_strPassWord;
    QProgressDialog* m_pProgressDlg;

    QRadioButton* m_pX32;
    QRadioButton* m_pX64;
};
