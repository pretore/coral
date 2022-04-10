#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <coral.h>

#include "private/coral.h"
#include "private/object.h"

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
    assert_true(coral_object_destroy(instance));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void (*on_destroy)(void*) = (void (*)(void *)) coral$object_destroy;
    assert_false(coral_object_init(NULL, on_destroy));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init_error_on_initialization(void **state) {
    void *instance = NULL;
    assert_true(coral_object_alloc(0, (void **) &instance));
    struct coral_object *object = coral$object_from(instance);
    coral$atomic_store(&object->ref_count, 1);
    void (*on_destroy)(void*) = (void (*)(void *)) coral$object_destroy;
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_object_init(instance, on_destroy));
    assert_int_equal(CORAL_ERROR_INITIALIZATION_FAILED, coral_error);
    assert_ptr_equal(NULL, object->on_destroy);
    assert_true(coral_object_destroy(instance));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init(void **state) {
    void *instance = NULL;
    assert_true(coral_object_alloc(0, (void **) &instance));
    struct coral_object *object = coral$object_from(instance);
    void (*on_destroy)(void*) = (void (*)(void *)) coral$object_destroy;
    coral_error = CORAL_ERROR_NONE;
    assert_true(coral_object_init(instance, on_destroy));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    assert_ptr_equal(on_destroy, object->on_destroy);
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
    void *instance = NULL;
    assert_true(coral_object_alloc(0, &instance));
    coral$object_destroy(instance);
    coral_autorelease_pool_drain();
}

static void check_object_retain_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *instance = NULL;
    assert_true(coral_object_alloc(0, &instance));
    assert_false(coral_object_retain(instance));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_range_destroy(instance);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_release_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *instance = NULL;
    assert_true(coral_object_alloc(0, &instance));
    assert_false(coral_object_release(instance));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_range_destroy(instance);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_autorelease_error_on_object_uninitialized(
        void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *instance = NULL;
    assert_true(coral_object_alloc(0, &instance));
    assert_false(coral_object_autorelease(instance));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_range_destroy(instance);
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
    struct coral_object *object = NULL;
    void (*on_destroy)(void*) = (void (*)(void *)) coral$object_destroy;
    assert_true(coral_object_alloc(0, (void **) &object));
    struct coral_object *object_ = coral$object_from(object);
    assert_true(coral_object_init(object, on_destroy));
    assert_int_equal(1, coral$atomic_load(&object_->ref_count));
    coral_object_retain(object);
    assert_int_equal(2, coral$atomic_load(&object_->ref_count));
    coral_object_release(object);
    assert_int_equal(1, coral$atomic_load(&object_->ref_count));
    coral_object_release(object);
    coral_autorelease_pool_drain();
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_object_alloc_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_alloc),
            cmocka_unit_test(check_object_init_error_on_null_object_ptr),
            cmocka_unit_test(check_object_init_error_on_initialization),
            cmocka_unit_test(check_object_init),
            cmocka_unit_test(check_object_destroy_error_on_null_object_ptr),
            cmocka_unit_test(check_object_destroy),
            cmocka_unit_test(check_object_retain_error_on_object_uninitialized),
            cmocka_unit_test(check_object_release_error_on_object_uninitialized),
            cmocka_unit_test(check_object_autorelease_error_on_object_uninitialized),
            cmocka_unit_test(check_object_retain_error_on_null_object_ptr),
            cmocka_unit_test(check_object_release_error_on_null_object_ptr),
            cmocka_unit_test(check_object_retain_and_release)
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
