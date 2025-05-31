// by Hao Weng
#include <iostream>
#include <limits>
#include <algorithm>
#include <iomanip>
#include "EodMarketDatam.h"
#include "StockGroupSplitter.h"
#include "CAARProcessor.h"
#include "Result.h"
#include "Plot.h"

using namespace std;
using StockMap = unordered_map<string, Stock>;

int main() {
    const string earningsFile = "Russell3000EarningsAnnouncements.csv";
    const string calendarFile = "trading_days.csv";
    const string apiToken = "680d245d00f434.03142319";

    StockMap allStocks, group1, group2, group3, badPrice;
    Stock benchmark("IWM");
    MarketCalendar calendar(calendarFile);
    Result result;

    int N = 0, M = 30, B = 40;
    bool dataLoaded = false;
    bool statsComputed = false;

    while (true) {
        cout << "\n1 - Enter N to retrieve 2N+1 days of historical price data for all stocks.\n";
        cout << "2 - Pull information for one stock from one group.\n";
        cout << "3 - Show AAR, AAR-STD, CAAR and CAAR-STD for one group.\n";
        cout << "4 - Plot Avg CAAR for 3 groups.\n";
        cout << "5 - Plot Std CAAR for 3 groups.\n";
        cout << "6 - Plot Avg AAR for 3 groups.\n";
        cout << "7 - Plot Std AAR for 3 groups.\n";
        cout << "8 - Identify stocks with bad prices.\n";
        cout << "9 - Exit.\n";
        cout << "Enter your choice and press return: ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[Error] Invalid input.\n";
            continue;
        }
        string startDate;

        if (choice == 1) {
            cout << "Enter N (30 <= N <= 60): ";
            cin >> N;
            if (N < 30 || N > 60) {
                cout << "[Error] N must be between 30 and 60.\n";
                continue;
            }

            cout << "Loading stock data, please wait..." << endl;
            allStocks = EODMarketData::LoadAllStocks(earningsFile, apiToken);

            startDate = EODMarketData::getFromDate();

            if (allStocks.count("IWM") == 0) {
                cout << "[Error] Benchmark IWM not found.\n";
                continue;
            }

            benchmark = allStocks["IWM"];

            for (auto& [symbol, stock] : allStocks){
                stock.computeReturns();
                if (stock.getTicker() != "IWM"){
                    stock.calculateDatePosition(startDate, calendar);
                    stock.calculateCummReturn();
                }
            }
            
            int length = benchmark.getSize();

            for (auto& [symbol, stock] : allStocks) {
                if (symbol != "IWM") {
                    stock.badPrice(N, length);
                    if (stock.getIsBadPrice() == false) stock.calculateAbReturn(benchmark);
                }
            }
            StockGroupSplitter::splitByGroup(allStocks, group1, group2, group3);
            for (auto& [symbol, stock] : allStocks) {
                if (symbol != "IWM") {
                    if (stock.getIsBadPrice()){
                        badPrice[stock.getTicker()] = stock;
                        if (stock.getGroup() == "1"){
                            group1.erase(stock.getTicker());
                        }
                        else if (stock.getGroup() == "2"){
                            group2.erase(stock.getTicker());
                        }
                        else group3.erase(stock.getTicker());
                        stock.setGroup("badPrice");
                    }
                }
            }
            dataLoaded = true;
            statsComputed = false;
            cout << "[Info] Stock data loaded successfully.\n";

        } else if (choice == 2) {
            if (!dataLoaded) {
                cout << "[Error] Load data first (option 1).\n";
                continue;
            }
        
            string ticker;
            cout << "Enter Symbol for Detailed Information: ";
            cin >> ticker;
            transform(ticker.begin(), ticker.end(), ticker.begin(), ::toupper);
        
            auto it = allStocks.find(ticker);
            if (it == allStocks.end()) {
                cout << "[Error] Stock not found.\n";
                for (const auto& pair : allStocks) {
                    std::cout << pair.first << std::endl;
                }
                continue;
            }
        
            Stock& stock = it->second;
            // Print metadata
            cout << "Ticker: " << ticker << "\n";
            cout << "Earning Announcement Date: " << stock.getAnnouncementDate() << "\n";
            cout << "Earning Period Ending: " << stock.getPeriodEndingDate() << "\n";
            cout << "Earning Estimate: " << stock.getEstimatedEPS() << "\n";
            cout << "Reported Earning Per Share: " << stock.getReportedEPS() << "\n";
            cout << "Earning Surprise $" << stock.getSurprise() << "\n";
            cout << "Earning Surprise Percentage: " << stock.getSurprisePercent() << "\n";
            if (stock.getGroup() == "1"){
                cout << "Group: Beat" << "\n";
            } else if (stock.getGroup() == "2") {
                cout << "Group: Meet" << "\n";
            } else if (stock.getGroup() == "3") {
                cout << "Group: Miss" << "\n";
            }
            cout << "No Enough Daily Price Date: " << boolalpha << stock.getIsBadPrice() << "\n";
            
        
            // Print Dates
            cout << "Stock Daily Price Dates:\n";
            stock.printDates(calendar, N);
        
            // Print Prices
            cout << "\nStock Daily Price:\n";
            stock.printPrices(N);

            // Print Returns
            cout << "\nStock Daily Returns:\n";
            stock.printReturns(N);
        
            // Print Cumulative Returns
            cout << "\nStock Cumulative Daily Returns:\n";
            stock.printCummReturns(N);
        
            // Print Abnormal Returns
            cout << "\nStock Abnormal Daily Returns:\n";
            if (stock.getIsBadPrice()) cout << "The stock is bad price.";
            else stock.printAbReturns(N);
        
            cout << "\n";
        } else if (choice == 3) {
            if (!dataLoaded) {
                cout << "[Error] Load data first (option 1).\n";
                continue;
            }
        
            // Prompt for group number
            int g;
            cout << "Enter group number (1=Beat, 2=Meet, 3=Miss): ";
            cin >> g;
        
            string label;
            if      (g == 1) label = "Beat";
            else if (g == 2) label = "Meet";
            else if (g == 3) label = "Miss";
            else {
                cout << "[Error] Invalid group.\n";
                continue;
            }
        
            // Only compute once
            if (!statsComputed) {
                if (group1.empty() || group2.empty() || group3.empty()) {
                    cout << "[Error] One or more stock groups are empty.\n";
                    cout << group1.size() << " " << group2.size() << " " << group3.size() << " " << badPrice.size() << endl;
                    continue;
                }
        
                // Use a valid startDate from any stock in the group
                CAARProcessor processor(N, M, B, calendar, startDate);
                result.compute(group1, group2, group3, processor);
                statsComputed = true;
            }
        
            const auto& resultMap = result.getResults();
            if (resultMap.count(label) == 0) {
                cout << "[Error] No statistics available for group: " << label << endl;
                continue;
            }
        
            const auto& mat = resultMap.at(label);
            if (mat.size() < 4) {
                cout << "[ERROR] Result matrix for group '" << label << "' has less than 4 rows.\n";
                continue;
            }
            
            if (mat[0].empty() || mat[1].empty() || mat[2].empty() || mat[3].empty()) {
                cout << "[ERROR] One or more result vectors are empty for group '" << label << "'.\n";
                continue;
            }
            
            cout << "Avg CAAR: " << endl << mat[0] << endl;
            cout << "Std CAAR: " << endl << mat[1] << endl;
            cout << "Avg AAR:  " << endl << mat[2] << endl;
            cout << "Std AAR:  " << endl << mat[3] << endl;
        

        } else if (choice == 4 && dataLoaded) {
            if (!statsComputed) {
                cout << "Haven't compute the statistics yet, run option 3 first. \n";
                continue;
            }
            plotAvgCAAR(result.getResults());
        } else if (choice == 5 && dataLoaded) {
            if (!statsComputed) {
                cout << "Haven't compute the statistics yet, run option 3 first. \n";
                continue;
            }
            plotStdCAAR(result.getResults());
        } else if (choice == 6 && dataLoaded) {
            if (!statsComputed) {
                cout << "Haven't compute the statistics yet, run option 3 first. \n";
                continue;
            }
            plotAvgAAR(result.getResults());
        } else if (choice == 7 && dataLoaded) {
            if (!statsComputed) {
                cout << "Haven't compute the statistics yet, run option 3 first. \n";
                continue;
            }
            plotStdAAR(result.getResults());
        } else if (choice == 8 && dataLoaded) {
            cout << "[Info] Stocks with insufficient price data:\n";
            for (const auto& [ticker, stock] : allStocks) {
                if (stock.getIsBadPrice())
                    cout << "- " << ticker << endl;
            }
        } else if (choice == 9) {
            break;
        } else {
            cout << "[Error] Invalid Input. Load data first (option 1).\n";
        }
    }

    return 0;
}
