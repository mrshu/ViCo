#include "vico.h"

uint8_t vico_config = 0;

void vico_func_rpc()
{
	vico_config |= VICO_RPC;
}

void vico_func_shell()
{
	vico_config ^= VICO_RPC;
}



#ifdef ARDUINO

// from http://forum.pololu.com/viewtopic.php?f=10&t=989#p4218
void vico_func_free()
{
	vico_func();
	
	extern int __bss_end;
	int ret;
	vico_return("%d",  ((int)&ret) - ((int)&__bss_end));
			
}

void vico_func_millis()
{
	vico_func();
	vico_return("%ld", millis());
}


void vico_func_pinmode()
{
	uint8_t pin, mode;
	vico_args("%d %d", &pin, &mode);
	pinMode(pin, mode);
}

void vico_func_dr()
{
	uint8_t pin;
	vico_args("%d", &pin);
	vico_return("%d", digitalRead(pin));

}

void vico_func_dw()
{
	int pin, val;
	vico_args("%d %d", &pin, &val);
}

void vico_func_ar()
{
	uint8_t pin;
	vico_args("%d", &pin);
	vico_return("%d", analogRead(pin));
}

void vico_func_aw()
{
	uint8_t pin;
	int val;
	vico_args("%d %d", &pin, &val);
}

#endif
void vico_func_start()
{
	char *tmp;
	tmp = NULL;
	tmp = (char *) malloc(31 * sizeof(char));
	int time;
	vico_args("%30s %d", tmp, &time);
	
	vico_task_start(tmp, time);
}

void vico_func_stop()
{
	uint8_t id;
	vico_args("%d", &id);

	vico_task_stop(id);
}


void vico_func_set()
{
	char* name;
	name = (char *) malloc(31 * sizeof(char));
	char* val;
	val = (char *) malloc(31 * sizeof(char));

	vico_args("%30s %30s", name, val);
	
	vico_var_set(name, val);
	free(name);
	free(val);

}

void vico_func_p()
{
	char* name;
	name = (char *) malloc(31 * sizeof(char));
	vico_args("%30s", name);
	
	vico_return("%s", vico_var_get(name));
}

/*
void vico_func_echo()
{
	vico_func();
	
	uint8_t slash = 0;
	while(*vico_buff != '\0'){
		if
	}
	vico_println(vico_buff);
}
*/

void vico_run(void)
{
	if (vico_available()){
		vico_process(vico_in());
	}
	
	vico_tasks_run();

}
#ifdef ARDUINO
void vico_init(unsigned long baud)
{
	vico_inout_init(baud);
#else
void vico_init()
{
#endif
	vico_fn_add("ls", &vico_func_ls);	
	vico_fn_add("rpc", &vico_func_rpc);	
	vico_fn_add("shell", &vico_func_shell);	

#ifdef SHELL
	vico_println("beign");
	vico_print(VICO_PS1);
#endif


#ifdef ARDUINO

	vico_fn_add("free", &vico_func_free);	
	vico_fn_add("millis", &vico_func_millis);	


	vico_fn_add("pm", &vico_func_pinmode);
	vico_fn_add("dr", &vico_func_dr);
	vico_fn_add("dw", &vico_func_dw);
	vico_fn_add("ar", &vico_func_ar);
	vico_fn_add("aw", &vico_func_aw);
#endif
/*
	vico_fn_add("start", &vico_func_start);
	vico_fn_add("stop", &vico_func_stop);
	vico_fn_add("ps", &vico_func_ps);

	vico_fn_add("=", &vico_func_set);
	vico_fn_add("p", &vico_func_p);

*/
}
