// Composant2.cpp : Defines the exported functions for the DLL application.
//

/*
// This is an example of an exported variable
COMPOSANT2_API int nComposant2=0;

// This is an example of an exported function.
COMPOSANT2_API int fnComposant2(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see Composant2.h for the class definition
CComposant2::CComposant2()
{
	return;
}
*/
// Composant2.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"
#include "Composant2.h"
#include "Composant6.h" 
#include "Composant7.h" 
//#include "Composant2Version.h"


using namespace std;
const double SPOT = 100;

	Composant2::Composant2(){
	};
	double Composant2::doMonteCarlo(string typePayOff, double maturity, double strike, int nbIterations){
		string s1= "NEGATIVE VALUE";
		string s2= "MISSED DATA";
		
		if (typePayOff.empty() || maturity == NULL || strike == NULL || nbIterations == NULL)
			throw s2;
		else if (maturity < 0 || strike < 0 || nbIterations < 0)
			throw s1;
		
		vector<double> vecteurPayOff(nbIterations);
		double sumPayOff = 0;
		//double* vectorFromC6_tmp;
		vector<double> vectorFromC6_tmp(maturity);
		double payOff_tmp;
		double esperancePayOff;

		//Recuperation des n PayOff; soit n le nbIterations.
		for (int i = 0; i < nbIterations; i++){
			vectorFromC6_tmp = getPath(maturity, SPOT);//Recuperation des 504 VA
			payOff_tmp = getPricePath(typePayOff, vectorFromC6_tmp, strike, maturity);//Alimentation du C7 par le tableau des 504 VA, puis recuperation du payOff associe en fonction du type d option traite (asiat, euro ect.)
			vecteurPayOff.push_back(payOff_tmp);//Ajout du payOff dans le vecteur de PayOffs
		}

		//Calcul de la somme des PayOff
		for (vector<double>::iterator j = vecteurPayOff.begin(); j != vecteurPayOff.end(); ++j){
			sumPayOff = sumPayOff + *j;
		}

		esperancePayOff = sumPayOff / nbIterations;

		//EXCEPTION si l esperance du payoff est manquante ou negative
		if (esperancePayOff == NULL )// || typePayOff.empty() || maturity == NULL || strike == NULL || nbIterations == NULL)
			throw s2;
		else if (esperancePayOff < 0 )//|| maturity < 0 || strike < 0 || nbIterations < 0)
			throw s1;

		//Calcul de l'esperance : Somme PayOff / Nb PayOff
		return esperancePayOff;
	};

	//double* Composant2::getPath(){
	vector<double> Composant2::getPath(double maturity, double spot){
		//Recupere un vector depuis Composant6::getChemin(int jours, double spot)
		string s1 = "MISSED DATA", s2 = "NEGATIVE VALUE";
		vector<double> path(maturity);
		path = getChemin(maturity, spot);

		if(path.size()<maturity)
			throw s1;
		// EXCEPTION si une des valeur du tableau est manquante ou negative.
		for (int z(0); z<path.size(); z++)
		{
			if (path[z] < 0)
				throw s2;
		}
		return path;
	};

	double Composant2::getPricePath(string typePayOff, vector<double> vecteur, double strike, double maturity){
		//Recupere un double depuis Composant7::pricePath(String typePayOff, double path[], double strike, double maturity)
		double priceOfPath = 0;
		//Conversion du vector en double*
		double* path = &vecteur[0];
		string error1="MISSED DATA", error2 = "NEGATIVE VALUE", error3 = "VALUE GREATER THAN 1000000";

		//priceOfPath = Composant7::pricePath(typePayOff, path, strike, maturity); - TODO
		priceOfPath = pricePath(typePayOff, path, strike, maturity);
		//EXCEPTION si la valeur retourne par C7 est manquante, negative ou > 1000000
		if (priceOfPath == NULL || strike == NULL || maturity == NULL)
			throw error1;
		else if (priceOfPath < 0 || strike < 0 || maturity < 0)
			throw error2;
		else if (priceOfPath > 1000000)// || strike > 1000000 || maturity < 1000000)
			throw error3;

		return priceOfPath;
	};
	/*
	char * Composant2::getComposant2Version()
	{
	return "Composant 2 version " COMPOSANT_VERSION_STR;
	};

	*/




