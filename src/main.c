
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "mprintf.h"

#include "greatest.h"

#define     BUFFER_SIZE     50

TEST d_flag(const char * restrict format_str, uint32_t test_value)
{
    char expected[BUFFER_SIZE] = "";
    char actual[BUFFER_SIZE] = "";

    sprintf(expected, format_str, test_value);
    sprintf_(actual, format_str, test_value);

    ASSERT_STR_EQ(expected, actual);

    PASS();
}

// Add definitions that need to be in the test runner's main file.
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();  // command-line options, initialization.

    uint32_t i = 0;

    while(i++ < 10){
        RUN_TESTp(d_flag, "%d", (uint32_t)rand());
    }

    // RUN_TEST(x_should_equal_1);

    GREATEST_MAIN_END();    // display results
}