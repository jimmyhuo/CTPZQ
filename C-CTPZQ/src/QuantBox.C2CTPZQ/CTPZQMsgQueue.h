#pragma once

#include "QuantBox.C2CTPZQ.h"
#include "include\LockFreeQ.h"

class CCTPZQMsgQueue
{
	//响应队列中可能出现的消息类型（按字母排序）
	enum EnumMsgType
	{
		E_fnOnConnect,	
		E_fnOnDisconnect,	
		E_fnOnErrRtnOrderAction,
		E_fnOnErrRtnOrderInsert,
		E_fnOnRspError,
		E_fnOnRspOrderAction,
		E_fnOnRspOrderInsert,
		E_fnOnRspQryDepthMarketData,
		E_fnOnRspQryInstrument,
		E_fnOnRspQryInstrumentCommissionRate,
		E_fnOnRspQryInstrumentMarginRate,
		E_fnOnRspQryInvestorPosition,
		E_fnOnRspQryInvestorPositionDetail,
		E_fnOnRspQryOrder,
		E_fnOnRspQryTrade,
		E_fnOnRspQryTradingAccount,
		E_fnOnRtnDepthMarketData,
		E_fnOnRtnInstrumentStatus,
		E_fnOnRtnOrder,
		E_fnOnRtnTrade,
	};

	struct SMsgItem
	{
		EnumMsgType							type;			//消息类型
		void*								pApi;			//指向类对象的指针
		CZQThostFtdcRspInfoField			RspInfo;		//响应信息
		bool								bIsLast;		//是否最后一个包
		union{
			int								nRequestID;
			ConnectionStatus				Status;
		};		
		union{
			CZQThostFtdcDepthMarketDataField			DepthMarketData;	//E_fnOnRspQryDepthMarketData与E_fnOnRtnDepthMarketData返回的数据格式相同
			CZQThostFtdcInputOrderField					InputOrder;
			CZQThostFtdcInputOrderActionField			InputOrderAction;
			CZQThostFtdcInstrumentField					Instrument;
			CZQThostFtdcInstrumentCommissionRateField	InstrumentCommissionRate;
			CZQThostFtdcInstrumentMarginRateField		InstrumentMarginRate;
			CZQThostFtdcInstrumentStatusField			InstrumentStatus;
			CZQThostFtdcInvestorPositionField			InvestorPosition;
			CZQThostFtdcInvestorPositionDetailField		InvestorPositionDetail;
			CZQThostFtdcOrderField						Order;
			CZQThostFtdcOrderActionField				OrderAction;
			CZQThostFtdcRspUserLoginField				RspUserLogin;
			CZQThostFtdcTradeField						Trade;
			CZQThostFtdcTradingAccountField				TradingAccount;
		};
	};

public:
	CCTPZQMsgQueue(void)
	{
		m_hThread = NULL;
		m_bRunning = false;

		//回调函数地址指针
		m_fnOnConnect = NULL;
		m_fnOnDisconnect = NULL;
		m_fnOnErrRtnOrderAction = NULL;
		m_fnOnErrRtnOrderInsert = NULL;
		m_fnOnRspError = NULL;
		m_fnOnRspOrderAction = NULL;
		m_fnOnRspOrderInsert = NULL;
		m_fnOnRspQryDepthMarketData = NULL;
		m_fnOnRspQryInstrument = NULL;
		m_fnOnRspQryInstrumentCommissionRate = NULL;
		m_fnOnRspQryInstrumentMarginRate = NULL;
		m_fnOnRspQryInvestorPosition = NULL;
		m_fnOnRspQryInvestorPositionDetail = NULL;
		m_fnOnRspQryOrder = NULL;
		m_fnOnRspQryTrade = NULL;
		m_fnOnRspQryTradingAccount = NULL;
		m_fnOnRtnDepthMarketData = NULL;
		m_fnOnRtnInstrumentStatus = NULL;
		m_fnOnRtnOrder = NULL;
		m_fnOnRtnTrade = NULL;

		m_hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	}
	virtual ~CCTPZQMsgQueue(void)
	{
		StopThread();
		Clear();

		CloseHandle(m_hEvent);
	}

public:
	//清空队列
	void Clear();

	//可以由外部发起，顺序处理队列触发回调函数
	bool Process();

	//由内部启动线程，内部主动调用Process触发回调
	void StartThread();
	void StopThread();

