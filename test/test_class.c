#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <coral.h>

#include "private/class.h"
#include "test/cmocka.h"

static void check_object_destroy_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_class_destroy(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_alloc_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_class_alloc(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_alloc(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *object;
    assert_true(coral_class_alloc(&object));
    assert_non_null(object);
    assert_true(coral_class_destroy(object));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_class_init(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *object = NULL;
    assert_true(coral_class_alloc(&object));
    assert_non_null(object);
    assert_true(coral_class_init(object));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_class_hash_code(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_class_hash_code((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *object;
    assert_true(coral_class_alloc(&object));
    size_t hash;
    assert_false(coral_class_hash_code(object, &hash));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_class_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *object;
    assert_true(coral_class_alloc(&object));
    assert_true(coral_class_init(object));
    size_t hash;
    assert_true(coral_class_hash_code(object, &hash));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_class_is_equal(NULL, (void *)1, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_class_is_equal((void *)1, NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_class_is_equal((void *)1, (void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *i, *o;
    assert_true(coral_class_alloc(&i));
    assert_true(coral_class_init(i));
    assert_true(coral_class_alloc(&o));
    assert_true(coral_class_init(o));
    bool result;
    assert_true(coral_class_is_equal(i, i, &result));
    assert_true(result);
    assert_true(coral_class_is_equal(o, i, &result));
    assert_false(result);
    assert_true(coral_class_is_equal(o, o, &result));
    assert_true(result);
    assert_true(coral_class_is_equal(i, o, &result));
    assert_false(result);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_retain_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_class_retain(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_retain_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *object;
    assert_true(coral_class_alloc(&object));
    assert_false(coral_class_retain(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_class_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_release_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_class_release(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_release_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *object;
    assert_true(coral_class_alloc(&object));
    assert_false(coral_class_release(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_class_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_autorelease_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_class_autorelease(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_autorelease_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *object;
    assert_true(coral_class_alloc(&object));
    assert_false(coral_class_autorelease(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_class_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_method_add_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_class_method_add(NULL, (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_method_add_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_class_method_add((void *) 1, NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_class_method_add((void *) 1, (void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_method_add_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    char *METHOD_NAME_BOB = "bob";
    struct coral_class_method_name name = {};
    assert_false(coral_class_method_add((void *) 1, &name, (void *) 1));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    name.data = METHOD_NAME_BOB;
    assert_false(coral_class_method_add((void *) 1, &name, (void *) 1));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    name.size = strlen(METHOD_NAME_BOB);
    name.data = NULL;
    assert_false(coral_class_method_add((void *) 1, &name, (void *) 1));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_method_add_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    char *METHOD_NAME_CHARLIE = "charlie";
    struct coral_class_method_name name = {
                .data = METHOD_NAME_CHARLIE,
                .size = strlen(METHOD_NAME_CHARLIE)
    };
    struct coral_class *object;
    assert_true(coral_class_alloc(&object));
    assert_false(coral_class_method_add(object, &name, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_class_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_method_add(void **state) {
    coral_error = CORAL_ERROR_NONE;
    char *METHOD_NAME_CHARLIE = "charlie";
    struct coral_class_method_name name = {
            .data = METHOD_NAME_CHARLIE,
            .size = strlen(METHOD_NAME_CHARLIE)
    };
    struct coral_class *object;
    assert_true(coral_class_alloc(&object));
    assert_true(coral_class_init(object));
    assert_true(coral_class_method_add(object, &name, (void *) 1));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_method_add_error_on_method_already_exists(void **state) {
    coral_error = CORAL_ERROR_NONE;
    char *METHOD_NAME_DANIEL = "daniel";
    struct coral_class_method_name i = {
            .data = METHOD_NAME_DANIEL,
            .size = strlen(METHOD_NAME_DANIEL)
    };
    struct coral_class *object;
    assert_true(coral_class_alloc(&object));
    assert_true(coral_class_init(object));
    assert_true(coral_class_method_add(object, &i, (void *) 1));
    struct coral_class_method_name o = i;
    assert_ptr_not_equal(&i, &o);
    assert_false(coral_class_method_add(object, &o, (void *) 2));
    assert_int_equal(CORAL_ERROR_METHOD_ALREADY_EXISTS, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_method_remove_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_class_method_remove(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_method_remove_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_class_method_remove((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_method_remove_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    char *METHOD_NAME_ME = "me";
    struct coral_class_method_name name = {
            .data = METHOD_NAME_ME,
            .size = strlen(METHOD_NAME_ME)
    };
    struct coral_class *object;
    assert_true(coral_class_alloc(&object));
    assert_false(coral_class_method_remove(object, &name));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_class_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_method_remove_error_on_method_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    char *METHOD_NAME_ME = "me";
    struct coral_class_method_name name = {
            .data = METHOD_NAME_ME,
            .size = strlen(METHOD_NAME_ME)
    };
    struct coral_class *object;
    assert_true(coral_class_alloc(&object));
    assert_true(coral_class_init(object));
    assert_false(coral_class_method_remove(object, &name));
    assert_int_equal(CORAL_ERROR_METHOD_NOT_FOUND, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_method_remove(void **state) {
    coral_error = CORAL_ERROR_NONE;
    char *METHOD_NAME_ALLAN = "allan";
    struct coral_class_method_name i = {
            .data = METHOD_NAME_ALLAN,
            .size = strlen(METHOD_NAME_ALLAN)
    };
    struct coral_class *object;
    assert_true(coral_class_alloc(&object));
    assert_true(coral_class_init(object));
    assert_true(coral_class_method_add(object, &i, (void *) 1));
    assert_true(coral_class_method_remove(object, &i));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_method_get_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_class_method_get(NULL, (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_method_get_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_class_method_get((void *) 1, NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_class_method_get((void *) 1, (void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_method_get_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class_method_name i = {};
    assert_false(coral_class_method_add((void *) 1, &i, (void *) 1));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    i.data = (void *) 1;
    assert_false(coral_class_method_add((void *) 1, &i, (void *) 1));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    i.data = NULL;
    i.size = 1;
    assert_false(coral_class_method_add((void *) 1, &i, (void *) 1));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_method_get_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    char *METHOD_NAME_FRED = "fred";
    struct coral_class_method_name i = {
            .data = METHOD_NAME_FRED,
            .size = strlen(i.data)
    };
    struct coral_class *object;
    assert_true(coral_class_alloc(&object));
    coral_invokable_t func = NULL;
    assert_false(coral_class_method_get(object, &i, &func));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_class_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_method_get_error_on_method_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    char *METHOD_NAME_FRED = "fred";
    struct coral_class_method_name i = {
            .data = METHOD_NAME_FRED,
            .size = strlen(i.data)
    };
    struct coral_class *object;
    assert_true(coral_class_alloc(&object));
    assert_true(coral_class_init(object));
    coral_invokable_t func = NULL;
    assert_false(coral_class_method_get(object, &i, &func));
    assert_int_equal(CORAL_ERROR_METHOD_NOT_FOUND, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_method_get(void **state) {
    coral_error = CORAL_ERROR_NONE;
    char *METHOD_NAME_FRED = "fred";
    struct coral_class_method_name i = {
            .data = METHOD_NAME_FRED,
            .size = strlen(i.data)
    };
    struct coral_class *object;
    assert_true(coral_class_alloc(&object));
    assert_true(coral_class_init(object));
    coral_invokable_t func = NULL;
    assert_true(coral_class_method_add(object, &i, (void *) 10));
    assert_true(coral_class_method_get(object, &i, &func));
    assert_ptr_equal(10, func);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_object_destroy_error_on_null_object_ptr),
            cmocka_unit_test(check_object_alloc_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_alloc),
            cmocka_unit_test(check_object_init_error_on_null_object_ptr),
            cmocka_unit_test(check_object_init),
            cmocka_unit_test(check_object_hash_code_error_on_null_object_ptr),
            cmocka_unit_test(check_object_hash_code_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_hash_code_error_on_object_uninitialized),
            cmocka_unit_test(check_object_hash_code),
            cmocka_unit_test(check_object_is_equal_error_on_null_object_ptr),
            cmocka_unit_test(check_object_is_equal_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_is_equal),
            cmocka_unit_test(check_object_retain_error_on_null_object_ptr),
            cmocka_unit_test(check_object_retain_error_on_object_uninitialized),
            cmocka_unit_test(check_object_release_error_on_null_object_ptr),
            cmocka_unit_test(check_object_release_error_on_object_uninitialized),
            cmocka_unit_test(check_object_autorelease_error_on_null_object_ptr),
            cmocka_unit_test(check_object_autorelease_error_on_object_uninitialized),
            cmocka_unit_test(check_object_method_add_error_on_null_object_ptr),
            cmocka_unit_test(check_object_method_add_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_method_add_error_on_invalid_value),
            cmocka_unit_test(check_object_method_add_error_on_object_uninitialized),
            cmocka_unit_test(check_object_method_add),
            cmocka_unit_test(check_object_method_add_error_on_method_already_exists),
            cmocka_unit_test(check_object_method_remove_error_on_null_object_ptr),
            cmocka_unit_test(check_object_method_remove_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_method_remove_error_on_object_uninitialized),
            cmocka_unit_test(check_object_method_remove_error_on_method_not_found),
            cmocka_unit_test(check_object_method_remove),
            cmocka_unit_test(check_object_method_get_error_on_null_object_ptr),
            cmocka_unit_test(check_object_method_get_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_method_get_error_on_invalid_value),
            cmocka_unit_test(check_object_method_get_error_on_object_uninitialized),
            cmocka_unit_test(check_object_method_get_error_on_method_not_found),
            cmocka_unit_test(check_object_method_get),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
