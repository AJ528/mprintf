# mprintf
simple no-float 32-bit printf

### Status 
Does everything I need it to. Will fix bugs as I come across them.

### How to use this in an embedded project
1. Copy `mprint.h` and `mprint.c` into your project
2. In `mprint.c`, replace all the calls to `putchar()` with the appropriate one for your platform. In embedded projects, this is commonly a function that outputs data over UART.
3. Once `putchar()` is replaced, the `#include <stdio.h>` line in `mprintf.c` should no longer be needed. Feel free to delete it.
4. If needed, adjust the newline encoding in `print_newline()`.
5. Done!

### How to measure the size of this project
1. copy these files into an embedded project.
2. compile the project.
3. measure the size of the `mprintf.o` object file using `size -A`.
4. the total size is found from summing all relevant sections together.


### Details
Compiles down to 1554 bytes on `arm-none-eabi-gcc` with `-Os` optimizations. This can be made slightly smaller depending on which functions are actually called.
mprintf is about 450 lines of code (per David A. Wheeler's `SLOCCount`).

Max RAM usage can be controlled by changing the `printf()` buffer size. Shrinking the buffer saves RAM at the cost of decreasing the maximum string length `printf()` can handle.

Supports b, c, d, i, p, s, u, X, x, and % conversion flags, minimum field widths, and 0- +# format flags.

Designed with Cortex-M microcontrollers in mind, but there's no microcontroller-specific code in the file.

I also included testing using Greatest from https://github.com/silentbicycle/greatest. There's a number of tests that compare mprintf against the standard printf.
