#pragma once
#ifndef COMPOSANT7_H
#define COMPOSANT7_H

#ifdef COMPOSANT7_EXPORTS
	#define COMPOSANT7_INTERFACE __declspec(dllexport)
#else
	#define COMPOSANT7_INTERFACE __declspec(dllimport)
#endif

#include <string>
#include <vector>
using std::string;
COMPOSANT7_INTERFACE double pricePath2( string typePayOff, double* path, double strike, double maturity);
#endif

