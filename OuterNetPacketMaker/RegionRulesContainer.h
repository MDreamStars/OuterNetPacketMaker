/*!
*@file     RegionRulesContainer.h
*@brief    地区规则容器
*@author   maozg
*@date     2018年1月5日
*@remarks
*@version 3.0
*Copyright (c) 1998-2018 Glodon Corporation
*/
#pragma once
#include "..\common\PacketMakerCommon.h"
#include <vector>

using namespace std;

/*
* @brief 存储地区规则
*
* @author maozg  2018年1月5日
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
