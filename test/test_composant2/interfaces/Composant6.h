#pragma once
#ifndef COMPOSANT6_H
#define COMPOSANT6_H

#ifdef COMPOSANT6_EXPORTS
	#define COMPOSANT6_INTERFACE __declspec(dllexport)
#else
	#define COMPOSANT6_INTERFACE __declspec(dllimport)
#endif

#include<vector>
#include <iostream>
using std::vector;
using std::cout;
using std::endl;
// interface du composant1

//COMPOSANT1_INTERFACE char * getComposant1Version(void);
COMPOSANT6_INTERFACE vector<double> getChemin(int maturite, double spot);
#endif

