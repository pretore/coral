#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <coral.h>

#include "private/coral.h"
#include "private/range.h"

static void check_range_alloc_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_alloc(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_alloc(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    assert_int_equal(0, object->min);
    assert_int_equal(0, object->max);
    assert_null(object->step_func);
    assert_null(object->context_ref);
    assert_true(coral_range_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    struct coral_context *context;
    assert_true(coral_context_of(&context, NULL));
    struct coral_range_values values = {};
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    assert_false(coral_range_init(NULL,
                                  &values,
                                  coral$range_step_function_delta,
                                  context));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_true(coral_range_destroy(object));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_init_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    struct coral_context *context;
    assert_true(coral_context_of(&context, NULL));
    struct coral_range_values values = {};
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    assert_false(coral_range_init(object,
                                  &values,
                                  NULL,
                                  context));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_true(coral_range_destroy(object));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    struct coral_context *context;
    assert_true(coral_context_of(&context, NULL));
    struct coral_range_values values = {};
    assert_true(coral_range_init(object,
                                 &values,
                                 coral$range_step_function_delta,
                                 context));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_init_defaults_for_null_values(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    struct coral_context *context;
    assert_true(coral_context_of(&context, NULL));
    assert_true(coral_range_init(object,
                                 NULL,
                                 coral$range_step_function_delta,
                                 context));
    size_t value_;
    assert_true(coral_range_get_min(object, &value_));
    assert_int_equal(0, value_);
    assert_true(coral_range_get_max(object, &value_));
    assert_int_equal(SIZE_MAX, value_);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_retain_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_false(coral_range_retain(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_range_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_release_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_false(coral_range_release(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_range_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_range_autorelease_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_false(coral_range_autorelease(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_range_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get_min_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_false(coral_range_get_min(object, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_range_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get_max_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_false(coral_range_get_max(object, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_range_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_false(coral_range_get(object, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_range_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get_next_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_false(coral_range_get_next(object, 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_range_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get_prev_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_false(coral_range_get_prev(object, 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_range_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_destroy_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_destroy(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_retain_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_retain(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_release_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_release(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_autorelease_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_autorelease(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get_min_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get_min(NULL, (void *) 1));
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get_min_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get_min((void *) 1, NULL));
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get_min(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    struct coral_context *context;
    assert_true(coral_context_of(&context, NULL));
    struct coral_range_values values = {
            .first = 60,
            .last =100
    };
    assert_true(coral_range_init(object,
                                 &values,
                                 coral$range_step_function_delta,
                                 context));
    size_t first;
    assert_true(coral_range_get_min(object, &first));
    assert_int_equal(values.first, first);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get_max_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get_max(NULL, (void *) 1));
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get_max_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get_max((void *) 1, NULL));
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get_max(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    struct coral_context *context;
    assert_true(coral_context_of(&context, NULL));
    struct coral_range_values values = {
            .first = 60,
            .last =100
    };
    assert_true(coral_range_init(object,
                                 &values,
                                 coral$range_step_function_delta,
                                 context));
    size_t last;
    assert_true(coral_range_get_max(object, &last));
    assert_int_equal(values.last, last);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get(NULL, (void *) 1));
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get((void *) 1, NULL));
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    struct coral_context *context;
    assert_true(coral_context_of(&context, NULL));
    struct coral_range_values A = {
            .first = 60,
            .last =100
    };
    assert_true(coral_range_init(object,
                                 &A,
                                 coral$range_step_function_delta,
                                 context));
    struct coral_range_values B = {};
    assert_true(coral_range_get(object, &B));
    assert_int_equal(A.first, B.first);
    assert_int_equal(A.last, B.last);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get_next_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get_next(NULL, 0, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get_next_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get_next((void *) 1, 20, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get_next_error_on_end_of_sequence(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    struct coral_range_values values = {0, 1};
    assert_true(coral_range_of_delta(&object, &values, 1));
    size_t size_;
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    assert_false(coral_range_get_next(object, 1, &size_));
    assert_int_equal(CORAL_ERROR_END_OF_SEQUENCE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get_next(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    struct coral_range_values values = {
            .first = 80,
            .last =100
    };
    assert_true(coral_range_of_delta(&object, &values, 1));
    size_t size_;
    assert_true(coral_range_get_next(object, 91, &size_));
    assert_int_equal(92, size_);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get_prev_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get_prev(NULL, 0, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get_prev_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get_prev((void *) 1, 20, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get_prev_error_on_end_of_sequence(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    struct coral_range_values values = {1, 0};
    assert_true(coral_range_of_delta(&object, &values, 1));
    size_t size_;
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    assert_false(coral_range_get_prev(object, 0, &size_));
    assert_int_equal(CORAL_ERROR_END_OF_SEQUENCE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_get_prev(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    struct coral_range_values values = {
            .first = 80,
            .last =100
    };
    assert_true(coral_range_of_delta(&object, &values, 1));
    size_t size_;
    assert_true(coral_range_get_prev(object, 91, &size_));
    assert_int_equal(90, size_);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_of_delta_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    struct coral_range_values values = {};
    assert_false(coral_range_of_delta(&object, &values, 0));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    values.last = 1;
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_of_delta(&object, &values, 10));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_of_delta(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    struct coral_range_values values = {
            .first = 100,
            .last =101
    };
    assert_true(coral_range_of_delta(&object, &values, 1));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_of_rate_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    struct coral_range_values values = {1, 5};
    assert_false(coral_range_of_rate(&object, &values, 0));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_of_rate(&object, &values, 10));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_range_of_rate(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    struct coral_range_values values = {
            .first = 10,
            .last = 5
    };
    assert_true(coral_range_of_rate(&object, &values, 1.5));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_range_alloc_error_on_null_argument_ptr),
            cmocka_unit_test(check_range_alloc),
            cmocka_unit_test(check_range_init_error_on_null_object_ptr),
            cmocka_unit_test(check_range_init_error_on_null_argument_ptr),
            cmocka_unit_test(check_range_init_defaults_for_null_values),
            cmocka_unit_test(check_range_init),
            cmocka_unit_test(check_range_retain_error_on_object_uninitialized),
            cmocka_unit_test(check_range_release_error_on_object_uninitialized),
            cmocka_unit_test(check_range_autorelease_error_on_object_uninitialized),
            cmocka_unit_test(check_range_get_min_error_on_object_uninitialized),
            cmocka_unit_test(check_range_get_max_error_on_object_uninitialized),
            cmocka_unit_test(check_range_get_error_on_object_uninitialized),
            cmocka_unit_test(check_range_get_next_error_on_object_uninitialized),
            cmocka_unit_test(check_range_get_prev_error_on_object_uninitialized),
            cmocka_unit_test(check_range_destroy_error_on_null_object_ptr),
            cmocka_unit_test(check_range_retain_error_on_null_object_ptr),
            cmocka_unit_test(check_range_release_error_on_null_object_ptr),
            cmocka_unit_test(check_range_autorelease_error_on_null_object_ptr),
            cmocka_unit_test(check_range_get_min_error_on_null_object_ptr),
            cmocka_unit_test(check_range_get_min_error_on_null_argument_ptr),
            cmocka_unit_test(check_range_get_min),
            cmocka_unit_test(check_range_get_max_error_on_null_object_ptr),
            cmocka_unit_test(check_range_get_max_error_on_null_argument_ptr),
            cmocka_unit_test(check_range_get_max),
            cmocka_unit_test(check_range_get_error_on_null_object_ptr),
            cmocka_unit_test(check_range_get_error_on_null_argument_ptr),
            cmocka_unit_test(check_range_get),
            cmocka_unit_test(check_range_get_next_error_on_null_object_ptr),
            cmocka_unit_test(check_range_get_next_error_on_null_argument_ptr),
            cmocka_unit_test(check_range_get_next_error_on_end_of_sequence),
            cmocka_unit_test(check_range_get_next),
            cmocka_unit_test(check_range_get_prev_error_on_null_object_ptr),
            cmocka_unit_test(check_range_get_prev_error_on_null_argument_ptr),
            cmocka_unit_test(check_range_get_prev_error_on_end_of_sequence),
            cmocka_unit_test(check_range_get_prev),
            cmocka_unit_test(check_range_of_delta_error_on_invalid_value),
            cmocka_unit_test(check_range_of_delta),
            cmocka_unit_test(check_range_of_rate_error_on_invalid_value),
            cmocka_unit_test(check_range_of_rate)
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
