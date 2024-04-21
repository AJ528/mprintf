
#include <stdio.h>
#include <stdint.h>
#include "mprintf.h"

#define BUFFER_SIZE     100

int main(void)
{
    // printf_("%d this is a test %x\n", 0xa3013b56, 0xdeadbeef);
    // printf("%d this is a test %x\n", 0xa3013b56, 0xdeadbeef);

    char src_buffer[] = "short test";
// "Green vines attached to the trunk of the tree had wound themselves \
// toward the top of the canopy. Ants used the vine as their private highway, avoiding all \
// the creases and crags of the bark, to freely move at top speed from top to bottom or  \
// bottom to top depending on their current chore. At least this was the way it was supposed \
// to be. Something had damaged the vine overnight halfway up the tree leaving a gap in the \
// once pristine ant highway.";



    char dest_buffer1[BUFFER_SIZE];
    uint32_t i = 0;
    uint32_t ret;

    ret = snprintf_(dest_buffer1, (uint32_t)sizeof(dest_buffer1), "output1: %-020sF", src_buffer);

    for(i = 0; i < sizeof(dest_buffer1); i++){
        putchar(dest_buffer1[i]);
    }
    putchar('E');
    putchar('\n');

    printf("ret = %u\n",ret);

    char dest_buffer2[BUFFER_SIZE];

    ret = snprintf(dest_buffer2, (uint32_t)sizeof(dest_buffer2), "output2: %-020sF", src_buffer);

    for(i = 0; i < sizeof(dest_buffer2); i++){
        putchar(dest_buffer2[i]);
    }
    putchar('E');
    putchar('\n');

    printf("ret = %u\n",ret);

    return (0);
}