// partageable.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "partageable.h"


// This is an example of an exported variable
PARTAGEABLE_API int npartageable=0;

// This is an example of an exported function.
PARTAGEABLE_API int fnpartageable(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see partageable.h for the class definition
Cpartageable::Cpartageable()
{
	return;
}
