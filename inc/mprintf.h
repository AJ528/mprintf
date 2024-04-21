
#include <stdarg.h>
#include <stdint.h>

uint32_t sprintf_(char * restrict out_str, const char * restrict format_str, ...);
uint32_t snprintf_(char * restrict out_str, uint32_t buf_len, const char * restrict format_str, ...);
uint32_t vsnprintf_(char * restrict out_str, uint32_t buf_len, const char * restrict format_str, va_list arg);
char * strncpy_(char * restrict dest_str, const char * restrict src_str, uint32_t len);
uint32_t strlen_(char * restrict str);
uint32_t printf_(const char * restrict format_str, ...);