#include <stdlib.h>

#include <stdio.h>

#include <string.h>

#include "task.h"

#include "list.h"

#include "schedulers.h"

#include "cpu.h"

struct node *list = NULL; //list for storing tasks

void add(char * name, int priority, int burst) { //function to copy arguments into structure and apending task in list
	Task *task = malloc(sizeof(Task));
	strcpy(task->name,name);
	task->burst = burst;
	task->priority = priority;
	insert(&list,task); //appending task in list
}

// invoke the scheduler
void schedule() {
	while(list){
		Task* task_to_run = list->task;
		struct node *present = list; //head node will be present task
		while(present){ //iteratig through whole list
			if(present->task->burst <= task_to_run->burst) //check for small burst time
				task_to_run = present->task; // exchanging tasks
			present = present->next; //moving to next node
		}
		run(task_to_run,task->burst); //running task with smallest burst
		delete(&list,task); //deleting that task form task list
	}
}
