/*!
*@file     RegionRulesContainer.cpp
*@brief    ������������
*@author   maozg
*@date     2018��1��5��
*@remarks
*@version 3.0
*Copyright (c) 1998-2018 Glodon Corporation
*/
#include "RegionRulesContainer.h"

/*!
*@brief
*@author       maozg
*@time         2018��1��5��
*@param
*@return
*/
RegionRulesContainer::RegionRulesContainer()
{

}

/*!
*@brief
*@author       maozg
*@time         2018��1��5��
*@param
*@return
*/
RegionRulesContainer::~RegionRulesContainer()
{

}

/*!
*@brief
*@author       maozg
*@time         2018��1��5��
*@param        const RegionRulesEnum& rulesItem
*@return       void
*/
void RegionRulesContainer::addItem(const RegionRulesEnum& rulesItem)
{
    m_oRegionRulesList.push_back(rulesItem);
}

/*!
*@brief
*@author       maozg
*@time         2018��1��5��
*@param
*@return       int
*/
int RegionRulesContainer::getCount()
{
    return m_oRegionRulesList.size();
}

/*!
*@brief
*@author       maozg
*@time         2018��1��5��
*@param        const int &nIndex
*@return       RegionRulesEnum
*/
RegionRulesEnum RegionRulesContainer::item(const int &nIndex)
{
    if (nIndex >= 0 && nIndex < getCount())
    {
        return m_oRegionRulesList[nIndex];
    }

    return AllRegionRules;
}

/*!
*@brief
*@author       maozg
*@time         2018��1��5��
*@param
*@return       bool
*/
bool RegionRulesContainer::isEmpty()
{
    return getCount() == 0;
}

/*!
*@brief
*@author       maozg
*@time         2018��1��5��
*@param
*@return       void
*/
void RegionRulesContainer::clear()
{
    m_oRegionRulesList.clear();
}
