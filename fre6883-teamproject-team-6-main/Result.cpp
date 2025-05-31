// by Qian Gao
#include "Result.h"
#include <unordered_map>
#include <iostream>

void Result::compute(const unordered_map<string, Stock>& beatGroup,
                     const unordered_map<string, Stock>& meetGroup,
                     const unordered_map<string, Stock>& missGroup,
                     CAARProcessor& processor)
{
    std::cout << "[INFO] Computing Beat group...\n";
    processor.computeAARandCAAR(beatGroup);
    const auto& a = processor.getAveCAAR();
    const auto& b = processor.getStdCAAR();
    const auto& c = processor.getAveAAR();
    const auto& d = processor.getStdAAR();

    if (a.empty() || b.empty() || c.empty() || d.empty()) {
        std::cerr << "[ERROR] Beat group computation failed. One or more vectors are empty.\n";
        return;
    }

    resultMap["Beat"] = { a, b, c, d };
    std::cout << "[INFO] Beat group stored.\n";

    std::cout << "[INFO] Computing Meet group...\n";
    processor.computeAARandCAAR(meetGroup);
    resultMap["Meet"] = {
        processor.getAveCAAR(),
        processor.getStdCAAR(),
        processor.getAveAAR(),
        processor.getStdAAR()
    };
    std::cout << "[INFO] Meet group stored.\n";

    std::cout << "[INFO] Computing Miss group...\n";
    processor.computeAARandCAAR(missGroup);
    resultMap["Miss"] = {
        processor.getAveCAAR(),
        processor.getStdCAAR(),
        processor.getAveAAR(),
        processor.getStdAAR()
    };
    std::cout << "[INFO] Miss group stored.\n";
}



const unordered_map<string, matrix>& Result::getResults() const {
    return resultMap;
}
