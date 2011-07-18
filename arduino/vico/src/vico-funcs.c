#ifndef _VICO_FUNC
#define _VICO_FUNC

#include "vico.h"

#include <stdlib.h>
#include <stdint.h>

#define VICO_FUNC_BCKG 0x01

struct vico_fntable{
	char* name;
	void(*fn)();
	uint8_t mask;
} *vico_fntable;

static int vico_fncount = 0;

struct vico_aliases{
	char *name;
	char *alias;
} *vico_aliases;
static int vico_alias_count = 0;

void vico_fn_add_mask(char* name, void(*fn)(), uint8_t mask)
{
        if(vico_fncount == 0) {
                vico_fntable = (struct vico_fntable*)
                        malloc(
                                ++vico_fncount * sizeof(struct vico_fntable)
                        );
        } else {
                vico_fntable = (struct vico_fntable*)
                        realloc(
                                vico_fntable,
                                ++vico_fncount * sizeof(struct vico_fntable)
                        );
        }

  	vico_fntable[vico_fncount - 1].name = name;
  	vico_fntable[vico_fncount - 1].fn = *fn;
        vico_fntable[vico_fncount - 1].mask = mask;

}

void vico_fn_rm(char* name)
{
	int i;
	for (i = 0; i < vico_fncount ; i++) {
		if(strcmp(name, vico_fntable[i].name) == 0){
			free(vico_fntable[i].name);

			vico_fntable[i].name = NULL;
			vico_fntable[i].fn = NULL;

			break;
		}
	}
}

int vico_fn_call(const char* name)
{
        int i;

        for (i = 0; i < vico_fncount ; i++) {
		if(vico_fntable[i].fn == NULL)
			continue;

		if (!strcmp(vico_fntable[i].name , name)) {
                      (*(vico_fntable[i].fn))();
                      return 1;
                }
        }
        return 0;
}

void vico_alias_add(char *name, char *alias)
{
        if(vico_alias_count == 0) {
                vico_aliases = (struct vico_aliases*)
                        malloc(
                                ++vico_alias_count * sizeof(struct vico_aliases)
                        );
        } else {
                vico_aliases = (struct vico_aliases*)
                        realloc(
                                vico_aliases,
                                ++vico_alias_count * sizeof(struct vico_aliases)
                        );
        }
	vico_aliases[vico_alias_count -1].name = name;
	vico_aliases[vico_alias_count -1].alias = alias;

}

char *vico_alias(char *name)
{
	int i;
	for (i = 0; i < vico_alias_count; i++){
#ifdef DEBUG
		vico_print(vico_aliases[i].name);
		vico_print(" -> ");
		vico_println(vico_aliases[i].alias);
#endif
		if(strcmp(vico_aliases[i].name, name) == 0){
			return vico_aliases[i].alias;
		}
	}

	return 0;

}

void vico_func_ls(void)
{
	int i;

	for (i = 0; i < vico_fncount ; i++) {
		if(vico_fntable[i].fn == NULL)
			continue;

		vico_print(vico_fntable[i].name);
		vico_print("\t -> \t");
		vico_print_hex((int) *(vico_fntable[i].fn));
		vico_println("");
	}
}


void vico_func_help(void)
{
	

}



char **args(const char* in, int *argc)
{
	char *tmp;
	tmp = (char *) malloc(sizeof(char));
	int len = 0;
	
	char **argv;
	
	uint8_t in_str = 0, in_sstr = 0;
	
	argv = NULL;
	argv = (char **) malloc(sizeof( *argv ));
	*argc = 0;

	do {
		if ((*in == ' ' || *in == '\0') && (!in_str && !in_sstr)) {
			tmp = (char *) realloc(tmp, (len + 1) * sizeof(char));
			tmp[len++] = '\0';


			argv = (char **) realloc(argv, (*argc + 1) * sizeof( char *));

			argv[*argc] = (char *) malloc((len+1) * sizeof(char));
			argv[*argc] = strdup(tmp);

			free(tmp);
			tmp = (char *) malloc(sizeof(char));
			len = 0;

			*argc += 1;
		
		} else if (*in == '"' && in_sstr == 0 && *(in - 1) != '\\' ) {
			in_str = !in_str;
		} else if (*in == '\'' && in_str == 0 && *(in - 1) != '\\' )  {
			in_sstr = !in_sstr;
		} else {
			/* escaping \" and \' */
			if ((*in == '"' || *in == '\'') && *(in - 1) == '\\') {
				tmp[len - 1] = *in;
			} else {
				tmp = (char *) realloc(tmp, (len + 1) * sizeof(char));
				tmp[len++] = *in;
			}
		}
		

	} while (*in++ != '\0');

	return argv;

}

#endif

