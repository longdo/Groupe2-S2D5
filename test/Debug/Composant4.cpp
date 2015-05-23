//
//  composant5.cpp
//  Created by Steven CHEN on 07/04/2015.
//  Copyright (c) 2015 Steven CHEN. All rights reserved.

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include "composant5.h"

using namespace std;
/******************************Données *********************************************/
vector<vector<double>> tableVolInput;
vector<vector<double>> tableVolOutput;//second derivatives
vector<double> vStrike;
vector<double> vMaturite;
/*******************************Fonctions utilitaires*******************************/
//function Tokenize
void Tokenize(const string& str,
              vector<string>& tokens,
              const string& delimiters = " ")
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);
    
    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

//fonction affichage de la lecture des données
void affichageDonneesInput(){
    if(tableVolInput.size()!= 0){
        for(int i =0;i<vMaturite.size();i++)
            for(int j =0;j<vStrike.size();j++) {
                cout << vMaturite.at(i) << "  " << vStrike.at(j) << "  " << tableVolInput.at(i).at(j);
                cout << endl;
            }
    }
    else
        cerr << "Table de données input vide !" << endl;
}

//fonction controle de données des inputs
bool controleDonnesInput(){
    //si pas de données lues
    if(vMaturite.size()==0 || vStrike.size()==0 || tableVolInput.size()==0 )
        return false;
    //check si le nombre de données lues est cohérents
    if(vMaturite.size()*vStrike.size()!= tableVolInput.at(0).size()*tableVolInput.size())
        return false;
    return true;
}

void spline(double x[], double y[], int n, double yp1, double ypn, double y2[])
/*Given arrays x[1..n] and y[1..n] containing a tabulated function, i.e., yi = f(xi), with
 x1 < x2 <...< xN , and given values yp1 and ypn for the first derivative of the interpolating
 function at points 1 and n, respectively, this routine returns an array y2[1..n] that contains
 the second derivatives of the interpolating function at the tabulated points xi. If yp1 and/or
 ypn are equal to 1 × 1030 or larger, the routine is signaled to set the corresponding boundary
 condition for a natural spline, with zero second derivative on that boundary.*/
{
    int i,k;
    float p,qn,sig,un;
    float u [n-1];
    //u=vector(1,n-1);
    if (yp1 > 0.99e30)
        y2[1]=u[1]=0.0;
    else {
        y2[1] = -0.5;
        u[1]=(3.0/(x[2]-x[1]))*((y[2]-y[1])/(x[2]-x[1])-yp1);
    }
    for (i=2;i<=n-1;i++) {
        sig=(x[i]-x[i-1])/(x[i+1]-x[i-1]);
        p=sig*y2[i-1]+2.0;
        y2[i]=(sig-1.0)/p;
        u[i]=(y[i+1]-y[i])/(x[i+1]-x[i]) - (y[i]-y[i-1])/(x[i]-x[i-1]);
        u[i]=(6.0*u[i]/(x[i+1]-x[i-1])-sig*u[i-1])/p;
    }
    if (ypn > 0.99e30)
        qn=un=0.0;
    else {
        qn=0.5;
        un=(3.0/(x[n]-x[n-1]))*(ypn-(y[n]-y[n-1])/(x[n]-x[n-1]));
    }
    cout<<"";
    y2[n]=(un-qn*u[n-1])/(qn*y2[n-1]+1.0);
    for (k=n-1;k>=1;k--)
        y2[k]=y2[k]*y2[k+1]+u[k];
    //delete [] u;
}

void splint(double xa[], double ya[], double y2a[], int n, double x, double *y)
/*Given the arrays xa[1..n] and ya[1..n], which tabulate a function (with the xai’s in order),
 and given the array y2a[1..n], which is the output from spline above, and given a value of
 x, this routine returns a cubic-spline interpolated value y.*/
{
    void nrerror(char error_text[]);
    int klo,khi,k;
    double h,b,a;
    klo=1;
    khi=n;
    while (khi-klo > 1) {
        k=(khi+klo) >> 1;
        if (xa[k] > x) khi=k;
        else klo=k;
    }
    h=xa[khi]-xa[klo];
    if (h == 0.0)
        cout<<("Bad xa input to routine splint");
    a=(xa[khi]-x)/h;
    b=(x-xa[klo])/h;
    *y=a*ya[klo]+b*ya[khi]+((a*a*a-a)*y2a[klo]+(b*b*b-b)*y2a[khi])*(h*h)/6.0;
    
}

