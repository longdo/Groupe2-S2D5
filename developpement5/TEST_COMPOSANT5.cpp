// testLong.cpp†: dÈfinit le point d'entrÈe pour l'application console.
//

//#include "stdafx.h"
//#include <sstream>
//
//  test.cpp
//  ComponentFinalV1_0
//
//  Created by Long DO on 4/15/15.
//  Copyright (c) 2015 Long DO. All rights reserved.
//

//TO COPY TO MAIN.CPP FOR TESTS
#include "math.h"
#include <iostream>
using namespace std;

const int NB_MATURITY = 12;
const int NB_STRIKE = 12;

const double STRIKE_OD = 1;
const double FACTOR_STRIKE = 0.1;

//*************************************
//*               MATRIX              *
//*************************************
double **intermediateValueMatrix;
double **localVolMatrix;

void remplirMatriceInput(double ** matrix,int cas);
void afficheTAB(double ** matrix);

//*************************************
//*          OTHER FUNCTIONS          *
//*************************************
void initMatrix(double **matriceVol){
    //matrix creation
    intermediateValueMatrix = new double * [NB_STRIKE];
    localVolMatrix = new double * [NB_STRIKE];
    
    for(int i=0;i<12;i++){
        intermediateValueMatrix[i] = new double[12];
        localVolMatrix[i] = new double[12];
    }
    
    //matrix duplication
    for (int i = 0; i < NB_STRIKE; i++)
        for (int j = 0; j < NB_MATURITY; j++){
            intermediateValueMatrix[i][j] = matriceVol[i][j];
            localVolMatrix[i][j] = matriceVol[i][j];
        }
}

void fillIntermediateValueMatrix(double **matriceVol){
    for (int i = 1; i < NB_STRIKE; i++)
        for (int j = 1; j < NB_MATURITY; j++)
            intermediateValueMatrix[i][j] = intermediateValueMatrix[0][j] * pow(matriceVol[i][j], 2.0);
}

double nominator(int i, int j){
    double n = 0.0, d = 0.0;
    
    n = intermediateValueMatrix[i][j] - intermediateValueMatrix[i][j-1];
    d = intermediateValueMatrix[0][j] - intermediateValueMatrix[0][j-1];
    
    return n/d;
}

double denominator1(int i, int j){
    double n1 = 0.0, n2 = 0.0, d1 = 0.0, d2 = 0.0;
    
    n1 = log10(intermediateValueMatrix[i][0]);
    d1 = intermediateValueMatrix[i][j];
    
    n2 = intermediateValueMatrix[i][j] - intermediateValueMatrix[i-1][j];
    d2 = log10(intermediateValueMatrix[i][0]) - log10(intermediateValueMatrix[i-1][0]);
    
    return 1 - (n1/d1) * (n2/d2);
}

double denominator2(int i, int j){
    double n2 = 0.0, n3 =0.0, d1 = 0.0, d2 = 0.0, d3 = 0.0;
    
    d1 = intermediateValueMatrix[i][j];
    
    n2 = log10(intermediateValueMatrix[i][0]);
    d2 = intermediateValueMatrix[i][j];
    
    n3 = intermediateValueMatrix[i][j] - intermediateValueMatrix[i-1][j];
    d3 = log10(intermediateValueMatrix[i][0]) - log10(intermediateValueMatrix[i-1][0]);
    
    return 0.25 * (-0.25 - (1.0/d1) + pow(n2, 2.0)/pow(d2, 2.0)) * pow(n3/d3, 2.0);
}

double denominator3(int i, int j){
    double n = 0.0, d = 0.0;
    
    n = intermediateValueMatrix[i+1][j] - 2 * intermediateValueMatrix[i][j] + intermediateValueMatrix[i-1][j];
    d = log10(intermediateValueMatrix[i+1][0]) - log10(intermediateValueMatrix[i-1][0]);
    
    return 0.5 * n/d;
}

void fillLocalVolMatrix(double **matriceVol){
    //localVol(0) = impliciteVol(0)
    for (int i = 1; i < NB_STRIKE; i++)
        localVolMatrix[i][1] = matriceVol[i][1];
    
    //localVol(1) to localVol(M)
    for (int i = 1; i < NB_STRIKE-1; i++)
        for (int j = 2; j < NB_MATURITY; j++){
            localVolMatrix[i][j] = sqrt(nominator(i, j) / (denominator1(i, j) + denominator2(i, j) + denominator3(i, j)));
        }
    
    for (int i = 1; i < NB_STRIKE; i++)
        localVolMatrix[i][11] = matriceVol[i][11];
}

