
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

TEST snprintf_test2(const char * restrict format_str, uint32_t test_value, uint32_t test_value2)
{
    char expected[BUFFER_SIZE * 2] = "";
    char actual[BUFFER_SIZE * 2] = "";
    expected[BUFFER_SIZE] = '\0';
    actual[BUFFER_SIZE] = '\0';

    snprintf(expected, BUFFER_SIZE, format_str, test_value, test_value2);
    snprintf_(actual, BUFFER_SIZE, format_str, test_value, test_value2);

    ASSERT_STR_EQ(expected, actual);

    PASS();
}

TEST meta_number_test(const char * restrict meta_format_str, uint32_t test_value)
{
	char format_str[BUFFER_SIZE * 2] = "";

    const char format_flags[] = {'b', 'd', 'i', 'p', 'u', 'x', 'X', '%'};
    const uint32_t ff_size = (uint32_t)sizeof(format_flags);

    uint32_t i = 0;

    for(i = 0; i < ff_size; i++){
        snprintf(format_str, sizeof(format_str), meta_format_str, format_flags[i]);
        CHECK_CALL(snprintf_test(format_str, test_value));
    }

	PASS();
}

TEST meta_number_test2(const char * restrict meta_format_str, uint32_t test_value, uint32_t test_value2)
{
	char format_str[BUFFER_SIZE * 2] = "";

    const char format_flags[] = {'b', 'd', 'i', 'p', 'u', 'x', 'X', '%'};
    const uint32_t ff_size = (uint32_t)sizeof(format_flags);

    uint32_t i = 0;
    uint32_t j = 0;

    for(i = 0; i < ff_size; i++){
        for(j = 0; j < ff_size; j++){
            snprintf(format_str, sizeof(format_str), meta_format_str, format_flags[i], format_flags[j]);
            CHECK_CALL(snprintf_test2(format_str, test_value, test_value2));
        }
    }

	PASS();
}

SUITE(meta_number_suite){

    RUN_TESTp(meta_number_test, "basic meta %%%c number test", 150); 
    RUN_TESTp(meta_number_test, "large meta %%%c number test", 0xffffffff); 
    RUN_TESTp(meta_number_test, "end string %%%c", 684); 
    RUN_TESTp(meta_number_test, "%%%cbegin string", 9762813); 
    RUN_TESTp(meta_number_test, "weird character \0 %%%c test", 777); 
    RUN_TESTp(meta_number_test, "weird character  %%%c\r test", 628); 
    RUN_TESTp(meta_number_test, "weird character  %%%c test\n", 198973); 
    RUN_TESTp(meta_number_test, "weird char\nacter  %%%c test", 369518);
    RUN_TESTp(meta_number_test, "very very very very very very very very very very long string  %%%c test\n", 8064166);

    RUN_TESTp(meta_number_test2, "%%%c two string test %%%c", 9466813, 3872198);
    RUN_TESTp(meta_number_test2, "two %%%c two string test %%%c too\n", 9466813, 3872198);

    uint32_t i = 0;

    for(i = 0; i < BUFFER_SIZE; i++){
        char format_str[BUFFER_SIZE * 2] = "";

        snprintf(format_str, sizeof(format_str), "left justify meta %%%%-%d%%c number test", i);
        RUN_TESTp(meta_number_test, format_str, 150);
        snprintf(format_str, sizeof(format_str), "leading 0 meta %%%%0%d%%c number test", i);
        RUN_TESTp(meta_number_test, format_str, 150);
        snprintf(format_str, sizeof(format_str), "right justify meta %%%%%d%%c number test", i);
        RUN_TESTp(meta_number_test, format_str, 150);
        snprintf(format_str, sizeof(format_str), "positive sign meta %%%%+%d%%c number test", i);
        RUN_TESTp(meta_number_test, format_str, 150);
        snprintf(format_str, sizeof(format_str), "space meta %%%% %d%%c number test", i);
        RUN_TESTp(meta_number_test, format_str, 150);
        snprintf(format_str, sizeof(format_str), "left align space meta %%%%- %d%%c number test", i);
        RUN_TESTp(meta_number_test, format_str, 150);
        snprintf(format_str, sizeof(format_str), "multi flag meta %%%%-+ 0%d%%c number test", i);
        RUN_TESTp(meta_number_test, format_str, 150);
    }

    RUN_TESTp(sprintf_test, "prefix %#x number test", 0xdeadbeef);
    //note: I purposely do not test the prefix of capital X. Printf prints a prefix of "0X" but mprint will print "0x".
    RUN_TESTp(sprintf_test, "prefix %#b number test", 0xdeadbeef);
}


// Add definitions that need to be in the test runner's main file.
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();  // command-line options, initialization.

    RUN_SUITE(meta_number_suite);

    GREATEST_MAIN_END();    // display results
}
