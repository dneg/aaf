#include <console.h> /* Mac command line window */
#include <fstream.h>
#include "DataInput.h"

void getInputData(int *pargc, char **argv, char *inputFile)
{		
	char *newstring = NULL;
	
	/* try to open inputFile for read */
	ifstream ifsCommandLine(inputFile);
		
	/* Test to make sure it is open.  If not it doesn't exist */
	if (ifsCommandLine.is_open()) 
	{
		char inputstr[100];
		int  count = 0;

		/* Determine the number of lines */
		while (	ifsCommandLine >> inputstr )
			++count;
 
		/* *pargc = count + 1 extra for appName */
		*pargc = count + 1;  
		
		/* Create the array of char pointers */
		*argv = new char [*pargc];

		/* Reset file pointer to beginning of file */
		ifsCommandLine.clear();
		ifsCommandLine.seekg(0);

		count = 0;
		/* Allocate for argv[0] assuming inputFile = AppName.inp */
		int inputFileLength = strlen(inputFile);
		newstring = new char [inputFileLength-3];
		newstring[inputFileLength-4] = NULL;
		strncpy(newstring, inputFile, inputFileLength-4);
		argv[0] = newstring;

		/* Get Data */
		while (	ifsCommandLine >> inputstr )	
		{
			newstring = new char [strlen(inputstr)+1];
			strcpy(newstring, inputstr);
			argv[++count] = newstring;
		}
		ifsCommandLine.close();
	}
	else	
	{	
		char **tempargv = NULL;
		int tempargc = 0;

		/* console window for mac */
		tempargc = ccommand(&tempargv);

		*pargc = tempargc;
		
		/* Create the array of char pointers */
		*argv = new char [*pargc];

		/* Fill it in */
		for (int i = 0; i<*pargc; ++i)
		{
			newstring = new char [strlen(tempargv[i])+1];
			strcpy(newstring, tempargv[i]);
			argv[i] = newstring;
		}
	}
}


void cleanUpInputData(int pargc, char **argv)
{
	for ( int i=0; i<pargc; ++i)
		delete [] argv[i];
}
