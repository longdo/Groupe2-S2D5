// test_composant2.cpp : définit les fonctions exportées pour l'application DLL.
//

#include "stdafx.h"


#include "stdafx.h"
#include <iostream>
#include <typeinfo> //to Use typeid().name()
#include <algorithm>
#include "Composant2.h"
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::transform;

const int T = 504;
const int NB =  10;

bool testGetPath(string exception, const string expectedExceptions[], const double* returnedArray)
{
	bool correctBehaviour = true;
	int i=0;
	if(exception !="")
	{
		if ( exception == expectedExceptions[0] )
		{
			bool wrongType = false;
			do
			{	
				wrongType = ( typeid(returnedArray[++i]).name() != "double"); 
			}
			while (i < T && !wrongType) ;
			correctBehaviour = wrongType;
			if(!correctBehaviour)
				cout<<"FAILED : WRONG TYPE EXCEPTION TRIGGERED WHEREAS IT SHOULD NOT"<<endl;
		}
		else if (expectedExceptions[1] == exception)
		{
			
			bool negativeValue = false;
			do
			{	
				negativeValue = ( returnedArray[++i] < 0 );
			}
			while (i < T && !negativeValue) ;
			correctBehaviour = negativeValue;
			if(!correctBehaviour)
				cout<<"FAILED : NEGATIVE VALUE EXCEPTION TRIGGERED WHEREAS IT SHOULD NOT"<<endl;
		}

		else if(expectedExceptions[2] == exception )
		{
			bool missedValue = false;
			i= 0;
			do
			{
				missedValue = returnedArray[i] == NULL || returnedArray[i] ==0;
				i++;
			}
			while(i < T && !missedValue);
			correctBehaviour = missedValue;
			if(correctBehaviour)
				cout<<"GET PATH TESTS OK"<<endl<<endl<<endl;
			else
				cout<<"FAILED : MISSING VALUES TRIGGERED WHEREAS IT SHOULD NOT"<<endl;
		}
		else
			cout<<"FAILED : UNKNOWN EXCEPTION "<<endl;
	}
	else
	{
		do
		{
			correctBehaviour = returnedArray[i] != NULL && returnedArray[i] > 0 && typeid(returnedArray[++i]).name() == "double";
			if(!correctBehaviour)
			{
				if (returnedArray[i] ==NULL)
					cout<<"FAILED : MISSING DATA EXCEPTION UNTRIGGERED"<<endl;
				else if(returnedArray[i] <0 )
					cout<<"FAILED : NEGATIVE VALUE EXCEPTION UNTRIGGERED"<<endl;
				else 
					cout<<"FAILED : WRONG TYPE OF DATA EXCEPTION UNTRINGGERED"<<endl;
			}
			i++;
		}
		while(correctBehaviour && i < T);
	}
	return correctBehaviour;
}

