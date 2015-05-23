#ifndef TEST_COMPOSANT5_H
#define TEST_COMPOSANT5_H

#ifdef TEST_COMPOSANT5_EXPORTS
	#define TEST_COMPOSANT5_INTERFACE __declspec(dllexport)
#else
	#define TEST_COMPOSANT5_INTERFACE __declspec(dllimport)
#endif

#include <string>
#include <vector>
using std::string;
TEST_COMPOSANT5_INTERFACE void initMatrix(double **matriceVol);
TEST_COMPOSANT5_INTERFACE bool initialiseVolatiliteImplicite(double ** matriceVol);
TEST_COMPOSANT5_INTERFACE double ** getMatriceW();
TEST_COMPOSANT5_INTERFACE double ** getMatriceW();
TEST_COMPOSANT5_INTERFACE bool initialiseVolatiliteImplicite(double ** matriceVol);
TEST_COMPOSANT5_INTERFACE void remplirMatriceInput(double ** matrix,int cas);
TEST_COMPOSANT5_INTERFACE double ** getMatriceVolatiliteLocale();
TEST_COMPOSANT5_INTERFACE double interpolationLineaireVolatiliteLocale(double strike, double maturity);
TEST_COMPOSANT5_INTERFACE void afficheTAB(double ** matrix);

/*void remplirMatriceInput(double ** matrix,int cas);
void afficheTAB(double ** matrix);*/
//void initMatrix(double **matriceVol);

#endif


