#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define SHELL 1
#include <vico.h>

void test()
{
	int t;
	vico_args("%d", &t);
	printf("test, args: %s, t = %d\n", vico_buff, t);
	vico_return("%d", t+1);
}

void ls()
{
	vico_print("listing");
}

void rm()
{
	char i[20];
	vico_args("%s", &i);
	printf("Removing %s\n", i);
	vico_exec("ls;");

}

void milis()
{
	vico_func();

	vico_return("%d", rand());
}

int main(void)
{

	//vico_fn_add("A", &test);
	vico_init();
	
	vico_fn_add("l", &ls);
	vico_fn_add("rm", &rm);
	vico_fn_add("test", &test);
	vico_fn_add("m", &milis);

	vico_alias_add("time", "rpc;m;shell;");
	vico_exec("ls;");
	while(1){
		char input[200];	
		gets(input);
		int i;
		for(i = 0; i < strlen(input); i++){
			vico_process(input[i]);
		}
		vico_process('\n');
	}

	return 0;
}
