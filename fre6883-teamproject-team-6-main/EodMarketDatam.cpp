// EodMarketDatam.cpp
// by Wenbo Gao, Chris Li, and Yutong Wu
#include "EodMarketDatam.h"
#include "ThreadPool.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <curl/curl.h>
#include <ctime>
#include <cstring>
#include <mutex>
#include <unordered_map>
#include <atomic>
#include <cstdlib>

using std::string;
using std::vector;
using std::unordered_map;

std::string EODMarketData::globalFromDate = "";
std::string EODMarketData::globalToDate = "";

// MM/DD/YYYY → YYYY-MM-DD
static string toIsoDate(const string &mdY) {
    int m,d,y;
    if (std::sscanf(mdY.c_str(), "%d/%d/%d", &m,&d,&y) != 3)
        return mdY;
    char buf[11];
    std::sprintf(buf, "%04d-%02d-%02d", y, m, d);
    return buf;
}

vector<EODMarketData::CsvRow>
EODMarketData::readCsv(const string &csvFile) {
    std::ifstream fin(csvFile);
    if (!fin) throw std::runtime_error("Cannot open " + csvFile);
    string line, tmp;
    std::getline(fin, line); // skip header

    vector<CsvRow> rows;
    while (std::getline(fin, line)) {
        std::stringstream ss(line);
        CsvRow r;
        std::getline(ss, r.ticker,     ',');
        std::getline(ss, tmp,          ','); r.date             = toIsoDate(tmp);
        std::getline(ss, tmp,          ','); r.periodEndingDate = toIsoDate(tmp);
        std::getline(ss, tmp,          ','); r.estimate         = std::stod(tmp);
        std::getline(ss, tmp,          ','); r.reported         = std::stod(tmp);
        std::getline(ss, tmp,          ','); r.surprise         = std::stod(tmp);
        std::getline(ss, tmp,          ','); r.surprisePct      = std::stod(tmp);
        rows.push_back(std::move(r));
    }
    return rows;
}

void EODMarketData::sortAndGroup(vector<CsvRow> &rows) {
    std::sort(rows.begin(), rows.end(),
              [](auto &a, auto &b){ return a.surprisePct > b.surprisePct; });
    size_t n = rows.size(), third = n/3;
    for (size_t i = 0; i < n; ++i)
        rows[i].surprisePct = (i < third ? 1.0 : (i < 2*third ? 2.0 : 3.0));
}

#if defined(_WIN32)
#define timegm _mkgmtime
#endif
static std::time_t str2time(const string &ymd) {
    int y,m,d;
    std::sscanf(ymd.c_str(), "%d-%d-%d", &y,&m,&d);
    std::tm t{}; t.tm_year=y-1900; t.tm_mon=m-1; t.tm_mday=d; t.tm_isdst=0;
    return timegm(&t);
}
static string time2str(std::time_t tt) {
    std::tm t{}; gmtime_r(&tt,&t);
    char buf[11]; std::strftime(buf,sizeof(buf),"%Y-%m-%d",&t);
    return buf;
}

string EODMarketData::addDays(const string &date, int days) {
    return time2str(str2time(date) + days*86400);
}

size_t EODMarketData::writeCallback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t real = size*nmemb;
    auto *m = static_cast<Memory*>(userdata);
    m->mem = static_cast<char*>(std::realloc(m->mem, m->size + real + 1));
    std::memcpy(m->mem + m->size, ptr, real);
    m->size += real;
    m->mem[m->size] = 0;
    return real;
}

