
// header guard
#ifndef __MPRINTF_H
#define __MPRINTF_H


#include <stdarg.h>
#include <stdint.h>

int32_t printf_(const char * restrict format_str, ...);
int32_t sprintf_(char * restrict out_str, const char * restrict format_str, ...);
int32_t snprintf_(char * restrict out_str, uint32_t buf_len, const char * restrict format_str, ...);
int32_t vsnprintf_(char * restrict out_str, uint32_t buf_len, const char * restrict format_str, va_list arg);
char * strncpy_(char * restrict dest_str, const char * restrict src_str, uint32_t len);
uint32_t strlen_(const char * restrict str);

#endif // __MPRINTF_H