#include "BinomialTreeModel02.h"
#include "Option08.h"
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;
using namespace fre;
int main()
{
  int N = 8;
  double U = 1.15125, D = 0.86862, R = 1.00545;
  double S0 = 106.00, K = 100.00;
  BinomialTreeModel Model(S0, U, D, R);
  ofstream fout;
  fout.open("Results.txt");
  Call call(N, K);
  OptionCalculation callCalculation(&call);

  BinLattice<double> PriceTree;
  BinLattice<double> XTree;
  BinLattice<double> YTree;

  fout << "European call prices by PriceByCRR:"
       << fixed << setprecision(3) << callCalculation.PriceByCRR(Model) << endl
       << endl;
  fout << "European call prices by HW6 PriceByCRR:"
       << fixed << setprecision(3) << callCalculation.PriceByCRR(Model, PriceTree, XTree, YTree)
       << endl
       << endl;
  fout << "Stock positions in replicating strategy:" << endl
       << endl;
  XTree.Display(fout);
  fout << "Money market account positions in replicating strategy:" << endl
       << endl;
  YTree.Display(fout);

  // Put Option 部分
  Put put(N, K);
  OptionCalculation putCalculation(&put);

  BinLattice<double> PutPriceTree;
  BinLattice<double> PutXTree;
  BinLattice<double> PutYTree;

  fout << endl;

  fout << "European put prices by PriceByCRR: "
       << fixed << setprecision(3) << putCalculation.PriceByCRR(Model) << endl
       << endl;

  fout << "European put prices by HW6 PriceByCRR: "
       << fixed << setprecision(3) << putCalculation.PriceByCRR(Model, PutPriceTree, PutXTree, PutYTree) << endl
       << endl;

  fout << "Stock positions in replicating strategy:" << endl
       << endl;
  PutXTree.Display(fout);

  fout << "Money market account positions in replicating strategy:" << endl
       << endl;
  PutYTree.Display(fout);

  return 0;
}