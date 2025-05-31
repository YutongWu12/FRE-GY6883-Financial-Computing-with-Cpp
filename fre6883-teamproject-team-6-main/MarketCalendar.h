// by Qian Gao
#ifndef MARKET_CALENDAR_H
#define MARKET_CALENDAR_H

#include <string>
#include <vector>

class MarketCalendar {
private:
    std::vector<std::string> calendar; // Sorted list of business dates in YYYY-MM-DD format

public:
    // Constructor loads calendar from file
    MarketCalendar(const std::string& filename);

    // Return number of business days between two dates (inclusive or exclusive if needed)
    int countBusinessDays(const std::string& from, const std::string& to) const;

    const std::vector<std::string>& getCalendar() const;
};

#endif // MARKET_CALENDAR_H
