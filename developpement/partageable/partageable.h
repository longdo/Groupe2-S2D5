// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PARTAGEABLE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PARTAGEABLE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef PARTAGEABLE_EXPORTS
#define PARTAGEABLE_API __declspec(dllexport)
#else
#define PARTAGEABLE_API __declspec(dllimport)
#endif

// This class is exported from the partageable.dll
class PARTAGEABLE_API Cpartageable {
public:
	Cpartageable(void);
	// TODO: add your methods here.
};

extern PARTAGEABLE_API int npartageable;

PARTAGEABLE_API int fnpartageable(void);
