#include <iostream>
#include <iomanip>
#include "BSModel.h"
#include "BinomialTreeModel02.h"
#include "Option08.h"
#include "BinLattice02.h"

using namespace std;
using namespace fre;

int main()
{
     double S0 = 106.0;
     double r = 0.058;
     double sigma = 0.46;
     double T = 0.75;
     double K = 100.0;
     int N = 8;
     double h = T / N;

     cout << fixed << setprecision(3);

     // Step 1: 构造模型
     BSModel bsModel(S0, r, h, sigma);
     BinomialTreeModel model = bsModel.generateModel();

     cout << "S0 = " << S0 << endl;
     cout << "r  = " << r << endl;
     cout << "sigma = " << sigma << endl;
     cout << "T = " << T << endl;
     cout << "K = " << K << endl;
     cout << "N = " << N << endl
          << endl;

     cout << "U = " << model.GetU() << endl;
     cout << "D = " << model.GetD() << endl;
     cout << "R = " << model.GetR() << endl
          << endl;

     // Step 2: 构造 American Call Option
     Call theCall(N, K);
     OptionCalculation optCalc(&theCall);

     // Step 3: 构建 Lattice 容器
     BinLattice<double> priceTree;
     BinLattice<bool> stoppingTree;

     // Step 4: 调用 Snell 方法计算价格
     double price = optCalc.PriceBySnell(model, priceTree, stoppingTree);
     cout << "American call option price = " << price << endl;

     return 0;
}

/*


    S0 = 106.000 r = 0.058 sigma = 0.460 T = 0.750 K = 100.000 N = 8

    U = 1.151 D = 0.869 R = 1.005

    American call option price = 21.682
                                    */
