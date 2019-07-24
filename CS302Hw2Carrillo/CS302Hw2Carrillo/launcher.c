/*
   Course: CS30200
   Name: Grace Carrillo	
   Email: gcarril@pnw.edu
   Assignment: 2
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// function prototype
void printError(char* functionName);

int main( )
{
	#define NUMBER_OF_PROCESSES 5
	LPTSTR lpCommandLine[NUMBER_OF_PROCESSES];
	PROCESS_INFORMATION pi[NUMBER_OF_PROCESSES];
	
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

   //set environment variables
    char* wPath = getenv("windir");
  	
	char notePath [100];
   	char wordPath [150];
   	char cmdPath [100];
   	char calcPath [120];
   	char expPath [120];
   	
    sprintf(notePath,"%s\\notepad.exe",wPath);  
    sprintf(wordPath,"%s\\Windows NT\\Accessories\\wordpad.exe",getenv("ProgramFiles")); 
   	sprintf(cmdPath,"%s /T:F9",getenv("ComSpec"));
   	sprintf(calcPath,"%s\\System32\\calc.exe",wPath);  
	sprintf(expPath,"%s\\explorer.exe",wPath);

   	//commandline paths
	lpCommandLine[0] = notePath; 
	lpCommandLine[1] = wordPath;
	lpCommandLine[2] = cmdPath;
	lpCommandLine[3] = calcPath;
    lpCommandLine[4] = expPath;
    
    //new cmd setting
    putenv("prompt=Speak to me$g");
	si.lpTitle = "What is your command?"; 
	  
	#define TRUE 1
	while(TRUE)
	{
		int choice;
		printf("Please make a choice from the following list\n"); 
		printf("  0:Quit\n"); 
		printf("  1:Run Notepad\n");
		printf("  2:Run Wordpad\n");
		printf(" *3:cmd shell\n");
		printf("  4:Run Calculator\n");
		printf("  5:Run Explorer\n");
		printf("Enter your choice now: ");
		scanf("%d", &choice); 
		
		//exit 
		if(choice == 0)
			return 0; 
		
		//create error
		if( !CreateProcess(NULL, lpCommandLine[choice-1], NULL, NULL, FALSE,
                         HIGH_PRIORITY_CLASS | CREATE_NEW_CONSOLE,
                         NULL, NULL, &si, &pi[choice-1]))
      		{
      	 		printError("CreateProcess");
			}		
		//create cmd process 		
		else 	
		{
			if(choice == 3)
			{
				SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS); 
				printf("Started program %d with pid = %d\n", choice, (int)pi[choice-1].dwProcessId);
				
		   		WaitForSingleObject(pi[choice-1].hProcess, INFINITE); 
		    	printf("  waiting for program %d to terminate...\n", choice); 
		    	
		    	DWORD exitValue; 
		    	GetExitCodeProcess(pi[choice-1].hProcess,&exitValue);
		    	printf("  program %d exited with return value %d\n\n", choice, exitValue); 	
	      	}		
			
			//create other processes 
			else 
			{
	      		printf("Started program %d with pid = %d\n\n", choice, (int)pi[choice-1].dwProcessId);
	 		}
     	} 	
    	CloseHandle(pi[choice-1].hThread);
    	CloseHandle(pi[choice-1].hProcess);
    }
}

//printError		
void printError(char* functionName)
{
	LPVOID lpMsgBuf;
	int error_no;
	error_no = GetLastError();   
	FormatMessage(
	     FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
	     NULL,
	     error_no,
	     MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
	    (LPTSTR) &lpMsgBuf,
	    0,
	    NULL
	   );   
	   // Display the string.
	fprintf(stderr, "\n%s failed on error %d: ", functionName, error_no);
	fprintf(stderr, (char*)lpMsgBuf);
	   
	// Free the buffer.
	LocalFree( lpMsgBuf );	  	   
	//ExitProcess(1);  // terminate the program
}		
			
		

