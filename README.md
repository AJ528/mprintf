# mprintf
simple no-float 32-bit printf

### Status 

Does everything I need it to. Will fix bugs as I come across them.

### Details

Compiles down to 1370 bytes on `arm-none-eabi-gcc` with `-Os` optimizations.
mprintf is about 400 lines of code (per David A. Wheeler's `SLOCCount`)

Supports b, c, d, i, p, s, u, X, x, and % conversion flags, minimum field widths, and 0- +# format flags.

Designed with Cortex-M microcontrollers in mind, but there's no microcontroller-specific code in the file.

I also included testing using Greatest from https://github.com/silentbicycle/greatest. There's a number of tests that compare mprintf against the standard printf.