int _tmain(int argc, char* argv[])
{
	/*
	std::vector<double> test;
	test = getChemin(100,504);
	for (int i = 0 ; i < 504 ; i++)
		cout<<test.at(i)<<endl;
	*/
	bool correctBehaviour=true;
	string doMcErrors[4],getPathErrors[3],pricePathErrors[4],esperancePayOffErrors[3];
	int maturity[5], nbIterations[5];
	double strike[5];
	string payOff[5], exception="";
	getPathErrors[0] = "WRONG TYPE OF DATA",
	getPathErrors[1] = "NEGATIVE VALUE";
	getPathErrors[2] = "MISSED DATA";
	
	
	
	Composant2 *composant = new Composant2();
	double *returnedArray=NULL;
	int i = -1;

	//1 test the getPath Function
	cout<<"GET PATH FUNCTION FUNCTIONNAL TESTS"<<endl<<"***********************************"<<endl<<endl;
	try
	{
		returnedArray = composant->getPath();
	}
	catch (string s)
	{
		transform(s.begin(),s.end(),s.begin(),::toupper); // convert toUpper case a string
		testGetPath(s,getPathErrors,returnedArray);
		exception = s;
	}
	if(exception == "")
		testGetPath("",getPathErrors,returnedArray);
	
	//2 .. test the doMonteCarlo Function
	
	cout<<"GET PATH FUNCTION FUNCTIONNAL TESTS"<<endl<<"***********************************"<<endl<<endl;

	// jeu de tests:
	maturity[0] = -1;
	maturity[1] = T;
	maturity[2] = T;
	maturity[3] = T;
	maturity[4] = T;
	payOff[0] = "CALL";
	payOff[1] = "PUT";
	payOff[2] = "PUT";
	payOff[3] = "404 NOT FOUND";
	payOff[4] = "CALL";
	nbIterations[0]= 3;
	nbIterations[1]= 2;
	nbIterations[2]= -3;
	nbIterations[3]= 1;
	nbIterations[4]= 1;
	strike[0] = 15;
	strike[1] = -5;
	strike[2] = 15;
	strike[3] = 15;
	strike[4] = 15;
	doMcErrors[0] = "WRONG TYPE OF DATA";
	doMcErrors[1] = "NEGATIVE VALUE";
	doMcErrors[2] = "MISSED DATA";// IMPOSSIBLE TO TEST ==> This error has been detected sooner
	i = 1;
	do
	{
		// 1 test negative value exceptions for maturity, number of trials, strike
		try
		{
			composant->doMonteCarlo(payOff[i],maturity[i],strike[i],nbIterations[i]);
		}
		catch(string s)
		{
			transform(s.begin(),s.end(),s.begin(),::toupper); // convert toUpper case a string
			correctBehaviour = (s ==doMcErrors[2]); 
			if (!correctBehaviour)
				cout<<"FAILED : NEGATIVE VALUE EXCEPTION EXPECTED. RECEIVED : '" <<s<<"'"<<endl;
		}
		i++;
	}
	while (correctBehaviour && i < 3);
	// 2 test wrong data exceptions
	if(correctBehaviour)
	{
		payOff[0] = "15"; 
		maturity[0] = 15;
		try
		{
			composant->doMonteCarlo(payOff[i],maturity[i],strike[i],nbIterations[i]);
		}
		catch(string s)
		{
			transform(s.begin(),s.end(),s.begin(),::toupper); // convert toUpper case a string
			correctBehaviour = (s == doMcErrors[1]); 
			if(!correctBehaviour)
				cout<<"FAILED : NEGATIVE VALUE EXCEPTION EXPECTED. RECEIVED : '"<< s<<"'"<<endl;
		}
	}
	
	try
	{
		composant->doMonteCarlo(payOff[4],maturity[4],strike[4],nbIterations[4]);
	}
	catch(string s)
	{
		if (s!="")
		{
			transform(s.begin(),s.end(),s.begin(),toupper);
			cout<<"FAILED : '"<<s<<"' EXCEPTION TRIGGERED WHEREAS DATA ARE CORRECT"<<endl;
		}
	}

	//3..TESTS OF THE PRICE PATH FUNCTION
	cout<<endl<<"PRICE PATH PATH FUNCTION FUNCTIONNAL TESTS"<<endl<<"***********************************"<<endl<<endl;
	maturity[0] = T;
	maturity[1] = T;
	maturity[2] = T;
	maturity[3] = T;
	maturity[4] = T;
	payOff[0] = "-1";
	payOff[1] = "PUT";
	payOff[2] = "PUT";
	payOff[3] = "CALL";
	payOff[4] = "CALL";
	strike[0] = 15;
	strike[1] = -5;
	strike[2] = 15;
	strike[3] = 15;
	strike[4] = 15;
	pricePathErrors[0] = "WRONG TYPE OF DATA",
	pricePathErrors[1] = "NEGATIVE VALUE";
	pricePathErrors[2] = "MISSED DATA";
	pricePathErrors[3] = "VALUE GRATER THAN 1 000 000";
	double *path = new double[T];
	for (i = 0 ; i < T ; i++)
			path[i] = 0.5;
	
	do
	{
		i=0;
		try
		{
			composant->pricePath(payOff[i],path,strike[i],maturity[i]);
		}
		catch(string s)
		{
			transform(s.begin(),s.end(),s.begin(),toupper);
			correctBehaviour = (s ==pricePathErrors[i]); 
			if(!correctBehaviour)
				cout<<"FAILED : "<< pricePathErrors[i] <<"EXCEPTION UNTRIGGERED. RECEIVED : '"<<s<<"'"<<endl;
		}
		i++;
	}
	while (correctBehaviour && i < 2);
	path[T/2] = NULL;
	path[T+50]=NULL;
	try
	{
		composant->pricePath(payOff[i],path,strike[i],maturity[i]);
	}
	catch(string s)
	{
		transform(s.begin(),s.end(),s.begin(),toupper);
		correctBehaviour = (s == pricePathErrors[i]);
		if(!correctBehaviour)
			cout<<"FAILED : MISSING DATA EXCEPTION UNTRIGGERERD"<<endl;
	}
	i = 3;
	path[T-2] = 1000001;
	path[T+50] = 100;
	try
	{
		composant->pricePath(payOff[i],path,strike[i],maturity[i]);
	}
	catch(string s)
	{
		transform(s.begin(),s.end(),s.begin(),toupper);
		correctBehaviour = (s == pricePathErrors[i]);
		if(!correctBehaviour)
			cout<<"FAILED : VALUE > 1.10^6 EXCEPTION UNTRIGGERERD"<<endl;
	}
	cout<<endl<<endl;
	i = 4;
	try
	{
		composant->pricePath(payOff[i],path,strike[i],maturity[i]);
	}
	catch(string s)
	{
		if(s !="")
		{
			transform(s.begin(),s.end(),s.begin(),toupper);
			cout<<"FAILED : '"<<s<<"' EXCEPTION TRIGGERED WHEREAS DATA ARE CORRECT"<<endl;
		}
	}
	return 0;
}