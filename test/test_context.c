#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <coral.h>

#include "private/coral.h"
#include "private/context.h"

static void check_context_alloc_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_context_alloc(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_context_alloc(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_context *object;
    assert_true(coral_context_alloc(&object));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    assert_null(object->data);
    assert_true(coral_context_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_context_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_context *object;
    assert_true(coral_context_alloc(&object));
    assert_false(coral_context_init(NULL, (void *)1));
    assert_true(coral_context_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_context_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_context *object;
    assert_true(coral_context_alloc(&object));
    assert_true(coral_context_init(object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_context_destroy_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_context_destroy(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_context_of_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_context_of(NULL, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_context_of(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_context *object;
    assert_true(coral_context_of(&object, NULL));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_context_retain_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_context *object;
    assert_true(coral_context_alloc(&object));
    assert_false(coral_context_retain(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_context_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_context_release_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_context *object;
    assert_true(coral_context_alloc(&object));
    assert_false(coral_context_release(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_context_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void
check_context_autorelease_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_context *object;
    assert_true(coral_context_alloc(&object));
    assert_false(coral_context_autorelease(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_context_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_context_get_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_context *object;
    assert_true(coral_context_alloc(&object));
    assert_false(coral_context_get(object, (void*)1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_context_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_context_retain_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_context_retain(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_context_release_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_context_release(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_context_get_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_context *object;
    assert_true(coral_context_alloc(&object));
    assert_true(coral_context_init(object, NULL));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    assert_false(coral_context_get(object, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_context_get(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_context *object;
    assert_true(coral_context_alloc(&object));
    assert_true(coral_context_init(object, NULL));
    assert_null(object->data);
    size_t* size_;
    assert_true(coral_context_get(object, (void **) &size_));
    *size_ = 50;
    assert_int_equal(50, *(size_t*)(&object->data));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_context_alloc_error_on_null_argument_ptr),
            cmocka_unit_test(check_context_alloc),
            cmocka_unit_test(check_context_init_error_on_null_object_ptr),
            cmocka_unit_test(check_context_init),
            cmocka_unit_test(check_context_destroy_error_on_null_argument_ptr),
            cmocka_unit_test(check_context_of_error_on_null_argument_ptr),
            cmocka_unit_test(check_context_of),
            cmocka_unit_test(check_context_retain_error_on_object_uninitialized),
            cmocka_unit_test(check_context_release_error_on_object_uninitialized),
            cmocka_unit_test(check_context_autorelease_error_on_object_uninitialized),
            cmocka_unit_test(check_context_get_error_on_object_uninitialized),
            cmocka_unit_test(check_context_retain_error_on_null_object_ptr),
            cmocka_unit_test(check_context_release_error_on_null_object_ptr),
            cmocka_unit_test(check_context_get_error_on_null_argument_ptr),
            cmocka_unit_test(check_context_get),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}