	//将外部的函数地址注册到队列(按字母排序)
	void RegisterCallback(fnOnConnect pCallback){m_fnOnConnect = pCallback;}
	void RegisterCallback(fnOnDisconnect pCallback){m_fnOnDisconnect = pCallback;}
	void RegisterCallback(fnOnErrRtnOrderAction pCallback){m_fnOnErrRtnOrderAction = pCallback;}
	void RegisterCallback(fnOnErrRtnOrderInsert pCallback){m_fnOnErrRtnOrderInsert = pCallback;}
	void RegisterCallback(fnOnRspError pCallback){m_fnOnRspError = pCallback;}
	void RegisterCallback(fnOnRspOrderAction pCallback){m_fnOnRspOrderAction = pCallback;}
	void RegisterCallback(fnOnRspOrderInsert pCallback){m_fnOnRspOrderInsert = pCallback;}
	void RegisterCallback(fnOnRspQryDepthMarketData pCallback){m_fnOnRspQryDepthMarketData = pCallback;}
	void RegisterCallback(fnOnRspQryInstrument pCallback){m_fnOnRspQryInstrument = pCallback;}
	void RegisterCallback(fnOnRspQryInstrumentCommissionRate pCallback){m_fnOnRspQryInstrumentCommissionRate = pCallback;}
	void RegisterCallback(fnOnRspQryInstrumentMarginRate pCallback){m_fnOnRspQryInstrumentMarginRate = pCallback;}
	void RegisterCallback(fnOnRspQryInvestorPosition pCallback){m_fnOnRspQryInvestorPosition = pCallback;}
	void RegisterCallback(fnOnRspQryInvestorPositionDetail pCallback){m_fnOnRspQryInvestorPositionDetail = pCallback;}
	void RegisterCallback(fnOnRspQryOrder pCallback){m_fnOnRspQryOrder = pCallback;}
	void RegisterCallback(fnOnRspQryTrade pCallback){m_fnOnRspQryTrade = pCallback;}
	void RegisterCallback(fnOnRspQryTradingAccount pCallback){m_fnOnRspQryTradingAccount = pCallback;}
	void RegisterCallback(fnOnRtnDepthMarketData pCallback){m_fnOnRtnDepthMarketData = pCallback;}
	void RegisterCallback(fnOnRtnInstrumentStatus pCallback){m_fnOnRtnInstrumentStatus = pCallback;}
	void RegisterCallback(fnOnRtnOrder pCallback){m_fnOnRtnOrder = pCallback;}
	void RegisterCallback(fnOnRtnTrade pCallback){m_fnOnRtnTrade = pCallback;}

