// by Jiayi Zhang
#include "Plot.h"
#include <cstdio>
#include <unordered_map>
#include <iostream>

using RowMatrix = std::vector<std::vector<double>>;

// -----------------------------------------------------------------------------
// General plotting:
//   data: key -> legend text, value -> y series; x is auto-mapped so midpoint corresponds to 0,
//   title: title of the plot
// -----------------------------------------------------------------------------
void plotData(const std::unordered_map<std::string,std::vector<double>>& data,
              const std::string &title)
{
    FILE* gp = popen("gnuplot -persist","w");
    if (!gp) {
        std::cerr<<"Error: cannot open pipe to gnuplot\n";
        return;
    }

    // Basic settings: title, labels, grid, and central axis (x=0)
    fprintf(gp,
        "set title '%s'\n"
        "set xlabel 'Announcement Date'\n"
        "set ylabel 'Value'\n"
        "set grid\n"
        "set arrow from 0,graph 0 to 0,graph 1 nohead lc rgb 'red' lw 2\n",
        title.c_str()
    );

    // Construct the plot command
    fprintf(gp,"plot ");
    bool first = true;
    for (auto& kv : data) {
        if (!first) fprintf(gp,", ");
        fprintf(gp,"'-' with lines title \"%s\"", kv.first.c_str());
        first = false;
    }
    fprintf(gp,"\n");

    // Compute offset (data length / 2)
    size_t offset = data.begin()->second.size() / 2;

    // Send data block for each series
    for (auto& kv : data) {
        const auto& vec = kv.second;
        for (size_t i = 0; i < vec.size(); ++i) {
            int x = static_cast<int>(i) - static_cast<int>(offset);
            fprintf(gp, "%d %g\n", x, vec[i]);
        }
        fprintf(gp,"e\n");
    }

    // After sending data, instruct gnuplot to wait for a key press before closing
    fprintf(gp, "pause -1 'Press any key in this gnuplot window to close'\n");
    fflush(gp);

    pclose(gp);  // Close pipe, which triggers gnuplot to exit and close the window
}

// -----------------------------------------------------------------------------

// Internal helper: extract row-th row from M and pass to plotData
static void plotRow(const std::unordered_map<std::string,RowMatrix>& M,
                    int row, const std::string &title)
{
    // First split the row into legend -> vector<double>
    std::unordered_map<std::string,std::vector<double>> data;
    for (auto& kv : M) {
        data[kv.first] = kv.second[row];
    }
    plotData(data, title);
}

void plotAvgAAR  (const std::unordered_map<std::string,RowMatrix>& M){
    plotRow(M, 2, "Avg AAR for 3 Groups"); 
}
void plotStdAAR  (const std::unordered_map<std::string,RowMatrix>& M){
    plotRow(M, 3, "Std AAR for 3 Groups");
}
void plotAvgCAAR (const std::unordered_map<std::string,RowMatrix>& M){
    plotRow(M, 0, "Avg CAAR for 3 Groups");
}
void plotStdCAAR (const std::unordered_map<std::string,RowMatrix>& M){
    plotRow(M, 1, "Std CAAR for 3 Groups");
}