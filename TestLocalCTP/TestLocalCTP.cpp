// TestLocalCTP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
// 本文件是测试 LocalCTP 的 DEMO, 仅用于展示 LocalCTP 的部分功能。

#include <iostream>
#include <cstring>
#include <string>
#include <fstream> // std::ofstream
#include <atomic>
#include <thread>
#include <chrono>
#include <mutex>
#include <map>
#include "ThostFtdcTraderApi.h"//CTP交易的头文件
#include <iconv.h>
#include <stdexcept>

// 复制自 HS_HFT 的 iconv_convert(见 include/hs_hft_tools/hs_hft_tools.hpp),
// 用于把 CTP 回调中的 GBK 字符串转成 UTF-8 后再打印.
// 与示例的差异: 转换失败直接抛异常(不静默返回空串), 字符集用 GB18030(GB2312/GBK 的超集).
class iconv_convert
{
public:
    static std::string gbk_to_utf8(const std::string& strGBK)
    {
        const std::size_t length = strGBK.size() * 2 + 1;
        std::string strResult(length, '\0');
        if (code_convert("GB18030", "UTF-8", strGBK, &strResult[0], length) < 0)
        {
            throw std::runtime_error("iconv_convert::gbk_to_utf8 failed, str:" + strGBK);
        }
        strResult.resize(std::strlen(strResult.c_str()));
        return strResult;
    }

private:
    static int code_convert(const char* from_charset, const char* to_charset,
        const std::string& strIn, char* outbuf, std::size_t outlen)
    {
        iconv_t cd = iconv_open(to_charset, from_charset);
        // iconv_open 失败时返回 (iconv_t)-1 而非 0
        if (cd == (iconv_t)-1)
        {
            return -1;
        }
        std::memset(outbuf, 0, outlen);
        char* inbuf = const_cast<char*>(strIn.c_str());
        char* outptr = outbuf;
        std::size_t inlen = strIn.size();
        std::size_t outleft = outlen - 1; // 保留 NUL 终止位
        const int ret = iconv(cd, &inbuf, &inlen, &outptr, &outleft);
        iconv_close(cd);
        return (ret == (std::size_t)-1) ? -1 : 0;
    }
};

// 打印 CTP 回调中的 GBK 字符串前统一转成 UTF-8
inline std::string u8(const char* gbkStr)
{
    return iconv_convert::gbk_to_utf8(std::string(gbkStr));
}
CThostFtdcInputOrderField generateNewOrderMsg(const char* OrderRef, const char* InstrumentID = "MA401")
{
    CThostFtdcInputOrderField InputOrder = { 0 };
    strcpy(InputOrder.UserID, "TestUserID");
    strcpy(InputOrder.InvestorID, "TestUserID");
    strcpy(InputOrder.AccountID, "TestUserID");
    strcpy(InputOrder.BrokerID, "9876");
    strcpy(InputOrder.ExchangeID, "CZCE");
    strcpy(InputOrder.InstrumentID, InstrumentID);
    strcpy(InputOrder.OrderRef, OrderRef);
    InputOrder.VolumeTotalOriginal = 1;
    InputOrder.LimitPrice = 5000.0;
    InputOrder.Direction = THOST_FTDC_D_Buy;
    InputOrder.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    InputOrder.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    InputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    InputOrder.VolumeCondition = THOST_FTDC_VC_AV;
    InputOrder.ContingentCondition = THOST_FTDC_CC_Immediately;
    InputOrder.TimeCondition = THOST_FTDC_TC_GFD;//若为 THOST_FTDC_TC_IOC + THOST_FTDC_VC_CV 则为IOC订单
    return InputOrder;
}

