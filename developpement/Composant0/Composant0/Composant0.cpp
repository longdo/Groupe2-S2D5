#include "Composant0.h"
#include "Composant0Version.h"
#include <math.h>


double composant0(double p1, double p2)
{
	return pow(p1,p2);
}


char * getComposant0Version()
{
	return "Composant 0 version " COMPOSANT_VERSION_STR;
}
