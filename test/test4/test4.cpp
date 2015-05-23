//
//  test5.cpp
//  Testcomposant5
//
//  Created by Nicolas EZE on 12/04/2015.
//  Copyright (c) 2015 Nicolas EZE. All rights reserved.
//

#include <iostream>
#include "composant5.h"
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>

int main(int argc, const char * argv[]) {
    
    //TEST Fichier corrompu
    std::cout << "Test 1 - Cas d'utilisation 1 avec un fichier corrompu" << std::endl;
    
    bool result = findVolatilityData("K:\\VollImpCorrupted.data"); //Chemin a renseigné
    if(result==false)
    {
        std::cout << "Resultat Test 1:" << result << "Test fichier corrompu OK" << std::endl;
        
    }
    else
    {
        std::cout << "Resultat Test 1:" << result << "Test fichier corrompu KO" << std::endl;
    }
    
    std::cout<<std::endl;
    std::cout << "Fin Test 1 - Cas d'utilisation 1 avec un fichier corrompu" << std::endl;
    
    
    
    //Test fichier avec une seule erreur
    std::cout << "Test 2 - Cas d'utilisation 1 avec un fichier contenant une seule erreur" << std::endl;
    
    result = findVolatilityData("K:\\VollImpSimpleError.data"); //Chemin a renseigné
    if(result==false)
    {
        std::cout << "Resultat Test 2:" << result << "Test fichier une seule erreur OK" << std::endl;
        
    }
    else
    {
        std::cout << "Resultat Test 2:" << result << "Test fichier une seule erreur KO" << std::endl;
    }
    std::cout<<std::endl;
    std::cout << "Fin Test 2 - Cas d'utilisation 1 avec un fichier contenant une seule erreur" << std::endl;
    
    
    
    
    
    
    /* PREMIER CAS avec FICHIER (Maturité, strike, volatilié) SIMPLE */
    
    std::cout << "Test 3 - Cas d'utilisation 1 avec un fichier simple + Cas d'utilisation 2 dans 6 cas différents" << std::endl;
    
    
    //Test fichier
    result = findVolatilityData("K:\\VollImpSimple.data"); //Chemin a renseigné
    if(result==true)
    {
        std::cout << "Resultat Test 3 - Part 1:" << result << "Test fichier simple OK" << std::endl;
    }
    else
    {
        std::cout << "Resultat Test 3 - Part 1:" << result << "Test fichier simple KO" << std::endl;
    }
    
    //Test use case 2 - vol incluse dans le fichier
    float vol = findVolatility(20, 0.443532);
    if(vol==127)
    {
        std::cout << "Resultat Test 3 - Part 2:" << vol << "Test donnees du fichier OK" << std::endl;
    }
    else
    {
        std::cout << "Resultat Test 3 - Part 2:" << vol << "Test donnees du fichier KO" << std::endl;
    }
    
    //Test use case 2 - extrapolation sur maturite
    vol = findVolatility(20, 0);
    if(vol==107)
    {
        std::cout << "Resultat Test 3 - Part 3:" << vol << "Test extrapolation sur maturite OK" << std::endl;
    }
    else
    {
        std::cout << "Resultat Test 3 - Part 3:" << vol << "Test extrapolation sur maturite KO" << std::endl;
    }
    
    //Test use case 2 - extrapolation sur strike
    vol = findVolatility(70, 0.941821);
    if(vol==114)
    {
        std::cout << "Resultat Test 3 - Part 4:" << vol << "Test extrapolation sur strike OK" << std::endl;
    }
    else
    {
        std::cout << "Resultat Test 3 - Part 4:" << vol << "Test extrapolation sur strike KO" << std::endl;
    }
    
    //Test use case 2 - interpolation spline cubique sur un strike (dans le cas du fichier simple pas de spline -> interpolation linéaire)
    vol = findVolatility(20, 0.232717);
    if(vol==157.525)
    {
        std::cout << "Resultat Test 3 - Part 5:" << vol << "Test interpolation spline cubique sur un strike OK" << std::endl;
    }
    else
    {
        std::cout << "Resultat Test 3 - Part 5:" << vol << "Test interpolation spline cubique sur un strike KO" << std::endl;
    }
    
    //Test use case 2 - interpolation lineaire sur maturite
    vol = findVolatility(35, 1.19097);
    if(vol==108.76)
    {
        std::cout << "Resultat Test 3 - Part 6:" << vol << "Test interpolation lineaire sur maturite OK" << std::endl;
    }
    else
    {
        std::cout << "Resultat Test 3 - Part 6:" << vol << "Test interpolation lineaire sur maturite KO" << std::endl;
    }
    
    //Test use case 2 - interpolation spline cubique sur strike et lineaire sur maturite
    vol = findVolatility(25, 0.8172485);
    if(vol==119)
    {
        std::cout << "Resultat Test 3 - Part 7:" << vol << "Test interpolation spline cubique sur strike et lineaire sur maturite OK" << std::endl;
    }
    else
    {
        std::cout << "Resultat Test 3 - Part 7:" << vol << "Test interpolation spline cubique sur strike et lineaire sur maturite KO" << std::endl;
    }
    std::cout<<std::endl;
    std::cout << "Fin Test 3 - Cas d'utilisation 1 avec un fichier simple + Cas d'utilisation 2 dans 6 cas différents" << std::endl;
    
    
    
    
    
    //DEUXIEME CAS avec FICHIER (Maturité, strike, volatilié) NORMAL
    std::cout << "Test 4 - Cas d'utilisation 1 avec un fichier normal + Cas d'utilisation 2 dans 6 cas différents" << std::endl;std::endl;
    
    //Test fichier
    result = findVolatilityData("K:\\VollImpLegit.data"); //Chemin a renseigné
    if(result==true)
    {
        std::cout << "Resultat Test 4 - Part 1:" << result << "Test fichier normal OK" << std::endl;
    }
    else
    {
        std::cout << "Resultat Test 4 - Part 1:" << result << "Test fichier normal KO" << std::endl;
    }
    
    //Test use case 2 - vol incluse dans le fichier
    vol = findVolatility(20, 15.0281);
    if(vol==0.0230388)
    {
        std::cout << "Resultat Test 4 - Part 2:" << vol << "Test donnees du fichier OK" << std::endl;
    }
    else
    {
        std::cout << "Resultat Test 4 - Part 2:" << vol << "Test donnees du fichier KO" << std::endl;
    }
    
    //Test use case 2 - extrapolation sur maturite
    vol = findVolatility(1.0, 16);
    if(vol==0.343353)
    {
        std::cout << "Resultat Test 3 - Part 3:" << vol << "Test extrapolation sur maturite OK" << std::endl;
    }
    else
    {
        std::cout << "Resultat Test 3 - Part 3:" << vol << "Test extrapolation sur maturite KO" << std::endl;
    }
    
    //Test use case 2 - extrapolation sur strike
    vol = findVolatility(0.19, 0.117728);
    if(vol==1.62114)
    {
        std::cout << "Resultat Test 3 - Part 4:" << vol << "Test extrapolation sur strike OK" << std::endl;
    }
    else
    {
        std::cout << "Resultat Test 3 - Part 4:" << vol << "Test extrapolation sur strike KO" << std::endl;
    }
    
    //Test use case 2 - interpolation spline cubique sur un strike
    vol = findVolatility(1.0, 14);
    if(vol==0.347782)
    {
        std::cout << "Resultat Test 3 - Part 5:" << vol << "Test interpolation spline cubique sur un strike OK" << std::endl;
    }
    else
    {
        std::cout << "Resultat Test 3 - Part 5:" << vol << "Test interpolation spline cubique sur un strike KO" << std::endl;
    }
    
    //Test use case 2 - interpolation lineaire sur maturite
    vol = findVolatility(2.2, 0.117728);
    if(vol==0.325899)
    {
        std::cout << "Resultat Test 3 - Part 6:" << vol << "Test interpolation lineaire sur maturite OK" << std::endl;
    }
    else
    {
        std::cout << "Resultat Test 3 - Part 6:" << vol << "Test interpolation lineaire sur maturite KO" << std::endl;
    }
    
    //Test use case 2 - interpolation spline cubique sur strike et lineaire sur maturite
    vol = findVolatility(6.23, 6);
    if(vol==0.116357)
    {
        std::cout << "Resultat Test 3 - Part 7:" << vol << "Test interpolation spline cubique sur strike et lineaire sur maturite OK" << std::endl;
    }
    else
    {
        std::cout << "Resultat Test 3 - Part 7:" << vol << "Test interpolation spline cubique sur strike et lineaire sur maturite KO" << std::endl;
    }
    
    std::cout<<std::endl;
    std::cout << "Fin Test 4 - Cas d'utilisation 1 avec un fichier normal + Cas d'utilisation 2 dans 6 cas différents" << std::endl;
    
    return 0;
    
}

