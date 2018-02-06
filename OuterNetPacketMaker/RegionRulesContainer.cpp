/*!
*@file     RegionRulesContainer.cpp
*@brief    地区规则容器
*@author   maozg
*@date     2018年1月5日
*@remarks
*@version 3.0
*Copyright (c) 1998-2018 Glodon Corporation
*/
#include "RegionRulesContainer.h"

/*!
*@brief
*@author       maozg
*@time         2018年1月5日
*@param
*@return
*/
RegionRulesContainer::RegionRulesContainer()
{

}

/*!
*@brief
*@author       maozg
*@time         2018年1月5日
*@param
*@return
*/
RegionRulesContainer::~RegionRulesContainer()
{

}

/*!
*@brief
*@author       maozg
*@time         2018年1月5日
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
*@time         2018年1月5日
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
*@time         2018年1月5日
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
*@time         2018年1月5日
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
*@time         2018年1月5日
*@param
*@return       void
*/
void RegionRulesContainer::clear()
{
    m_oRegionRulesList.clear();
}
