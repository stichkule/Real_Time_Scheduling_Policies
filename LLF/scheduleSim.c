#include <stdio.h>
#define MASK_BIT(x, bit) ((x) & (0x1) << (bit))
#define SET_BIT(x, bit) ((x) |= ((0x1) << (bit))) 

typedef struct
{
	unsigned int WCET;
	unsigned int period;
	int deadline;
} task_t;

void init_task(task_t *t, unsigned int WCET, unsigned int period, int deadline)
{
	t->WCET = WCET;
	t->period = period;
	t->deadline = deadline;
}

void init_tasks(task_t t[], unsigned int WCET[], unsigned int period[], int deadline[], unsigned int numTasks)
{
	unsigned int i;
	for(i = 0; i < numTasks; i++)
	{
		init_task(&(t[i]),WCET[i],period[i],deadline[i]);
	}
}

void copy_tasks(task_t tasks_to_be_copied[], task_t place_to_copy[], unsigned int numTasks)
{
	unsigned int i;
	for(i = 0; i < numTasks; i++)
	{
		place_to_copy[i].WCET = tasks_to_be_copied[i].WCET;
		place_to_copy[i].period = tasks_to_be_copied[i].period;
		place_to_copy[i].deadline = tasks_to_be_copied[i].deadline;
	}
}

void print_task(task_t t)
{
	printf("exe time: %u period: %u deadline: %u \n", t.WCET, t.period, t.deadline);
}

void print_tasks(task_t t[], unsigned int numTasks)
{
	unsigned int i;
	for (i = 0; i < numTasks; ++i)
	{
		printf("task %u: ", i);
		print_task(t[i]);
	}
	
}

float utilization(task_t t[], unsigned int numTasks)
{
	float util = 0;

	unsigned int i;
	for(i = 0; i < numTasks; i++)
	{
		util += t[i].WCET/t[i].period;
	}

	return util;
}

typedef unsigned int (*sched_algo_t)(task_t[], unsigned int);

unsigned int llf(task_t t[], unsigned int numTasks)
{
	unsigned int least_index = 0;
	int least_laxity = 0xffff; //**********************need to change to max int*******************
	int current_laxity;
	unsigned int i;
	for (i = 0; i < numTasks; i++)
	{
		if(t[i].WCET > 0)
		{
			current_laxity = t[i].deadline - t[i].WCET;
		}
		
		if(current_laxity < least_laxity)
		{
			least_index = i;
			least_laxity = current_laxity;
		}
	}
	return least_index;
}

void sim_schedule(task_t t[], unsigned int numTasks, unsigned int schedule[], unsigned int schedule_length, sched_algo_t algo)
{
	numTasks ++;//add 1 for idle
	task_t placeholder_task[numTasks]; 
	copy_tasks(t, placeholder_task, numTasks - 1);

	//idle
	init_task(&(placeholder_task[numTasks-1]), 99, 9999, 9999);

	//index of current task
	unsigned int current_task;
	unsigned int i;
	for(i = 0; i < schedule_length; i++)
	{
		schedule[i] = 0;

		//determine current task
		current_task = algo(placeholder_task, numTasks);
		
		//"run" current task for 1 timestep
		placeholder_task[current_task].WCET -= 1;
		
		//decrement all time
		unsigned int j;
		for (j = 0; j < numTasks; j++)
		{
			placeholder_task[j].period -= 1;
			placeholder_task[j].deadline -= 1;
		
			//check for missed deadlines
			if(placeholder_task[j].deadline <= 0)
			{
				if(placeholder_task[j].WCET > 0)
				{
					SET_BIT(schedule[i], j + 16); //***********need a define for 16************
				}
			}

			//reset tasks
			if(placeholder_task[j].period <= 0)
			{
				placeholder_task[j].WCET = t[j].WCET;
				placeholder_task[j].period = t[j].period;
				placeholder_task[j].deadline = t[j].deadline;
			}

		}
		
		SET_BIT(schedule[i], current_task);
	}
}

// void printBits(size_t const size, void const * const ptr)
// {
//     unsigned char *b = (unsigned char*) ptr;
//     unsigned char byte;
//     int i, j;

//     for (i=size-1;i>=0;i--)
//     {
//         for (j=7;j>=0;j--)
//         {
//             byte = (b[i] >> j) & 1;
//             printf("%u", byte);
//         }
//     }
//     puts("");
// }

// void print_schedule(unsigned int schedule[], unsigned int length)
// {
// 	unsigned int i;
// 	for (i = 0; i < length; ++i)
// 	{
// 		unsigned int tmp = schedule[i];
// 		printBits(sizeof(tmp),&tmp);
// 		printf("\n");
// 	}
// }

void print_schedule(unsigned int schedule[], unsigned int length)
{
	unsigned int i;
	unsigned int count;
	for (i = 0; i < length; ++i)
	{
		count = 0;
		while(MASK_BIT(schedule[i], count) == 0)
		{
			count++;
		}
		printf("%u ", count);
	}
	printf("\n");
}

void print_missed(unsigned int schedule[], unsigned int length)
{
	unsigned int i;
	unsigned int count;
	for (i = 0; i < length; ++i)
	{
		printf("[ ");
		
		for(count = sizeof(schedule[0]); count > sizeof(schedule[0])/2; count--)
		{
			if(MASK_BIT(schedule[i], count) == 1)
			{
				printf("%u", count);
			}
		}
		printf("] ");
	}
	printf("\n");
}

int main()
{
	#define NUM_TASKS (4)
	#define EXE_LEN (24)

	unsigned int WCET[NUM_TASKS] = {1,2,4,6};
	unsigned int deadline[NUM_TASKS] = {6,8,12,24};
	int period[NUM_TASKS] = {6,8,12,24};

	task_t tasks[NUM_TASKS];
	init_tasks(tasks, WCET, deadline, period, NUM_TASKS);

	print_tasks(tasks, NUM_TASKS);
	printf("task %u: idle\n", NUM_TASKS);
	printf("\nexecution length: %u\n", EXE_LEN);

	unsigned int schedule[EXE_LEN];
	sim_schedule(tasks, NUM_TASKS, schedule, EXE_LEN, llf);

	printf("schedule: ");
	print_schedule(schedule, EXE_LEN);
	printf("missed  : ");
	print_missed(schedule, EXE_LEN);

}