string EODMarketData::downloadCsv(const string &ticker,
                                  const string &from,
                                  const string &to,
                                  const string &token) {
    string url = "https://eodhistoricaldata.com/api/eod/" 
               + ticker + ".US?from=" + from 
               + "&to=" + to 
               + "&period=d&fmt=csv&api_token=" + token;
    CURL *h = curl_easy_init();
    if (!h) throw std::runtime_error("curl init failed for " + ticker);
    Memory mem{nullptr,0};
    curl_easy_setopt(h, CURLOPT_URL,           url.c_str());
    curl_easy_setopt(h, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(h, CURLOPT_WRITEDATA,     &mem);
    curl_easy_setopt(h, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(h, CURLOPT_SSL_VERIFYHOST, 0L);
    if (curl_easy_perform(h) != CURLE_OK) {
        std::free(mem.mem);
        throw std::runtime_error("curl error for " + ticker);
    }
    long code = 0;
    curl_easy_getinfo(h, CURLINFO_RESPONSE_CODE, &code);
    curl_easy_cleanup(h);
    if (code != 200) {
        std::free(mem.mem);
        throw std::runtime_error("HTTP " + std::to_string(code) 
                                 + " for " + ticker);
    }
    string result(mem.mem, mem.size);
    std::free(mem.mem);
    return result;
}

vector<std::pair<string,double>>
EODMarketData::parsePriceCsv(const string &csvText) {
    vector<std::pair<string,double>> v;
    std::stringstream ss(csvText);
    string line;
    std::getline(ss, line); // header
    while (std::getline(ss, line)) {
        if (line.size()<12 || line[4]!='-') continue;
        std::stringstream ls(line);
        string date, tmp;
        std::getline(ls, date, ',');
        for (int i = 0; i < 4; ++i) std::getline(ls, tmp, ',');
        std::getline(ls, tmp, ',');
        v.emplace_back(date, std::stod(tmp));
    }
    std::sort(v.begin(), v.end(),
              [](auto &a, auto &b){ return a.first < b.first; });
    return v;
}

unordered_map<string,Stock>
EODMarketData::LoadAllStocks(const string &csvFile,
                             const string &tok) {
    curl_global_init(CURL_GLOBAL_ALL);
    auto rows = readCsv(csvFile);
    sortAndGroup(rows);

    // 计算窗口
    string earliest = rows.front().date, latest = rows.front().date;
    for (auto &r: rows) {
        earliest = std::min(earliest, r.date);
        latest   = std::max(latest,   r.date);
    }
    string from = addDays(earliest,-90), to = addDays(latest,90);

    EODMarketData::globalFromDate = from;
    EODMarketData::globalToDate   = to;

    unordered_map<string,Stock> mp;
    std::mutex mpMu;
    std::atomic<int> counter{0};
    int total = int(rows.size())+1;
    const int barWidth=40;
    std::cout<<"Downloading "<<total<<" tickers:\n";

    ThreadPool pool(12);
    for (auto &r: rows) {
        pool.enqueue([&,r](){
            vector<double> prices;
            while (prices.empty()) {
                try {
                    auto txt  = downloadCsv(r.ticker,from,to,tok);
                    auto full = parsePriceCsv(txt);
                    prices.clear();
                    for (auto &p:full) prices.push_back(p.second);
                } catch(...){}
            }
            Stock s(r.ticker);
            // 填充分组标签
            s.setGroup(std::to_string(int(r.surprisePct)));
            s.setEPSInfo(r.date, r.estimate, r.reported, r.surprisePct);
            s.setPeriodEndingDate(r.periodEndingDate);
            s.setDailyPrice(prices);
            s.computeReturns();
            {
                std::lock_guard<std::mutex> lk(mpMu);
                mp.emplace(r.ticker, std::move(s));
            }
            int done=++counter; float frac=float(done)/total; int pos=int(frac*barWidth);
            std::cout<<"\r["; for(int i=0;i<barWidth;++i) std::cout<<(i<pos?'=':(i==pos?'>':' '));
            std::cout<<"] "<<done<<" / "<<total<<std::flush;
        });
    }
    // Download IWM
    pool.enqueue([&](){
        const string tic="IWM";
        vector<double> prices;
        while(prices.empty()){
            try{
                auto txt  = downloadCsv(tic,from,to,tok);
                auto full = parsePriceCsv(txt);
                prices.clear(); for(auto &p:full) prices.push_back(p.second);
            }catch(...){}}
        Stock s(tic); s.setDailyPrice(prices); s.computeReturns();
        {
            std::lock_guard<std::mutex> lk(mpMu);
            mp.emplace(tic,std::move(s));
        }
        int done=++counter; float frac=float(done)/total; int pos=int(frac*barWidth);
        std::cout<<"\r["; for(int i=0;i<barWidth;++i) std::cout<<(i<pos?'=':(i==pos?'>':' '));
        std::cout<<"] "<<done<<" / "<<total<<std::flush;
    });

    std::cout<<"\nDownload complete.\n";
    curl_global_cleanup();
    return mp;
}

const std::string& EODMarketData::getFromDate() {
        return globalFromDate;
}
    
const std::string& EODMarketData::getToDate() {
    return globalToDate;
}