	//响应结果处理后入队列(按字母排序)
	void Input_OnConnect(void* pApi,CZQThostFtdcRspUserLoginField *pRspUserLogin,ConnectionStatus result);
	void Input_OnDisconnect(void* pApi,CZQThostFtdcRspInfoField *pRspInfo,ConnectionStatus step);
	void Input_OnErrRtnOrderAction(void* pTraderApi,CZQThostFtdcOrderActionField *pOrderAction, CZQThostFtdcRspInfoField *pRspInfo);
	void Input_OnErrRtnOrderInsert(void* pTraderApi,CZQThostFtdcInputOrderField *pInputOrder, CZQThostFtdcRspInfoField *pRspInfo);
	void Input_OnRspError(void* pApi,CZQThostFtdcRspInfoField* pRspInfo,int nRequestID,bool bIsLast);
	void Input_OnRspOrderAction(void* pTraderApi,CZQThostFtdcInputOrderActionField *pInputOrderAction, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void Input_OnRspOrderInsert(void* pTraderApi,CZQThostFtdcInputOrderField *pInputOrder, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void Input_OnRspQryDepthMarketData(void* pTraderApi,CZQThostFtdcDepthMarketDataField *pDepthMarketData, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void Input_OnRspQryInvestorPosition(void* pTraderApi,CZQThostFtdcInvestorPositionField *pInvestorPosition, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void Input_OnRspQryInvestorPositionDetail(void* pTraderApi,CZQThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void Input_OnRspQryInstrument(void* pTraderApi,CZQThostFtdcInstrumentField *pInstrument, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void Input_OnRspQryInstrumentCommissionRate(void* pTraderApi,CZQThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void Input_OnRspQryInstrumentMarginRate(void* pTraderApi,CZQThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void Input_OnRspQryOrder(void* pTraderApi,CZQThostFtdcOrderField *pOrder, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void Input_OnRspQryTrade(void* pTraderApi,CZQThostFtdcTradeField *pTrade, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void Input_OnRspQryTradingAccount(void* pTraderApi,CZQThostFtdcTradingAccountField *pTradingAccount, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void Input_OnRtnDepthMarketData(void* pMdApi,CZQThostFtdcDepthMarketDataField *pDepthMarketData);
	void Input_OnRtnInstrumentStatus(void* pTraderApi,CZQThostFtdcInstrumentStatusField *pInstrumentStatus);
	void Input_OnRtnOrder(void* pTraderApi,CZQThostFtdcOrderField *pOrder);
	void Input_OnRtnTrade(void* pTraderApi,CZQThostFtdcTradeField *pTrade);
private:
	friend DWORD WINAPI ProcessThread(LPVOID lpParam);
	void RunInThread();

	//响应结果直接入队列
	void _Input(SMsgItem* pMsgItem);
	//队列中的消息判断分发
	void _Output(SMsgItem* pMsgItem);

	//响应输出
	void Output_OnConnect(SMsgItem* pItem)
	{
		if(m_fnOnConnect)
			(*m_fnOnConnect)(pItem->pApi,&pItem->RspUserLogin,pItem->Status);
	}
	void Output_OnDisconnect(SMsgItem* pItem)
	{
		if(m_fnOnDisconnect)
			(*m_fnOnDisconnect)(pItem->pApi,&pItem->RspInfo,pItem->Status);
	}
	void Output_OnErrRtnOrderAction(SMsgItem* pItem)
	{
		if(m_fnOnErrRtnOrderAction)
			(*m_fnOnErrRtnOrderAction)(pItem->pApi,&pItem->OrderAction,&pItem->RspInfo);
	}
	void Output_OnErrRtnOrderInsert(SMsgItem* pItem)
	{
		if(m_fnOnErrRtnOrderInsert)
			(*m_fnOnErrRtnOrderInsert)(pItem->pApi,&pItem->InputOrder,&pItem->RspInfo);
	}
	void Output_OnRspError(SMsgItem* pItem)
	{
		if(m_fnOnRspError)
			(*m_fnOnRspError)(pItem->pApi,&pItem->RspInfo,pItem->nRequestID,pItem->bIsLast);
	}
	void Output_OnRspOrderAction(SMsgItem* pItem)
	{
		if(m_fnOnRspOrderAction)
			(*m_fnOnRspOrderAction)(pItem->pApi,&pItem->InputOrderAction,&pItem->RspInfo,pItem->nRequestID,pItem->bIsLast);
	}
	void Output_OnRspOrderInsert(SMsgItem* pItem)
	{
		if(m_fnOnRspOrderInsert)
			(*m_fnOnRspOrderInsert)(pItem->pApi,&pItem->InputOrder,&pItem->RspInfo,pItem->nRequestID,pItem->bIsLast);
	}
	void Output_OnRspQryInvestorPosition(SMsgItem* pItem)
	{
		if(m_fnOnRspQryInvestorPosition)
			(*m_fnOnRspQryInvestorPosition)(pItem->pApi,&pItem->InvestorPosition,&pItem->RspInfo,pItem->nRequestID,pItem->bIsLast);
	}
	void Output_OnRspQryInvestorPositionDetail(SMsgItem* pItem)
	{
		if(m_fnOnRspQryInvestorPositionDetail)
			(*m_fnOnRspQryInvestorPositionDetail)(pItem->pApi,&pItem->InvestorPositionDetail,&pItem->RspInfo,pItem->nRequestID,pItem->bIsLast);
	}
	void Output_OnRspQryDepthMarketData(SMsgItem* pItem)
	{
		if(m_fnOnRspQryDepthMarketData)
			(*m_fnOnRspQryDepthMarketData)(pItem->pApi,&pItem->DepthMarketData,&pItem->RspInfo,pItem->nRequestID,pItem->bIsLast);
	}
	void Output_OnRspQryInstrument(SMsgItem* pItem)
	{
		if(m_fnOnRspQryInstrument)
			(*m_fnOnRspQryInstrument)(pItem->pApi,&pItem->Instrument,&pItem->RspInfo,pItem->nRequestID,pItem->bIsLast);
	}
	void Output_OnRspQryInstrumentCommissionRate(SMsgItem* pItem)
	{
		if(m_fnOnRspQryInstrumentCommissionRate)
			(*m_fnOnRspQryInstrumentCommissionRate)(pItem->pApi,&pItem->InstrumentCommissionRate,&pItem->RspInfo,pItem->nRequestID,pItem->bIsLast);
	}
	void Output_OnRspQryInstrumentMarginRate(SMsgItem* pItem)
	{
		if(m_fnOnRspQryInstrumentMarginRate)
			(*m_fnOnRspQryInstrumentMarginRate)(pItem->pApi,&pItem->InstrumentMarginRate,&pItem->RspInfo,pItem->nRequestID,pItem->bIsLast);
	}
	void Output_OnRspQryOrder(SMsgItem* pItem)
	{
		if(m_fnOnRspQryOrder)
			(*m_fnOnRspQryOrder)(pItem->pApi,&pItem->Order,&pItem->RspInfo,pItem->nRequestID,pItem->bIsLast);
	}
	void Output_OnRspQryTrade(SMsgItem* pItem)
	{
		if(m_fnOnRspQryTrade)
			(*m_fnOnRspQryTrade)(pItem->pApi,&pItem->Trade,&pItem->RspInfo,pItem->nRequestID,pItem->bIsLast);
	}
	void Output_OnRspQryTradingAccount(SMsgItem* pItem)
	{
		if(m_fnOnRspQryTradingAccount)
			(*m_fnOnRspQryTradingAccount)(pItem->pApi,&pItem->TradingAccount,&pItem->RspInfo,pItem->nRequestID,pItem->bIsLast);
	}
	void Output_OnRtnDepthMarketData(SMsgItem* pItem)
	{
		if(m_fnOnRtnDepthMarketData)
			(*m_fnOnRtnDepthMarketData)(pItem->pApi,&pItem->DepthMarketData);
	}
	void Output_OnRtnInstrumentStatus(SMsgItem* pItem)
	{
		if(m_fnOnRtnInstrumentStatus)
			(*m_fnOnRtnInstrumentStatus)(pItem->pApi,&pItem->InstrumentStatus);
	}
	void Output_OnRtnOrder(SMsgItem* pItem)
	{
		if(m_fnOnRtnOrder)
			(*m_fnOnRtnOrder)(pItem->pApi,&pItem->Order);
	}
	void Output_OnRtnTrade(SMsgItem* pItem)
	{
		if(m_fnOnRtnTrade)
			(*m_fnOnRtnTrade)(pItem->pApi,&pItem->Trade);
	}

private:
	HANDLE						m_hEvent;
	bool						m_bRunning;
	HANDLE						m_hThread;

	MSQueue<SMsgItem*>			m_queue;			//响应队列

	//回调函数指针（按字母排序）
	fnOnConnect							m_fnOnConnect;
	fnOnDisconnect						m_fnOnDisconnect;
	fnOnErrRtnOrderAction				m_fnOnErrRtnOrderAction;
	fnOnErrRtnOrderInsert				m_fnOnErrRtnOrderInsert;
	fnOnRspError						m_fnOnRspError;
	fnOnRspOrderAction					m_fnOnRspOrderAction;
	fnOnRspOrderInsert					m_fnOnRspOrderInsert;
	fnOnRspQryDepthMarketData			m_fnOnRspQryDepthMarketData;
	fnOnRspQryInstrument				m_fnOnRspQryInstrument;
	fnOnRspQryInstrumentCommissionRate	m_fnOnRspQryInstrumentCommissionRate;
	fnOnRspQryInstrumentMarginRate		m_fnOnRspQryInstrumentMarginRate;
	fnOnRspQryInvestorPosition			m_fnOnRspQryInvestorPosition;
	fnOnRspQryInvestorPositionDetail	m_fnOnRspQryInvestorPositionDetail;
	fnOnRspQryOrder						m_fnOnRspQryOrder;
	fnOnRspQryTrade						m_fnOnRspQryTrade;
	fnOnRspQryTradingAccount			m_fnOnRspQryTradingAccount;
	fnOnRtnDepthMarketData				m_fnOnRtnDepthMarketData;
	fnOnRtnInstrumentStatus				m_fnOnRtnInstrumentStatus;
	fnOnRtnOrder						m_fnOnRtnOrder;
	fnOnRtnTrade						m_fnOnRtnTrade;
};

