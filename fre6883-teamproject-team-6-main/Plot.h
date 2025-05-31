// by Jiayi Zhang
#ifndef PLOT_H
#define PLOT_H

#include <unordered_map>
#include <string>
#include <vector>

// General plotting function: takes legend->y series mapping and a title for the plot
void plotData(const std::unordered_map<std::string,std::vector<double>>& data,
              const std::string &title);

// Four convenience interfaces: extract the corresponding row from resultMap and pass to plotData
void plotAvgAAR  (const std::unordered_map<std::string,std::vector<std::vector<double>>>& resultMap);
void plotStdAAR  (const std::unordered_map<std::string,std::vector<std::vector<double>>>& resultMap);
void plotAvgCAAR (const std::unordered_map<std::string,std::vector<std::vector<double>>>& resultMap);
void plotStdCAAR (const std::unordered_map<std::string,std::vector<std::vector<double>>>& resultMap);

#endif // PLOT_H
