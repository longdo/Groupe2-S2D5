#pragma once
#ifndef COMPOSANT0_H
#define COMPOSANT0_H

#ifdef COMPOSANT0_EXPORTS
	#define COMPOSANT0_INTERFACE __declspec(dllexport)
#else
	#define COMPOSANT0_INTERFACE __declspec(dllimport)
#endif


COMPOSANT0_INTERFACE double composant0(double p1, double p2);

COMPOSANT0_INTERFACE char * getComposant0Version();

#endif