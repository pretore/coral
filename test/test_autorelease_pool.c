#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <coral.h>

#include "private/coral.h"
#include "private/autorelease_pool.h"

// TODO: autorelease_pool must work correctly ...

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {

    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}