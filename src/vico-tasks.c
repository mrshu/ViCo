#ifndef _VICO_TASKS
#define _VICO_TASKS

#include "vico.h"

#include <stdlib.h>
#include <stdint.h>


struct vico_tasks{
	char* name;
	unsigned long lastrun;
	unsigned int delay;
} *vico_tasks;

static int vico_taskcount = 0;

void vico_task_start(char* name, unsigned int delay)
{
	if (vico_taskcount == 0){
		vico_tasks = (struct vico_tasks*)
			malloc( 
				++vico_taskcount * sizeof(struct vico_tasks)
			);
	} else {
		vico_tasks = (struct vico_tasks*)
			realloc( 
				vico_tasks,
				++vico_taskcount * sizeof(struct vico_tasks)
			);
	}

//	printf("task: %s\n", name);

	vico_tasks[vico_taskcount - 1].name = name;
	vico_tasks[vico_taskcount - 1].delay = delay;
	vico_tasks[vico_taskcount - 1].lastrun = 0;

}

void vico_task_stop(uint8_t id)
{
	if (id >= vico_taskcount)
		return;

	if (vico_tasks[id].name == NULL)
		return;

		
	free(vico_tasks[id].name);

	vico_tasks[id].name = NULL;
	vico_tasks[id].delay = 0;
	vico_tasks[id].lastrun = 0;
		
			

}

void vico_tasks_run(void)
{
	int i;
	for (i = 0; i < vico_taskcount; i++) {
		if (vico_tasks[i].name == NULL)
			continue;

		if ((vico_tasks[i].delay + vico_tasks[i].lastrun )<=
			millis()){

//			printf("rtask: %s\n", vico_tasks[i].name);
			char *tmp;
			tmp = (char *) malloc(
				(strlen(vico_tasks[i].name) + 2) * sizeof(char) 
				);
			
			*tmp = '\0';
			strcat(tmp, vico_tasks[i].name);
			strcat(tmp, ";\0");

//			printf("running: %s\n", tmp);

			vico_exec(tmp);
			vico_tasks[i].lastrun = millis();

			free(tmp);
		}
	}

}


void vico_func_ps(void)
{
	int i;
	for (i = 0; i < vico_taskcount; i++) {
		if (vico_tasks[i].name == NULL)
			continue;
		
		vico_print_int(i);
		vico_print("\t:\t");
		vico_println(vico_tasks[i].name);
	}

}

#endif

