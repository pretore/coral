#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <coral.h>
#include <limits.h>

#include "private/coral.h"
#include "test/cmocka.h"
#include "test/wrap.h"

static void check_compare_void_ptr(void **state) {
    assert_int_equal(-1, coral_compare_void_ptr((void *) 1, (void *) 2));
    assert_int_equal(0, coral_compare_void_ptr((void *) 1, (void *) 1));
    assert_int_equal(1, coral_compare_void_ptr((void *) 3, (void *) 2));
}

static void check_compare_size_t_ptr(void **state) {
    size_t A = 0;
    size_t B = 1;
    assert_int_equal(-1, coral_compare_size_t(&A, &B));
    A = B;
    assert_int_equal(0, coral_compare_size_t(&A, &B));
    A += 1;
    assert_int_equal(1, coral_compare_size_t(&A, &B));
}

static void check_add_size_t_error_on_argument_is_null(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_add_size_t(0, 0, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_add_size_t_with_minimum(void **state) {
    coral_error = CORAL_ERROR_NONE;
    size_t result = 10;
    assert_true(coral_add_size_t(0, 0, &result));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_add_size_t_with_maximum(void **state) {
    coral_error = CORAL_ERROR_NONE;
    size_t result = 88;
    assert_true(coral_add_size_t(SIZE_MAX, 0, &result));
    assert_int_equal(SIZE_MAX, result);
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    result = 100;
    assert_true(coral_add_size_t(0, SIZE_MAX, &result));
    assert_int_equal(SIZE_MAX, result);
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    result = 56;
    assert_false(coral_add_size_t(SIZE_MAX, SIZE_MAX, &result));
    assert_int_equal(56, result);
    assert_int_equal(CORAL_ERROR_OVERFLOW_OCCURRED, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_multiply_size_t_error_on_argument_is_null(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_multiply_size_t(0, 0, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_multiply_size_t_with_minimum(void **state) {
    coral_error = CORAL_ERROR_NONE;
    size_t result = 100;
    assert_true(coral_multiply_size_t(SIZE_MAX, 0, &result));
    assert_int_equal(0, result);
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    result = 88;
    assert_true(coral_multiply_size_t(0, SIZE_MAX, &result));
    assert_int_equal(0, result);
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    result = 11;
    assert_true(coral_multiply_size_t(0, 0, &result));
    assert_int_equal(0, result);
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_multiply_size_t_with_maximum(void **state) {
    coral_error = CORAL_ERROR_NONE;
    size_t result = 100;
    assert_false(coral_multiply_size_t(SIZE_MAX, SIZE_MAX, &result));
    assert_int_equal(100, result);
    assert_int_equal(CORAL_ERROR_OVERFLOW_OCCURRED, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_minimum_size_t_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_minimum_size_t(0, 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_minimum_size_t(void **state) {
    coral_error = CORAL_ERROR_NONE;
    size_t out;
    assert_true(coral_minimum_size_t(1, 2, &out));
    assert_int_equal(1, out);
    assert_true(coral_minimum_size_t(5, 3, &out));
    assert_int_equal(3, out);
    coral_error = CORAL_ERROR_NONE;
}

static void check_maximum_size_t_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_maximum_size_t(0, 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_maximum_size_t(void **state) {
    coral_error = CORAL_ERROR_NONE;
    size_t out;
    assert_true(coral_maximum_size_t(1, 2, &out));
    assert_int_equal(2, out);
    assert_true(coral_maximum_size_t(5, 3, &out));
    assert_int_equal(5, out);
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_reference_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_set_reference(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    struct coral_reference *ref;
    assert_false(coral_set_reference(&ref, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_reference(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *object;
    assert_true(coral_integer_of_size_t(&object, 10));
    struct coral_reference *ref;
    assert_true(coral_set_reference(&ref, object));
    assert_true(coral_reference_release(ref));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_clear_reference_error_on_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_reference *ref;
    assert_true(coral_reference_alloc(&ref));
    assert_false(coral_clear_reference(&ref));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_reference_destroy(ref));
    coral_error = CORAL_ERROR_NONE;
}

static void check_clear_reference(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_true(coral_clear_reference(NULL));
    struct coral_reference *ref = NULL;
    assert_true(coral_clear_reference(&ref));
    struct coral_integer *object;
    assert_true(coral_integer_of_size_t(&object, 28319));
    assert_true(coral_set_reference(&ref, object));
    assert_true(coral_clear_reference(&ref));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_weak_reference_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    struct coral_object *object;
    assert_true(coral_object_alloc(0, (void **) &object));
    assert_true(coral_object_init(object, class));
    assert_false(coral_set_weak_reference(NULL, object));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    struct coral_weak_reference *ref;
    assert_false(coral_set_weak_reference(&ref, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_weak_reference(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    struct coral_object *object;
    assert_true(coral_object_alloc(0, (void **) &object));
    assert_true(coral_object_init(object, class));
    struct coral_weak_reference *ref;
    assert_true(coral_set_weak_reference(&ref, object));
    assert_true(coral_weak_reference_release(ref));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_clear_weak_reference_error_on_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_weak_reference *ref;
    assert_true(coral_weak_reference_alloc(&ref));
    assert_false(coral_clear_weak_reference(&ref));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_weak_reference_destroy(ref));
    coral_error = CORAL_ERROR_NONE;
}

static void check_clear_weak_reference(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    assert_true(coral_clear_weak_reference(NULL));
    struct coral_weak_reference *ref = NULL;
    assert_true(coral_clear_weak_reference(&ref));
    struct coral_integer *object;
    assert_true(coral_integer_of_size_t(&object, 821739));
    assert_true(coral_set_weak_reference(&ref, object));
    assert_true(coral_clear_weak_reference(&ref));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void
check_exponential_usleep_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_exponential_usleep(NULL, 0));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_exponential_usleep_error_on_invalid_maximum_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    uint8_t state_ = 100;
    assert_false(coral_exponential_usleep(&state_, 0));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    assert_int_equal(100, state_);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_exponential_usleep_error_on_invalid_state_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    uint8_t state_ = UINT8_MAX;
    assert_false(coral_exponential_usleep(&state_, 999999));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    assert_int_equal(0, state_);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_exponential_usleep_error_on_boundary_state_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    uint8_t state_ = CHAR_BIT * sizeof(unsigned int) - 1;
    assert_false(coral_exponential_usleep(&state_, 999999));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    assert_int_equal(0, state_);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_exponential_usleep_error_on_failed_syscall(void **state) {
    coral_error = CORAL_ERROR_NONE;
    nanosleep_is_overridden = true;
    will_return(__wrap_nanosleep, -1);
    uint8_t state_ = 0;
    assert_false(coral_exponential_usleep(&state_, 999999));
    assert_int_equal(CORAL_ERROR_SYSCALL_FAILED, coral_error);
    assert_int_equal(0, state_);
    nanosleep_is_overridden = false;
    coral_error = CORAL_ERROR_NONE;
}

static void check_exponential_usleep_success(void **state) {
    coral_error = CORAL_ERROR_NONE;
    uint8_t state_ = 0;
    for (uint8_t i = 0; CORAL_ERROR_NONE == coral_error && i < 10;
         i++, assert_true(coral_exponential_usleep(&state_, 999999)));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    assert_int_equal(10, state_);
    coral_error = CORAL_ERROR_NONE;
}

static void check_swap_void_ptr(void **state) {
    void *values[] = {(void *) 1, (void *) 2};
    coral$swap_void_ptr(&values[0], &values[1]);
    assert_ptr_equal((void *) 2, values[0]);
    assert_ptr_equal((void *) 1, values[1]);
}

static void check_swap_size_t(void **state) {
    size_t values[] = {1, 2};
    coral$swap_size_t(&values[0], &values[1]);
    assert_int_equal(2, values[0]);
    assert_int_equal(1, values[1]);
}

static void check_atomic_load(void **state) {
    atomic_size_t value = 1;
    assert_int_equal(value, coral$atomic_load(&value));
}

static void check_atomic_store(void **state) {
    atomic_size_t value = 10;
    assert_int_equal(10, coral$atomic_load(&value));
    coral$atomic_store(&value, 100);
    assert_int_equal(100, coral$atomic_load(&value));
}

static void check_atomic_compare_exchange(void **state) {
    atomic_size_t value = 13;
    assert_true(coral$atomic_compare_exchange(&value, 13, 100));
    assert_int_equal(100, coral$atomic_load(&value));
    assert_false(coral$atomic_compare_exchange(&value, 13, 100));
}

static void check_atomic_load_ptr(void **state) {
    atomic_uintptr_t ptr = 5;
    assert_ptr_equal(ptr, coral$atomic_load_ptr(&ptr));
}

static void check_atomic_store_ptr(void **state) {
    atomic_uintptr_t ptr = 100;
    assert_ptr_equal(100, coral$atomic_load_ptr(&ptr));
    coral$atomic_store_ptr(&ptr, 10);
    assert_ptr_equal(10, coral$atomic_load_ptr(&ptr));
}

static void check_atomic_compare_exchange_ptr(void **state) {
    atomic_uintptr_t ptr = 40;
    assert_true(coral$atomic_compare_exchange_ptr(&ptr, 40, 120));
    assert_ptr_equal(120, coral$atomic_load_ptr(&ptr));
    assert_false(coral$atomic_compare_exchange_ptr(&ptr, 40, 120));
}

static void check_retain(void **state) {
    atomic_size_t ref_counter = 1;
    coral$retain(&ref_counter);
    assert_int_equal(2, ref_counter);
}

static void object$destroy(void *object) {
    function_called();
    free(object);
}

static void check_release(void **state) {
    struct object_t {
        atomic_size_t ref_counter;
    };
    struct object_t *obj = calloc(1, sizeof(*obj));
    assert_true(obj);
    assert_int_equal(0, atomic_fetch_add(&obj->ref_counter, 2));
    expect_function_calls(object$destroy, 1);
    /* should only decrease reference count */
    coral$release(obj, &obj->ref_counter, object$destroy);
    assert_int_equal(1, coral$atomic_load(&obj->ref_counter));
    /* will call destroy which will free the object */
    coral$release(obj, &obj->ref_counter, object$destroy);
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_compare_void_ptr),
            cmocka_unit_test(check_compare_size_t_ptr),
            cmocka_unit_test(check_add_size_t_error_on_argument_is_null),
            cmocka_unit_test(check_add_size_t_with_minimum),
            cmocka_unit_test(check_add_size_t_with_maximum),
            cmocka_unit_test(check_multiply_size_t_error_on_argument_is_null),
            cmocka_unit_test(check_multiply_size_t_with_minimum),
            cmocka_unit_test(check_multiply_size_t_with_maximum),
            cmocka_unit_test(check_minimum_size_t_error_on_null_argument_ptr),
            cmocka_unit_test(check_minimum_size_t),
            cmocka_unit_test(check_maximum_size_t_error_on_null_argument_ptr),
            cmocka_unit_test(check_maximum_size_t),
            cmocka_unit_test(check_set_reference_error_on_null_argument_ptr),
            cmocka_unit_test(check_set_reference),
            cmocka_unit_test(check_clear_reference_error_on_uninitialized),
            cmocka_unit_test(check_clear_reference),
            cmocka_unit_test(check_set_weak_reference_error_on_null_argument_ptr),
            cmocka_unit_test(check_set_weak_reference),
            cmocka_unit_test(check_clear_weak_reference_error_on_uninitialized),
            cmocka_unit_test(check_clear_weak_reference),
            cmocka_unit_test(check_exponential_usleep_error_on_null_argument_ptr),
            cmocka_unit_test(check_exponential_usleep_error_on_invalid_maximum_value),
            cmocka_unit_test(check_exponential_usleep_error_on_invalid_state_value),
            cmocka_unit_test(check_exponential_usleep_error_on_boundary_state_value),
            cmocka_unit_test(check_exponential_usleep_error_on_failed_syscall),
            cmocka_unit_test(check_exponential_usleep_success),
            cmocka_unit_test(check_swap_void_ptr),
            cmocka_unit_test(check_swap_size_t),
            cmocka_unit_test(check_atomic_load),
            cmocka_unit_test(check_atomic_store),
            cmocka_unit_test(check_atomic_compare_exchange),
            cmocka_unit_test(check_atomic_load_ptr),
            cmocka_unit_test(check_atomic_store_ptr),
            cmocka_unit_test(check_atomic_compare_exchange_ptr),
            cmocka_unit_test(check_retain),
            cmocka_unit_test(check_release),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}