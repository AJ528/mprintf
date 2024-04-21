
#include <stdio.h>
#include <stdint.h>
#include "mprintf.h"

int main(void)
{
    // printf_("%d this is a test %x\n", 0xa3013b56, 0xdeadbeef);
    // printf("%d this is a test %x\n", 0xa3013b56, 0xdeadbeef);

    char buffer[100];
    uint32_t i = 0;
    uint32_t ret;

    ret = snprintf_(buffer, (uint32_t)sizeof(buffer), "%u this %i is %d a test %x of the %u character %d buffer %x length\n", 
                0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef);

    for(i = 0; i < sizeof(buffer); i++){
        putchar(buffer[i]);
    }
    putchar('\n');

    printf("ret = %u\n",ret);

    ret = snprintf(buffer, (uint32_t)sizeof(buffer), "%u this %i is %d a test %x of the %u character %d buffer %x length\n", 
                0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef);
    printf("ret = %u\n",ret);

    char buffer2[100] = "\0";
    
    ret = snprintf(buffer2, (uint32_t)sizeof(buffer2), "%-60s\ntestes\n", buffer);
    
    for(i = 0; i < sizeof(buffer2); i++){
        putchar(buffer2[i]);
    }
    putchar('\n');

    printf("ret = %u\n",ret);

    return (0);
}