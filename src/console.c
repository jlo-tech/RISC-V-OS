#include <console.h>

static volatile char *base = (void*)0x10000000;

static char map(u8 n) {

  switch(n) {
      case 0:
        return '0';
      case 1:
        return '1';
      case 2:
        return '2';
      case 3:
        return '3';
      case 4:
        return '4';
      case 5:
        return '5';
      case 6:
        return '6';
      case 7:
        return '7';
      case 8:
        return '8';
      case 9:
        return '9';
      default:
        return ' ';
    }
}

void putc_console(char c) {

  while((*(base + 0x05) & 0x40) == 0);
  *base = c;
}

void puts_console(char* s) {

  while(*s) {
    putc_console(*s++);
  }
}

void putu_console(u64 n) {

  if(n == 0) {
    putc_console ('0');
    return;
  }

  u8 i = 0;
  char buf[20]; /* maximal length of an number with 64 Bit */

  /* clear the buffer */
  for(u8 j = 0; j < 20; j++) {
    buf[j] = '\0';
  }

  /* generate string */
  while(n > 0) {
    u8 r = n % 10;
    buf[i] = map(r);
    n = n / 10;
    i++;
  }

  /* print the buffer */
  for(i8 k = 19; k >= 0; k--) {
    if(buf[k] != '\0') {
      putc_console(buf[k]);
    }
  }
}

void putd_console(i64 n) {

  if(n == 0) {
    putc_console ('0');
    return;
  }

  if(n < 0) {
    putc_console('-');
    n = -n;
  }

  u8 i = 0;
  char buf[19]; /* maximal length of an number with 64 Bit */

  /* clear the buffer */
  for(u8 j = 0; j < 19; j++) {
    buf[j] = '\0';
  }

  /* generate string */
  while(n > 0) {
    u8 r = n % 10;
    buf[i] = map(r);
    n = n / 10;
    i++;
  }

  /* print the buffer */
  for(i8 k = 18; k >= 0; k--) {
    if(buf[k] != '\0') {
      putc_console(buf[k]);
    }
  }
}

void putb_console(u64 n) {

  if(n == 0) {
    putc_console ('0');
    return;
  }

  u8 i = 0;
  char buf[64]; /* maximal length of an number with 64 Bit */

  /* clear the buffer */
  for(u8 j = 0; j < 64; j++) {
    buf[j] = '\0';
  }

  /* generate string */
  while(n > 0) {
    u8 r = n % 2;
    buf[i] = map(r);
    n = n >> 1;
    i++;
  }

  /* print the buffer */
  for(i8 k = 63; k >= 0; k--) {
    if(buf[k] != '\0') {
      putc_console(buf[k]);
    }
  }
}

void putf_console(char *s, ...) {

  va_list arg;
  va_start(arg, s);

  char last = '\0';
  while(*s != '\0') {

    if(last != '%' && *s != '%') {
      putc_console (*s);
    }

    if(last == '%') {
      switch(*s) {
        case 'c':
          putc_console(va_arg(arg, int));
          break;
        case 's':
          puts_console(va_arg(arg, char*));
          break;
        case 'u':
          putu_console(va_arg(arg, unsigned long));
          break;
        case 'd':
          putd_console(va_arg(arg, signed long));
          break;
        case 'b':
          putb_console(va_arg(arg, unsigned long));
          break;
        default:
          break;
      }
    }

    last = *s;
    s++;
  }
}

void getc_console(char* c) {

  /* wait for character */
  while( (*(base + 5) & 1) == 0);

  /* read it */
  *c = *(base);

  /* return */
  return;
}

void gets_console(char* s, usize len) {

  char c;
  usize i = 0;
  while (i < len - 1) {

    getc_console(&c);
    putc_console(c);

    *(s + i) = c;

    /* check for carriage return */
    if(c == (char)13) {
      s[i] = '\0';
      putc_console('\n');
      return;
    }

    i++;
  }

  s[len-1] = '\0';

  putc_console('\n');

  return;
}
