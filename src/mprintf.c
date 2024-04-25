#include "mprintf.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// maximum length of a single number including sign and padding
// size of 34 should be enough to hold any 32 bit number converted to 
// binary (widest format) plus a prefix of "0b"
#define NUM_MAX_WIDTH    34

struct format_flags {
    bool fill_zero;         // if there's padding, should it be zero? otherwise pad with space
    bool left_align;        // is the number left-algined?
    bool sign_space;        // sign_space = (positive_space | display_sign | is_negative)
    bool positive_space;    // ' ' flag
    bool display_sign;      // '+' flag
    bool is_negative;       // tells us to display a negative sign
    bool display_prefix;    // tells us to display a prefix before the base 2 or base 16 number ("0x" or "0b")
    bool capitalize;        // tells us to use captial letters for hexadecimal
    uint32_t min_width;     // stores the minimum field width
 
    uint32_t base;
};

static const char lc_map[] = "0123456789abcdef";
static const char uc_map[] = "0123456789ABCDEF";

static uint32_t insert_string(char * restrict out_str, uint32_t out_str_len, 
                    const char * restrict in_str, struct format_flags flags);
static uint32_t convert_number(char * restrict out_str, uint32_t out_str_len,
                        uint32_t value, struct format_flags flags);
static uint32_t reverse_string(char * restrict out_str, uint32_t out_str_len, 
                    const char * restrict in_str, uint32_t in_str_len, 
                    struct format_flags flags);

uint32_t printf_(const char * restrict format_str, ...)
{
    char output_buffer[1000];
    va_list arg;
    uint32_t ret;

    va_start(arg, format_str);

    ret = vsnprintf_(output_buffer, UINT32_MAX, format_str, arg);

    va_end(arg);

    for(uint32_t i = 0; i < ret; i++){
        putchar(output_buffer[i]);
    }

    return(ret);
}

uint32_t sprintf_(char * restrict out_str, const char * restrict format_str, ...)
{
    va_list arg;
    uint32_t ret;

    va_start(arg, format_str);

    ret = vsnprintf_(out_str, UINT32_MAX, format_str, arg);

    va_end(arg);

    return(ret);
}

uint32_t snprintf_(char * restrict out_str, uint32_t buf_len, const char * restrict format_str, ...)
{
    va_list arg;
    uint32_t ret;

    va_start(arg, format_str);

    ret = vsnprintf_(out_str, buf_len, format_str, arg);

    va_end(arg);

    return(ret);
}

