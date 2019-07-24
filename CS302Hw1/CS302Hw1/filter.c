#include <stdio.h> 
#include <stdlib.h> 

int main(int argc, char *argv[ ])
{ 
    int column = 3;
	int precision = 13; 
    
   // configuration check
   FILE *fp = fopen("filter.cfg", "r");  
	if(fp == NULL)
	{
    	printf("Configuration file does not exist.\n"); 
    }
   
	else
	{
     fscanf(fp,"%d", &column); 
     fscanf(fp,"%d", &precision);  
	}
    
     //environment variable 
    char *getcolumn = getenv("CS302HW1COLUMNS");
    char *getprecision = getenv("CS302HW1PRECISION");
    
    if(getcolumn != NULL)
    	column = atoi(getcolumn);
     
    if(getprecision != NULL)
    	precision = atoi(getprecision); 
	
    //command line argument 
	if(argc>1)
    {
		precision = atoi(argv[1]);  
        if(argv[2]!= NULL)
		 column = atoi(argv[2]);	
 	}
 	
	float num;       
	int r = scanf("%f", &num); 
	int i; 

	while(r != -1) 
	{  
	   	for(i = 0; i < column; i++)
		{			
			printf("%*.*f""  ", 5+precision, precision, num);
            r = scanf("%f", &num);
	   	    if(r == -1)
	   	        break;
		}
		printf("\n");    
	}
    return 0;
 }
 
 


   
   
