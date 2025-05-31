// StockGroupSplitter.h
// by Chris Li
#ifndef STOCK_GROUP_SPLITTER_H
#define STOCK_GROUP_SPLITTER_H

#include <unordered_map>
#include <string>
#include "Stock.h"

class StockGroupSplitter {
public:
    using StockMap = std::unordered_map<std::string, Stock>;

    static void splitByGroup(const StockMap& allStocks,
                             StockMap& group1,
                             StockMap& group2,
                             StockMap& group3);
};

#endif // STOCK_GROUP_SPLITTER_H