// assumes format_str ends with '\0'
// returns the theoretical number of characters written to buffer, assuming infininte buffer space
uint32_t vsnprintf_(char * restrict out_str, uint32_t buf_len, const char * restrict format_str, va_list arg)
{
    uint32_t read_index = 0;
    uint32_t write_index = 0;
    //length of output string if we run into no buffer limits
    uint32_t max_str_len = 0;

    if(buf_len == 0){
        return 0;
    }

    // assuming buf_len isn't 0, the logic gets a bit cleaner

    // record how long out_str can be
    uint32_t out_str_len = buf_len - 1;

    // read the format string until we reach the end
    while(format_str[read_index] != '\0'){
        // remember where we started reading from
        const uint32_t read_start = read_index;
        //look for '%' or '\0'
        while((format_str[read_index] != '%') && (format_str[read_index] != '\0')){
            read_index++;
        }

        uint32_t copy_len = 0;
        // if we read further than out_str can hold
        if((read_index - read_start) > out_str_len){
            // only copy over what out_str can hold
            copy_len = out_str_len;
        }else{
            // copy everything
            copy_len = read_index - read_start;
        }
        // regardless, record max length to max_str_len
        max_str_len += read_index - read_start;
        strncpy_(&out_str[write_index], &format_str[read_start], copy_len);
        write_index += copy_len;
        out_str_len -= copy_len;

        if(format_str[read_index] == '%'){
            // handle formatting
            struct format_flags flags ={
                .fill_zero = false,
                .left_align = false,
                .positive_space = false,
                .display_sign = false,
                .min_width = 0,
                .display_prefix = false,
                .capitalize = false,
                .is_negative = false,
                .base = 10
            };
            uint32_t value;
            // skip '%'
            read_index++;

            //check for flags
flag_check:
            switch(format_str[read_index]){
                case '0':
                    flags.fill_zero = true;
                    read_index++;
                    goto flag_check;
                case '-':
                    flags.left_align = true;
                    read_index++;
                    goto flag_check;
                case ' ':
                    flags.positive_space = true;
                    flags.sign_space = true;
                    read_index++;
                    goto flag_check;
                case '+':
                    flags.display_sign = true;
                    flags.sign_space = true;
                    read_index++;
                    goto flag_check;
                case '#':
                    flags.display_prefix = true;
                    read_index++;
                    goto flag_check;
                default:
                    break;
            }

            //check for field width
            while(format_str[read_index] >= '0' && format_str[read_index] <= '9'){
                flags.min_width = flags.min_width * 10;
                flags.min_width += format_str[read_index] - '0';
                read_index++;
            }

            bool run_convert_number = false;

            //get conversion specifier
            switch(format_str[read_index++]){
                case 'b':
                {
                    value = va_arg(arg, uint32_t);
                    run_convert_number = true;
                    flags.base = 2;
                    flags.positive_space = false;
                    flags.display_sign = false;
                    flags.sign_space = false;
                    break;
                }
                case 'c':
                {
                    value = va_arg(arg, uint32_t);
                    if(out_str_len != 0){
                        out_str[write_index++] = (char)value;
                        out_str_len--;
                    }
                    max_str_len++;
                    break;
                }
                case 'd':
                case 'i':
                {
                    value = va_arg(arg, uint32_t);
                    if((int32_t)value < 0){
                        flags.is_negative = true;
                        flags.sign_space = true;
                        //make it positive
                        value = -(int32_t)value;
                    }
                    run_convert_number = true;
                    flags.base = 10;
                    break;
                }
                case 'p':
                {
                    value = va_arg(arg, uint32_t);
                    run_convert_number = true;
                    flags.base = 16;
                    flags.display_prefix = true;
                    break;
                }
                case 's':
                {
                    const char *arg_str = va_arg(arg, char*);

                    uint32_t str_len = insert_string(&out_str[write_index], 
                                                out_str_len, arg_str, flags);
                    if(str_len > out_str_len){
                        write_index += out_str_len;
                        out_str_len = 0;
                    }else{
                        write_index += str_len;
                        out_str_len -= str_len;
                    }
                    max_str_len += str_len;


                    break;
                }
                case 'u':
                {
                    value = va_arg(arg, uint32_t);
                    run_convert_number = true;
                    flags.base = 10;
                    flags.display_sign = false;
                    flags.sign_space = false;
                    break;
                }
                case 'X':
                {
                    value = va_arg(arg, uint32_t);
                    run_convert_number = true;
                    flags.base = 16;
                    flags.capitalize = true;
                    flags.positive_space = false;
                    flags.display_sign = false;
                    flags.sign_space = false;
                    break;
                }
                case 'x':
                {
                    value = va_arg(arg, uint32_t);
                    run_convert_number = true;
                    flags.base = 16;
                    flags.positive_space = false;
                    flags.display_sign = false;
                    flags.sign_space = false;
                    break;
                }
                case '%':
                {
                    if(out_str_len != 0){
                        out_str[write_index++] = '%';
                        out_str_len--;
                    }
                    max_str_len++;
                    break;
                }
            }
            if(run_convert_number == true){
                uint32_t num_str_len = convert_number(&out_str[write_index], out_str_len, value, flags);
                if(num_str_len > out_str_len){
                    write_index += out_str_len;
                    out_str_len = 0;
                }else{
                    write_index += num_str_len;
                    out_str_len -= num_str_len;
                }
                max_str_len += num_str_len;
            }
        }
    }

    // terminate the string
    out_str[write_index++] = '\0';
    return max_str_len;

}

uint32_t strlen_(const char * restrict str)
{
    uint32_t len = 0;

    while(str[len++] != '\0');

    // return the length of the string excluding the null byte
    return (len - 1);
}

char * strncpy_(char * restrict dest_str, const char * restrict src_str, uint32_t len)
{
    uint32_t read_index = 0;

    // while src_str isn't empty and we haven't copied over len characters
    while((read_index < len) && (src_str[read_index] != '\0'))
    {
        dest_str[read_index] = src_str[read_index];
        read_index++;
    }

    // if src_str was shorter than len, pad the remaining string with '\0'
    while(read_index < len)
    {
        dest_str[read_index] = '\0';
        read_index++;
    }

    return(dest_str);
}

