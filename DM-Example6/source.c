/*
Code is minor modification of original source code from Sam Siewert
*/


#include <math.h>
#include <stdio.h>

#define TRUE 1
#define FALSE 0
#define U32_T unsigned int

//ex 6
U32_T ex6_period[] = {2, 5,7, 13};
U32_T ex6_wcet[] = {1, 1,1,2};
U32_T ex6_deadline[]={2,3,7,15};


int completion_time_feasibility(U32_T numServices, U32_T period[], U32_T wcet[], U32_T deadline[]);
int scheduling_point_feasibility(U32_T numServices, U32_T period[], U32_T wcet[], U32_T deadline[]);


int main(void)
{ 
    int i;
	U32_T numServices;
    
    printf("******** Completion Test Feasibility Example\n");
   
   printf("Ex-6 U=%4.2f (C1=1, C2=1, C3=1, C4=2; T1=2, T2=5, T3=7, T4=13; D1=2, D2=3, D3=7, D4=15): ",
		   ((1.0/2.0) + (1.0/5.0) + (1.0/7.0)+ (2.0/13.0)));
	numServices = sizeof(ex6_period)/sizeof(U32_T);
    if(completion_time_feasibility(numServices, ex6_period, ex6_wcet, ex6_deadline) == TRUE)
        printf("FEASIBLE\n");
    else
        printf("INFEASIBLE\n");

	printf("\n\n");
    printf("******** Scheduling Point Feasibility Example\n");

   printf("Ex-6 U=%4.2f (C1=1, C2=1, C3=1, C4=2; T1=2, T2=5, T3=7, T4=13; D1=2, D2=3, D3=7, D4=15): ",
		   ((1.0/2.0) + (1.0/5.0) + (1.0/7.0)+ (2.0/13.0)));
	numServices = sizeof(ex6_period)/sizeof(U32_T);
    if(scheduling_point_feasibility(numServices, ex6_period, ex6_wcet, ex6_deadline) == TRUE)
        printf("FEASIBLE\n");
    else
        printf("INFEASIBLE\n");


}

///////////////////////////////// End of main

int completion_time_feasibility(U32_T numServices, U32_T period[], U32_T wcet[], U32_T deadline[])
{
  int i, j;
  U32_T an, anext;
  
  // assume feasible until we find otherwise
  int set_feasible=TRUE;
   
  //printf("numServices=%d\n", numServices);
  
  for (i=0; i < numServices; i++)
  {
       an=0; anext=0;
       
       for (j=0; j <= i; j++)
       {
           an+=wcet[j];
       }
       
	   //printf("i=%d, an=%d\n", i, an);

       while(1)
       {
             anext=wcet[i];
	     
             for (j=0; j < i; j++)
                 anext += ceil(((double)an)/((double)period[j]))*wcet[j];
		 
             if (anext == an)
                break;
             else
                an=anext;

//			printf("an=%d, anext=%d\n", an, anext);
       }
       
//	   printf("an=%d, deadline[%d]=%d\n", an, i, deadline[i]);

       if (an > deadline[i])
       {
          set_feasible=FALSE;
       }
  }
  
  return set_feasible;
}


int scheduling_point_feasibility(U32_T numServices, U32_T period[], 
								 U32_T wcet[], U32_T deadline[])
{
   int rc = TRUE, i, j, k, l, status, temp;

   for (i=0; i < numServices; i++) // iterate from highest to lowest priority
   {
      status=0;

      for (k=0; k<=i; k++) 
      {
          for (l=1; l <= (floor((double)period[i]/(double)period[k])); l++)
          {
               temp=0;

               for (j=0; j<=i; j++) temp += wcet[j] * ceil((double)l*(double)period[k]/(double)period[j]);

               if (temp <= (l*period[k]))
			   {
				   status=1;
				   break;
			   }
           }
           if (status) break;
      }
      if (!status) rc=FALSE;
   }
   return rc;
}
