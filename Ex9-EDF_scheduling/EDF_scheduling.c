/*
Referred to code from https://github.com/salilkapur/process-scheduling-algorithms/blob/master/edf.c
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
Example 9	T1	6	C1	1
			T2	8	C2	2
			T3	12	C3	4
			T4	24	C4	6


*/

#define no_of_tasks (4)

int WCET[no_of_tasks] = {1,2,4,6}; // execution times
int deadline[no_of_tasks] = {6,8,12,24}; // deadlines
int period[no_of_tasks] = {6,8,12,24}; // periods (equal to deadlines)
int period_LCM;
int i,j,h;
float f;

void get_period_LCM() // computes LCM of periods
{
    int lcm[no_of_tasks];
    for(i=0;i<no_of_tasks;i++)
    {
        lcm[i] = period[i]; // initialize lcm array
    }
    h = 0;
    int lowest_value;
    int lowest_value_index;
    while(h==0)
    {
        h=1;
        lowest_value = lcm[0];
        lowest_value_index=0;
        for(i=0;i<no_of_tasks;i++)
        {
            if(lowest_value != lcm[i])
            {
                h=0;
                if(lowest_value>lcm[i]) // if current global LCM greater than LCM of task
                {
                    lowest_value = lcm[i];
                    lowest_value_index = i; // update task index whose period was used
                }
            }
        }
        if(h == 0) // LCM changed; update it
        {
            lcm[lowest_value_index] = lowest_value + period[lowest_value_index];
        }
    }
    period_LCM = lcm[0];
    printf("\nSchedulability will be evaluated over the LCM of periods which is %d\n",period_LCM);
}

float schedulability(){
    float utilization = 0;
    float x,y;
    printf("The CPU utilization is: ");
    for(i=0;i<no_of_tasks;i++) // compute utilization as sum of individual task utilizations (Ci/Ti)
    {
        x = WCET[i];
        y = period[i];
        utilization = utilization + x/y;
        printf("(%d/%d) ",WCET[i],period[i]);
        if(i==no_of_tasks-1) break;
        else printf("+ ");
    }
    printf(" = %f",utilization);
    return utilization;
}

void schedule(){ // computes EDF schedule
    int earliest_deadline;
    int earliest_deadline_index;
    int edf_schedule[period_LCM];
    int cycles_left[no_of_tasks];
    int next_deadline[no_of_tasks];
    int cycles_completed[no_of_tasks];

    for(i=0;i<no_of_tasks;i++)
    {
        next_deadline[i] = deadline[i]; // next deadline of task
        cycles_left[i] = WCET[i]; // time steps remaining for task execution
        cycles_completed[i]=0; // time steps completed
    }
    for(i=0;i<period_LCM;i++)
    {
        printf("(%d,%d) : ",i,i+1); // print current time slot
        earliest_deadline = period_LCM;
        earliest_deadline_index = -1;
        for(j=0;j<no_of_tasks;j++) // check all tasks
        {
            if(cycles_left[j] > 0) // if task has not executed completely
            {
                if(earliest_deadline > next_deadline[j]) // if earliest deadline beyond current task deadline
                {
                    earliest_deadline = next_deadline[j]; // schedule current task
                    earliest_deadline_index = j;

                }
            }
        }
    printf("     [Task executed = %d]\n",earliest_deadline_index); // current task assigned to time slot
    cycles_left[earliest_deadline_index]--; // decrement task's execution cycles left

    for(j=0;j<no_of_tasks;j++)
    {
        if(cycles_completed[j] == (period[j]-1)) // if task is approaching period
        {
            next_deadline[j] = deadline[j]; // set deadline for it
            cycles_left[j] = WCET[j]; // load cycles left for execution with WCET
            cycles_completed[j] = 0; // reset cycles completed
        }
        else
        {
            if(next_deadline[j]>0) // if time remaining until next deadline
            {
                next_deadline[j]--; // decrement it by one time slot
            }
            else
            {
                if(cycles_left[j] > 0) // if deadline arrived && non-zero cycles left to be executed
                {
                    printf("\n the process %d cannot be completed to capacity",j); // cannot complete task
                }
            }
            cycles_completed[j]++; // increment cycles completed
        }
    }
  }
}

int main(void) {
    printf("Task parameters:\n"); // print task parameters
    printf("Task_i | Ci, Di, Ti\n");
    printf("-------------------\n");
    for(i=0;i<no_of_tasks;i++)
    {
        printf("Task_%d | %d,  %d,  %d\n",i,WCET[i],deadline[i],period[i]);
    }
    get_period_LCM();
    printf("\nSchedulability Test:\n"); // commence schedulability test
    f=schedulability();
    if(f<=1)
    {
        printf("\nThe system is schedulable because the CPU utilization %f <= 1",f);
        printf("\n\nEDF Schedule\n");
        printf("\nTime slice      Task executed\n");
        schedule();
    }
    else
    {
        printf("\nThe system is not schedulable because the CPU utilization %f > 1\n",f);
    }
    return 0;
}
