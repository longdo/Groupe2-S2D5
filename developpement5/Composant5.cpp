//
//  main.cpp
//  ComponentFinalV1_0
//
//  Created by Long DO on 4/15/15.
//  Copyright (c) 2015 Long DO. All rights reserved.
//

#include "math.h"
#include "Composant5Version.h"
#include <iostream>
#include <sstream>

// TODO : Include Component 4 when functions are available
//#include "Composant4.h" 

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

//*************************************
//*         MODULE4'S FUNCTIONS       *
//*************************************
bool findVolatilityData(string filename){
    return true;
}

double findVolatility(double strike, double maturity){
    return 0.0;
}

//*************************************
//*               MATRIX              *
//*************************************
double **impliciteVolMatrix;
double **intermediateValueMatrix;
double **localVolMatrix;

//*************************************
//*          OTHER FUNCTIONS          *
//*************************************
void deleteMatrix(){
    for(int i = 0; i < NB_MATURITY+1; i++){
        delete [] impliciteVolMatrix[i];
        delete [] intermediateValueMatrix[i];
        delete [] localVolMatrix[i];
    }
    
    delete [] impliciteVolMatrix;
    delete [] intermediateValueMatrix;
    delete [] localVolMatrix;
}

void initMatrix(){
    //if matrix are allocated, delete these matrix before allocating the new ones
    if(allocateTables){
        deleteMatrix();
        allocateTables = false;
    }
    
    //matrix creation
    impliciteVolMatrix = new double * [NB_STRIKE+1];
    intermediateValueMatrix = new double * [NB_STRIKE+1];
    localVolMatrix = new double * [NB_STRIKE+1];
    
    for(int i = 0; i < NB_MATURITY+1; i++){
        impliciteVolMatrix[i] = new double[NB_MATURITY+1];
        intermediateValueMatrix[i] = new double[NB_MATURITY+1];
        localVolMatrix[i] = new double[NB_MATURITY+1];
    }
    
    allocateTables = true;
    
    //matrix' first case
    impliciteVolMatrix[0][0] = -1.0;
    intermediateValueMatrix[0][0] = -1.0;
    localVolMatrix[0][0] = -1.0;
    
    //strike value
    //fictitious value to compute derivatives
    impliciteVolMatrix[1][0] = 0.29;
    intermediateValueMatrix[1][0] = 0.29;
    localVolMatrix[1][0] = 0.29;
    
    //real 1st strike value
    impliciteVolMatrix[2][0] = STRIKE_OD;
    intermediateValueMatrix[2][0] = STRIKE_OD;
    localVolMatrix[2][0] = STRIKE_OD;
    
    for(int i = 3; i < NB_STRIKE+1; i++){
        impliciteVolMatrix[i][0] = impliciteVolMatrix[i-1][0] * FACTOR_STRIKE;
        intermediateValueMatrix[i][0] = intermediateValueMatrix[i-1][0] * FACTOR_STRIKE;
        localVolMatrix[i][0] = localVolMatrix[i-1][0] * FACTOR_STRIKE;
    }
    
    //maturity value = matrix' index
    for (int j = 1; j < NB_MATURITY+1; j++){
        impliciteVolMatrix[0][j] = (j - 1) * dt;
        intermediateValueMatrix[0][j] = (j - 1) * dt;
        localVolMatrix[0][j] = (j - 1) * dt;
    }
    
    //the rest
    for (int i = 1; i < NB_STRIKE+1; i++)
        for (int j = 1; j < NB_MATURITY+1; j++){
            impliciteVolMatrix[i][j] = 0.0;
            intermediateValueMatrix[i][j] = 0.0;
            localVolMatrix[i][j] = 0.0;
        }
}

void fillImpliciteVolMatrix(){
    for (int i = 1; i < NB_STRIKE+1; i++)
        for (int j = 1; j < NB_MATURITY+1; j++)
			//param1 : strike; param2 : maturity
            impliciteVolMatrix[i][j] = findVolatility(impliciteVolMatrix[i][0], impliciteVolMatrix[0][j]);
}

