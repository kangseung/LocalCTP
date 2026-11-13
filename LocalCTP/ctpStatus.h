#pragma once
// 本文件为 UTF-8 编码. 字符串字面量中的中文写成 GBK 字节的八进制转义(\NNN),
// 以保证写入 CTP 的 char 字段的仍是 GBK 编码(本文件原为 GB2312 编码, 会让 clangd 报 invalid UTF-8).
// 每个常量上方保留 UTF-8 中文原文以便阅读.
#include "ThostFtdcUserApiDataType.h"
#include "stdafx.h"

namespace localCTP{

// CTP:认证失败(UserID和BrokerID不能为空哦)
constexpr TThostFtdcErrorMsgType ErrMsgUserInfoIsEmpty = "CTP:\310\317\326\244\312\247\260\334(UserID\272\315BrokerID\262\273\304\334\316\252\277\325\305\266)";
// CTP:认证失败(UserID和BrokerID需要和上次登录时相同哦)
constexpr TThostFtdcErrorMsgType ErrMsgUserInfoNotSameAsLastTime = "CTP:\310\317\326\244\312\247\260\334(UserID\272\315BrokerID\320\350\322\252\272\315\311\317\264\316\265\307\302\274\312\261\317\340\315\254\305\266)";
// CTP:登录失败(没有认证)
constexpr TThostFtdcErrorMsgType ErrMsgNotAuth = "CTP:\265\307\302\274\312\247\260\334(\303\273\323\320\310\317\326\244)";
// CTP:登录失败(UserID和BrokerID需要和认证时相同哦)
constexpr TThostFtdcErrorMsgType ErrMsgUserInfoNotSameAsAuth = "CTP:\265\307\302\274\312\247\260\334(UserID\272\315BrokerID\320\350\322\252\272\315\310\317\326\244\312\261\317\340\315\254\305\266)";
// CTP:重复的报单(OrderRef需比此前的最大值要大哦)
constexpr TThostFtdcErrorMsgType ErrMsgDuplicateOrder = "CTP:\326\330\270\264\265\304\261\250\265\245(OrderRef\320\350\261\310\264\313\307\260\265\304\327\356\264\363\326\265\322\252\264\363\305\266)";
// CTP:不合法的报单数量(需为正数)
constexpr TThostFtdcErrorMsgType ErrMsg_INVALID_ORDERSIZE = "CTP:\262\273\272\317\267\250\265\304\261\250\265\245\312\375\301\277(\320\350\316\252\325\375\312\375)";
// CTP:交易所代码错误
constexpr TThostFtdcErrorMsgType ErrMsg_EXCHANGE_ID_IS_WRONG = "CTP:\275\273\322\327\313\371\264\372\302\353\264\355\316\363";
// CTP:找不到合约
constexpr TThostFtdcErrorMsgType ErrMsg_INSTRUMENT_NOT_FOUND = "CTP:\325\322\262\273\265\275\272\317\324\274";
// CTP:不支持的价格(需要是PriceTick的整数倍):
constexpr TThostFtdcErrorMsgType ErrMsg_BAD_PRICE_VALUE = "CTP:\262\273\326\247\263\326\265\304\274\333\270\361(\320\350\322\252\312\307PriceTick\265\304\325\373\312\375\261\266):";
// CTP:订单不存在哦
constexpr TThostFtdcErrorMsgType ErrMsg_NotExistOrder = "CTP:\266\251\265\245\262\273\264\346\324\332\305\266";
// CTP:订单已结束无法再撤哦
constexpr TThostFtdcErrorMsgType ErrMsg_AlreadyDoneOrder = "CTP:\266\251\265\245\322\321\275\341\312\370\316\336\267\250\324\331\263\267\305\266";
// CTP:订单只能撤单无法改单哦
constexpr TThostFtdcErrorMsgType ErrMsg_NotSupportModifyOrder = "CTP:\266\251\265\245\326\273\304\334\263\267\265\245\316\336\267\250\270\304\265\245\305\266";
// CTP:交易所不支持的价格类型(OrderPriceType)
constexpr TThostFtdcErrorMsgType ErrMsg_PRICETYPE_NOTSUPPORT_BYEXCHANGE = "CTP:\275\273\322\327\313\371\262\273\326\247\263\326\265\304\274\333\270\361\300\340\320\315(OrderPriceType)";
// CTP:报单字段有误(本系统只支持投机类型)(CombHedgeFlag)
constexpr TThostFtdcErrorMsgType ErrMsg_BAD_FIELD_ONLY_SPECULATION = "CTP:\261\250\265\245\327\326\266\316\323\320\316\363(\261\276\317\265\315\263\326\273\326\247\263\326\315\266\273\372\300\340\320\315)(CombHedgeFlag)";
// CTP:不支持特殊的条件单哦(ContingentCondition)
constexpr TThostFtdcErrorMsgType ErrMsg_NotSupportContingentCondition = "CTP:\262\273\326\247\263\326\314\330\312\342\265\304\314\365\274\376\265\245\305\266(ContingentCondition)";
// CTP:不支持的有效期类型哦(TimeCondition)
constexpr TThostFtdcErrorMsgType ErrMsg_NotSupportTimeCondition = "CTP:\262\273\326\247\263\326\265\304\323\320\320\247\306\332\300\340\320\315\305\266(TimeCondition)";
// CTP:该合约没有行情数据:
constexpr TThostFtdcErrorMsgType ErrMsg_NoMarketData = "CTP:\270\303\272\317\324\274\303\273\323\320\320\320\307\351\312\375\276\335:";
// CTP:该合约没有保证金率数据
constexpr TThostFtdcErrorMsgType ErrMsg_INSTRUMENT_MARGINRATE_NOT_FOUND = "CTP:\270\303\272\317\324\274\303\273\323\320\261\243\326\244\275\360\302\312\312\375\276\335";
// CTP:该合约没有手续费率数据:
constexpr TThostFtdcErrorMsgType ERRMSG_INSTRUMENT_COMMISSIONRATE_NOT_FOUND = "CTP:\270\303\272\317\324\274\303\273\323\320\312\326\320\370\267\321\302\312\312\375\276\335:";// not used
// CTP:可用资金不足
constexpr TThostFtdcErrorMsgType ERRMSG_AVAILABLE_NOT_ENOUGH = "CTP:\277\311\323\303\327\312\275\360\262\273\327\343";
// CTP:平仓时持仓不足,当前可平数量是:
constexpr TThostFtdcErrorMsgType ERRMSG_AVAILABLE_POSITION_NOT_ENOUGH = "CTP:\306\275\262\326\312\261\263\326\262\326\262\273\327\343,\265\261\307\260\277\311\306\275\312\375\301\277\312\307:";
// CTP:平今时持仓不足,当前可平数量是:
constexpr TThostFtdcErrorMsgType ERRMSG_AVAILABLE_TODAY_POSITION_NOT_ENOUGH = "CTP:\306\275\275\361\312\261\263\326\262\326\262\273\327\343,\265\261\307\260\277\311\306\275\312\375\301\277\312\307:";
constexpr char CONDITIONAL_ORDER_SYSID_PREFIX[10] = "TJBD_";
constexpr char STR_YUE[10] = "\324\302";

// 通过触发条件类型,判断是否是(符合本系统需求的)条件单
inline bool isConditionalType(TThostFtdcContingentConditionType	contingentCondition)
{
    return contingentCondition == THOST_FTDC_CC_LastPriceGreaterThanStopPrice ||
        contingentCondition == THOST_FTDC_CC_LastPriceGreaterEqualStopPrice ||
        contingentCondition == THOST_FTDC_CC_LastPriceLesserThanStopPrice ||
        contingentCondition == THOST_FTDC_CC_LastPriceLesserEqualStopPrice;
}

inline std::string getStatusMsgByStatus(TThostFtdcOrderStatusType status)
{
    switch (status)
    {
        case THOST_FTDC_OST_AllTraded: // '0'
            return "\310\253\262\277\263\311\275\273";                
        case THOST_FTDC_OST_PartTradedQueueing: // '1'
            return "\262\277\267\326\263\311\275\273";
        case THOST_FTDC_OST_PartTradedNotQueueing: // '2'
            return "\262\277\267\326\263\311\275\273\262\273\324\332\266\323\301\320\326\320";
        case THOST_FTDC_OST_NoTradeQueueing: // '3'
            return "\316\264\263\311\275\273";
        case THOST_FTDC_OST_NoTradeNotQueueing: // '4'
            return "\316\264\263\311\275\273\262\273\324\332\266\323\301\320\326\320";
        case THOST_FTDC_OST_Canceled: // '5'
            return "\322\321\263\267\265\245";
        case THOST_FTDC_OST_Unknown: // 'a'
            return "\316\264\326\252";
        case THOST_FTDC_OST_NotTouched: // 'b'
            return "\311\320\316\264\264\245\267\242";
        case THOST_FTDC_OST_Touched: // 'c'
            return "\322\321\264\245\267\242";
        default:
            return "\316\264\326\252";
    }
}

inline bool isTradingDay(const CLeeDateTime& date)
{
    const int dayOfWeek = date.GetDayOfWeek();
    if (dayOfWeek == 6 || dayOfWeek == 0)//TODO:不会判断长假假日
    {
        return false;
    }
    return true;
}

inline std::string getNextTradingDay(CLeeDateTime dt)
{
    do {
        dt += CLeeDateTimeSpan(1, 0, 0, 0);
    } while (!isTradingDay(dt));
    return dt.Format("%Y%m%d");
}

inline std::string get_direction_name(const std::string& direction)
{
    static const std::map<std::string, std::string> direction_name_map{
        {std::string(1, THOST_FTDC_D_Buy), "\302\362   "},
        {std::string(1, THOST_FTDC_D_Sell), "   \302\364"}
    };
    auto it_direction = direction_name_map.find(direction);
    if (it_direction != direction_name_map.end())
    {
        return it_direction->second;
    }
    else
    {
        return "\316\264\326\252";
    }
}
inline std::string get_direction_name(TThostFtdcDirectionType dir)
{
    return get_direction_name(std::string(1, dir));
}
inline std::string get_exchange_name(const std::string& ExchangeID)
{
    static const std::map<std::string, std::string> exchange_name_map{
        {"INE", "\304\334\324\264\326\320\320\304"},
        {"SHFE", "\311\317\306\332\313\371"},
        {"CFFEX", "\326\320\275\360\313\371"},
        {"DCE", "\264\363\311\314\313\371"},
        {"CZCE", "\326\243\311\314\313\371"},
        {"GFEX", "\271\343\306\332\313\371"},
        {"BSE", "\261\261\275\273\313\371"},
        {"SSE", "\311\317\275\273\313\371"},
        {"SHSE", "\311\317\275\273\313\371"},
        {"SZSE", "\311\356\275\273\313\371"}
    };
    auto it_exchange = exchange_name_map.find(ExchangeID);
    if (it_exchange != exchange_name_map.end())
    {
        return it_exchange->second;
    }
    else
    {
        return "\316\264\326\252";
    }
}
inline std::string get_open_close_name(const std::string& open_or_close)
{
    static const std::map<std::string, std::string> open_or_close_name_map{
        {std::string(1, THOST_FTDC_OF_Open), "\277\252"},
        {std::string(1, THOST_FTDC_OF_Close), "  \306\275"},
        {std::string(1, THOST_FTDC_OF_CloseToday), "\306\275\275\361"},
        {std::string(1, THOST_FTDC_OF_CloseYesterday), "\306\275\327\362"},
        {std::string(1, THOST_FTDC_OF_ForceClose), "\307\277\306\275"},
        {std::string(1, THOST_FTDC_OF_ForceOff), "\307\277\274\365"},
        {std::string(1, THOST_FTDC_OF_LocalForceClose), "\261\276\265\330\307\277\306\275"}
    };
    auto it_open_or_close = open_or_close_name_map.find(open_or_close);
    if (it_open_or_close != open_or_close_name_map.end())
    {
        return it_open_or_close->second;
    }
    else
    {
        return "\316\264\326\252";
    }
}
inline std::string get_open_close_name(TThostFtdcOffsetFlagType open_or_close)
{
    return get_open_close_name(std::string(1, open_or_close));
}
//判断是否是期权品种
inline bool isOptions(TThostFtdcProductClassType ProductClass)
{
    return ProductClass == THOST_FTDC_PC_Options ||
        ProductClass == THOST_FTDC_PC_SpotOption;
}

///平仓明细
struct CloseDetail
{
    ///经纪公司代码
    TThostFtdcBrokerIDType BrokerID;
    ///投资者代码
    TThostFtdcInvestorIDType InvestorID;
    ///交易所代码
    TThostFtdcExchangeIDType ExchangeID;
    ///合约代码
    TThostFtdcInstrumentIDType InstrumentID;
    ///开仓日期(交易日)
    TThostFtdcDateType OpenDate;
    ///开仓价格
    TThostFtdcPriceType OpenPrice;
    ///开仓成交编号
    TThostFtdcTradeIDType OpenTradeID;
    ///平仓日期(交易日)
    TThostFtdcDateType CloseDate;
    ///平仓时间
    TThostFtdcTimeType CloseTime;
    ///平仓成交价格
    TThostFtdcPriceType ClosePrice;
    ///平仓成交编号
    TThostFtdcTradeIDType CloseTradeID;
    ///平仓手数
    TThostFtdcVolumeType CloseVolume;
    ///平仓成交的买卖方向
    TThostFtdcDirectionType Direction;
    ///昨结算价
    TThostFtdcPriceType PreSettlementPrice;
    ///平仓盈亏
    TThostFtdcMoneyType CloseProfit;
    ///实际平仓类型(平今or平昨)
    TThostFtdcOffsetFlagType CloseFlag;
    ///权利金收支
    TThostFtdcMoneyType CashIn;
};

///结算单
struct SettlementData
{
    ///经纪公司代码
    TThostFtdcBrokerIDType BrokerID;
    ///投资者代码
    TThostFtdcInvestorIDType InvestorID;
    ///结算单内容
    std::string SettlementContent;
    ///结算单日期(交易日)
    TThostFtdcDateType TradingDay;
    ///确认日期
    TThostFtdcDateType ConfirmDay;
    ///确认时间
    TThostFtdcTimeType ConfirmTime;
};

const std::string SETTLEMENT_CONTEXT(
R"(settlement_header1=_                                            LocalCTP系统                                           
settlement_header2=_                                                                    制表时间 Creation Date：%8s
settlement_header3=_----------------------------------------------------------------------------------------------------
settlement_header4=_                             交易结算单(盯市) Settlement Statement(MTM)                             
settlement_header5=_客户号 Client ID：  %-16s客户名称 Client Name：%s
settlement_header6=_日期 Date：%8s
settlement_header7=_
settlement_header8=_
settlement_header9=_
settlement_header10=_
settlement_account_summary1=_                   资金状况  币种：人民币  Account Summary  Currency：CNY 
settlement_account_summary2=_----------------------------------------------------------------------------------------------------
settlement_account_summary3=_期初结存 Balance b/f：               %13.2f  基础保证金 Initial Margin：        %13.2f
settlement_account_summary4=_出 入 金 Deposit/Withdrawal：        %13.2f  期末结存 Balance c/f：             %13.2f
settlement_account_summary5=_平仓盈亏 Realized P/L：              %13.2f  质 押 金 Pledge Amount：           %13.2f
settlement_account_summary6=_持仓盯市盈亏 MTM P/L：               %13.2f  客户权益 Client Equity：：         %13.2f
settlement_account_summary7=_期权执行盈亏 Exercise P/L：          %13.2f  货币质押保证金占用 FX Pledge Occ.：%13.2f
settlement_account_summary8=_手 续 费 Commission：                %13.2f  保证金占用 Margin Occupied：       %13.2f
settlement_account_summary9=_行权手续费 Exercise Fee：            %13.2f  交割保证金 Delivery Margin：       %13.2f
settlement_account_summary10=_交割手续费 Delivery Fee：            %13.2f  多头期权市值 Market value(long)：  %13.2f
settlement_account_summary11=_货币质入 New FX Pledge：             %13.2f  空头期权市值 Market value(short)： %13.2f
settlement_account_summary12=_货币质出 FX Redemption：             %13.2f  市值权益 Market value(equity)：    %13.2f
settlement_account_summary13=_质押变化金额 Chg in Pledge Amt：     %13.2f  可用资金 Fund Avail.：             %13.2f
settlement_account_summary14=_权利金收入 Premium received：        %13.2f  风 险 度 Risk Degree：            %13.2f%%
settlement_account_summary15=_权利金支出 Premium paid：            %13.2f  应追加资金 Margin Call：           %13.2f
settlement_account_summary16=_运行模式 Running Mode：              %s  结算时间 Settle Time：       %s
settlement_account_summary17=_
settlement_deposit_withdrawal_head1=_                                        出入金明细 Deposit/Withdrawal 
settlement_deposit_withdrawal_head2=_----------------------------------------------------------------------------------------------------------------
settlement_deposit_withdrawal_head3=_|发生日期|       出入金类型       |      入金      |      出金      |                   说明                   |
settlement_deposit_withdrawal_head4=_|  Date  |          Type          |    Deposit     |   Withdrawal   |                   Note                   |
settlement_deposit_withdrawal_head5=_----------------------------------------------------------------------------------------------------------------
settlement_deposit_withdrawal_single_record1=_|%-8s|出入金                  |%16.2f|%16.2f|%-42s|
settlement_deposit_withdrawal_end1=_----------------------------------------------------------------------------------------------------------------
settlement_deposit_withdrawal_end2=_|共%4d条|                        |%16.2f|%16.2f|                                          |
settlement_deposit_withdrawal_end3=_----------------------------------------------------------------------------------------------------------------
settlement_deposit_withdrawal_end4=_出入金---Deposit/Withdrawal     银期转账---Bank-Futures Transfer    银期换汇---Bank-Futures FX Exchange
settlement_deposit_withdrawal_end5=_
settlement_trade_head1=_                                                              成交记录 Transaction Record 
settlement_trade_head2=_---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
settlement_trade_head3=_|成交日期| 交易所 |       品种       |      合约      |买/卖|   投/保    |  成交价  | 手数 |   成交额   |       开平       |  手续费  |  平仓盈亏  |     权利金收支      |  成交序号  |
settlement_trade_head4=_|  Date  |Exchange|     Product      |   Instrument   | B/S |    S/H     |   Price  | Lots |  Turnover  |       O/C        |   Fee    |Realized P/L|Premium Received/Paid|  Trans.No. |
settlement_trade_head5=_---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
settlement_trade_single_record1=_|%-8s|%-8s|%-18s|%-16s|%-5s|投          |%10.3f|%6d|%12.2f|%-18s|%10.2f|%12.2f|%21.2f|%-12s|
settlement_trade_end1=_---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
settlement_trade_end2=_|共%4d条|        |                  |                      |            |          |%6d|%12.2f|                  |%10.2f|%12.2f|%21.2f|            |
settlement_trade_end3=_---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
settlement_trade_end4=_能源中心---INE  上期所---SHFE   中金所---CFFEX  大商所---DCE   郑商所---CZCE   广期所---GFEX
settlement_trade_end5=_买---Buy   卖---Sell
settlement_trade_end6=_投---Speculation  保---Hedge  套---Arbitrage 般---General
settlement_trade_end7=_开---Open 平---Close 平今---Close Today 强平---Forced Liquidation 平昨---Close Prev. 强减---Forced Reduction 本地强平---Local Forced Liquidation 
settlement_trade_end8=_
settlement_position_closed_head1=_                                                         平仓明细 Position Closed 
settlement_position_closed_head2=_----------------------------------------------------------------------------------------------------------------------------------------------------------------------
settlement_position_closed_head3=_| 平仓日期 | 交易所 |       品种       |      合约      |开仓日期 |买/卖|   手数   |     开仓价    |     昨结算     |   成交价   |  平仓盈亏  |     权利金收支      |
settlement_position_closed_head4=_|Close Date|Exchange|      Product     |   Instrument   |Open Date| B/S |   Lots   |Pos. Open Price|   Prev. Sttl   |Trans. Price|Realized P/L|Premium Received/Paid|
settlement_position_closed_head5=_----------------------------------------------------------------------------------------------------------------------------------------------------------------------
settlement_position_closed_single_record1=_|%-10s|%-8s|%-18s|%-16s|%-9s|%-5s|%10d|%15.3f|%16.3f|%12.3f|%12.2f|%21.2f|
settlement_position_closed_end1=_----------------------------------------------------------------------------------------------------------------------------------------------------------------------
settlement_position_closed_end2=_|共%6d条|        |                  |                |         |     |%10d|               |                |            |%12.2f|%21.2f|
settlement_position_closed_end3=_----------------------------------------------------------------------------------------------------------------------------------------------------------------------
settlement_position_closed_end4=_能源中心---INE  上期所---SHFE   中金所---CFFEX  大商所---DCE   郑商所---CZCE   广期所---GFEX
settlement_position_closed_end5=_买---Buy   卖---Sell 
settlement_position_closed_end6=_
settlement_position_detail_head1=_                                              持仓明细 Positions Detail
settlement_position_detail_head2=_-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
settlement_position_detail_head3=_| 交易所 |       品种       |      合约      |开仓日期 |   投/保    |买/卖|持仓量 |    开仓价     |     昨结算     |     结算价     |  浮动盈亏  |  盯市盈亏 |  保证金   |       期权市值       |
settlement_position_detail_head4=_|Exchange|     Product      |   Instrument   |Open Date|    S/H     | B/S |Positon|Pos. Open Price|   Prev. Sttl   |Settlement Price| Accum. P/L |  MTM P/L  |  Margin   | Market Value(Options)|
settlement_position_detail_head5=_-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
settlement_position_detail_single_record1=_|%-8s|%-18s|%-16s|%9s|投          |%-5s|%7d|%15.3f|%16.3f|%16.3f|%12.2f|%11.2f|%11.2f|                  0.00|
settlement_position_detail_end1=_-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
settlement_position_detail_end2=_|共%4d条|                  |                |         |            |     |%7d|               |                |                |%12.2f|%11.2f|%11.2f|                  0.00|
settlement_position_detail_end3=_-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
settlement_position_detail_end4=_能源中心---INE  上期所---SHFE   中金所---CFFEX  大商所---DCE   郑商所---CZCE   广期所---GFEX
settlement_position_detail_end5=_买---Buy   卖---Sell  
settlement_position_detail_end6=_投---Speculation  保---Hedge  套---Arbitrage 般---General
settlement_position_detail_end7=_
settlement_position_head1=_                                                         持仓汇总 Positions
settlement_position_head2=_------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
settlement_position_head3=_|       品种       |      合约      |    买持     |    买均价   |     卖持     |    卖均价    |  昨结算  |  今结算  |持仓盯市盈亏|  保证金占用   |  投/保     |   多头期权市值   |   空头期权市值    |
settlement_position_head4=_|      Product     |   Instrument   |  Long Pos.  |Avg Buy Price|  Short Pos.  |Avg Sell Price|Prev. Sttl|Sttl Today|  MTM P/L   |Margin Occupied|    S/H     |Market Value(Long)|Market Value(Short)|
settlement_position_head5=_------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
settlement_position_single_record1=_|%-18s|%-16s|%13d|%13.3f|%14d|%14.3f|%10.3f|%10.3f|%12.2f|%15.2f|投          |              0.00|               0.00|
settlement_position_end1=_------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
settlement_position_end2=_|共%6d条        |                |%13d|             |%14d|              |          |          |%12.2f|%15.2f|            |              0.00|               0.00|
settlement_position_end3=_------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
settlement_position_end4=_
settlement_position_end5=_
settlement_position_end6=_
settlement_position_end7=_
settlement_position_end8=_)");


} // end namespace localCTP