/*
void correctLocalVolMatrix(double **matriceVol){
    for (int i = 0; i < NB_STRIKE; i++)
        for (int j = 0; j < NB_MATURITY; j++){
            stringstream out;
            string s;
            out<< localVolMatrix[i][j];
            s = out.str();
            if(s == "-1.#IND")
                localVolMatrix[i][j] = matriceVol[i][j];
        }
}
 */

void getIndexFromValue(double strike, double maturity, int &sindex, int &mindex){
    int s = 0, m = 0;
    
    //strike index
    while (localVolMatrix[s][0] != strike && s <= NB_STRIKE)
        s++;
    
    
    if (localVolMatrix[s][0] == strike)
        sindex = s;
    
    //maturity index
    while (localVolMatrix[0][m] != maturity && m <= NB_MATURITY)
        m++;
    
    if (localVolMatrix[0][m] == maturity)
        mindex = m;
}

bool toDoInterpolation(double strike){
    int s = 0;
    bool todo = false;
    
    while (localVolMatrix[s][0] != strike && s <= NB_STRIKE)
        s++;
    
    if (s > NB_STRIKE)
        todo = true;
    
    return todo;
}

double interpolationLineaireVolatiliteLocale(double strike, double maturity){
    int indexSD = 0, indexSU = 0, indexM = 0;
    double strikeD = 0.0, strikeU = 0.0;
    
    indexSD = ((strike - STRIKE_OD) / FACTOR_STRIKE) + 1;
    indexSU = indexSD + 1;
    
    strikeD = localVolMatrix[indexSD][0];
    strikeU = localVolMatrix[indexSU][0];
    
    getIndexFromValue(strikeU, maturity, indexSU, indexM);
    
    return localVolMatrix[indexSU][indexM] - (((strikeU - strike) * (localVolMatrix[indexSU][indexM] - localVolMatrix[indexSD][indexM])) / (strikeU - strikeD));
}

//*************************************
//*          TESTS' FUNCTIONS         *
//*************************************
bool initialiseVolatiliteImplicite(double ** matriceVol){
    initMatrix(matriceVol);
    
    //matrix filling
    fillIntermediateValueMatrix(matriceVol);
    fillLocalVolMatrix(matriceVol);
    
    //matrix correction
    //correctLocalVolMatrix(matriceVol);
    
    return true;
}

double ** getMatriceW(){
    return intermediateValueMatrix;
}

double ** getMatriceVolatiliteLocale(){
    return localVolMatrix;
}

