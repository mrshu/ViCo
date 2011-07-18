#ifndef _VICO_SERIAL
#define _VICO_SERIAL

#include "vico.h"

#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>


static int vico_buffer_len = 0;

uint8_t vico_returned = 0;

char *vico_buffer;
char *vico_buff;
char *vico_func;

void vico_buffer_append(char i)
{
        vico_buffer = (char *)
                realloc(vico_buffer,
                        (vico_buffer_len + 1) * sizeof(char)
                );
        vico_buffer[vico_buffer_len++] = i;
}

inline void vico_buffer_free(void)
{
	if(vico_buffer != NULL){
		free(vico_buffer);
        	vico_buffer = NULL;
	}

        if(vico_func != NULL){
		free(vico_func);
		vico_func = NULL;
	}

        vico_buffer_len = 0;
}

/*
  Processes input stream. 
*/


void vico_process(char input)
{
	if (vico_buffer == NULL)
		vico_buffer = (char *) malloc(sizeof(char));
#ifndef ARDUINO
	if (input == '\n'){
#else
	if (input == 13) {
#endif
		vico_buffer_append(';');
		vico_buffer_append('\0');
#ifdef SHELL
		vico_println(" ");
#endif
		vico_exec(vico_buffer);
		vico_buffer_free();
#ifdef SHELL
		vico_print(VICO_PS1);
#endif
	} else if (input == 3) { /* reseting Arduino */
		void (*rst)() = 0; 
		rst();

	} else if ((input == 8) || (input == 0x7f)) { /* BACKPSACE and DELETE */
		if (vico_buffer_len > 0){
			vico_buffer_len--;

			vico_out((char) 8);
			vico_out(' ');
			vico_out((char) 8);
		}

	} else {
#ifdef SHELL
		vico_out(input);
#endif
		vico_buffer_append(input);
	}

}

void vico_exec(char *input)
{
	char *buffer;
	char *func = NULL;
	buffer = (char *) malloc(sizeof(char));
	int len = 0;
	do {
		if(*input == ';') {
			if (len == 0)
				continue;

			buffer = (char *) realloc(buffer, 
				(len + 1) * sizeof(char));
			buffer[len++] = '\0';


			/* in case of calling a procedure */
			if(func == NULL) {
				char *tmp;

				// gives pointer to aliased string
				if ((tmp = vico_alias(buffer)) != 0){
					vico_exec(tmp);
					// free(buffer);

				} else {
					vico_func = buffer;
					vico_fn_call(vico_func);
				}


			} else {
				vico_func = func;
				vico_buff = buffer;
		#ifdef DEBUG
				vico_print("buf: '");
				vico_print(vico_buff);
				vico_println("'");

				vico_print("func: ");
				vico_println(vico_func);
		#endif
				vico_fn_call(vico_func);

			}
			
			if (vico_returned == 0 && (vico_config & VICO_RPC)){
				vico_out('%');
				vico_print(vico_func);
				vico_out('$');
				
			}

			if (vico_returned == 1)
				vico_returned = 0;
			
			
			len = 0;
			free(func);
			func = vico_func = NULL;

			free(buffer);
			buffer = NULL;

		/* ignoring whitespace */
		} else if (isspace(*input) && len == 0) {
			continue;

		/* separate the function */
		} else if (*input == ' ' && func == NULL) {

			buffer = (char *) realloc(buffer, 
						(len + 1) * sizeof(char));
			buffer[len++] = '\0';

			func = buffer;
			buffer = (char *) malloc(sizeof(char));
			len = 0;

		} else {

			buffer = (char *) realloc(buffer, 
						(len + 1) * sizeof(char));
			buffer[len++] = *input;

		}
	
	} while(*(++input) != '\0');
}

// from bitlash/src/bitlast-serial.c
void vico_print_int_base(int i, uint8_t n)
{
	char buf[8 * sizeof(uint8_t)];           // stack for the digits
        char *ptr = buf;
        if (i == 0) {
                vico_out('0');
                return;
        }
	

        while (i > 0) {
                *ptr++ = i % n;
                i /= n;
        }
        while (--ptr >= buf) vico_out((*ptr < 10) ? (*ptr + '0') : (*ptr - 10 + 'A'));
}

void vico_print_int(int n)
{
	if (n < 0) {
		vico_out('-');
		n = -n;
	}

	vico_print_int_base(n, 10);
}

void vico_print_hex(int n)
{
	vico_print("0x");
	vico_print_int_base(n, 16);
}

#endif
