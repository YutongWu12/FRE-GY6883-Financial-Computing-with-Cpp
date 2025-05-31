// by Qian Gao
#include "MarketCalendar.h"
#include <fstream>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>

// Subtract one day from a date string in yyyy-mm-dd format and return the new date string
std::string subtractOneDay(const std::string& dateStr) {
    if (dateStr == "2025-01-01") return "2024-12-31";
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    tm.tm_mday -= 1;
    std::mktime(&tm);  // Normalize the date
    std::ostringstream out;
    out << std::put_time(&tm, "%Y-%m-%d");
    return out.str();
}

MarketCalendar::MarketCalendar(const std::string& filename) {
    std::ifstream fin(filename);
    std::string line;
    while (std::getline(fin, line)) {
        if (!line.empty()) calendar.push_back(line);
    }
}

int MarketCalendar::countBusinessDays(const std::string& from, const std::string& to) const {
    // Find the start date
    auto it1 = std::find(calendar.begin(), calendar.end(), from);
    if (it1 == calendar.end()) {
        std::cerr << from << " [Error] Start date not found in the calendar.\n";
        return -1;
    }

    // If 'to' is not in the calendar, keep subtracting one day until found
    std::string toDate = to;
    auto it2 = std::find(calendar.begin(), calendar.end(), toDate);
    while (it2 == calendar.end()) {
        toDate = subtractOneDay(toDate);
        it2 = std::find(calendar.begin(), calendar.end(), toDate);
        // (Optional) To avoid infinite loops, you can check if it goes before 'from'
        if (toDate < from) {
            std::cerr << to << " [Error] No valid business day found before after adjustment.\n";
            return -1;
        }
    }

    // Compute the absolute difference in business days
    return static_cast<int>(std::abs(std::distance(it1, it2)));
}

const std::vector<std::string>& MarketCalendar::getCalendar() const {
    return calendar;
}