int main(int argc, const char * argv[]) {
    double ** matriceInput1 = new double *[12];
    double ** matriceOutputW = new double *[12];
    double ** matriceOutputVol = new double *[12];
    for(int i=0;i<12;i++){
        matriceInput1[i] = new double[12];
        matriceOutputW[i] = new double[12];
        matriceOutputVol[i] = new double[12];
    }
    
    //Cas test 1
    remplirMatriceInput(matriceInput1, 1);
    cout << "Matrice de départ : " << endl;
    afficheTAB(matriceInput1);
    cout << "\n" << endl;
    
    cout << "Initiatlisation de la matrice et affichage de la matrice W : " <<endl;
    initialiseVolatiliteImplicite(matriceInput1);
    matriceOutputW = getMatriceW();
    afficheTAB(matriceOutputW);
    cout << "\n" << endl;
    /* Valeur attendu :
     -1		0		0,2			0,4			0,6			0,8			1			1,2			1,4			1,6			1,8			2
     1,00   	 -     	 0,008   	 0,016   	 0,024   	 0,032   	 0,040   	 0,048   	 0,056   	 0,064   	 0,072   	 0,080
     1,10   	 -     	 0,008   	 0,016   	 0,024   	 0,032   	 0,040   	 0,048   	 0,056   	 0,064   	 0,072   	 0,080
     1,20   	 -     	 0,008   	 0,016   	 0,024   	 0,032   	 0,040   	 0,048   	 0,056   	 0,064   	 0,072   	 0,080
     1,30   	 -     	 0,008   	 0,016   	 0,024   	 0,032   	 0,040   	 0,048   	 0,056   	 0,064   	 0,072   	 0,080
     1,40   	 -     	 0,008   	 0,016   	 0,024   	 0,032   	 0,040   	 0,048   	 0,056   	 0,064   	 0,072   	 0,080
     1,50   	 -     	 0,008   	 0,016   	 0,024   	 0,032   	 0,040   	 0,048   	 0,056   	 0,064   	 0,072   	 0,080
     1,60   	 -     	 0,008   	 0,016   	 0,024   	 0,032   	 0,040   	 0,048   	 0,056   	 0,064   	 0,072   	 0,080
     1,70   	 -     	 0,008   	 0,016   	 0,024   	 0,032   	 0,040   	 0,048   	 0,056   	 0,064   	 0,072   	 0,080
     1,80   	 -     	 0,008   	 0,016   	 0,024   	 0,032   	 0,040   	 0,048   	 0,056   	 0,064   	 0,072   	 0,080
     1,90   	 -     	 0,008   	 0,016   	 0,024   	 0,032   	 0,040   	 0,048   	 0,056   	 0,064   	 0,072   	 0,080
     2,00   	 -     	 0,008   	 0,016   	 0,024   	 0,032   	 0,040   	 0,048   	 0,056   	 0,064   	 0,072   	 0,080
     */
    
    matriceOutputVol = getMatriceVolatiliteLocale();
    cout << "Matrice de Vol :"<<endl;
    afficheTAB(matriceOutputVol);
    cout << "\n" << endl;
    /*Valeur attendu :
     -1		0			0,2			0,4			0,6			0,8			1			1,2			1,4			1,6			1,8			2
     1,00
     1,10   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200
     1,20   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200
     1,30   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200
     1,40   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200
     1,50   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200
     1,60   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200
     1,70   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200
     1,80   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200
     1,90   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200   	 0,200
     2,00
     */
    
    //Cas test 2
    remplirMatriceInput(matriceInput1, 2);
    cout << "Matrice de départ : " << endl;
    afficheTAB(matriceInput1);
    cout << "\n" << endl;
    
    cout << "Initiatlisation de la matrice et affichage de la matrice W : " <<endl;
    initialiseVolatiliteImplicite(matriceInput1);
    matriceOutputW = getMatriceW();
    afficheTAB(matriceOutputW);
    cout << "\n" << endl;
    /*Valeur attendu :
     -1		0		0,2			0,4			0,6			0,8			1			1,2			1,4			1,6			1,8			2
     1,00   	 -     	 0,008   	 0,016   	 0,024   	 0,032   	 0,040   	 0,048   	 0,056   	 0,064   	 0,072   	 0,080
     1,10   	 -     	 0,009   	 0,018   	 0,024   	 0,036   	 0,048   	 0,057   	 0,067   	 0,065   	 0,075   	 0,087
     1,20   	 -     	 0,010   	 0,018   	 0,026   	 0,041   	 0,057   	 0,067   	 0,074   	 0,077   	 0,085   	 0,098
     1,30   	 -     	 0,012   	 0,020   	 0,027   	 0,048   	 0,058   	 0,077   	 0,084   	 0,082   	 0,093   	 0,111
     1,40   	 -     	 0,013   	 0,020   	 0,028   	 0,052   	 0,058   	 0,080   	 0,093   	 0,088   	 0,096   	 0,124
     1,50   	 -     	 0,013   	 0,021   	 0,028   	 0,059   	 0,063   	 0,089   	 0,106   	 0,089   	 0,106   	 0,143
     1,60   	 -     	 0,014   	 0,021   	 0,030   	 0,066   	 0,071   	 0,097   	 0,113   	 0,102   	 0,113   	 0,153
     1,70   	 -     	 0,015   	 0,023   	 0,033   	 0,074   	 0,073   	 0,105   	 0,122   	 0,119   	 0,119   	 0,162
     1,80   	 -     	 0,016   	 0,025   	 0,036   	 0,078   	 0,083   	 0,110   	 0,134   	 0,132   	 0,138   	 0,172
     1,90   	 -     	 0,018   	 0,026   	 0,041   	 0,079   	 0,093   	 0,112   	 0,149   	 0,139   	 0,151   	 0,194
     2,00   	 -     	 0,018   	 0,027   	 0,043   	 0,080   	 0,094   	 0,121   	 0,158   	 0,154   	 0,160   	 0,215
     */
    
    matriceOutputVol = getMatriceVolatiliteLocale();
    cout << "Matrice de Vol :"<<endl;
    afficheTAB(matriceOutputVol);
    cout << "\n" << endl;
    /*Valeur attendu :
     -1			0			0,2			0,4			0,6			0,8			1			1,2			1,4			1,6			1,8			2
     1,00
     1,10   	 0,205   	 0,243   	 0,233   	 0,175   	 0,269   	 0,306   	 0,264   	 0,305   	#NOMBRE!	 0,223   	 0,263
     1,20   	 0,207   	 0,248   	 0,204   	 0,205   	 0,329   	 0,486   	 0,356   	 0,225   	 0,191   	 0,289   	 0,339
     1,30   	 0,223   	 0,340   	 0,232   	 0,201   	 0,599   	 0,231   	 0,641   	 0,311   	#NOMBRE!	 0,304   	 0,594
     1,40   	 0,223   	 0,290   	 0,203   	 0,213   	 0,470   	 0,176   	 0,339   	 0,387   	#NOMBRE!	 0,225   	 0,727
     1,50   	 0,236   	 0,304   	 0,225   	 0,197   	 1,183   	 0,196   	 3,030   	#NOMBRE!	#NOMBRE!	 0,604   	#NOMBRE!
     1,60   	 0,243   	 0,292   	 0,193   	 0,262   	 2,978   	#NOMBRE!	 0,685   	 0,422   	 0,316   	 0,352   	 0,868
     1,70   	 0,261   	 0,649   	 0,729   	 0,655   	#NOMBRE!	#NOMBRE!	 1,166   	 0,740   	 0,127   	#NOMBRE!	 0,702
     1,80   	 0,272   	#NOMBRE!	 0,660   	#NOMBRE!	 0,653   	#NOMBRE!	 0,619   	#NOMBRE!	 0,093   	#NOMBRE!	 0,820
     1,90   	 0,288   	#NOMBRE!	 0,225   	#NOMBRE!	 0,518   	#NOMBRE!	 0,326   	#NOMBRE!	#NOMBRE!	#NOMBRE!	#NOMBRE!
     2,00
     PS : #NOMBRE! est une valeur impossible. Ils ont été laissé ici pour des questions d'esthétisme.
     Variance négative possible donc error.
     SI error --> renvoyer la valeur d'entrée.
     */
    
    cout << "Ceci est un test d'interpolation : \n" <<endl;
    cout << interpolationLineaireVolatiliteLocale(1.35, 0.2)<< endl;
    
    for(int i=0;i<12;i++){
        delete [] matriceInput1[i];
        delete [] matriceOutputW[i];
        delete [] matriceOutputVol[i];
        
    }
    delete [] matriceInput1;
    delete [] matriceOutputW;
    delete [] matriceOutputVol;
    
    
    return 0;
}