CThostFtdcInputOrderActionField generateCancelOrderMsg(const char* OrderRef, const char* InstrumentID = "MA401",
    int frontID = 0, int sessionID = 0)
{
    CThostFtdcInputOrderActionField InputOrderAction = { 0 };
    strcpy(InputOrderAction.UserID, "TestUserID");
    strcpy(InputOrderAction.InvestorID, "TestUserID");
    strcpy(InputOrderAction.BrokerID, "9876");
    strcpy(InputOrderAction.ExchangeID, "CZCE");
    strcpy(InputOrderAction.InstrumentID, InstrumentID);
    strcpy(InputOrderAction.OrderRef, OrderRef);
    InputOrderAction.FrontID = frontID;
    InputOrderAction.SessionID = sessionID;
    InputOrderAction.ActionFlag = THOST_FTDC_AF_Delete;
    return InputOrderAction;
}

CThostFtdcTraderApi* pApi = nullptr;
int g_frontID = 0;
int g_sessionID = 0;

class MySpi : public CThostFtdcTraderSpi
{
public:
    //界面/主线程需要等待认证回调完成后再发登录请求(回调由消息队列线程触发)
    std::atomic<bool> m_authenticated{ false };
    //组合单分腿成交统计（--auto 模式的自检用）：按合约代码累加成交手数/条数
    std::atomic<int> m_rtn_trade_count{ 0 };
    std::mutex m_combo_trade_mutex;
    std::map<std::string, int> m_combo_leg_volume;   // 合约 -> 累计成交手数
    std::map<std::string, std::string> m_combo_leg_direction; // 合约 -> 首次成交方向

private:
    void OnFrontConnected(){
        std::cout << "收到连接成功通知!" << std::endl;
        CThostFtdcReqAuthenticateField ReqAuthenticateField = { "9876", "TestUserID", "Test", "TestAuthCode", "TestAppID" };
        pApi->ReqAuthenticate(&ReqAuthenticateField, 100);
    }
    void OnRspAuthenticate(CThostFtdcRspAuthenticateField* pRspAuthenticateField, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
        std::cout << "收到认证响应! " << " UserID:" << pRspAuthenticateField->UserID
            << " errorID:"<< pRspInfo->ErrorID<<" errorMsg:"<< u8(pRspInfo->ErrorMsg) << std::endl;
        m_authenticated = true;
    }
    void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
        std::cout << "收到登录响应! " << " UserID:" << pRspUserLogin->UserID
            << " errorID:" << pRspInfo->ErrorID << " errorMsg:" << u8(pRspInfo->ErrorMsg)
            << " TradingDay:" << pRspUserLogin->TradingDay << std::endl;
        g_frontID = pRspUserLogin->FrontID;
        g_sessionID = pRspUserLogin->SessionID;
    }
    void OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
        std::cout << "收到查询合约响应! " << " errorID:" << pRspInfo->ErrorID << " errorMsg:" << u8(pRspInfo->ErrorMsg)
            << ", bIsLast:" << bIsLast << std::endl;
        if (pInstrument)
        {
            std::cout << "InstrumentID:" << pInstrument->InstrumentID
                << ", ExchangeID:" << pInstrument->ExchangeID
                << ", InstrumentName:" << u8(pInstrument->InstrumentName)
                << ", PriceTick:" << pInstrument->PriceTick
                << ", VolumeMultiple:" << pInstrument->VolumeMultiple
                << std::endl;
        }
    }
    void OnRspQryClassifiedInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
        std::cout << "收到查询分类合约响应! " << " errorID:" << pRspInfo->ErrorID << " errorMsg:" << u8(pRspInfo->ErrorMsg)
            << ", bIsLast:" << bIsLast << std::endl;
        if (pInstrument)
        {
            std::cout << "InstrumentID:" << pInstrument->InstrumentID
                << ", ExchangeID:" << pInstrument->ExchangeID
                << ", InstrumentName:" << u8(pInstrument->InstrumentName)
                << ", PriceTick:" << pInstrument->PriceTick
                << ", VolumeMultiple:" << pInstrument->VolumeMultiple
                << std::endl;
        }
    }
    void OnRspQryOrder(CThostFtdcOrderField* pOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
        std::cout << "收到查询报单响应! " << " errorID:" << pRspInfo->ErrorID << " errorMsg:" << u8(pRspInfo->ErrorMsg)
            << ", bIsLast:" << bIsLast << std::endl;
        if (pOrder)
        {
            std::cout << "InstrumentID:" << pOrder->InstrumentID
                << ", ExchangeID:" << pOrder->ExchangeID
                << ", OrderSysID:" << pOrder->OrderSysID
                << ", OrderStatus:" << pOrder->OrderStatus
                << std::endl;
        }
    }
    void OnRspQryTrade(CThostFtdcTradeField* pTrade, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
        std::cout << "收到查询成交响应! " << " errorID:" << pRspInfo->ErrorID << " errorMsg:" << u8(pRspInfo->ErrorMsg)
            << ", bIsLast:" << bIsLast << std::endl;
        if (pTrade)
        {
            std::cout << "InstrumentID:" << pTrade->InstrumentID
                << ", ExchangeID:" << pTrade->ExchangeID
                << ", OrderSysID:" << pTrade->OrderSysID
                << ", TradeID:" << pTrade->TradeID
                << std::endl;
        }
    }
    void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField* pInvestorPosition, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
        std::cout << "收到查询持仓响应! " << " errorID:" << pRspInfo->ErrorID << " errorMsg:" << u8(pRspInfo->ErrorMsg)
            << ", bIsLast:" << bIsLast << std::endl;
        if (pInvestorPosition)
        {
            std::cout << "InstrumentID:" << pInvestorPosition->InstrumentID
                << ", ExchangeID:" << pInvestorPosition->ExchangeID
                << ", PosiDirection:" << (pInvestorPosition->PosiDirection== THOST_FTDC_PD_Long ? "多头":"空头")
                << ", PositionDate:" << pInvestorPosition->PositionDate
                << ", Position:" << pInvestorPosition->Position
                << std::endl;
        }
    }
    void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField* pSettlementInfo, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
        std::cout << "收到查询结算单响应! " << " errorID:" << pRspInfo->ErrorID << " errorMsg:" << u8(pRspInfo->ErrorMsg)
            << ", bIsLast:" << bIsLast << std::endl;
        if (pSettlementInfo)
        {
            //将结算单保存到本地文件中,文件名包含 账户名 和 结算单的交易日
            static std::ofstream o(std::string("./") + pSettlementInfo->AccountID
                + "_" + pSettlementInfo->TradingDay + "_settlement.log");
            o << pSettlementInfo->Content;//Content末尾可能正好有中文字符被截断(即分别在两个不同的消息里返回), 这里没有做处理
            o.flush();

            std::cout << "结算单 交易日:" << pSettlementInfo->TradingDay
                << ", 消息正文:" << u8(pSettlementInfo->Content)
                << std::endl;
            
        }
    }
    void OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
        std::cout << "\n收到(有错误的)报单响应! " << " UserID:" << pInputOrder->UserID
            << " InstrumentID:" << pInputOrder->InstrumentID
            << " errorID:" << pRspInfo->ErrorID << " errorMsg:" << u8(pRspInfo->ErrorMsg) << std::endl;
    }
    void OnRtnOrder(CThostFtdcOrderField* pOrder) {
        std::cout << "\n收到报单通知! " << " UserID:" << pOrder->UserID
            << " InstrumentID:" << pOrder->InstrumentID
            << " OrderSysID:" << pOrder->OrderSysID
            << " OrderStatus:" << pOrder->OrderStatus << " StatusMsg:" << u8(pOrder->StatusMsg) << std::endl;
    }
    void OnRtnTrade(CThostFtdcTradeField* pTrade) {
        ++m_rtn_trade_count;
        // 自检只统计组合单（OrderRef=2000）的分腿成交：普通单/条件单/期权的成交不能污染组合判定
        if (std::strcmp(pTrade->OrderRef, "2000") == 0)
        {
            std::lock_guard<std::mutex> guard(m_combo_trade_mutex);
            m_combo_leg_volume[pTrade->InstrumentID] += pTrade->Volume;
            m_combo_leg_direction.emplace(pTrade->InstrumentID,
                pTrade->Direction == THOST_FTDC_D_Buy ? "买入" : "卖出");
        }
        std::cout << "\n收到成交通知! " << " UserID:" << pTrade->UserID
            << " InstrumentID:" << pTrade->InstrumentID
            << " Direction:" << (pTrade->Direction == THOST_FTDC_D_Buy ? "买入":"卖出")
            << " OffsetFlag:" << (pTrade->OffsetFlag == THOST_FTDC_OF_Open ? "开仓" : "平仓")
            << " Volume:" << pTrade->Volume << " Price:" << pTrade->Price << std::endl;
    }
    void OnRspOrderAction(CThostFtdcInputOrderActionField* pInputOrderAction, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
        std::cout << "\n收到(有错误的)撤单响应! " << " UserID:" << pInputOrderAction->UserID
            << " InstrumentID:" << pInputOrderAction->InstrumentID
            << " errorID:" << pRspInfo->ErrorID << " errorMsg:" << u8(pRspInfo->ErrorMsg) << std::endl;
    }

};


