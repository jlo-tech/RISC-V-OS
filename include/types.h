#ifndef TYPES_H
#define TYPES_H

#define NULL (void*)0

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

typedef float f32;
typedef double f64;

/* register type */
typedef unsigned long reg_t;

/* usize type */
typedef unsigned long usize;

/* boolean type */
typedef unsigned int bool;

#define false 0
#define true 1

#endif
