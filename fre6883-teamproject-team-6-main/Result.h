// by Qian Gao
#ifndef RESULT_H
#define RESULT_H

#include <string>
#include <unordered_map>
#include <vector>
#include "Stock.h"
#include "CAARProcessor.h"

using namespace std;
typedef vector<vector<double>> matrix;

class Result {
private:
    // key: group name ("Beat", "Meet", "Miss")
    // value: matrix of 4 rows: avgCAAR, stdCAAR, avgAAR, stdAAR
    unordered_map<string, matrix> resultMap;

public:
    // Compute results for all groups
    void compute(const unordered_map<string, Stock>& beatGroup,
                 const unordered_map<string, Stock>& meetGroup,
                 const unordered_map<string, Stock>& missGroup,
                 CAARProcessor& processor);

    // Accessor
    const unordered_map<string, matrix>& getResults() const;
};

#endif // RESULT_H
