/*!
*@file     RegionRulesContainer.h
*@brief    ������������
*@author   maozg
*@date     2018��1��5��
*@remarks
*@version 3.0
*Copyright (c) 1998-2018 Glodon Corporation
*/
#pragma once
#include "..\common\PacketMakerCommon.h"
#include <vector>

using namespace std;

/*
* @brief �洢��������
*
* @author maozg  2018��1��5��
*
* @class RegionRulesContainer
*/
class RegionRulesContainer
{
public:
    RegionRulesContainer();
    ~RegionRulesContainer();

    void addItem(const RegionRulesEnum& rulesItem);
    int getCount();
    RegionRulesEnum item(const int &nIndex);
    bool isEmpty();
    void clear();
private:
    vector<RegionRulesEnum> m_oRegionRulesList;
};
