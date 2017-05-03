#include "stdafx.h"
#include "ChartData.h"
#include <sstream>
#include <iostream>
#include "Utility.h"

using namespace System;
using namespace Newtonsoft::Json;
using namespace System::Collections;

value struct ChartBidAskSample
{
public:
	double CloseAsk;
	double CloseBid;
	double HighAsk;
	double HighBid;
	double LowAsk;
	double LowBid;
	double OpenAsk;
	double OpenBid;
	String^ Time;
};

value struct ChartTradeSample
{
public:
	double Open;
	double Close;
	double High;
	double Low;
	double OpenInterest;
	double Volume;
	String^ Time;
};

ref class ConvertedChartBidAskData
{
public:
	long dataVersion;
	cli::array<ChartBidAskSample>^ data;
};

ref class ConvertedChartTradeData
{
public:
	long dataVersion;
	cli::array<ChartTradeSample>^ data;
};

ConvertedChartBidAskData^ getDeserializedBidAskData(System::String^ jsonString)
{
	return JsonConvert::DeserializeObject<ConvertedChartBidAskData^>(jsonString);
};

ConvertedChartTradeData^ getDeserializedTradeData(System::String^ jsonString)
{
	return JsonConvert::DeserializeObject<ConvertedChartTradeData^>(jsonString);
};

CChartData::CChartData(CListCtrl &listCtrl)
	: m_listCtrl(listCtrl)
{
}

void CChartData::SetDataInListCtrl(std::string& chartData, bool isTrade)
{
	String^ json(CppStringToSysString(chartData));
	if (isTrade)
	{
		ConvertedChartTradeData^ convertedData = getDeserializedTradeData(json);

		for (int i = 0; i < convertedData->data->Length; ++i)
		{
			m_listCtrl.InsertItem(LVIF_TEXT, i, SysStringToCppString(convertedData->data[i].Time).c_str(), 0, 0, 0, 0);
			m_listCtrl.SetItemText(i, 1, SysStringToCppString(convertedData->data[i].Open.ToString()).c_str());
			m_listCtrl.SetItemText(i, 2, SysStringToCppString(convertedData->data[i].High.ToString()).c_str());
			m_listCtrl.SetItemText(i, 3, SysStringToCppString(convertedData->data[i].Low.ToString()).c_str());
			m_listCtrl.SetItemText(i, 4, SysStringToCppString(convertedData->data[i].Close.ToString()).c_str());
			m_listCtrl.SetItemText(i, 5, SysStringToCppString(convertedData->data[i].OpenInterest.ToString()).c_str());
			m_listCtrl.SetItemText(i, 6, SysStringToCppString(convertedData->data[i].Volume.ToString()).c_str());
		}
	}
	else
	{
		ConvertedChartBidAskData^ convertedData = getDeserializedBidAskData(json);

		for (int i = 0; i < convertedData->data->Length; ++i)
		{
			m_listCtrl.InsertItem(LVIF_TEXT, i, SysStringToCppString(convertedData->data[i].Time).c_str(), 0, 0, 0, 0);
			m_listCtrl.SetItemText(i, 1, SysStringToCppString(convertedData->data[i].OpenBid.ToString()).c_str());
			m_listCtrl.SetItemText(i, 2, SysStringToCppString(convertedData->data[i].HighBid.ToString()).c_str());
			m_listCtrl.SetItemText(i, 3, SysStringToCppString(convertedData->data[i].LowBid.ToString()).c_str());
			m_listCtrl.SetItemText(i, 4, SysStringToCppString(convertedData->data[i].CloseBid.ToString()).c_str());
			m_listCtrl.SetItemText(i, 5, SysStringToCppString(convertedData->data[i].OpenAsk.ToString()).c_str());
			m_listCtrl.SetItemText(i, 6, SysStringToCppString(convertedData->data[i].HighAsk.ToString()).c_str());
			m_listCtrl.SetItemText(i, 7, SysStringToCppString(convertedData->data[i].LowAsk.ToString()).c_str());
			m_listCtrl.SetItemText(i, 8, SysStringToCppString(convertedData->data[i].CloseAsk.ToString()).c_str());
		}
	}
}