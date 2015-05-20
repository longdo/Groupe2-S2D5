#include <iostream>

#include <string>
using std::string;

class Composant2{
public:
	Composant2();//Constructeur
	double doMonteCarlo(string typePayOff, double maturity, double strike, int nbIterations);
	double* getPath();
	double pricePath(string typePayOff, double* vecteur, double strike, double maturity);

private:
	


};