//fonction affichage données output
void affichageDonnesOutput(){
    for(int i=0;i<vStrike.size();i++){
        cout<<"Strike:"<<vStrike.at(i)<<endl;
        for(int j=0;j<vMaturite.size();j++){
            cout<<"\t de maturite"<<vMaturite.at(j);
            cout<<"coefficient derive:"<<endl;
            for(int k=0;k<vMaturite.size();k++)
                cout<<tableVolOutput.at(i).at(k);
        }
        cout<<endl;
        
    }
}

double interpolationLinear(double y1,double y2,double x1, double x2, double xi){
    return y1+ (y2-y1)*(xi-x1)/(x2-x1);
}

void splineCubique(int x){//calcule le spline cubique pour chaque strike
    vector<double> derive;
    //appel à la fonction splint
    //abscisse
    double strike[vStrike.size()+1];
    strike[0]=-1;
    for(int i =0;i<vStrike.size();i++)
        strike[i+1]=vStrike.at(i);
    for(int i =0;i<vStrike.size();i++)
        strike[i+1]=vStrike.at(i);
    //récupérer les ordonnées => volatilite
    double volatilite[vStrike.size()+1];
    volatilite[0]=-1;
    for(int j=0;j<vStrike.size();j++)
        volatilite[j+1]= tableVolInput.at(x).at(j);
    //cout<< "valeur de volatilite pour un nouveau strike"<<endl;
    //for(int j=0;j<vMaturite.size();j++)
    // cout<< volatilite[j]<<endl;
    //double * result = new double[vMaturite.size()];
    double result[vStrike.size()+1];
    spline(strike, volatilite, vStrike.size()+1, 0, 0, result);
    //cout<< "resultat"<<endl;
    for(int i=0;i<vStrike.size()+1;i++)
        derive.push_back(result[i]);
    // for(int i=0;i<vStrike.size()+1;i++)
    //    cout<<result[i]<<" ";
    tableVolOutput.push_back(derive);
    derive.clear();
}

//transform input data to output Volatility
void calculSpline(){
    //initialisation des emplacements
    tableVolOutput.clear();
    for(int i=0;i<vMaturite.size();i++){
        //pour chaque maturite on calcule les splines associées
        splineCubique(i);
    }
    // cout<<"calculSpline executed"<<endl;
    
}

bool findVolatilityData(string filename)
{
    //cout<< "FindVolatilityData methode Executed"<<endl;
    // la vol et le strike doivent être divisées par 100
    ifstream fichier(filename, ios::in);  // on ouvre le fichier en lecture
    if(fichier)  // si l'ouverture a réussi
    {
        tableVolInput.clear();
        tableVolOutput.clear();
        vStrike.clear();
        vMaturite.clear();
        bool strikeOK = false;
        bool nextMat = true;
        //cout<<"Lecture de fichier"<<endl;
        string ligne;
        vector<double> volMat;
        // lecture des données
        while(getline(fichier, ligne)){// tant que l'on peut mettre la ligne dans "contenu"
            vector<string> tokens;
            Tokenize(ligne,tokens);//token la ligne en 3 tokens
            if(tokens.size() < 3){// si saut de ligne => nouvelle maturité
                tableVolInput.push_back(volMat);//on range un vecteur de volatilites dans le vecteur tableVolInput
                volMat.clear();//vider la colonne de volMat
                nextMat = true;//il faut lire la maturité suivante
                strikeOK = true;//on a lu tous les strike
            }
            else {//ligne de données
                if(nextMat) {//doit-on lire la maturite?
                    double mat = atof(tokens.at(0).c_str());
                    //cout<<mat<<endl;
                    vMaturite.push_back(mat);
                    nextMat = false;
                }
                if(!strikeOK) {//doit-on lire le strike?
                    double strike = atof(tokens.at(1).c_str());
                    //cout<<strike<<endl;
                    vStrike.push_back(strike/100);
                }
                //lecture de la volatilite
                double volatilite = atof(tokens.at(2).c_str());
                //cout<<volatilite<<endl;
                volMat.push_back(volatilite/100);
                
            }
        }
        //cout << "fin"<<endl;
        fichier.close();
        //fin lecture des données
        if(!controleDonnesInput())
            return false;
        //cout<<"Bonne lecture des données"<<endl;
        calculSpline();//remplit le tableau de données en output
        return true;
    }
    else  // sinon
        cerr << "Impossible d'ouvrir le fichier !" << endl;
    return false;
}


int containsStrike(double strike){
    for(int i=0;i<vStrike.size();i++)
        if(vStrike.at(i)== strike)
            return i;
    return -1;
}


int containsMaturity(double mat){
    for(int i=0;i<vMaturite.size();i++)
        if(vMaturite.at(i)== mat)
            return i;
    return -1;
}

