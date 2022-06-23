#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <errno.h>
#include <coral.h>

#include "private/coral.h"
#include "private/object.h"
#include "test/wrap.h"

static void check_object_class_error_on_null_argument(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_object_class(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_class(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    assert_non_null(class);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_alloc_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_object_alloc(0, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_alloc(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *instance = 0;
    assert_true(coral_object_alloc(0, &instance));
    assert_non_null(instance);
    assert_true(coral_object_destroy(instance));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_object_init(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_object_init((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init_error_on_initialization(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *instance = NULL;
    assert_true(coral_object_alloc(0, (void **) &instance));
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    assert_true(coral_object_init(instance, class));
    assert_false(coral_object_init(instance, class));
    assert_int_equal(CORAL_ERROR_INITIALIZATION_FAILED, coral_error);
    assert_true(coral_object_destroy(instance));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init(void **state) {
    void *instance = NULL;
    assert_true(coral_object_alloc(0, (void **) &instance));
    struct coral_class *class = NULL;
    coral_required_true(coral_object_class(&class));
    coral_error = CORAL_ERROR_NONE;
    assert_true(coral_object_init(instance, class));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_destroy_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_object_destroy(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_destroy(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *instance = NULL;
    assert_true(coral_object_alloc(0, &instance));
    coral_object_destroy(instance);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_retain_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *instance = NULL;
    assert_true(coral_object_alloc(0, &instance));
    assert_false(coral_object_retain(instance));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_object_destroy(instance);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_release_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *instance = NULL;
    assert_true(coral_object_alloc(0, &instance));
    assert_false(coral_object_release(instance));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_object_destroy(instance);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_autorelease_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *instance = NULL;
    assert_true(coral_object_alloc(0, &instance));
    assert_false(coral_object_autorelease(instance));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_object_destroy(instance);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_retain_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_object_retain(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_release_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_object_release(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_retain_and_release(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_object *object = NULL;
    assert_true(coral_object_alloc(0, (void **) &object));
    struct coral_object *object_ = coral$object_from(object);
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    assert_true(coral_object_init(object, class));
    size_t count;
    assert_true(coral$object$get_ref_count(object_, &count));
    assert_int_equal(1, count);
    assert_true(coral_object_retain(object));
    assert_true(coral$object$get_ref_count(object_, &count));
    assert_int_equal(2, count);
    assert_true(coral_object_release(object));
    assert_true(coral$object$get_ref_count(object_, &count));
    assert_int_equal(1, count);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_object_is_equal(NULL, (void *)1, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_object_is_equal((void *)1, NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_object_is_equal((void *)1, (void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *i;
    assert_true(coral_object_alloc(0, &i));
    bool result;
    assert_false(coral_object_is_equal(i, i, &result));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_object_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *i, *o;
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    assert_true(coral_object_alloc(0, &o));
    assert_true(coral_object_init(o, class));
    bool result;
    assert_true(coral_object_is_equal(i, o, &result));
    assert_false(result);
    assert_true(coral_object_is_equal(i, i, &result));
    assert_true(result);
    assert_true(coral_object_is_equal(o, i, &result));
    assert_false(result);
    assert_true(coral_object_is_equal(o, o, &result));
    assert_true(result);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_dispatch_is_equal_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *i;
    assert_true(coral_object_alloc(0, &i));
    bool result;
    struct is_equal_args args = {
            .out = &result
    };
    args.other = i;
    assert_false(coral_object_dispatch(i, true, is_equal, &args));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_object_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_dispatch_is_equal(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *i, *o;
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    assert_true(coral_object_alloc(0, &o));
    assert_true(coral_object_init(o, class));
    bool result;
    struct is_equal_args args = {
            .out = &result
    };
    args.other = o;
    assert_true(coral_object_dispatch(i, true, is_equal, &args));
    assert_false(result);
    args.other = i;
    assert_true(coral_object_dispatch(i, true, is_equal, &args));
    assert_true(result);
    args.other = o;
    assert_true(coral_object_dispatch(o, true, is_equal, &args));
    assert_true(result);
    args.other = i;
    assert_true(coral_object_dispatch(o, true, is_equal, &args));
    assert_false(result);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_object_hash_code(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_object_hash_code((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *i;
    assert_true(coral_object_alloc(0, &i));
    size_t result;
    assert_false(coral_object_hash_code(i, &result));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_object_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *i;
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    size_t result = 0;
    assert_true(coral_object_hash_code(i, &result));
    assert_int_not_equal(result, 0);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_dispatch_hash_code_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *i;
    assert_true(coral_object_alloc(0, &i));
    size_t result;
    struct hash_code_args args = {
            .out = &result
    };
    assert_false(coral_object_dispatch(i, true, hash_code, &args));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_object_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_dispatch_hash_code(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *i;
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    size_t result = 0;
    struct hash_code_args args = {
            .out = &result
    };
    assert_true(coral_object_dispatch(i, true, hash_code, &args));
    assert_int_not_equal(result, 0);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_copy_with_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_object_copy(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_copy_with_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_object_copy((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_copy_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *i = check_object_copy_error_on_invalid_value;
    assert_false(coral_object_copy(i, &i));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_copy_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *i, *o = NULL;
    assert_true(coral_object_alloc(0, &i));
    assert_false(coral_object_copy(i, &o));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_object_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_copy(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    // TODO: implement an object copy test
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_object_class_error_on_null_argument),
            cmocka_unit_test(check_object_class),
            cmocka_unit_test(check_object_alloc_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_alloc),
            cmocka_unit_test(check_object_init_error_on_null_object_ptr),
            cmocka_unit_test(check_object_init_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_init_error_on_initialization),
            cmocka_unit_test(check_object_init),
            cmocka_unit_test(check_object_destroy_error_on_null_object_ptr),
            cmocka_unit_test(check_object_destroy),
            cmocka_unit_test(check_object_retain_error_on_object_uninitialized),
            cmocka_unit_test(check_object_release_error_on_object_uninitialized),
            cmocka_unit_test(check_object_autorelease_error_on_object_uninitialized),
            cmocka_unit_test(check_object_retain_error_on_null_object_ptr),
            cmocka_unit_test(check_object_release_error_on_null_object_ptr),
            cmocka_unit_test(check_object_retain_and_release),
            cmocka_unit_test(check_object_is_equal_error_on_null_object_ptr),
            cmocka_unit_test(check_object_is_equal_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_is_equal_error_on_object_uninitialized),
            cmocka_unit_test(check_object_is_equal),
            cmocka_unit_test(check_object_dispatch_is_equal_error_on_object_uninitialized),
            cmocka_unit_test(check_object_dispatch_is_equal),
            cmocka_unit_test(check_object_hash_code_error_on_null_object_ptr),
            cmocka_unit_test(check_object_hash_code_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_hash_code_error_on_object_uninitialized),
            cmocka_unit_test(check_object_hash_code),
            cmocka_unit_test(check_object_dispatch_hash_code_error_on_object_uninitialized),
            cmocka_unit_test(check_object_dispatch_hash_code),
            cmocka_unit_test(check_object_copy_with_null_object_ptr),
            cmocka_unit_test(check_object_copy_with_null_argument_ptr),
            cmocka_unit_test(check_object_copy_error_on_invalid_value),
            cmocka_unit_test(check_object_copy_error_on_object_uninitialized),
            cmocka_unit_test(check_object_copy)
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
