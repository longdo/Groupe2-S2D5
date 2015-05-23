// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the COMPOSANT2_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// COMPOSANT2_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
/*#ifdef COMPOSANT2_EXPORTS
#define COMPOSANT2_API __declspec(dllexport)
#else
#define COMPOSANT2_API __declspec(dllimport)
#endif

// This class is exported from the Composant2.dll
class COMPOSANT2_API CComposant2 {
public:
	CComposant2(void);
	// TODO: add your methods here.
};

extern COMPOSANT2_API int nComposant2;

COMPOSANT2_API int fnComposant2(void);
*/

#include <string>
#include <vector>
/*#define COMPOSANT2_VERSION 1,0,0,0
#define COMPOSANT_VERSION_STR "1,0,0,0"*/

#ifndef COMPOSANT2_H
#define COMPOSANT2_H

#ifdef COMPOSANT2_EXPORTS
	#define COMPOSANT2_INTERFACE __declspec(dllexport)
#else
	#define COMPOSANT2_INTERFACE __declspec(dllimport)
#endif


class Composant2{
public:
	COMPOSANT2_INTERFACE Composant2();//Constructeur
	/*static*/ COMPOSANT2_INTERFACE double doMonteCarlo(std::string, double, double, int);
	/*static*/ COMPOSANT2_INTERFACE std::vector<double> getPath(double, double);
	/*static*/ COMPOSANT2_INTERFACE double pricePath(std::string, std::vector<double>, double, double);
	/*static*/ COMPOSANT2_INTERFACE char * getComposant2Version(void);
private:

#endif


};