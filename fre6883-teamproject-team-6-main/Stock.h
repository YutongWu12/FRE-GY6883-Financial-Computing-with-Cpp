// Stock.h
// by Yutong Wu
#ifndef STOCK_H
#define STOCK_H

#include <string>
#include <iomanip>
#include <vector>
#include "MarketCalendar.h"
#include <ostream>

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    for (size_t i = 0; i < v.size(); ++i) {
        os << std::fixed << std::setprecision(3) << std::setw(10) << v[i];
        if ((i - 0 + 1) % 10 == 0) os << "\n";
    }
    if ((v.size() - 0) % 10 != 0) os << "\n";
    return os;
}

class Stock {
private:
    std::string ticker;
    std::string group;
    std::string announcementDate;
    std::string periodEndingDate;
    double      estimatedEPS;
    double      reportedEPS;
    double      surprise;
    double      surprisePercent;
    std::vector<double> dailyPrice;
    std::vector<double> returns;
    std::vector<double> abReturn;
    std::vector<double> cummReturn;
    int datePosition;
    bool isBadPrice;

public:
    Stock() = default;
    explicit Stock(const std::string& ticker);

    void setTicker(const std::string& t);
    const std::string& getTicker() const;

    void setGroup(const std::string& g);
    const std::string& getGroup() const;

    void setAnnouncementDate(const std::string& d);
    const std::string& getAnnouncementDate() const;

    void setPeriodEndingDate(const std::string& d);
    const std::string& getPeriodEndingDate() const;

    void setEstimatedEPS(double e);
    double getEstimatedEPS() const;

    void setReportedEPS(double r);
    double getReportedEPS() const;

    void setSurprise(double s);
    double getSurprise() const;

    void setSurprisePercent(double sp);
    double getSurprisePercent() const;

    void setEPSInfo(const std::string& annDate,
                    double estEPS,
                    double repEPS,
                    double surprisePct);

    void setDailyPrice(const std::vector<double>& prices);
    const std::vector<double>& getDailyPrice() const;

    double getSize() const;
    bool getIsBadPrice() const;

    void calculateDatePosition(const std::string &startDate, MarketCalendar &calendar);

    void printReturns(int N) const;
    void printPrices(int N) const;
    void printCummReturns(int N) const;
    void printAbReturns(int N) const;
    void printDates(const MarketCalendar& calendar, int N) const;

    void computeReturns();

    void calculateCummReturn();

    void calculateAbReturn(const Stock &benchmark);
    std::vector<double> getAbReturn(int N) const;

    std::vector<std::string> getDateWindow(const MarketCalendar& calendar, int N) const;

    void badPrice(int N, int length);

    int getPosition() const {return datePosition;}
};

#endif // STOCK_H