/*
    insert the input string into the output string.
    insert a maximum of out_str_len characters.
    if min_width > in_str length, add padding to beginning or end
    depending on if it's left-aligned.
    return number of characters written if you ignore out_str_len
*/
static uint32_t insert_string(char * restrict out_str, uint32_t out_str_len, 
                    const char * restrict in_str, struct format_flags flags)
{
    uint32_t in_str_len = strlen_(in_str);
    uint32_t write_index = 0;
    uint32_t pad_len = 0;   // how much to pad (if there's space)
    uint32_t max_len = 0;   // max length we would write if you ignore out_str_len
    if(flags.min_width > in_str_len){
        pad_len = flags.min_width - in_str_len;
        max_len = flags.min_width;
    }else{
        max_len = in_str_len;
    }

    // if we should pad spaces before we insert the string
    if((flags.left_align == false) && (pad_len > 0)){
        while((write_index < pad_len) && (out_str_len > 0)){
            out_str[write_index++] = ' ';
            out_str_len--;
        }
    }

    uint32_t copy_len = 0;

    if(out_str_len > in_str_len){
        copy_len = in_str_len;
    }else{
        copy_len = out_str_len;
    }

    strncpy_(&out_str[write_index], in_str, copy_len);
    write_index += copy_len;
    out_str_len -= copy_len;

    // if we should pad spaces after we insert the string
    if((flags.left_align == true) && (pad_len > 0)){
        while((write_index < flags.min_width) && (out_str_len > 0)){
            out_str[write_index++] = ' ';
            out_str_len--;
        }
    }
    return max_len;
}

static uint32_t convert_number(char * restrict out_str, uint32_t out_str_len,
                        uint32_t value, struct format_flags flags)
{
    char num_buffer[NUM_MAX_WIDTH];
    uint32_t cur_val = value;
    uint32_t num_str_len = 0;

    // calculate what each digit should be
    // this algorithm produces a backwards string
    do{
        uint32_t digit = cur_val % flags.base;
        char c_digit;
        if(flags.capitalize == true){
            c_digit = uc_map[digit];
        }else{
            c_digit = lc_map[digit];
        }
        num_buffer[num_str_len++] = c_digit;
        cur_val /= flags.base;
    }while(cur_val != 0);

    if(flags.display_prefix == true){
        if(flags.base == 16){
            num_buffer[num_str_len++] = 'x';
            num_buffer[num_str_len++] = '0';
        }else if(flags.base == 2){
            num_buffer[num_str_len++] = 'b';
            num_buffer[num_str_len++] = '0';
        }else{  // flag base is 10 and we don't print a prefix for that
            flags.display_prefix = false;
        }
    }

    // order matters for these if statements. 
    // if negative, a minus sign should always be displayed
    // a plus sign should override a space if both are flagged
    if(flags.positive_space == true){
        num_buffer[num_str_len] = ' ';
    }
    if(flags.display_sign == true){
        num_buffer[num_str_len] = '+';
    }
    if(flags.is_negative){
        num_buffer[num_str_len] = '-';
    }

    if(flags.sign_space){
        num_str_len++;
    }

    // reverse the backwards string and add appropriate padding
    uint32_t max_len = reverse_string(out_str, out_str_len, 
                            num_buffer, num_str_len, flags);

    return max_len;
}

static uint32_t reverse_string(char * restrict out_str, uint32_t out_str_len, 
                    const char * restrict in_str, uint32_t in_str_len, 
                    struct format_flags flags)
{
    if(out_str_len == 0){
        return 0;
    }

    uint32_t pad_len = 0;

    // if there is padding, calculate it
    if(flags.min_width > in_str_len){
        pad_len = flags.min_width - in_str_len;
    }

    // this is the maximum length of the string if you ignore out_str_len
    uint32_t max_len = pad_len + in_str_len;

    if(flags.left_align == true){
        while((out_str_len > 0) && (in_str_len > 0)){
            out_str_len--;
            in_str_len--;            
            *(out_str++) = in_str[in_str_len];
        }

        while((out_str_len > 0) && (pad_len > 0)){
            out_str_len--;
            pad_len--;            
            *(out_str++) = ' ';
        }
    }else{  // right align number
        if(flags.fill_zero == true){
            // if there is a sign, print that before filling with zeros
            if(flags.sign_space == true){
                //this is safe because we know out_str_len is at least 1
                *(out_str++) = in_str[--in_str_len];
                out_str_len--;
            }
            if(flags.display_prefix == true){
                uint32_t prefix_len = 2;    //prefix is always 2 characters long
                while((out_str_len > 0) && (prefix_len > 0)){
                    *(out_str++) = in_str[--in_str_len];
                    out_str_len--;
                    prefix_len--;
                }
            }

            while((out_str_len > 0) && (pad_len > 0)){
                out_str_len--;
                pad_len--;                
                *(out_str++) = '0';
            }
            
            while((out_str_len > 0) && (in_str_len > 0)){
                out_str_len--;
                in_str_len--;                
                *(out_str++) = in_str[in_str_len];
            }

        }else{ // right align number and pad with spaces
            while((out_str_len > 0) && (pad_len > 0)){
                out_str_len--;
                pad_len--;                
                *(out_str++) = ' ';
            }

            while((out_str_len > 0) && (in_str_len > 0)){
                out_str_len--;
                in_str_len--;
                *(out_str++) = in_str[in_str_len];
            }
        }
    }

    return max_len;
}