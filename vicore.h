#ifndef _VICORE_H
#define _VICORE_H 1

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
void vicore_print(char *x);
void vicore_out(char x);
void vicore_println(char x);

void vicore_inout_init(unsigned long baud);
int vicore_available();
char vicore_in();
*/
#define vicore_print(x) Serial.print(x)
#define vicore_out(x) Serial.print(x)
#define vicore_println(x) Serial.println(x)

#define vicore_inout_init(x) Serial.begin(x)
#define vicore_available Serial.available
#define vicore_in Serial.read


#else

#define vicore_print(x) printf("%s", x)
#define vicore_out(x) printf("%c", x)
#define vicore_println(x) printf("%s\n", x)

#define vicore_inout_init(baud);  
#define vicore_available() 1
#define vicore_in getchar

#include <time.h>
#define millis() time(0)

#endif

#define VICORE_MAX_CHARS 80
#define vicore_args(fmt, ...) sscanf(vicore_buff, fmt, __VA_ARGS__); \
char *_vifunc_; _vifunc_ = vicore_func;

#define vicore_func(); char *_vifunc_; _vifunc_ = vicore_func;


#define vicore_return(fmt, ...) \
{char *tmp; \
tmp = (char *) malloc(sizeof(char) * VICORE_MAX_CHARS); \
if (vicore_config & VICORE_RPC) { \
sprintf(tmp, "%%%s " fmt "$", _vifunc_ , __VA_ARGS__);} else { \
sprintf(tmp, fmt, __VA_ARGS__); } \
vicore_println(tmp); \
free(tmp);vicore_returned = 1;}



void vicore_fn_add_mask(char* name, void(*fn)(), uint8_t mask);
#define vicore_fn_add(name, fn)  vicore_fn_add_mask(name, fn, 0)
int vicore_fn_call(const char* name);

char* vicore_alias(char *name);
void vicore_alias_add(char *name, char *alias);

/* vicore-serial.c */
extern char *vicore_buffer;
extern char *vicore_buff;
extern char *vicore_func;
extern uint8_t vicore_returned; 

void vicore_buffer_append(char i);

void vicore_process(char input);
void vicore_exec(char *input);


void vicore_print_int(int i);
void vicore_print_int_base(int i, uint8_t n);
void vicore_print_hex(int i);


void vicore_func_ls(void);
#ifdef ARDUINO
void vicore_init(unsigned long baud);
#else
void vicore_init();
#endif

void vicore_run(void);

#define VICORE_RPC 0x8

#define VICORE_PS1 "vi> "

extern uint8_t vicore_config;

void vicore_task_stop(uint8_t id);
void vicore_task_start(char* name, unsigned int delay);
void vicore_tasks_run(void);
void vicore_func_ps(void);

void vicore_var_set(char* name, char* val);
char* vicore_var_get(char* name);


#endif
