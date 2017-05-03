#pragma once

#include "stdafx.h"
#include <string>

class CChartData
{
public:
	CChartData(CListCtrl &listCtrl);

public:
	void SetDataInListCtrl(std::string& chartData, bool isTrade);

private:
	CListCtrl& m_listCtrl;
};