int indiceStrike(double mat){
    for(int i=0;i<vStrike.size();i++){
        if(vMaturite.at(i)> mat)
            return i-1;
    }
    return -1;
}

double calculVolatilite(int indiceStrike,int indiceMaturite,double strike){
    double result =0;
    //tableau maturite
    double * tabStrike =new double[vStrike.size()+1];
    double * tabDerive =new double[vStrike.size()+1];
    double * tabvolatilite = new double[vStrike.size()+1];
    //double tabStrike[vStrike.size()+1];
    tabStrike[0]=-1;
    for(int i =0;i<vStrike.size();i++)
        tabStrike[i+1]=vStrike.at(i);
    //tableau derive
    //double tabDerive[vStrike.size()+1];
    for(int i =0;i<vStrike.size()+1;i++)
        tabDerive[i]=tableVolOutput.at(indiceMaturite).at(i);
    //tableau volatilite
    tabvolatilite[0]=-1;
    for(int i=0;i<vStrike.size();i++)
        tabvolatilite[i+1]=tableVolInput.at(indiceMaturite).at(i);
    //cout<<"tablea derive"<<endl;
    //for(int i=0;i<vStrike.size()+1;i++)
    //    cout<<tabDerive[i]<<","<<endl;
    //for(int i=0;i<vStrike.size()+1;i++)
    //    cout<<tabStrike[i]<<","<<endl;
    //cout<<"vola"<<endl;
    //for(int i=0;i<vStrike.size()+1;i++)
    //   cout<<tabvolatilite[i]<<","<<endl;
    splint(tabStrike, tabvolatilite,tabDerive, vStrike.size()+1, strike, &result);
    delete [] tabStrike;
    delete [] tabDerive;
    return(result);
    
}

double findVolatility(double strike, double maturity){
    //cout<<"find volatility"<<endl;
    double result =0;
    int indiceMaturite = containsMaturity(maturity);
    //si maturité existe
    if(indiceMaturite!=-1){//easy
        //cout<<"goodMaturity"<<endl;
        int indiceStrike = containsStrike(strike);
        if(indiceStrike!=-1){
            //cout<<"goodstrike"<<endl;
            result = calculVolatilite(indiceStrike,indiceMaturite,strike);
        }
        else{
            cout<<"badStrike"<<endl;
            //extrapolation
            cout<< "strike:"<<strike <<"vstrike"<<vStrike.at(0)<<endl;
            if(strike<vStrike.at(0)){//si le strike est plus petit
                result = findVolatility(vStrike.at(0),maturity);
            }
            else{
                if(strike>vStrike.at(vStrike.size()-1))
                    result = findVolatility(vStrike.at(vStrike.size()-1),maturity);
                else
                    result = calculVolatilite(indiceStrike,indiceMaturite,strike);
            }
        }
        cout<<endl;
        return result;
    }
    else{
        //cout<< "bad maturity"<<endl;
        if(maturity<vMaturite.at(0))
            result = findVolatility(strike, vMaturite.at(0));
        else{
            //cout<< "mat grande";
            if(maturity> vMaturite.at(vMaturite.size()-1))
                result = findVolatility(strike, vMaturite.at(vMaturite.size()-1));
            else{
                //cout<<"interpolation lineaire"<<endl;
                int indice=indiceStrike(maturity);//petit indice
                double x1= vMaturite.at(indice);
                double x2= vMaturite.at(indice+1);
                cout<<x1<<" "<<maturity<<" "<<x2<<endl;
                //cas 1 bon strike
                if(containsStrike(strike)!=-1){
                    //good strike
                    cout<<" good strike:"<<strike<<endl;
                    result = interpolationLinear(findVolatility(strike, x1),findVolatility(strike, x2), x1,x2, maturity);
                }
                else{
                    //cas 2 mauvais strike
                    cout<<"maturité cherchée:"<<maturity<<endl;
                    cout<<"maturité inf:"<<x1<<endl;
                    cout<<"maturité sup:"<<x2<<endl;
                    double y1 =findVolatility(strike, x1);
                    double y2 = findVolatility(strike, x2);
                    cout<<" volatilité calculé maturité inf:"<< y1<<endl;
                    cout<<" volatilité calculé maturité sup:"<< y2<<endl;
                    result= interpolationLinear(y1,y2,x1,x2 , maturity);
                }
            }
            
        }
        cout<<endl;
        return result;
        
    }
}

/*
int main(int argc, const char * argv[]) {
    
    bool result =findVolatilityData("/Users/chenstpro/Documents/VolImp.data");
    return 0;
}*/