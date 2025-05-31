// StockGroupSplitter.cpp
// by Chris Li
#include "StockGroupSplitter.h"

void StockGroupSplitter::splitByGroup(const StockMap& allStocks,
                                      StockMap& group1,
                                      StockMap& group2,
                                      StockMap& group3)
{
    for (const auto& kv : allStocks) {
        const auto& ticker = kv.first;
        const auto& stock  = kv.second;
        if      (stock.getGroup() == "1") group1.emplace(ticker, stock);
        else if (stock.getGroup() == "2") group2.emplace(ticker, stock);
        else if (stock.getGroup() == "3") group3.emplace(ticker, stock);
    }
}
