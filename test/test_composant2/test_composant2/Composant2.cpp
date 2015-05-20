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
#include "stdafx.h"
#include <vector>
#include "Composant2.h"
#include "Composant2Version.h"
//#include <string>

using namespace std;

	Composant2::Composant2(){
	};
	double Composant2::doMonteCarlo(string typePayOff, double maturity, double strike, int nbIterations){
		vector<double> vecteurPayOff(nbIterations);
		double sumPayOff = 0;
		double* vectorFromC6_tmp;
		double payOff_tmp;
		double esperancePayOff;

		//Recuperation des n PayOff; soit n le nbIterations.
		for (int i = 0; i < nbIterations; i++){
			vectorFromC6_tmp = getPath();//Recuperation des 504 VA
			payOff_tmp = pricePath(typePayOff, vectorFromC6_tmp, strike, maturity);//Alimentation du C7 par le tableau des 504 VA, puis recuperation du payOff associe en fonction du type d option traite (asiat, euro ect.)
			vecteurPayOff.push_back(payOff_tmp);//Ajout du payOff dans le vecteur de PayOffs
		}

		//Calcul de la somme des PayOff
		for (vector<double>::iterator j = vecteurPayOff.begin(); j != vecteurPayOff.end(); ++j){
			sumPayOff = sumPayOff + *j;
		}

		esperancePayOff = sumPayOff / nbIterations;

		//EXCEPTION si l esperance du payoff est manquante ou negative
		if (esperancePayOff == NULL)
			throw "MISSED VALUE";
		else if (esperancePayOff < 0)
			throw "NEGATIVE VALUE";

		//Calcul de l'esperance : Somme PayOff / Nb PayOff
		return esperancePayOff;
	};

	double* Composant2::getPath(){
		//TODO : RÈcupÈrer un double* depuis le C6
		double* path=NULL;
		//path= fonction du C6 a appeler via DLL
		// EXCEPTION si une des valeur du tableau est manquante ou negative.
		for (int z(0); z<sizeof(path); z++)
		{
			if (path[z] == NULL)
				throw "MISSED DATA";
			else if (path[z] < 0)
				throw "NEGATIVE VALUE";
		}
		
		return path;
	};

	double Composant2::pricePath(string typePayOff, double* vecteur, double strike, double maturity){
		//TODO : RÈcupÈrer un double depuis le C7
		double priceOfPath = 0;
		//priceOfPath = fonction du C7 a appeler via DLL
		
		//EXCEPTION si la valeur retourne par C7 est manquante, negative ou > 1000000
		if (priceOfPath == NULL)
			throw "MISSED DATA";
		else if (priceOfPath < 0)
			throw "NEGATIVE VALUE";
		else if (priceOfPath > 1000000)
			throw "VALUE GREATER THAN 1000000";

		return priceOfPath;
	};

	char * Composant2::getComposant2Version()
	{
	return "Composant 2 version " COMPOSANT_VERSION_STR;
	};






