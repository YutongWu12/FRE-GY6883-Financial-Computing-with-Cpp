// by Qian Gao
#ifndef CAARPROCESSOR_H
#define CAARPROCESSOR_H

#include "Stock.h"
#include "MarketCalendar.h"
#include <unordered_map>
#include <string>
#include <vector>
using namespace std;
typedef vector<vector<double>> matrix;

class CAARProcessor {
private:
    matrix AAR;
    matrix CAAR;
    int N, M, bstime;
    MarketCalendar Calendar;
    string startDate;
    vector<double> avgAAR, stdAAR, avgCAAR, stdCAAR;

public:
    CAARProcessor(int N_, int M_, int bstime_, MarketCalendar Calendar_, string startDate_): N(N_), M(M_), bstime(bstime_), Calendar(Calendar_), startDate(startDate_){}

    matrix bootstrap(const unordered_map<string, Stock>& stockGroup);

    // Compute AAR and CAAR from a group of stocks
    void computeAARandCAAR(const unordered_map<string, Stock>& stockGroup);

    // Accessors
    const vector<double>& getAveAAR() const {return avgAAR; }
    const vector<double>& getStdAAR() const {return stdAAR; }
    const vector<double>& getAveCAAR() const {return avgCAAR; }
    const vector<double>& getStdCAAR() const {return stdCAAR; }
};

#endif // CAARPROCESSOR_H