void remplirMatriceInput(double ** matrix,int cas){
    //Val -1 au depart
    matrix[0][0]=-1;
    //maturity
    matrix[0][1] = 0;
    for(int j=2;j<12;j++){
        matrix[0][j] = matrix[0][j-1] + 0.2;
    }
    //strike
    matrix[1][0] = 1;
    for(int i=2;i<12;i++){
        matrix[i][0] = matrix[i-1][0] + 0.1;
    }
    if (cas == 1){
        //volatility
        for(int i =1;i<12;i++){
            for(int j =1;j<12;j++){
                matrix[i][j] = 0.2;
            }
        }
    }
    
    else if (cas == 2){
        matrix[1][1]=0.2000;	matrix[1][2]=0.2000;	matrix[1][3]=0.2000;	matrix[1][4]=0.2000;	matrix[1][5]=0.2000;	matrix[1][6]=0.2000;	matrix[1][7]=0.2000;	matrix[1][8]=0.2000;	matrix[1][9]=0.2000;	matrix[1][10]=0.2000;	matrix[1][11]=0.2000;
        matrix[2][1]=0.2047;	matrix[2][2]=0.2177;	matrix[2][3]=0.2141;	matrix[2][4]=0.2017;	matrix[2][5]=0.2127;	matrix[2][6]=0.2194;	matrix[2][7]=0.2180;	matrix[2][8]=0.2189;	matrix[2][9]=0.2015;	matrix[2][10]=0.2039;	matrix[2][11]=0.2084;
        matrix[3][1]=0.2069;	matrix[3][2]=0.2265;	matrix[3][3]=0.2150;	matrix[3][4]=0.2075;	matrix[3][5]=0.2257;	matrix[3][6]=0.2393;	matrix[3][7]=0.2368;	matrix[3][8]=0.2304;	matrix[3][9]=0.2188;	matrix[3][10]=0.2174;	matrix[3][11]=0.2212;
        matrix[4][1]=0.2225;	matrix[4][2]=0.2433;	matrix[4][3]=0.2228;	matrix[4][4]=0.2116;	matrix[4][5]=0.2442;	matrix[4][6]=0.2407;	matrix[4][7]=0.2540;	matrix[4][8]=0.2453;	matrix[4][9]=0.2264;	matrix[4][10]=0.2269;	matrix[4][11]=0.2358;
        matrix[5][1]=0.2232;	matrix[5][2]=0.2502;	matrix[5][3]=0.2247;	matrix[5][4]=0.2152;	matrix[5][5]=0.2552;	matrix[5][6]=0.2415;	matrix[5][7]=0.2575;	matrix[5][8]=0.2573;	matrix[5][9]=0.2343;	matrix[5][10]=0.2313;	matrix[5][11]=0.2489;
        matrix[6][1]=0.2359;	matrix[6][2]=0.2563;	matrix[6][3]=0.2289;	matrix[6][4]=0.2166;	matrix[6][5]=0.2712;	matrix[6][6]=0.2506;	matrix[6][7]=0.2731;	matrix[6][8]=0.2758;	matrix[6][9]=0.2356;	matrix[6][10]=0.2422;	matrix[6][11]=0.2676;
        matrix[7][1]=0.2433;	matrix[7][2]=0.2601;	matrix[7][3]=0.2291;	matrix[7][4]=0.2222;	matrix[7][5]=0.2866;	matrix[7][6]=0.2668;	matrix[7][7]=0.2840;	matrix[7][8]=0.2843;	matrix[7][9]=0.2530;	matrix[7][10]=0.2500;	matrix[7][11]=0.2770;
        matrix[8][1]=0.2609;	matrix[8][2]=0.2726;	matrix[8][3]=0.2410;	matrix[8][4]=0.2331;	matrix[8][5]=0.3049;	matrix[8][6]=0.2703;	matrix[8][7]=0.2952;	matrix[8][8]=0.2952;	matrix[8][9]=0.2723;	matrix[8][10]=0.2567;	matrix[8][11]=0.2842;
        matrix[9][1]=0.2715;	matrix[9][2]=0.2869;	matrix[9][3]=0.2512;	matrix[9][4]=0.2459;	matrix[9][5]=0.3116;	matrix[9][6]=0.2873;	matrix[9][7]=0.3026;	matrix[9][8]=0.3089;	matrix[9][9]=0.2876;	matrix[9][10]=0.2764;	matrix[9][11]=0.2931;
        matrix[10][1]=0.2881;	matrix[10][2]=0.2997;	matrix[10][3]=0.2537;	matrix[10][4]=0.2619;	matrix[10][5]=0.3151;	matrix[10][6]=0.3058;	matrix[10][7]=0.3053;	matrix[10][8]=0.3264;	matrix[10][9]=0.2943;	matrix[10][10]=0.2892;	matrix[10][11]=0.3118;
        matrix[11][1]=0.2920;	matrix[11][2]=0.3030;	matrix[11][3]=0.2619;	matrix[11][4]=0.2692;	matrix[11][5]=0.3162;	matrix[11][6]=0.3060;	matrix[11][7]=0.3180;	matrix[11][8]=0.3362;	matrix[11][9]=0.3107;	matrix[11][10]=0.2979;	matrix[11][11]=0.3277;
    }
}

void afficheTAB(double ** matrix){
    for(int i =0;i<12;i++){
        for(int j =0;j<12;j++){
            cout<< " "<<matrix[i][j]<<" ";
        }
        cout << endl;
    }
}