#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <coral.h>

#include "private/range.h"
#include "test/cmocka.h"

static void check_invalidate_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$range$invalidate(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_invalidate(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$range object;
    memset(&object, 0xff, sizeof(object));
    object.context = NULL;
    assert_true(coral$range$invalidate(&object));
    assert_int_equal(0, object.values.first);
    assert_int_equal(0, object.values.last);
    assert_null(object.step_func);
    coral_error = CORAL_ERROR_NONE;
}

static void check_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values i = {};
    assert_false(coral$range$init(NULL, i, (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_init_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values i = {};
    assert_false(coral$range$init((void *) 1, i, NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static bool step_func_delta_of_one(const size_t current,
                                   const bool previous,
                                   const struct coral_range_values values,
                                   void *context,
                                   size_t *out) {
    assert_non_null(out);
    function_called();
    *out = previous ? current - 1 : 1 + current;
    return true;
}

static void check_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$range object = {};
    struct coral_range_values i = {SIZE_MAX >> 2, SIZE_MAX >> 1};
    assert_true(coral$range$init(&object, i, step_func_delta_of_one, NULL));
    assert_int_equal(i.first, object.values.first);
    assert_int_equal(i.last, object.values.last);
    assert_ptr_equal(step_func_delta_of_one, object.step_func);
    assert_null(object.context);
    assert_true(coral$range$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_first_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$range$get_first(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_first_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$range$get_first((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_first(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$range object = {};
    struct coral_range_values i = {SIZE_MAX >> 1, SIZE_MAX >> 2};
    assert_true(coral$range$init(&object, i, step_func_delta_of_one, NULL));
    size_t first;
    assert_true(coral$range$get_first(&object, &first));
    assert_int_equal(i.first, first);
    assert_true(coral$range$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_last_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$range$get_last(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_last_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$range$get_last((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_last(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$range object;
    struct coral_range_values i = {SIZE_MAX >> 1, SIZE_MAX >> 4};
    assert_true(coral$range$init(&object, i, step_func_delta_of_one, NULL));
    size_t last;
    assert_true(coral$range$get_last(&object, &last));
    assert_int_equal(i.last, last);
    assert_true(coral$range$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_minimum_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$range$get_minimum(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_minimum_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$range$get_minimum((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_minimum(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {123, 10};
    struct coral$range object = {};
    assert_true(coral$range$init(&object, values, step_func_delta_of_one,
                                 NULL));
    size_t minimum;
    assert_true(coral$range$get_minimum(&object, &minimum));
    assert_int_equal(values.last, minimum);
    assert_true(coral$range$invalidate(&object));
    values.last = 124;
    assert_true(coral$range$init(&object, values, step_func_delta_of_one,
                                 NULL));
    assert_true(coral$range$get_minimum(&object, &minimum));
    assert_int_equal(values.first, minimum);
    assert_true(coral$range$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_maximum_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$range$get_maximum(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_maximum_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$range$get_maximum((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_maximum(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {123, 10};
    struct coral$range object = {};
    assert_true(coral$range$init(&object, values, step_func_delta_of_one,
                                 NULL));
    size_t maximum;
    assert_true(coral$range$get_maximum(&object, &maximum));
    assert_int_equal(values.first, maximum);
    assert_true(coral$range$invalidate(&object));
    values.last = 124;
    assert_true(coral$range$init(&object, values, step_func_delta_of_one,
                                 NULL));
    assert_true(coral$range$get_maximum(&object, &maximum));
    assert_int_equal(values.last, maximum);
    assert_true(coral$range$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_is_inclusive_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$range$is_inclusive(NULL, 0, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_is_inclusive_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$range$is_inclusive((void *) 1, 0, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_is_inclusive(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {123, 10};
    struct coral$range object = {};
    assert_true(coral$range$init(&object, values, step_func_delta_of_one,
                                 NULL));
    bool is_inclusive;
    assert_true(coral$range$is_inclusive(&object, values.last, &is_inclusive));
    assert_true(is_inclusive);
    assert_true(coral$range$is_inclusive(&object, values.last - 1,
                                         &is_inclusive));
    assert_false(is_inclusive);
    assert_true(coral$range$is_inclusive(&object, values.first, &is_inclusive));
    assert_true(is_inclusive);
    assert_true(coral$range$is_inclusive(&object, 1 + values.first,
                                         &is_inclusive));
    assert_false(is_inclusive);
    assert_true(coral$range$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_next_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$range$get_next(NULL, 0, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_next_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$range$get_next((void *) 1, 0, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_next_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$range object = {};
    struct coral_range_values i = {SIZE_MAX >> 1, SIZE_MAX >> 4};
    assert_true(coral$range$init(&object, i, step_func_delta_of_one, NULL));
    size_t next;
    assert_false(coral$range$get_next(&object, 0, &next));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    assert_true(coral$range$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_next_error_on_end_of_sequence(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$range object = {};
    struct coral_range_values i = {SIZE_MAX >> 1, SIZE_MAX >> 4};
    assert_true(coral$range$init(&object, i, step_func_delta_of_one, NULL));
    size_t current;
    assert_true(coral_maximum_size_t(i.first, i.last, &current));
    size_t next;
    expect_function_call(step_func_delta_of_one);
    assert_false(coral$range$get_next(&object, current, &next));
    assert_int_equal(CORAL_ERROR_END_OF_SEQUENCE, coral_error);
    assert_true(coral$range$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_next(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$range object;
    struct coral_range_values i = {SIZE_MAX >> 1, SIZE_MAX >> 4};
    assert_true(coral$range$init(&object, i, step_func_delta_of_one, NULL));
    size_t current;
    assert_true(coral_minimum_size_t(i.first, i.last, &current));
    size_t next;
    expect_function_call(step_func_delta_of_one);
    assert_true(coral$range$get_next(&object, current, &next));
    assert_int_equal(1 + current, next);
    assert_true(coral$range$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_prev_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$range$get_prev(NULL, 0, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_prev_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$range$get_prev((void *) 1, 0, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_prev_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$range object = {};
    struct coral_range_values i = {SIZE_MAX >> 1, SIZE_MAX >> 4};
    assert_true(coral$range$init(&object, i, step_func_delta_of_one, NULL));
    size_t prev;
    assert_false(coral$range$get_prev(&object, SIZE_MAX, &prev));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    assert_true(coral$range$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_prev_error_on_end_of_sequence(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$range object = {};
    struct coral_range_values i = {SIZE_MAX >> 1, SIZE_MAX >> 4};
    assert_true(coral$range$init(&object, i, step_func_delta_of_one, NULL));
    size_t current;
    assert_true(coral_minimum_size_t(i.first, i.last, &current));
    size_t prev;
    expect_function_call(step_func_delta_of_one);
    assert_false(coral$range$get_prev(&object, current, &prev));
    assert_int_equal(CORAL_ERROR_END_OF_SEQUENCE, coral_error);
    assert_true(coral$range$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_prev(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$range object = {};
    struct coral_range_values i = {SIZE_MAX >> 1, SIZE_MAX >> 4};
    assert_true(coral$range$init(&object, i, step_func_delta_of_one, NULL));
    size_t current;
    assert_true(coral_maximum_size_t(i.first, i.last, &current));
    size_t prev;
    expect_function_call(step_func_delta_of_one);
    assert_true(coral$range$get_prev(&object, current, &prev));
    assert_int_equal(current - 1, prev);
    assert_true(coral$range$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_class_error_on_null_argument(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_class(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_class(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_range_class(&class));
    assert_non_null(class);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_destroy_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_destroy(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_alloc_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_alloc(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_alloc(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object = NULL;
    assert_true(coral_range_alloc(&object));
    assert_non_null(object);
    assert_true(coral_range_destroy(object));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {};
    assert_false(coral_range_init(NULL, values, (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {};
    assert_false(coral_range_init((void *) 1, values, NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {};
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_true(coral_range_init(object, values, step_func_delta_of_one, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_first_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get_first(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_first_error_on_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get_first((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_first_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_false(coral_range_get_first(object, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_range_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_first(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {82, 12112};
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_true(coral_range_init(object, values, step_func_delta_of_one,
                                 NULL));
    size_t first = 0;
    assert_true(coral_range_get_first(object, &first));
    assert_int_equal(values.first, first);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_last_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get_last(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_last_error_on_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get_last((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_last_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_false(coral_range_get_last(object, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_range_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_last(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {82, 12112};
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_true(coral_range_init(object, values, step_func_delta_of_one,
                                 NULL));
    size_t last = 0;
    assert_true(coral_range_get_last(object, &last));
    assert_int_equal(values.last, last);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_minimum_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get_minimum(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_minimum_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get_minimum((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_get_minimum_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_false(coral_range_get_minimum(object, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_range_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_minimum(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {122, 10};
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_true(coral_range_init(object, values, step_func_delta_of_one, NULL));
    size_t minimum;
    assert_true(coral_range_get_minimum(object, &minimum));
    assert_int_equal(values.last, minimum);
    values.last = 123;
    assert_true(coral_range_alloc(&object));
    assert_true(coral_range_init(object, values, step_func_delta_of_one, NULL));
    assert_true(coral_range_get_minimum(object, &minimum));
    assert_int_equal(values.first, minimum);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_maximum_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get_maximum(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_maximum_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get_maximum((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_get_maximum_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_false(coral_range_get_maximum(object, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_range_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_maximum(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {122, 10};
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_true(coral_range_init(object, values, step_func_delta_of_one, NULL));
    size_t maximum;
    assert_true(coral_range_get_maximum(object, &maximum));
    assert_int_equal(values.first, maximum);
    values.last = 123;
    assert_true(coral_range_alloc(&object));
    assert_true(coral_range_init(object, values, step_func_delta_of_one, NULL));
    assert_true(coral_range_get_maximum(object, &maximum));
    assert_int_equal(values.last, maximum);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_inclusive_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_is_inclusive(NULL, 0, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_inclusive_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_is_inclusive((void *) 1, 0, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_is_inclusive_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_false(coral_range_is_inclusive(object, 0, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_range_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_inclusive(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {123, 10};
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_true(coral_range_init(object, values, step_func_delta_of_one,
                                 NULL));
    bool is_inclusive;
    assert_true(coral_range_is_inclusive(object, values.last, &is_inclusive));
    assert_true(is_inclusive);
    assert_true(coral_range_is_inclusive(object, values.last - 1,
                                         &is_inclusive));
    assert_false(is_inclusive);
    assert_true(coral_range_is_inclusive(object, values.first, &is_inclusive));
    assert_true(is_inclusive);
    assert_true(coral_range_is_inclusive(object, 1 + values.first,
                                         &is_inclusive));
    assert_false(is_inclusive);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_next_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get_next(NULL, 0, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_next_error_on_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get_next((void *) 1, 0, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_next_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    struct coral_range_values values = {50, 100};
    assert_true(coral_range_init(object, values, step_func_delta_of_one, NULL));
    size_t next;
    assert_false(coral_range_get_next(object, 10, &next));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_next_error_on_end_of_sequence(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    struct coral_range_values values = {50, 100};
    assert_true(coral_range_init(object, values, step_func_delta_of_one, NULL));
    size_t next;
    expect_function_call(step_func_delta_of_one);
    assert_false(coral_range_get_next(object, 100, &next));
    assert_int_equal(CORAL_ERROR_END_OF_SEQUENCE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_next_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_false(coral_range_get_next(object, 1, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_range_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_next(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    struct coral_range_values values = {50, 100};
    assert_true(coral_range_init(object, values, step_func_delta_of_one, NULL));
    size_t next;
    expect_function_call(step_func_delta_of_one);
    assert_true(coral_range_get_next(object, 99, &next));
    assert_int_equal(values.last, next);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_prev_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get_prev(NULL, 0, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_prev_error_on_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_get_prev((void *) 1, 0, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_prev_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    struct coral_range_values values = {50, 100};
    assert_true(coral_range_init(object, values, step_func_delta_of_one, NULL));
    size_t prev;
    assert_false(coral_range_get_prev(object, 110, &prev));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_prev_error_on_end_of_sequence(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    struct coral_range_values values = {50, 100};
    assert_true(coral_range_init(object, values, step_func_delta_of_one, NULL));
    size_t prev;
    expect_function_call(step_func_delta_of_one);
    assert_false(coral_range_get_prev(object, 50, &prev));
    assert_int_equal(CORAL_ERROR_END_OF_SEQUENCE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_prev_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_false(coral_range_get_prev(object, 1, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_range_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_prev(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    struct coral_range_values values = {50, 100};
    assert_true(coral_range_init(object, values, step_func_delta_of_one, NULL));
    size_t prev;
    expect_function_call(step_func_delta_of_one);
    assert_true(coral_range_get_prev(object, 51, &prev));
    assert_int_equal(values.first, prev);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_of_delta_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {273, 123};
    assert_false(coral_range_of_delta(NULL, values, 10));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_of_delta_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    struct coral_range_values values = {273, 123};
    assert_false(coral_range_of_delta(&object, values, 0));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_of_delta(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    struct coral_range_values values = {10, 123};
    assert_true(coral_range_of_delta(&object, values, 4));
    size_t out;
    assert_true(coral_range_get_next(object, 40, &out));
    assert_int_equal(44, out);
    assert_true(coral_range_get_prev(object, 33, &out));
    assert_int_equal(29, out);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_of_rate_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {273, 123};
    assert_false(coral_range_of_rate(NULL, values, 2));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_of_rate_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    struct coral_range_values values = {273, 123};
    assert_false(coral_range_of_rate(&object, values, 0));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_of_rate(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    struct coral_range_values values = {10, 123};
    assert_true(coral_range_of_rate(&object, values, 2));
    size_t out;
    assert_true(coral_range_get_next(object, 40, &out));
    assert_int_equal(80, out);
    assert_true(coral_range_get_prev(object, 32, &out));
    assert_int_equal(16, out);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_hash_code(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_hash_code((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    assert_false(coral_range_hash_code(object, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_range_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    struct coral_range_values values = {50, 100};
    assert_true(coral_range_init(object, values, step_func_delta_of_one, NULL));
    size_t result;
    assert_true(coral_range_hash_code(object, &result));
    assert_int_equal((values.first >> 1) + (values.last >> 1), result);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_is_equal(NULL, (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_range_is_equal((void *) 1, (void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *object;
    assert_true(coral_range_alloc(&object));
    struct coral_range_values values = {50, 100};
    assert_true(coral_range_init(object, values, step_func_delta_of_one, NULL));
    size_t object_hash_code;
    assert_true(coral_range_hash_code(object, &object_hash_code));
    struct coral_range *other;
    assert_true(coral_range_alloc(&other));
    assert_true(coral_range_init(other, values, step_func_delta_of_one, NULL));
    size_t other_hash_code;
    assert_true(coral_range_hash_code(other, &other_hash_code));
    assert_ptr_not_equal(object, other);
    assert_int_equal(object_hash_code, other_hash_code);
    bool result;
    assert_true(coral_range_is_equal(object, other, &result));
    assert_true(result);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_invalidate_error_on_null_object_ptr),
            cmocka_unit_test(check_invalidate),
            cmocka_unit_test(check_init_error_on_null_object_ptr),
            cmocka_unit_test(check_init_error_on_null_argument_ptr),
            cmocka_unit_test(check_init),
            cmocka_unit_test(check_get_first_error_on_null_object_ptr),
            cmocka_unit_test(check_get_first_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_first),
            cmocka_unit_test(check_get_last_error_on_null_object_ptr),
            cmocka_unit_test(check_get_last_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_last),
            cmocka_unit_test(check_get_minimum_error_on_null_object_ptr),
            cmocka_unit_test(check_get_minimum_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_minimum),
            cmocka_unit_test(check_get_maximum_error_on_null_object_ptr),
            cmocka_unit_test(check_get_maximum_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_maximum),
            cmocka_unit_test(check_is_inclusive_error_on_null_object_ptr),
            cmocka_unit_test(check_is_inclusive_error_on_null_argument_ptr),
            cmocka_unit_test(check_is_inclusive),
            cmocka_unit_test(check_get_next_error_on_null_object_ptr),
            cmocka_unit_test(check_get_next_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_next_error_on_invalid_value),
            cmocka_unit_test(check_get_next_error_on_end_of_sequence),
            cmocka_unit_test(check_get_next),
            cmocka_unit_test(check_get_prev_error_on_null_object_ptr),
            cmocka_unit_test(check_get_prev_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_prev_error_on_invalid_value),
            cmocka_unit_test(check_get_prev_error_on_end_of_sequence),
            cmocka_unit_test(check_get_prev),
            cmocka_unit_test(check_object_class_error_on_null_argument),
            cmocka_unit_test(check_object_class),
            cmocka_unit_test(check_object_destroy_error_on_null_object_ptr),
            cmocka_unit_test(check_object_alloc_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_alloc),
            cmocka_unit_test(check_object_init_error_on_null_object_ptr),
            cmocka_unit_test(check_object_init_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_init),
            cmocka_unit_test(check_object_get_first_error_on_null_object_ptr),
            cmocka_unit_test(check_object_get_first_error_on_argument_ptr),
            cmocka_unit_test(check_object_get_first_error_on_object_uninitialized),
            cmocka_unit_test(check_object_get_first),
            cmocka_unit_test(check_object_get_last_error_on_null_object_ptr),
            cmocka_unit_test(check_object_get_last_error_on_argument_ptr),
            cmocka_unit_test(check_object_get_last_error_on_object_uninitialized),
            cmocka_unit_test(check_object_get_last),
            cmocka_unit_test(check_object_get_minimum_error_on_null_object_ptr),
            cmocka_unit_test(check_object_get_minimum_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_get_minimum_error_on_object_uninitialized),
            cmocka_unit_test(check_object_get_minimum),
            cmocka_unit_test(check_object_get_maximum_error_on_null_object_ptr),
            cmocka_unit_test(check_object_get_maximum_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_get_maximum_error_on_object_uninitialized),
            cmocka_unit_test(check_object_get_maximum),
            cmocka_unit_test(check_object_is_inclusive_error_on_null_object_ptr),
            cmocka_unit_test(check_object_is_inclusive_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_is_inclusive_error_on_object_uninitialized),
            cmocka_unit_test(check_object_is_inclusive),
            cmocka_unit_test(check_object_get_next_error_on_null_object_ptr),
            cmocka_unit_test(check_object_get_next_error_on_argument_ptr),
            cmocka_unit_test(check_object_get_next_error_on_invalid_value),
            cmocka_unit_test(check_object_get_next_error_on_end_of_sequence),
            cmocka_unit_test(check_object_get_next_error_on_object_uninitialized),
            cmocka_unit_test(check_object_get_next),
            cmocka_unit_test(check_object_get_prev_error_on_null_object_ptr),
            cmocka_unit_test(check_object_get_prev_error_on_argument_ptr),
            cmocka_unit_test(check_object_get_prev_error_on_invalid_value),
            cmocka_unit_test(check_object_get_prev_error_on_end_of_sequence),
            cmocka_unit_test(check_object_get_prev_error_on_object_uninitialized),
            cmocka_unit_test(check_object_get_prev),
            cmocka_unit_test(check_object_of_delta_error_on_null_object_ptr),
            cmocka_unit_test(check_object_of_delta_error_on_invalid_value),
            cmocka_unit_test(check_object_of_delta),
            cmocka_unit_test(check_object_of_rate_error_on_null_object_ptr),
            cmocka_unit_test(check_object_of_rate_error_on_invalid_value),
            cmocka_unit_test(check_object_of_rate),
            cmocka_unit_test(check_object_hash_code_error_on_null_object_ptr),
            cmocka_unit_test(check_object_hash_code_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_hash_code_error_on_object_uninitialized),
            cmocka_unit_test(check_object_hash_code),
            cmocka_unit_test(check_object_is_equal_error_on_null_object_ptr),
            cmocka_unit_test(check_object_is_equal_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_is_equal),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
