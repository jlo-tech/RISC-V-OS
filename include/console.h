#ifndef CONSOLE_H
#define CONSOLE_H

#include <types.h>

#include <stdarg.h>

void putc_console(char c);
void puts_console(char* s);
void putu_console(u64 n);
void putd_console(i64 n);
void putb_console(u64 n);

void putf_console(char *s, ...);

void getc_console(char* c);
void gets_console(char* s, usize len);

#endif
