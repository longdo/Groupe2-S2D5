#include "composant0.h"
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>


int main()
{
	double result;

	result=composant0(2,5);

	std::cout << "Resultat appel composant0 " << result << std::endl;

	char * versionComposant=getComposant0Version();

	std::cout << "Version composant :" << versionComposant << std::endl;

	Sleep(3000);
}
