// by Qian Gao
#include "CAARProcessor.h"
#include "vec_math.h"
#include "Stock.h"
#include <numeric>
#include <iostream>
#include <unordered_map>
#include <string>
#include <random>

using namespace std;

typedef vector<vector<double>> matrix;
matrix CAARProcessor::bootstrap(const unordered_map<string, Stock>& stockGroup) {

    matrix sample;
    if (stockGroup.empty()) {
        std::cerr << "[ERROR] Cannot bootstrap — stockGroup is empty.\n";
        return sample;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, stockGroup.size() - 1);

    // Convert map to vector for safe random indexing
    std::vector<const Stock*> stockList;
    for (const auto& [sym, s] : stockGroup)
        stockList.push_back(&s);

    for (int i = 0; i < M; ++i) {
        size_t idx = dis(gen);
        const Stock* stock = stockList[idx];

        const auto& returns = stock->getAbReturn(N);

        sample.push_back(returns); // 🔍 suspected crash location
    }

    return sample;
}

void CAARProcessor::computeAARandCAAR(const unordered_map<string, Stock>& stockGroup) {

    if (stockGroup.empty()) {
        std::cerr << "[ERROR] Stock group is empty.\n";
        return;
    }

    matrix bootResult;
    AAR.clear();
    CAAR.clear();

    for (int n = 0; n < bstime; n++){
        bootResult = bootstrap(stockGroup);
        AAR.push_back(mean(bootResult));
        CAAR.push_back(cumm(AAR[n]));
    }

    avgAAR = mean(AAR);
    stdAAR = stddev(AAR);
    avgCAAR = mean(CAAR);
    stdCAAR = stddev(CAAR);
}
