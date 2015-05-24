#include <iostream>
#ifndef COMPOSANT5_H
#define COMPOSANT5_H

#ifdef COMPOSANT5_EXPORTS
	#define COMPOSANT5_INTERFACE __declspec(dllexport)
#else
	#define COMPOSANT5_INTERFACE __declspec(dllimport)
#endif



using namespace std;


const int NB_MATURITY = 505;
const int NB_STRIKE = 502;
const int MATURITY_0D = 0;
const int MATURITY_2Y = 504;
const double dt = 1/252;

const double STRIKE_OD = 0.3;
const double STRIKE_2Y = 3.00737;
const double FACTOR_STRIKE = 1.00463;

const string ERROR1="C5 : Valeur trop petite";
const string ERROR2="C5 : Valeur trop grande";
const string ERROR3="C5 : Maturite erronee";
const string ERROR4="C5 : Matrice de volatilite implicite non calculee";

bool allocateTables = false;

COMPOSANT5_INTERFACE bool findVolatilityData(string filename);
COMPOSANT5_INTERFACE double findVolatility(double strike, double maturity);
COMPOSANT5_INTERFACE void deleteMatrix();
COMPOSANT5_INTERFACE void initMatrix();
COMPOSANT5_INTERFACE void fillImpliciteVolMatrix();
COMPOSANT5_INTERFACE void fillIntermediateValueMatrix();
COMPOSANT5_INTERFACE double nominator(int i, int j);
COMPOSANT5_INTERFACE double denominator1(int i, int j);
COMPOSANT5_INTERFACE double denominator2(int i, int j);
COMPOSANT5_INTERFACE double denominator3(int i, int j);
COMPOSANT5_INTERFACE void fillLocalVolMatrix();
COMPOSANT5_INTERFACE string getComponentVersion();

#endif