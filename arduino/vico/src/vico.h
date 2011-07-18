#ifndef _VICO_H
#define _VICO_H 1

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>


#if defined(HIGH) || defined(ARDUINO)
#define ARDUINO 1
#define SHELL 1

#include <avr/pgmspace.h>
#include "WProgram.h"
#include "WConstants.h"
/*
void vico_print(char *x);
void vico_out(char x);
void vico_println(char x);

void vico_inout_init(unsigned long baud);
int vico_available();
char vico_in();
*/
#define vico_print(x) Serial.print(x)
#define vico_out(x) Serial.print(x)
#define vico_println(x) Serial.println(x)

#define vico_inout_init(x) Serial.begin(x)
#define vico_available Serial.available
#define vico_in Serial.read


#else

#define vico_print(x) printf("%s", x)
#define vico_out(x) printf("%c", x)
#define vico_println(x) printf("%s\n", x)

#define vico_inout_init(baud);  
#define vico_available() 1
#define vico_in getchar

#include <time.h>
#define millis() time(0)

#endif

#define VICO_MAX_CHARS 80
#define vico_args(fmt, ...) sscanf(vico_buff, fmt, __VA_ARGS__); \
char *_vifunc_; _vifunc_ = vico_func;

#define vico_func(); char *_vifunc_; _vifunc_ = vico_func;


#define vico_return(fmt, ...) \
{char *tmp; \
tmp = (char *) malloc(sizeof(char) * VICO_MAX_CHARS); \
if (vico_config & VICO_RPC) { \
sprintf(tmp, "%%%s " fmt "$", _vifunc_ , __VA_ARGS__);} else { \
sprintf(tmp, fmt, __VA_ARGS__); } \
vico_println(tmp); \
free(tmp);vico_returned = 1;}



void vico_fn_add_mask(char* name, void(*fn)(), uint8_t mask);
#define vico_fn_add(name, fn)  vico_fn_add_mask(name, fn, 0)
int vico_fn_call(const char* name);

char* vico_alias(char *name);
void vico_alias_add(char *name, char *alias);

/* vico-serial.c */
extern char *vico_buffer;
extern char *vico_buff;
extern char *vico_func;
extern uint8_t vico_returned; 

void vico_buffer_append(char i);

void vico_process(char input);
void vico_exec(char *input);


void vico_print_int(int i);
void vico_print_int_base(int i, uint8_t n);
void vico_print_hex(int i);


void vico_func_ls(void);
#ifdef ARDUINO
void vico_init(unsigned long baud);
#else
void vico_init();
#endif

void vico_run(void);

#define VICO_RPC 0x8

#define VICO_PS1 "vi> "

extern uint8_t vico_config;

void vico_task_stop(uint8_t id);
void vico_task_start(char* name, unsigned int delay);
void vico_tasks_run(void);
void vico_func_ps(void);

void vico_var_set(char* name, char* val);
char* vico_var_get(char* name);


#endif
