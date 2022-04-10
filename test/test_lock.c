#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <coral.h>

#include "private/coral.h"
#include "private/lock.h"
#include "test/cmocka.h"
#include "test/wrap.h"

static void check_lock_alloc_error_on_null_argument_ptr(void** state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_alloc(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_lock_alloc(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *lock = NULL;
    assert_true(coral_lock_alloc(&lock));
    assert_int_equal(coral_error, CORAL_ERROR_NONE);
    assert_non_null(lock);
    assert_true(coral_lock_destroy(lock));
    assert_int_equal(coral_error, CORAL_ERROR_NONE);
    coral_error = CORAL_ERROR_NONE;
}

static void check_lock_init_error_on_null_object_ptr(void** state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_init(NULL));
    assert_int_equal(coral_error, CORAL_ERROR_OBJECT_PTR_IS_NULL);
    coral_error = CORAL_ERROR_NONE;
}

static void check_lock_init_error_on_initialization(void** state) {
    pthread_mutex_init_is_overridden = true;
    will_return(__wrap_pthread_mutex_init, 1);
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *lock = NULL;
    assert_true(coral_lock_alloc(&lock));
    assert_int_equal(coral_error, CORAL_ERROR_NONE);
    assert_non_null(lock);
    assert_false(coral_lock_init(lock));
    assert_int_equal(coral_error, CORAL_ERROR_INITIALIZATION_FAILED);
    coral_error = CORAL_ERROR_NONE;
    assert_true(coral_lock_destroy(lock));
    assert_int_equal(coral_error, CORAL_ERROR_NONE);
    pthread_mutex_init_is_overridden = false;
}

static void check_lock_init(void** state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *lock = NULL;
    assert_true(coral_lock_alloc(&lock));
    assert_int_equal(coral_error, CORAL_ERROR_NONE);
    assert_non_null(lock);
    assert_true(coral_lock_init(lock));
    assert_int_equal(coral_error, CORAL_ERROR_NONE);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_lock_destroy_error_on_null_argument_ptr(void** state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_destroy(NULL));
    assert_int_equal(coral_error, CORAL_ERROR_OBJECT_PTR_IS_NULL);
    coral_error = CORAL_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_lock_alloc_error_on_null_argument_ptr),
            cmocka_unit_test(check_lock_alloc),
            cmocka_unit_test(check_lock_init_error_on_null_object_ptr),
            cmocka_unit_test(check_lock_init_error_on_initialization),
            cmocka_unit_test(check_lock_init),
            cmocka_unit_test(check_lock_destroy_error_on_null_argument_ptr)
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}