int main(int argc, char** argv)
{
    // --auto：无人值守模式（跳过所有 std::cin），跑完打印自检结果后退出
    const bool auto_mode = (argc > 1 && std::strcmp(argv[1], "--auto") == 0);
    int i;
    pApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
    std::cout << pApi->GetApiVersion() << std::endl;
    //std::cout << pApi->GetTradingDay() << std::endl;
    MySpi spi;
    pApi->RegisterSpi(&spi);
    pApi->Init();

    if (!auto_mode) {
        std::cout << "请输入任意数字以登录..." << std::endl;
        std::cin >> i;
    } else {
        std::cout << "[AUTO] 跳过交互输入，自动登录" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    //等待认证回调完成, 否则ReqUserLogin会因尚未认证而失败
    while (!spi.m_authenticated)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    CThostFtdcReqUserLoginField ReqUserLoginField = { "", "9876", "TestUserID", "TestPassword" };
    pApi->ReqUserLogin(&ReqUserLoginField, 100);
    CThostFtdcQrySettlementInfoField QrySettlementInfo = { "9876", "TestUserID" };
    pApi->ReqQrySettlementInfo(&QrySettlementInfo, 0);//查询结算单
    CThostFtdcSettlementInfoConfirmField confirm = { "9876", "TestUserID" };
    pApi->ReqSettlementInfoConfirm(&confirm, 0);//确认结算单

    CThostFtdcQryInstrumentField QryInstrument = { 0 };
    strcpy(QryInstrument.ProductID, "IC");
    pApi->ReqQryInstrument(&QryInstrument, 108);
#if 0
    CThostFtdcQryClassifiedInstrumentField QryClassifiedInstrument = { 0 };
    strcpy(QryClassifiedInstrument.ProductID, "IF");
    QryClassifiedInstrument.ClassType = THOST_FTDC_INS_ALL;
    pApi->ReqQryClassifiedInstrument(&QryClassifiedInstrument, 108);
#endif

    CThostFtdcQryProductField QryProduct = { 0 };
    strcpy(QryProduct.ProductID, "jd");
    QryProduct.ProductClass = THOST_FTDC_PC_Futures;
    pApi->ReqQryProduct(&QryProduct, 108);

    // 查询完毕, 开始下单
    auto InputOrder = generateNewOrderMsg("1000", "MA509");
    int ret = pApi->ReqOrderInsert(&InputOrder,109);//没有行情数据时,下一单(预期被拒单)

    CThostFtdcDepthMarketDataField md = { 0 };
    //报入组合合约的委托, 并不需要有组合合约的行情, 而只需要有两个单腿合约的行情
    //strcpy(md.InstrumentID, "SPD MA309&MA401");
    //md.BidPrice1 = 4900;
    //md.AskPrice1 = 5000;
    //md.PreSettlementPrice = 6000;
    //pApi->RegisterFensUserInfo((CThostFtdcFensUserInfoField*)&md);//喂一个行情快照给API
    strcpy(md.InstrumentID, "MA509");
    md.BidPrice1 = 1000;
    md.AskPrice1 = 1010;
    md.SettlementPrice = md.PreSettlementPrice = 1020;
    strcpy(md.TradingDay, "20250228");
    strcpy(md.UpdateTime, "10:55:58");
    pApi->RegisterFensUserInfo((CThostFtdcFensUserInfoField*)&md);//喂一个行情快照给API
    strcpy(md.InstrumentID, "MA511");
    md.BidPrice1 = 2000;
    md.AskPrice1 = 2010;
    md.SettlementPrice = md.PreSettlementPrice = 2020;
    strcpy(md.UpdateTime, "10:55:59");
    pApi->RegisterFensUserInfo((CThostFtdcFensUserInfoField*)&md);//喂一个行情快照给API

    InputOrder = generateNewOrderMsg("2000", "SPD MA509&MA511");
    InputOrder.LimitPrice = -985;//实际会以1010-2000 = (-990)元的差价成交
    // 真实 CTP 的套利单（SP/SPD）两条腿都是开仓（一多一空），不是互换单；
    // 互换单(IsSwapOrder=1)要求第二腿已有持仓，用在开仓组合单上会被本地柜台拒掉。
    InputOrder.IsSwapOrder = 0;
    ret = pApi->ReqOrderInsert(&InputOrder, 109);//有行情数据后,再下一单(买入开仓成交)

    strcpy(InputOrder.OrderRef, "2001");
    InputOrder.Direction = THOST_FTDC_D_Sell;
    InputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
    //    InputOrder.LimitPrice = -1111;//卖出时价格足够低则会成交
    ret = pApi->ReqOrderInsert(&InputOrder, 109);//有行情数据后,再下一单(卖出平仓)

    auto InputOrderAction = generateCancelOrderMsg("2001", "SPD MA509&MA511", g_frontID, g_sessionID);
    ret = pApi->ReqOrderAction(&InputOrderAction, 110);//把这个单子撤掉

    InputOrder = generateNewOrderMsg("1002", "MA509");
    InputOrder.Direction = THOST_FTDC_D_Buy;
    InputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    InputOrder.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterThanStopPrice;//下一个条件单
    InputOrder.StopPrice = 4998;
    ret = pApi->ReqOrderInsert(&InputOrder, 109);

    CThostFtdcInputQuoteField marketData = { 0 };
    strcpy(marketData.InstrumentID, "MA509");
    marketData.BidPrice = 1000;
    marketData.AskPrice = 1010;
    strcpy(marketData.QuoteRef, "1020.0");// PreSettlementPrice
    strcpy(marketData.UserID, "4999");// LastPrice.   4999 > 4998
    strcpy(marketData.ForQuoteSysID, "4999");// SettlementPrice
    strcpy(marketData.BidOrderRef, "9999.0");// UpperLimitPrice
    strcpy(marketData.AskOrderRef, "500.0");// LowerLimitPrice
    strcpy(marketData.BusinessUnit, "8888888");// OpenInterest
    strcpy(marketData.ClientID, "10:00:00");// UpdateTime
    int Volume = 88888;
    pApi->ReqQuoteInsert(&marketData, Volume);//使用ReqQuoteInsert接口, 喂一个行情快照给API(以触发条件单), 

    strcpy(md.InstrumentID, "IO2509-C-3300");
    md.BidPrice1 = 10.0;
    md.AskPrice1 = 10.4;
    md.SettlementPrice = md.PreSettlementPrice = 9.0;
    pApi->RegisterFensUserInfo((CThostFtdcFensUserInfoField*)&md);//喂一个(期权)行情快照给API
    InputOrder = generateNewOrderMsg("1004", "IO2509-C-3300");
    strcpy(InputOrder.ExchangeID, "CFFEX");
    InputOrder.LimitPrice = 10.4;
    InputOrder.VolumeTotalOriginal = 2;
    ret = pApi->ReqOrderInsert(&InputOrder, 110);//有行情数据后,再下一单(买入开仓2手)
    strcpy(InputOrder.OrderRef, "1005");
    InputOrder.Direction = THOST_FTDC_D_Sell;
    InputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
    InputOrder.LimitPrice = 10.0;
    InputOrder.VolumeTotalOriginal = 1;
    ret = pApi->ReqOrderInsert(&InputOrder, 111);//有行情数据后,再下一单(卖出平仓1手)

    CThostFtdcQryOrderField QryOrder = { "9876","TestUserID" };
    strcpy(QryOrder.ExchangeID, "CZCE");// 有数条CZCE的委托记录,查询返回
    ret = pApi->ReqQryOrder(&QryOrder, 100);

    CThostFtdcQryTradeField QryTrade = { "9876","TestUserID" };
    strcpy(QryTrade.ExchangeID, "DCE");// 没有DCE的成交记录,查询返回空
    ret = pApi->ReqQryTrade(&QryTrade, 100);

    CThostFtdcQryTradingAccountField QryTradingAccount = { "9876","TestUserID" };
    ret = pApi->ReqQryTradingAccount(&QryTradingAccount, 100);

    CThostFtdcQryInvestorPositionField QryInvestorPosition = { "9876","TestUserID" };
    strcpy(QryTrade.ExchangeID, "CZCE");
    ret = pApi->ReqQryInvestorPosition(&QryInvestorPosition, 100);

    strcpy(md.TradingDay, "20250305");
    strcpy(md.UpdateTime, "09:00:00");
    pApi->RegisterFensUserInfo((CThostFtdcFensUserInfoField*)&md);//喂一个(晚于应执行结算的时间的)行情快照给API(让API进行结算)(仅限回测模式)

    if (!auto_mode) {
        std::cin >> i;
        std::cout << ret << std::endl;
        pApi->Release();
        std::cin >> i;
        return 0;
    }

    // --auto 自检：轮询等两腿成对回报到齐（最多 5s，避免固定等待不足/过长），
    // 再打印组合单分腿成交汇总并断言两腿各 1 手、方向相反。
    for (int wait_round = 0; wait_round < 100; ++wait_round)
    {
        bool both_legs_seen = false;
        {
            std::lock_guard<std::mutex> guard(spi.m_combo_trade_mutex);
            both_legs_seen = spi.m_combo_leg_volume.count("MA509") > 0 &&
                spi.m_combo_leg_volume.count("MA511") > 0;
        }
        if (both_legs_seen)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    std::cout << "\n[AUTO] 成交通知条数=" << spi.m_rtn_trade_count.load() << std::endl;
    bool combo_ok = false;
    {
        std::lock_guard<std::mutex> guard(spi.m_combo_trade_mutex);
        for (const auto& kv : spi.m_combo_leg_volume)
        {
            const auto it_direction = spi.m_combo_leg_direction.find(kv.first);
            std::cout << "[AUTO] 分腿成交 " << kv.first << " 累计手数=" << kv.second
                << " 方向=" << (it_direction == spi.m_combo_leg_direction.end() ? "?" : it_direction->second)
                << std::endl;
        }
        // 全部读取都在同一把锁内用局部变量完成，不用 operator[]（会插入并产生数据竞争）
        const auto it_vol1 = spi.m_combo_leg_volume.find("MA509");
        const auto it_vol2 = spi.m_combo_leg_volume.find("MA511");
        const auto it_dir1 = spi.m_combo_leg_direction.find("MA509");
        const auto it_dir2 = spi.m_combo_leg_direction.find("MA511");
        combo_ok = it_vol1 != spi.m_combo_leg_volume.end() && it_vol2 != spi.m_combo_leg_volume.end() &&
            it_dir1 != spi.m_combo_leg_direction.end() && it_dir2 != spi.m_combo_leg_direction.end() &&
            it_vol1->second == 1 && it_vol2->second == 1 &&
            it_dir1->second != it_dir2->second;
    }
    std::cout << "[AUTO] 组合单分腿成对回报检查: " << (combo_ok ? "PASS" : "FAIL") << std::endl;
    pApi->Release();
    return combo_ok ? 0 : 1;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
