#include <windows.h>
#include <stdio.h>	
#define TOTAL_CPU 8

typedef struct processor_data {
   int affinityMask;                /* affinity mask of this processor (just one bit set) */
   PROCESS_INFORMATION processInfo; /* process currently running on this processor */
   int running;                     /* 1 when this processor is running a task, 0 otherwise */
} ProcessorData;


/* function prototypes */
void printError(char* functionName);


int main(int argc, char *argv[])
{
   ProcessorData *processorPool; /* an array of ProcessorData structures */
   HANDLE *processHandles;       /* an array of handles to processes */

   if (argc < 3)
   {
      fprintf(stderr, "usage, %s  SCHEDULE_TYPE  SECONDS...\n", argv[0]);
      fprintf(stderr, "Where: SCHEDULE_TYPE = 0 means \"first come first serve\"\n");
      fprintf(stderr, "       SCHEDULE_TYPE = 1 means \"shortest job first\"\n");
      fprintf(stderr, "       SCHEDULE_TYPE = 2 means \"longest job first\"\n");
      return 0;
   }


   /* read the job duration times off the command-line */
    int time[argc-2];  
    int i,j, temp; 

	for(i = 0; i < argc-2; i++)
   		time[i] = atoi(argv[i+2]);
		
	for(i = 0; i < argc-2; i++)
		for(j = 0; j < argc-2; j++)
	{
		if((atoi(argv[1]) == 1) && (time[i] < time[j]))
		{
			temp = time[i];
			time[i] =time[j];
			time[j] = temp;
		}
		
		if((atoi(argv[1]) == 2) && (time[i] > time[j]))	
   		{
			temp = time[i];
			time[i] =time[j];
			time[j] =temp;
		}	
	}
		      	
   /* get the processor affinity mask for this process */
    DWORD_PTR processAffinityMask; 
    DWORD_PTR systemAffinityMask;  
  	
	GetProcessAffinityMask(GetCurrentProcess( ), &processAffinityMask, &systemAffinityMask); 

	 int bitMask[TOTAL_CPU];
	 int b = 1;
   	 int trackAffinity[TOTAL_CPU];
   	 int processorCount = 0;
   	 int bin = 1; 
    
	 for(i = 0; i < TOTAL_CPU; i++)
   	{
   		bitMask[i] = (processAffinityMask & b);
   		if(bitMask[i])
			{
				trackAffinity[processorCount]=bitMask[i]* bin; 
				processorCount++;
			} 
		b <<= 1;
		bin += bin*2;
		
	}
     
   /* create, and then initialize, the processor pool data structure */
	processorPool = (ProcessorData*)malloc(processorCount * sizeof(ProcessorData)); 
	
 	for(i = 0; i < processorCount; i++)
	{
		processorPool[i].affinityMask = trackAffinity[i];
 		processorPool[i].processInfo.hProcess = NULL; 
 		processorPool[i].running = 0; 
	}
	
   /* start the first group of processes */
    STARTUPINFO startInfo;
   	ZeroMemory(&startInfo, sizeof(startInfo));
   	startInfo.cb = sizeof(startInfo);
   	
	LPTSTR lpCommandLine[processorCount];
	HANDLE processorHandles[processorCount];
	int processorIndex[processorCount]; 

	int handleCount =0; 
	int x; 
	char command[argc+100]; 

	for(i = 0; i < processorCount; i++)
	{
		x = time[i];
    	sprintf(command, "computeProgram_64.exe %d", x); 
	
		lpCommandLine[i] = command; 

	 	if( !CreateProcess(NULL,lpCommandLine[i], NULL, NULL, TRUE,
	    	               HIGH_PRIORITY_CLASS|CREATE_SUSPENDED,
	        	           NULL, NULL, &startInfo, &(processorPool[i].processInfo)))
	        	           
	   {
	       printError("Error with CreateProcess");
	   }
	   
	   else
	   {
	      fprintf(stderr, "Started process %d\n", (int)processorPool[i].processInfo.dwProcessId);
	      SetProcessAffinityMask(GetCurrentProcess( ),processorPool[i].affinityMask);	
		  ResumeThread(processorPool[i].processInfo.hThread); 
	   }
	}
	 while (1)
   {
      DWORD result;
    /* get, from the processor pool, handles to the currently running processes */
    /* put those handles in an array */  
     int k = 0; 
     for(i = 0; i < processorCount; i++)
	 {
		if(processorPool[i].running==1)
	    {
	    	processorHandles[k] = processorPool[i].processInfo.hProcess;
			processorIndex[k] = i; 
			k++; 
		}
	 }
	 handleCount = k; 
	
	/* use a parallel array to keep track of where in the processor pool each handle came from */
	for(i = 0; i <  handleCount; i++)
	{
		processorHandles[i];
		processorIndex[i];       
	}  

    /* check that there are still processes running, if not, quit */
	if(handleCount==0)
  		return 0; 
	
   	/* wait for one of the processes to end */
   	int q ; 
	result = WaitForMultipleObjects(handleCount, processHandles, FALSE, INFINITE);
	q = result - WAIT_ABANDONED_0 ;
        
	/* translate result from an index in processHandles[] to an index in processorPool[] */
	processorPool[processorIndex[q]];

    /* close the handles of the finished process and update the processorPool array */
    CloseHandle(processorPool[processorIndex[q]].processInfo.hProcess);  
 	processorPool[processorIndex[q]].running = 0; 
	
	/* check if there is another process to run on the processor that just became free */
	int index = processorCount; 
	if(index >= argc-2)
		return 0;
	
	
	if( index <= argc-2) 
	{   
  		x = time[index]; 
  		sprintf(command, "computeProgram_64.exe %d", x); 
  		lpCommandLine[index] = command; 
   		
		if( !CreateProcess(NULL,lpCommandLine[index], NULL, NULL, TRUE,
	    	               HIGH_PRIORITY_CLASS|CREATE_SUSPENDED,
	        	           NULL, NULL, &startInfo, &(processorPool[processorIndex[q]].processInfo)))
	        	           
	   	{
	       	printError("Error with CreateProcess");
	   	}
	   
	   	else
	   	{
	      	fprintf(stderr, "Started process %d\n", (int)processorPool[processorIndex[q]].processInfo.dwProcessId);
	      	SetProcessAffinityMask(GetCurrentProcess( ),processorPool[processorIndex[q]].affinityMask);	
		  	ResumeThread(processorPool[processorIndex[q]].processInfo.hThread); 
		  	index++;
	   	}
     }
	}
	return 0;	
}
void printError(char* functionName)
{
   LPVOID lpMsgBuf;
   int error_no;
   error_no = GetLastError();
   FormatMessage(
         FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
         NULL,
         error_no,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), /* default language */
         (LPTSTR) &lpMsgBuf,
         0,
         NULL
   );
   /* Display the string. */
   fprintf(stderr, "\n%s failed on error %d: ", functionName, error_no);
   fprintf(stderr, (const char*)lpMsgBuf);
   /* Free the buffer. */
   LocalFree( lpMsgBuf );
}	
	
