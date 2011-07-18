#ifndef _VICO_VAR
#define _VICO_VAR

#include "vico.h"

#include <stdlib.h>
#include <stdint.h>

#define VICO_VAR_NONE 255

struct vico_vars{
	char* name;
	char* val;
} *vico_vars;

static uint8_t vico_varcount= 0;

void vico_var_set_new(char* name, char* val)
{
	if (vico_varcount == 0){
		vico_vars = (struct vico_vars*)
			malloc( 
				++vico_varcount * sizeof(struct vico_vars)
			);
	} else {
		vico_vars = (struct vico_vars*)
			realloc( 
				vico_vars,
				++vico_varcount * sizeof(struct vico_vars)
			);
	}

//	printf("task: %s\n", name);

	vico_vars[vico_varcount - 1].name = strdup(name);
	vico_vars[vico_varcount - 1].val = strdup(val);



}

uint8_t vico_var_get_id (char* name)
{
	int i;
	for (i = 0; i < vico_varcount; i++) {
		if (strcmp(name, vico_vars[i].name) == 0) {
			return i;
		}
	}
	return VICO_VAR_NONE;
}


void vico_var_set(char* name, char* val)
{
	
	uint8_t id = vico_var_get_id(name);
	if (id == VICO_VAR_NONE){
		vico_var_set_new(name, val);
	} else {
		free(vico_vars[id].val);
		vico_vars[id].val = strdup(val);
	}
	
}

char* vico_var_get(char* name)
{
	int i;
	for (i = 0; i < vico_varcount; i++) {
		if (strcmp(name, vico_vars[i].name) == 0) {
			return vico_vars[i].val;
		}
	}

	return 0;

}


#endif