void fillIntermediateValueMatrix(){
    for (int i = 1; i < NB_STRIKE+1; i++)
        for (int j = 1; j < NB_MATURITY+1; j++)
            intermediateValueMatrix[i][j] = intermediateValueMatrix[0][j] * pow(impliciteVolMatrix[i][j], 2.0);
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
    
    return 0.25 * (-0.25 - (1.0/d1) + pow(n2/d2, 2.0)) * pow(n3/d3, 2.0);
}

double denominator3(int i, int j){
    double n = 0.0, d = 0.0;
    
    n = intermediateValueMatrix[i+1][j] - 2 * intermediateValueMatrix[i][j] + intermediateValueMatrix[i-1][j];
    d = log10(intermediateValueMatrix[i+1][0]) - log10(intermediateValueMatrix[i-1][0]);
    
    return 0.5 * n/d;
}

void fillLocalVolMatrix(){
    //localVol(0) = impliciteVol(0)
    for (int i = 1; i < NB_STRIKE+1; i++)
        localVolMatrix[i][1] = impliciteVolMatrix[i][1];
    
    //localVol(1) to localVol(M-1)
    for (int i = 1; i < NB_STRIKE+1; i++)
        for (int j = 2; j < NB_MATURITY+1; j++)
            localVolMatrix[i][j] = sqrt(nominator(i, j) / (denominator1(i, j) + denominator2(i, j) + denominator3(i, j)));
}


void correctLocalVolMatrix(){
	stringstream out;
    for (int j = 1; j < NB_MATURITY+1; j++)
        localVolMatrix[1][j] = 0.0;
    
    for (int i = 1; i < NB_STRIKE+1; i++)
        for (int j = 1; j < NB_MATURITY+1; j++){
			out<< localVolMatrix[i][j];
			if(out.str() == "-1.#IND")
                localVolMatrix[i][j] = impliciteVolMatrix[i][j];
        }
    
    for (int j = 1; j < NB_MATURITY+1; j++)
        localVolMatrix[502][j] = 0.0;
}

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
    
    indexSD = (log10(strike/STRIKE_OD) / log10(FACTOR_STRIKE)) + 1;
    indexSU = indexSD + 1;
    
    strikeD = localVolMatrix[indexSD][0];
    strikeU = localVolMatrix[indexSU][0];
    
    getIndexFromValue(strikeU, maturity, indexSU, indexM);
    
    return localVolMatrix[indexSU][indexM] - (((strikeU - strike) * (localVolMatrix[indexSU][indexM] - localVolMatrix[indexSD][indexM])) / (strikeU - strikeD));
}

//*************************************
//*      INITIALIZATION FUNCTION      *
//*************************************
bool initializeLocalVol(string filename){
    try {
        if (findVolatilityData(filename)){
            initMatrix();
            
            //matrix filling
            fillImpliciteVolMatrix();
            fillIntermediateValueMatrix();
            fillLocalVolMatrix();
            
            //local vol matrix correcting
            correctLocalVolMatrix();
            
            return true;
        }
        else{
            throw ERROR4;
            
            return false;
        }
    } catch (exception ex) {
        return false;
    }
}

//*************************************
//*         MODULE6'S FUNCTION        *
//*************************************
double getLocalVol(double strike, double maturity){
    double localVol = 0.0;
    int sindex = 0, mindex = 0;
    
    if (strike < STRIKE_OD)
        throw ERROR1;
    else if (strike > STRIKE_2Y)
        throw ERROR2;
    else if (maturity < MATURITY_0D || maturity > MATURITY_2Y)
        throw ERROR3;
    else {
        if(toDoInterpolation(strike))
            localVol = interpolationLineaireVolatiliteLocale(strike, maturity);
        else{
            getIndexFromValue(strike, maturity, sindex, mindex);
            localVol = localVolMatrix[sindex][mindex];
        }
    }
    
    return localVol;
}


//*************************************
//*      COMPONENT VERSION FUNCTION      *
//*************************************

string getComponentVersion()
{
	string s = "";
	s = "COMPOSANT 5 VERSION " + COMPOSANT_VERSION;
	return s;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
