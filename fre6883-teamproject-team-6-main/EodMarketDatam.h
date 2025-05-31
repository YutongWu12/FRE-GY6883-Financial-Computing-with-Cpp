// EodMarketDatam.h
// by Wenbo Gao, Chris Li, and Yutong Wu
#ifndef EODMARKETDATA_H
#define EODMARKETDATA_H

#include "Stock.h"
#include <unordered_map>
#include <string>
#include <vector>

class EODMarketData {
public:
    // Batch download price and earnings announcement information for all stocks (including IWM) from a CSV file, returning a ticker->Stock mapping
    static std::unordered_map<std::string, Stock>
    LoadAllStocks(const std::string &csvFile,
                  const std::string &apiToken);
    static const std::string& getFromDate();
    static const std::string& getToDate();

private:
    // Structure of each CSV row: ticker,date,period_ending,estimate,reported,surprise,surprise%
    struct CsvRow {
        std::string ticker;            // Stock ticker symbol
        std::string date;              // Announcement date YYYY-MM-DD
        std::string periodEndingDate;  // Period ending date YYYY-MM-DD
        double      estimate;          // Estimated EPS
        double      reported;          // Reported EPS
        double      surprise;          // Earnings surprise (reported – estimate)
        double      surprisePct;       // Surprise percentage; also reused for grouping (1/2/3)
    };

    static std::string globalFromDate;
    static std::string globalToDate;

    static std::vector<CsvRow> readCsv(const std::string &file);
    static void sortAndGroup(std::vector<CsvRow> &rows);
    static std::string addDays(const std::string &ymd, int days);

    struct Memory { char *mem; size_t size; };
    static size_t writeCallback(void *ptr,
                                size_t size,
                                size_t nmemb,
                                void *userdata);
    static std::string downloadCsv(const std::string &ticker,
                                   const std::string &fromDate,
                                   const std::string &toDate,
                                   const std::string &token);
    static std::vector<std::pair<std::string,double>>
    parsePriceCsv(const std::string &csvText);
};

#endif // EODMARKETDATA_H
