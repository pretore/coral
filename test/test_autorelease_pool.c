#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <coral.h>

#include "private/coral.h"
#include "private/autorelease_pool.h"

static void check_start(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$autorelease_pool$entry *stack;
    coral$autorelease_pool$get_stack(&stack);
    assert_null(stack);
    struct coral$autorelease_pool$entry *items;
    coral$autorelease_pool$get_items(&items);
    assert_null(items);
    coral$autorelease_pool$start();
    coral$autorelease_pool$get_stack(&stack);
    assert_non_null(stack);
    coral$autorelease_pool$get_items(&items);
    assert_non_null(items);
    assert_ptr_equal(stack->object, items);
    assert_null(stack->next);
    assert_null(items->object);
    assert_null(items->next);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_end(void **state) {
    coral_error = CORAL_ERROR_NONE;
    coral$autorelease_pool$start();
    struct coral$autorelease_pool$entry *stack;
    struct coral$autorelease_pool$entry *items;
    coral$autorelease_pool$get_stack(&stack);
    assert_non_null(stack);
    coral$autorelease_pool$get_items(&items);
    assert_non_null(items);
    assert_ptr_equal(stack->object, items);
    assert_null(stack->next);
    assert_null(items->object);
    assert_null(items->next);
    coral$autorelease_pool$end();
    coral$autorelease_pool$get_stack(&stack);
    assert_null(stack);
    coral$autorelease_pool$get_items(&items);
    assert_null(items);
    coral_error = CORAL_ERROR_NONE;
}

static void check_add(void **state) {
    coral_error = CORAL_ERROR_NONE;
    coral$autorelease_pool$start();
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 0));
    struct coral$autorelease_pool$entry *stack;
    coral$autorelease_pool$get_stack(&stack);
    assert_non_null(stack);
    struct coral$autorelease_pool$entry *items;
    coral$autorelease_pool$get_items(&items);
    assert_non_null(items);
    assert_ptr_equal(stack->object, items->next);
    assert_null(stack->next);
    assert_ptr_equal(i, items->object);
    coral$autorelease_pool$end();
    coral$autorelease_pool$get_stack(&stack);
    assert_null(stack);
    coral$autorelease_pool$get_items(&items);
    assert_null(items);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_add_previous(void **state) {
    coral_error = CORAL_ERROR_NONE;
    coral$autorelease_pool$start();
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 0));
    assert_true(coral_integer_autorelease(i));
    struct coral$autorelease_pool$entry *stack;
    coral$autorelease_pool$get_stack(&stack);
    assert_non_null(stack);
    struct coral$autorelease_pool$entry *items;
    coral$autorelease_pool$get_items(&items);
    assert_non_null(items);
    coral$autorelease_pool$end();
    coral$autorelease_pool$get_stack(&stack);
    assert_null(stack);
    coral$autorelease_pool$get_items(&items);
    assert_non_null(items);
    assert_ptr_equal(i, items->object);
    assert_null(items->next);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_drain_with_nothing_to_do() {
    coral_error = CORAL_ERROR_NONE;
    struct coral$autorelease_pool$entry *stack;
    coral$autorelease_pool$get_stack(&stack);
    assert_null(stack);
    struct coral$autorelease_pool$entry *items;
    coral$autorelease_pool$get_items(&items);
    assert_null(items);
    coral_autorelease_pool_drain();
    coral$autorelease_pool$get_stack(&stack);
    assert_null(stack);
    coral$autorelease_pool$get_items(&items);
    assert_null(items);
    coral_error = CORAL_ERROR_NONE;
}

static void check_drain() {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 10));
    struct coral$autorelease_pool$entry *stack;
    coral$autorelease_pool$get_stack(&stack);
    assert_null(stack);
    struct coral$autorelease_pool$entry *items;
    coral$autorelease_pool$get_items(&items);
    assert_non_null(items);
    coral_autorelease_pool_drain();
    coral$autorelease_pool$get_stack(&stack);
    assert_null(stack);
    coral$autorelease_pool$get_items(&items);
    assert_null(items);
    coral_error = CORAL_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_start),
            cmocka_unit_test(check_end),
            cmocka_unit_test(check_add),
            cmocka_unit_test(check_add_previous),
            cmocka_unit_test(check_drain_with_nothing_to_do),
            cmocka_unit_test(check_drain),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}