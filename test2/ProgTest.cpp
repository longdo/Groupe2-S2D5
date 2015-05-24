// test_composant2.cpp?: d?finit les fonctions export?es pour l'application DLL.
//

#include "stdafx.h"


#include "stdafx.h"
#include <iostream>
#include <typeinfo> //to Use typeid().name()
#include <algorithm>
#include "Composant2.h"

using std::vector;
using std::cout;
using std::endl;
using std::string;
using std::transform;

const int T = 504;
const int NB =  10;

bool testGetPath(string exception, const string expectedExceptions[], vector<double> returnedArray)
{
	bool correctBehaviour = true;
	int i=0;
	if(exception !="")
	{
		//cout<<exception<<" EXCEPTION RECEIVED"<<endl;
	}
	else
	{
		do
		{
			string type =  typeid(returnedArray.at(i)).name();
			type = type.substr(0,6);
			correctBehaviour =  returnedArray.size() >= T  && returnedArray.at(i) >= 0 && type == "double";
			if(!correctBehaviour)
			{
				cout<<type<<endl;
				if (returnedArray.size() < T )
					cout<<"FAILED : MISSING DATA EXCEPTION UNTRIGGERED. Size is : "<<returnedArray.size()<<endl;
				else //if(returnedArray.at(i) <0 )
					cout<<"FAILED : NEGATIVE VALUE EXCEPTION UNTRIGGERED : value in the array is "<< returnedArray.at(0) <<endl;
				
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
	getPathErrors[0] = "WRONG TYPE OF DATA"; // REMOVED FROM SPECIFICATIONS
	getPathErrors[1] = "NEGATIVE VALUE";
	getPathErrors[2] = "MISSED DATA";
	
	Composant2 *composant = new Composant2();
	vector<double>returnedArray;
	int i = -1;
	
	//1 test the getPath Function
	cout<<"GET PATH FUNCTION FUNCTIONNAL TESTS"<<endl<<"***********************************"<<endl<<endl;
	//system("pause");
	try
	{
		returnedArray = composant->getPath(T,100);
	}
	catch (string s)
	{
		transform(s.begin(),s.end(),s.begin(),::toupper); // convert toUpper case a string
		testGetPath(s,getPathErrors,returnedArray);
		exception = s;
	}
	if(exception == "")
		testGetPath("",getPathErrors,returnedArray);
	
	//system("pause");
	//2 .. test the doMonteCarlo Function
	
	cout<<"DO MONTECARLO FUNCTION FUNCTIONNAL TESTS"<<endl<<"***********************************"<<endl<<endl;
	
	//system("pause");
	// jeu de tests:
	maturity[0] = -1;
	maturity[1] = T;
	maturity[2] = T;
	maturity[3] = T;
	maturity[4] = T;
	payOff[0] = "CALL";
	payOff[1] = "PUT";
	payOff[2] = "PUT";
	payOff[3] = "";
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
	//doMcErrors[0] = "WRONG TYPE OF DATA"; //==> removed from specis
	doMcErrors[1] = "NEGATIVE VALUE";
	doMcErrors[2] = "MISSED DATA";// IMPOSSIBLE TO TEST ==> This error has been detected sooner
	i = 0;
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
			correctBehaviour = (s ==doMcErrors[1]); 
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
			correctBehaviour = (s == doMcErrors[2]); 
			if(!correctBehaviour)
				cout<<"MISSING DATA EXCEPTION UNTRIGGERED. RECEIVED : '"<< s<<"'"<<endl;
		}
	}
	
	//system("pause");
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
	
	//system("pause");
	//3..TESTS OF THE PRICE PATH FUNCTION
	cout<<endl<<"PRICE PATH PATH FUNCTION FUNCTIONNAL TESTS"<<endl<<"***********************************"<<endl<<endl;
	
	//system("pause");
	maturity[0] = T;
	maturity[1] = T;
	maturity[2] = -T;
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
	pricePathErrors[0] = "WRONG TYPE OF DATA"; // DOES NOT EXISTS ANYMORE
	pricePathErrors[1] = "NEGATIVE VALUE";
	pricePathErrors[2] = "MISSED DATA";
	pricePathErrors[3] = "VALUE GRATER THAN 1 000 000";
	vector <double>path;
	for (i = 0 ; i < T ; i++)
		path.push_back(0.5);
	i=1;
	do
	{
		exception = "";
		try
		{
			composant->getPricePath(payOff[i],path,strike[i],maturity[i]);
		}
		catch(string s)
		{
			transform(s.begin(),s.end(),s.begin(),toupper);
			correctBehaviour = (s ==pricePathErrors[1]); 
			exception = s;
			if(!correctBehaviour)
				cout<<"FAILED : "<< pricePathErrors[1] <<" EXCEPTION UNTRIGGERED. RECEIVED : '"<<s<<"'"<<endl;
		}
		if (exception == "")
			cout<<"FAILED : " << pricePathErrors[1] <<" EXCEPTION UNTRIGGERED "<<endl;
		i++;
	}
	while (correctBehaviour && i < 3);
	path.pop_back();
	path.pop_back();
	try
	{
		composant->getPricePath(payOff[i],path,strike[i],maturity[i]);
	}
	catch(string s)
	{
		transform(s.begin(),s.end(),s.begin(),toupper);
		correctBehaviour = (s == pricePathErrors[2]);
		if(!correctBehaviour)
			cout<<"FAILED : MISSING DATA EXCEPTION UNTRIGGERERD"<<endl;
	}
	path.push_back(1000001);
	path.push_back(100);
	try
	{
		composant->getPricePath(payOff[i],path,strike[i],maturity[i]);
	}
	catch(string s)
	{
		transform(s.begin(),s.end(),s.begin(),toupper);
		correctBehaviour = (s == pricePathErrors[3]);
		if(!correctBehaviour)
			cout<<"FAILED : VALUE > 1.10^6 EXCEPTION UNTRIGGERERD"<<endl;
	}
	cout<<endl<<endl;
	i = 4;
	try
	{
		composant->getPricePath(payOff[i],path,strike[i],maturity[i]);
	}
	catch(string s)
	{
		if(s !="")
		{
			transform(s.begin(),s.end(),s.begin(),toupper);
			cout<<"FAILED : '"<<s<<"' EXCEPTION TRIGGERED WHEREAS DATA ARE CORRECT"<<endl;
		}
	}
	cout<<"FIN DES TESTS"<<endl;
	system("pause");
	return 0;
}