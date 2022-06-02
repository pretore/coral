#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <coral.h>

#include "private/weak_reference.h"
#include "private/autorelease_pool.h"
#include "private/object.h"
#include "private/coral.h"

static void check_invalidate_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$weak_reference$invalidate(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$weak_reference$init(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_init_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$weak_reference$init((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    struct coral_object *i;
    assert_true(coral_object_alloc(0, (void **)&i));
    assert_true(coral_object_init(i, class));
    struct coral$weak_reference o = {};
    assert_true(coral$weak_reference$init(&o, i));
    assert_true(coral$weak_reference$invalidate(&o));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$weak_reference$get(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$weak_reference$get((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    struct coral_object *i;
    assert_true(coral_object_alloc(0, (void **)&i));
    assert_true(coral_object_init(i, class));
    struct coral$weak_reference o = {};
    assert_true(coral$weak_reference$init(&o, i));
    o.object = NULL;
    struct coral_object *u = NULL;
    assert_false(coral$weak_reference$get(&o, (void **)&u));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    assert_true(coral$weak_reference$invalidate(&o));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_get(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    struct coral_object *i;
    assert_true(coral_object_alloc(0, (void **)&i));
    assert_true(coral_object_init(i, class));
    struct coral$weak_reference o = {};
    assert_true(coral$weak_reference$init(&o, i));
    struct coral_object *u = NULL;
    coral$autorelease_pool$start();
    assert_true(coral$weak_reference$get(&o, (void **)&u));
    coral$autorelease_pool$end();
    assert_non_null(u);
    assert_ptr_equal(i, u);
    assert_true(coral$weak_reference$invalidate(&o));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_class_error_on_null_argument(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_weak_reference_class(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_class(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_weak_reference_class(&class));
    assert_non_null(class);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_alloc_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_weak_reference_alloc(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_alloc(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_weak_reference *i = NULL;
    assert_true(coral_weak_reference_alloc(&i));
    assert_non_null(i);
    assert_true(coral_weak_reference_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_destroy_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_weak_reference_destroy(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_destroy(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_weak_reference *i = NULL;
    assert_true(coral_weak_reference_alloc(&i));
    assert_non_null(i);
    assert_true(coral_weak_reference_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_weak_reference_init(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_weak_reference_init((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class;
    assert_true(coral_class_alloc(&class));
    assert_true(coral_class_init(class));
    struct coral_weak_reference *i;
    assert_true(coral_weak_reference_alloc(&i));
    assert_true(coral_weak_reference_init(i, class));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_of_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_weak_reference_of(NULL, (void*)1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    struct coral_weak_reference *i;
    assert_false(coral_weak_reference_of(&i, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_of(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class;
    assert_true(coral_weak_reference_class(&class));
    struct coral_weak_reference *i = NULL;
    assert_true(coral_weak_reference_of(&i, class));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_weak_reference_hash_code(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_weak_reference_hash_code((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_weak_reference *i;
    assert_true(coral_weak_reference_alloc(&i));
    size_t code;
    assert_false(coral_weak_reference_hash_code(i, &code));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_weak_reference_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_weak_reference *i;
    assert_true(coral_weak_reference_alloc(&i));
    struct coral_class *class;
    assert_true(coral_class_alloc(&class));
    assert_true(coral_class_init(class));
    assert_true(coral_weak_reference_init(i, class));
    size_t c;
    assert_true(coral_weak_reference_hash_code(i, &c));
    size_t v;
    assert_true(coral_class_hash_code(class, &v));
    assert_int_equal(c, v);
    assert_true(coral_object_hash_code(i, &c));
    assert_int_not_equal(c, v);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_weak_reference_is_equal(NULL, (void* )1, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_weak_reference_is_equal((void *)1, NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_weak_reference_is_equal((void *)1, (void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_weak_reference *i, *o;
    assert_true(coral_weak_reference_alloc(&i));
    assert_true(coral_weak_reference_alloc(&o));
    struct coral_class *class = NULL;
    assert_true(coral_weak_reference_class(&class));
    assert_true(coral_weak_reference_init(o, class));
    bool result;
    assert_false(coral_weak_reference_is_equal(i, o, &result));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_error = CORAL_ERROR_NONE;
    result = true;
    assert_true(coral_weak_reference_is_equal(o, i, &result));
    assert_false(result);
    assert_true(coral_weak_reference_destroy(i));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_weak_reference_class(&class));
    struct coral_weak_reference *i, *o;
    assert_true(coral_weak_reference_alloc(&i));
    assert_true(coral_weak_reference_init(i, class));
    assert_true(coral_object_class(&class));
    assert_true(coral_weak_reference_alloc(&o));
    assert_true(coral_weak_reference_init(o, class));
    bool result;
    assert_true(coral_weak_reference_is_equal(i, i, &result));
    assert_true(result);
    assert_true(coral_weak_reference_is_equal(i, o, &result));
    assert_false(result);
    assert_true(coral_weak_reference_is_equal(o, i, &result));
    assert_false(result);
    assert_true(coral_weak_reference_is_equal(o, o, &result));
    assert_true(result);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_copy_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_weak_reference_copy(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_copy_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_weak_reference_copy((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_copy_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_weak_reference *i = (void*) 1;
    assert_false(coral_weak_reference_copy(i, &i));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_copy_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_weak_reference *i, *o = NULL;
    assert_true(coral_weak_reference_alloc(&i));
    assert_false(coral_weak_reference_copy(i, &o));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_weak_reference_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_copy(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_weak_reference *i;
    assert_true(coral_weak_reference_alloc(&i));
    struct coral_class *class = NULL;
    assert_true(coral_weak_reference_class(&class));
    assert_true(coral_weak_reference_init(i, class));
    struct coral_weak_reference *o = NULL;
    assert_true(coral_weak_reference_copy(i, &o));
    assert_ptr_not_equal(i, o);
    bool result;
    assert_true(coral_weak_reference_is_equal(i, o, &result));
    assert_true(result);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_retain_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_weak_reference_retain(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_retain_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_weak_reference *i;
    assert_true(coral_weak_reference_alloc(&i));
    assert_false(coral_weak_reference_retain(i));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_weak_reference_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_release_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_weak_reference_release(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_release_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_weak_reference *i;
    assert_true(coral_weak_reference_alloc(&i));
    assert_false(coral_weak_reference_release(i));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_weak_reference_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_autorelease_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_weak_reference_autorelease(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_autorelease_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_weak_reference *i;
    assert_true(coral_weak_reference_alloc(&i));
    assert_false(coral_weak_reference_autorelease(i));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_weak_reference_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_weak_reference_get(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_weak_reference_get((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_weak_reference *i;
    // TODO: implement this test...
    // ... waiting on notifications to be implemented
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_weak_reference *i;
    assert_true(coral_weak_reference_alloc(&i));
    struct coral_class *o = NULL;
    assert_false(coral_weak_reference_get(i, (void **)&o));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_weak_reference_destroy(i));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_weak_reference *i;
    assert_true(coral_weak_reference_alloc(&i));
    struct coral_class *class = NULL;
    assert_true(coral_weak_reference_class(&class));
    assert_true(coral_weak_reference_init(i, class));
    struct coral_class *o = NULL;
    assert_true(coral_weak_reference_get(i, (void **)&o));
    assert_ptr_equal(class, o);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_invalidate_error_on_null_object_ptr),
            cmocka_unit_test(check_init_error_on_null_object_ptr),
            cmocka_unit_test(check_init_error_on_null_argument_ptr),
            cmocka_unit_test(check_init),
            cmocka_unit_test(check_get_error_on_null_object_ptr),
            cmocka_unit_test(check_get_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_error_on_object_not_found),
            cmocka_unit_test(check_get),
            cmocka_unit_test(check_object_class_error_on_null_argument),
            cmocka_unit_test(check_object_class),
            cmocka_unit_test(check_object_alloc_error_on_null_object_ptr),
            cmocka_unit_test(check_object_alloc),
            cmocka_unit_test(check_object_destroy_error_on_null_object_ptr),
            cmocka_unit_test(check_object_destroy),
            cmocka_unit_test(check_object_init_error_on_null_object_ptr),
            cmocka_unit_test(check_object_init_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_init),
            cmocka_unit_test(check_object_of_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_of),
            cmocka_unit_test(check_object_hash_code_error_on_null_object_ptr),
            cmocka_unit_test(check_object_hash_code_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_hash_code_error_on_object_uninitialized),
            cmocka_unit_test(check_object_hash_code),
            cmocka_unit_test(check_object_is_equal_error_on_null_object_ptr),
            cmocka_unit_test(check_object_is_equal_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_is_equal_error_on_object_uninitialized),
            cmocka_unit_test(check_object_is_equal),
            cmocka_unit_test(check_object_copy_error_on_null_object_ptr),
            cmocka_unit_test(check_object_copy_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_copy_error_on_invalid_value),
            cmocka_unit_test(check_object_copy_error_on_object_uninitialized),
            cmocka_unit_test(check_object_copy),
            cmocka_unit_test(check_object_retain_error_on_null_object_ptr),
            cmocka_unit_test(check_object_retain_error_on_object_uninitialized),
            cmocka_unit_test(check_object_release_error_on_null_object_ptr),
            cmocka_unit_test(check_object_release_error_on_object_uninitialized),
            cmocka_unit_test(check_object_autorelease_error_on_null_object_ptr),
            cmocka_unit_test(check_object_autorelease_error_on_object_uninitialized),
            cmocka_unit_test(check_object_get_error_on_null_object_ptr),
            cmocka_unit_test(check_object_get_error_on_null_argument_ptr),
            //cmocka_unit_test(check_object_get_error_on_object_not_found),
            cmocka_unit_test(check_object_get_error_on_object_uninitialized),
            cmocka_unit_test(check_object_get),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
