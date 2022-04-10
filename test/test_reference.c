#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <coral.h>

#include "private/reference.h"
#include "private/object.h"
#include "private/coral.h"

static void check_ref_alloc_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_ref_alloc(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_ref_alloc(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_ref *object;
    assert_true(coral_ref_alloc(&object));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    assert_null(object->object);
    assert_true(coral_ref_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_ref_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_ref_init(NULL, (void*)1));
    coral_error = CORAL_ERROR_NONE;
}

static void check_ref_init_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_ref *object;
    assert_true(coral_ref_alloc(&object));
    assert_false(coral_ref_init(object, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_true(coral_ref_destroy(object));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_ref_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_context *context;
    assert_true(coral_context_of(&context, NULL));
    struct coral_object* context_ = coral$object_from(context);
    assert_int_equal(1, coral$atomic_load(&context_->ref_count));
    struct coral_ref *object;
    assert_true(coral_ref_alloc(&object));
    assert_true(coral_ref_init(object, context));
    assert_int_equal(2, coral$atomic_load(&context_->ref_count));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_ref_retain_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_ref *object;
    assert_true(coral_ref_alloc(&object));
    assert_false(coral_ref_retain(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_ref_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_ref_release_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_ref *object;
    assert_true(coral_ref_alloc(&object));
    assert_false(coral_ref_release(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_ref_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_ref_autorelease_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_ref *object;
    assert_true(coral_ref_alloc(&object));
    assert_false(coral_ref_autorelease(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_ref_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_ref_get_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_ref *object;
    assert_true(coral_ref_alloc(&object));
    assert_false(coral_ref_get(object, (void*)1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_ref_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_ref_retain_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_ref_retain(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_ref_release_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_ref_release(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_ref_autorelease_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_ref_autorelease(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_ref_get_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_ref_get(NULL, (void*)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_ref_get_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_ref *object;
    assert_true(coral_ref_alloc(&object));
    assert_false(coral_ref_get(object, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_ref_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_ref_get(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_object *object;
    assert_true(coral_object_alloc(0, (void **) &object));
    assert_true(coral_object_init(object, NULL));
    struct coral_ref *ref;
    assert_true(coral_ref_alloc(&ref));
    assert_true(coral_ref_init(ref, object));
    struct coral_object *object_ = coral$object_from(object);
    assert_int_equal(2, coral$atomic_load(&object_->ref_count));
    struct coral_object *check = NULL;
    assert_true(coral_ref_get(ref, (void **) &check));
    assert_ptr_equal(object, check);
    assert_int_equal(3, coral$atomic_load(&object_->ref_count));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_ref_alloc_error_on_null_argument_ptr),
            cmocka_unit_test(check_ref_alloc),
            cmocka_unit_test(check_ref_init_error_on_null_object_ptr),
            cmocka_unit_test(check_ref_init_error_on_null_argument_ptr),
            cmocka_unit_test(check_ref_init),
            cmocka_unit_test(check_ref_retain_error_on_object_uninitialized),
            cmocka_unit_test(check_ref_release_error_on_object_uninitialized),
            cmocka_unit_test(check_ref_autorelease_error_on_object_uninitialized),
            cmocka_unit_test(check_ref_get_error_on_object_uninitialized),
            cmocka_unit_test(check_ref_retain_error_on_null_object_ptr),
            cmocka_unit_test(check_ref_release_error_on_null_object_ptr),
            cmocka_unit_test(check_ref_autorelease_error_on_null_object_ptr),
            cmocka_unit_test(check_ref_get_error_on_null_object_ptr),
            cmocka_unit_test(check_ref_get_error_on_null_argument_ptr),
            cmocka_unit_test(check_ref_get),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
