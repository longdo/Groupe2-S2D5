#include <iostream>
using namespace std;

const int NB_MATURITY = 505;
const int NB_STRIKE = 502;
const int MATURITY_0D = 0;
const int MATURITY_2Y = 504;

const double STRIKE_OD = 0.3;
const double STRIKE_2Y = 3.00737;
const double FACTOR_STRIKE = 1.00463;
bool allocateTables = false;
bool findVolatilityData(string filename);
double findVolatility(double strike, double maturity);
void deleteMatrix();
void initMatrix();
void fillImpliciteVolMatrix();
void fillIntermediateValueMatrix();
double nominator(int i, int j);
double denominator1(int i, int j);
double denominator2(int i, int j);
double denominator3(int i, int j);
void fillLocalVolMatrix();
string getComponentVersion();