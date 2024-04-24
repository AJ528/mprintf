
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "mprintf.h"

#include "greatest.h"

#define     BUFFER_SIZE     50

TEST sprintf_test(const char * restrict format_str, uint32_t test_value)
{
    char expected[BUFFER_SIZE * 2] = "";
    char actual[BUFFER_SIZE * 2] = "";
    expected[BUFFER_SIZE] = '\0';
    actual[BUFFER_SIZE] = '\0';

    sprintf(expected, format_str, test_value);
    sprintf_(actual, format_str, test_value);

    ASSERT_STR_EQ(expected, actual);

    PASS();
}

TEST snprintf_test(const char * restrict format_str, uint32_t test_value)
{
    char expected[BUFFER_SIZE * 2] = "";
    char actual[BUFFER_SIZE * 2] = "";
    expected[BUFFER_SIZE] = '\0';
    actual[BUFFER_SIZE] = '\0';

    snprintf(expected, BUFFER_SIZE, format_str, test_value);
    snprintf_(actual, BUFFER_SIZE, format_str, test_value);

    ASSERT_STR_EQ(expected, actual);

    PASS();
}

SUITE(d_flag_suite) {
    RUN_TESTp(sprintf_test, "%d", (uint32_t)rand());
    RUN_TESTp(sprintf_test, "this is a %d test string 46 characters long. END", 150);
    RUN_TESTp(sprintf_test, "this is another %d test string longer than 46 characters END", 12345678);
    RUN_TESTp(sprintf_test, "testing %20d d flag formatting", 12345678);
    RUN_TESTp(sprintf_test, "testing %2d d flag formatting", 12345678);
    RUN_TESTp(sprintf_test, "testing % 8d d flag formatting", 12345678);
    RUN_TESTp(sprintf_test, "testing %-50d d flag formatting", 12345678);
    RUN_TESTp(sprintf_test, "testing %- d d flag formatting", 12345678);
    RUN_TESTp(sprintf_test, "testing %0+d d flag formatting", 12345678);
    RUN_TESTp(sprintf_test, "testing %+05d d flag formatting", 12345678);

    RUN_TESTp(snprintf_test, "%d", (uint32_t)rand());
    RUN_TESTp(snprintf_test, "this is a %d test string 46 characters long. END", 150);
    RUN_TESTp(snprintf_test, "this is another %d test string longer than 46 characters END", 12345678);
    RUN_TESTp(snprintf_test, "testing %20d d flag formatting", 12345678);
    RUN_TESTp(snprintf_test, "testing %2d d flag formatting", 12345678);
    RUN_TESTp(snprintf_test, "testing % 8d d flag formatting", 12345678);
    RUN_TESTp(snprintf_test, "this is a test string 46 characters long. %-10dEND", 12345678);
    RUN_TESTp(snprintf_test, "S%50dE", 12345678);
    RUN_TESTp(snprintf_test, "testing %- d d flag formatting", 12345678);
    RUN_TESTp(snprintf_test, "testing %0+d d flag formatting", 12345678);
    RUN_TESTp(snprintf_test, "testing %+05d d flag formatting", 12345678);
    RUN_TESTp(snprintf_test, "S%-50dE", 12345678);
    RUN_TESTp(snprintf_test, "S%49dE", 12345678);
    RUN_TESTp(snprintf_test, "S%48dE", 12345678);
    RUN_TESTp(snprintf_test, "S%47dE", 12345678);
}


// Add definitions that need to be in the test runner's main file.
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();  // command-line options, initialization.

    RUN_SUITE(d_flag_suite);

    GREATEST_MAIN_END();    // display results
}