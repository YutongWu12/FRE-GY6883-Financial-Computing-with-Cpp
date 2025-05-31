// Stock.cpp
// by Yutong Wu
#include "Stock.h"
#include "MarketCalendar.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "vec_math.h"

using namespace std;

void printMiddleSection(const vector<double>& v, int start, int end, int numCols = 10) {

    for (int i = start; i < end; ++i) {
        cout << fixed << setprecision(3) << setw(10) << v[i];
        if ((i - start + 1) % numCols == 0) cout << "\n";
    }
    if ((end - start) % numCols != 0) cout << "\n";
}

void printDateList(const vector<string>& v, int numCols = 6) {

    for (int i = 0; i < v.size(); ++i) {
        cout << setw(10) << v[i];
        if ((i + 1) % numCols == 0) cout << "\n";
    }
    if (v.size() % numCols != 0) cout << "\n";
}

Stock::Stock(const std::string& t)
    : ticker(t),
      estimatedEPS(0.0),
      reportedEPS(0.0),
      surprise(0.0),
      surprisePercent(0.0)
{}

void Stock::setTicker(const std::string& t) { ticker = t; }
const std::string& Stock::getTicker() const { return ticker; }

void Stock::setGroup(const std::string& g) { group = g; }
const std::string& Stock::getGroup() const { return group; }

void Stock::setAnnouncementDate(const std::string& d) { announcementDate = d; }
const std::string& Stock::getAnnouncementDate() const { return announcementDate; }

void Stock::setPeriodEndingDate(const std::string& d) { periodEndingDate = d; }
const std::string& Stock::getPeriodEndingDate() const { return periodEndingDate; }

void Stock::setEstimatedEPS(double e) { estimatedEPS = e; }
double Stock::getEstimatedEPS() const { return estimatedEPS; }

void Stock::setReportedEPS(double r) { reportedEPS = r; }
double Stock::getReportedEPS() const { return reportedEPS; }

void Stock::setSurprise(double s) { surprise = s; }
double Stock::getSurprise() const { return surprise; }

void Stock::setSurprisePercent(double sp) { surprisePercent = sp; }
double Stock::getSurprisePercent() const { return surprisePercent; }

void Stock::setEPSInfo(const std::string& annDate,
                       double estEPS,
                       double repEPS,
                       double surprisePct) {
    announcementDate = annDate;
    estimatedEPS     = estEPS;
    reportedEPS      = repEPS;
    surprise         = repEPS - estEPS;
    surprisePercent  = surprisePct;
}

void Stock::setDailyPrice(const std::vector<double>& prices) {
    dailyPrice = prices;
}
const std::vector<double>& Stock::getDailyPrice() const {
    return dailyPrice;
}

double Stock::getSize() const{
    return returns.size();
}

bool Stock::getIsBadPrice() const {
    return isBadPrice;
}

void Stock::calculateDatePosition(const std::string &startDate, MarketCalendar &calendar)
{   
    datePosition = calendar.countBusinessDays(startDate, announcementDate);
    if (datePosition == -1) cout << ticker << endl;
}

void Stock::printReturns(int N) const {
    printMiddleSection(returns, datePosition-N, datePosition+N);
}

void Stock::printPrices(int N) const {
    printMiddleSection(dailyPrice, datePosition-N, datePosition+N+1);
}

void Stock::printCummReturns(int N) const {
    printMiddleSection(cummReturn, datePosition-N, datePosition+N);
}

void Stock::printAbReturns(int N) const {
    printMiddleSection(abReturn, datePosition-N, datePosition+N);
}

void Stock::printDates(const MarketCalendar& calendar, int N) const {
    printDateList(this->getDateWindow(calendar, N));
}

void Stock::calculateAbReturn(const Stock &benchmark) {
    if (returns.size() != benchmark.returns.size()) {
        isBadPrice = true;
        return;
    }
    abReturn.clear();
    for (size_t i = 0; i < returns.size(); ++i) {
        abReturn.push_back(returns[i] - benchmark.returns[i]);
    }
}

// Safely retrieve a window of abnormal returns around datePosition
std::vector<double> Stock::getAbReturn(int N) const {
    std::vector<double> sample;
    // Ensure abReturn has data
    if (abReturn.empty()) {
        std::cerr << "[Error] abReturn is empty for ticker " << ticker << std::endl;
        return sample;
    }
    // Compute safe window boundaries
    int startIdx = std::max(0, datePosition - N);
    int endIdx = std::min(static_cast<int>(abReturn.size()), datePosition + N);
    // Collect values
    for (int i = startIdx; i < endIdx; ++i) {
        sample.push_back(abReturn[i]);
    }
    // Warn if window was adjusted
    if (startIdx != datePosition - N || endIdx != datePosition + N) {
        std::cerr << "[Warning] Requested abReturn window [" << datePosition - N 
                  << "," << datePosition + N << ") adjusted to [" 
                  << startIdx << "," << endIdx << ") due to data bounds." << std::endl;
    }
    return sample;
}


void Stock::computeReturns() {
    returns.clear();
    if (dailyPrice.size() < 2) return;
    for (size_t i = 1; i < dailyPrice.size(); ++i) {
        double p0 = dailyPrice[i - 1];
        double p1 = dailyPrice[i];
        if (p0 > 0.0)
            returns.push_back(std::log(p1 / p0));
        else
            returns.push_back(0.0);
    }
}

void Stock::calculateCummReturn(){
    cummReturn = cumm(returns);
}

std::vector<std::string> Stock::getDateWindow(const MarketCalendar& calendar, int N) const
{
    const std::vector<std::string>& dates = calendar.getCalendar();
    auto it = std::find(dates.begin(), dates.end(), announcementDate);
    if (it == dates.end()) {
        throw std::runtime_error("Announcement date not found in calendar.");
    }

    int idx = std::distance(dates.begin(), it);
    int start = std::max(0, idx - N);
    int end   = std::min((int)dates.size() - 1, idx + N);

    return std::vector<std::string>(dates.begin() + start, dates.begin() + end + 1);
}

void Stock::badPrice(int N, int length){ 
    if (dailyPrice.size()<length || dailyPrice.size()-1-datePosition < N || datePosition < N) isBadPrice = true;
    else isBadPrice = false;
}
