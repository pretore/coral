#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <errno.h>
#include <coral.h>

#include "private/coral.h"
#include "private/lock.h"
#include "test/cmocka.h"
#include "test/wrap.h"

static void check_invalidate_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$lock$invalidate(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_invalidate(void **state) {
    pthread_mutex_destroy_is_overridden = true;
    will_return(__wrap_pthread_mutex_destroy, 0);
    coral_error = CORAL_ERROR_NONE;
    struct coral$lock object = {};
    assert_true(coral$lock$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
    pthread_mutex_destroy_is_overridden = false;
}

static void check_invalidate_with_exponential_usleep(void **state) {
    pthread_mutex_destroy_is_overridden = true;
    will_return_count(__wrap_pthread_mutex_destroy, EBUSY, 5);
    will_return(__wrap_pthread_mutex_destroy, 0);
    coral_error = CORAL_ERROR_NONE;
    struct coral$lock object = {};
    assert_true(coral$lock$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
    pthread_mutex_destroy_is_overridden = false;
}

static void check_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$lock$init(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$lock object;
    assert_true(coral$lock$init(&object));
    assert_true(coral$lock$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_lock_error_on_null_object(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$lock$lock(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_lock_with_exponential_usleep(void **state) {
    pthread_mutex_lock_is_overridden = true;
    will_return_count(__wrap_pthread_mutex_lock, EAGAIN, 5);
    will_return(__wrap_pthread_mutex_lock, 0);
    coral_error = CORAL_ERROR_NONE;
    struct coral$lock object = {};
    assert_true(coral$lock$lock(&object));
    coral_error = CORAL_ERROR_NONE;
    pthread_mutex_lock_is_overridden = false;
}

static void check_lock_error_on_invalid_value(void **state) {
    pthread_mutex_lock_is_overridden = true;
    will_return(__wrap_pthread_mutex_lock, EINVAL);
    coral_error = CORAL_ERROR_NONE;
    struct coral$lock object = {};
    assert_false(coral$lock$lock(&object));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    pthread_mutex_lock_is_overridden = false;
}

static void check_lock_error_on_object_unavailable(void **state) {
    pthread_mutex_lock_is_overridden = true;
    will_return(__wrap_pthread_mutex_lock, EPERM);
    coral_error = CORAL_ERROR_NONE;
    struct coral$lock object = {};
    assert_false(coral$lock$lock(&object));
    assert_int_equal(CORAL_ERROR_OBJECT_UNAVAILABLE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    pthread_mutex_lock_is_overridden = false;
}

static void check_lock(void **state) {
    pthread_mutex_lock_is_overridden = true;
    will_return(__wrap_pthread_mutex_lock, 0);
    coral_error = CORAL_ERROR_NONE;
    struct coral$lock object = {};
    assert_true(coral$lock$lock(&object));
    coral_error = CORAL_ERROR_NONE;
    pthread_mutex_lock_is_overridden = false;
}

static void check_unlock_error_on_null_object(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$lock$unlock(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_unlock_with_exponential_usleep(void **state) {
    pthread_mutex_unlock_is_overridden = true;
    will_return_count(__wrap_pthread_mutex_unlock, EAGAIN, 5);
    will_return(__wrap_pthread_mutex_unlock, 0);
    coral_error = CORAL_ERROR_NONE;
    struct coral$lock object = {};
    assert_true(coral$lock$unlock(&object));
    coral_error = CORAL_ERROR_NONE;
    pthread_mutex_unlock_is_overridden = false;
}

static void check_unlock_error_on_invalid_value(void **state) {
    pthread_mutex_unlock_is_overridden = true;
    will_return(__wrap_pthread_mutex_unlock, EINVAL);
    coral_error = CORAL_ERROR_NONE;
    struct coral$lock object = {};
    assert_false(coral$lock$unlock(&object));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    pthread_mutex_unlock_is_overridden = false;
}

static void check_unlock_error_on_object_unavailable(void **state) {
    pthread_mutex_unlock_is_overridden = true;
    will_return(__wrap_pthread_mutex_unlock, EPERM);
    coral_error = CORAL_ERROR_NONE;
    struct coral$lock object = {};
    assert_false(coral$lock$unlock(&object));
    assert_int_equal(CORAL_ERROR_OBJECT_UNAVAILABLE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    pthread_mutex_unlock_is_overridden = false;
}

static void check_unlock(void **state) {
    pthread_mutex_unlock_is_overridden = true;
    will_return(__wrap_pthread_mutex_unlock, 0);
    coral_error = CORAL_ERROR_NONE;
    struct coral$lock object = {};
    assert_true(coral$lock$unlock(&object));
    coral_error = CORAL_ERROR_NONE;
    pthread_mutex_unlock_is_overridden = false;
}

static void check_object_class_error_on_null_argument(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_class(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_class(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_lock_class(&class));
    assert_non_null(class);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_destroy_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_destroy(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_alloc_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_alloc(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_alloc(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *object = NULL;
    assert_true(coral_lock_alloc(&object));
    assert_non_null(object);
    assert_true(coral_lock_destroy(object));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_init(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *object;
    assert_true(coral_lock_alloc(&object));
    assert_true(coral_lock_init(object));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_hash_code(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_hash_code((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *object;
    assert_true(coral_lock_alloc(&object));
    size_t code;
    assert_false(coral_lock_hash_code(object, &code));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_lock_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *object;
    assert_true(coral_lock_alloc(&object));
    assert_true(coral_lock_init(object));
    size_t code;
    assert_true(coral_lock_hash_code(object, &code));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_is_equal(NULL, (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_is_equal((void *) 1, (void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *i, *o;
    assert_true(coral_lock_alloc(&i));
    assert_true(coral_lock_init(i));
    assert_true(coral_lock_alloc(&o));
    assert_true(coral_lock_init(o));
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

static void check_object_copy_error_on_object_is_unavailable(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *i, *o = NULL;
    assert_true(coral_lock_alloc(&i));
    assert_true(coral_lock_init(i));
    assert_false(coral_object_copy(i, (void **)&o));
    /* mutex is not copyable */
    assert_int_equal(CORAL_ERROR_OBJECT_UNAVAILABLE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_lock_retain_error_on_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_retain(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_lock_retain_error_on_object_unavailable(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *object;
    assert_true(coral_lock_alloc(&object));
    assert_false(coral_lock_retain(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_lock_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_lock_release_error_on_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_release(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_lock_release_error_on_object_unavailable(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *object;
    assert_true(coral_lock_alloc(&object));
    assert_false(coral_lock_release(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_lock_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_lock_autorelease_error_on_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_autorelease(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_lock_autorelease_error_on_object_unavailable(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *object;
    assert_true(coral_lock_alloc(&object));
    assert_false(coral_lock_autorelease(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_lock_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_lock_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_lock(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_lock_error_on_object_is_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *i;
    assert_true(coral_lock_alloc(&i));
    assert_false(coral_lock_lock(i));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_lock_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_lock(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *object;
    assert_true(coral_lock_alloc(&object));
    assert_true(coral_lock_init(object));
    pthread_mutex_lock_is_overridden = true;
    will_return(__wrap_pthread_mutex_lock, 0);
    assert_true(coral_lock_lock(object));
    pthread_mutex_lock_is_overridden = false;
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_unlock_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_unlock(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_unlock_error_on_object_is_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *object;
    assert_true(coral_lock_alloc(&object));
    assert_false(coral_lock_unlock(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_lock_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_unlock_error_on_object_unavailable(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *object;
    assert_true(coral_lock_alloc(&object));
    assert_true(coral_lock_init(object));
    pthread_mutex_unlock_is_overridden = true;
    will_return(__wrap_pthread_mutex_unlock, EPERM);
    assert_false(coral_lock_unlock(object));
    assert_int_equal(CORAL_ERROR_OBJECT_UNAVAILABLE, coral_error);
    pthread_mutex_unlock_is_overridden = false;
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_unlock(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *object;
    assert_true(coral_lock_alloc(&object));
    assert_true(coral_lock_init(object));
    pthread_mutex_unlock_is_overridden = true;
    will_return(__wrap_pthread_mutex_unlock, 0);
    assert_true(coral_lock_unlock(object));
    pthread_mutex_unlock_is_overridden = false;
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_new_condition_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_new_condition(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_new_condition_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_new_condition((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_new_condition(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *object;
    assert_true(coral_lock_alloc(&object));
    assert_true(coral_lock_init(object));
    struct coral_lock_condition *condition;
    assert_true(coral_lock_new_condition(object, &condition));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_invalidate_error_on_null_object_ptr),
            cmocka_unit_test(check_invalidate),
            cmocka_unit_test(check_invalidate_with_exponential_usleep),
            cmocka_unit_test(check_init_error_on_null_object_ptr),
            cmocka_unit_test(check_init),
            cmocka_unit_test(check_lock_error_on_null_object),
            cmocka_unit_test(check_lock_with_exponential_usleep),
            cmocka_unit_test(check_lock_error_on_invalid_value),
            cmocka_unit_test(check_lock_error_on_object_unavailable),
            cmocka_unit_test(check_lock),
            cmocka_unit_test(check_unlock_error_on_null_object),
            cmocka_unit_test(check_unlock_with_exponential_usleep),
            cmocka_unit_test(check_unlock_error_on_invalid_value),
            cmocka_unit_test(check_unlock_error_on_object_unavailable),
            cmocka_unit_test(check_unlock),
            cmocka_unit_test(check_object_class_error_on_null_argument),
            cmocka_unit_test(check_object_class),
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
            cmocka_unit_test(check_object_copy_error_on_object_is_unavailable),
            cmocka_unit_test(check_object_lock_retain_error_on_object_ptr),
            cmocka_unit_test(check_object_lock_retain_error_on_object_unavailable),
            cmocka_unit_test(check_object_lock_release_error_on_object_ptr),
            cmocka_unit_test(check_object_lock_release_error_on_object_unavailable),
            cmocka_unit_test(check_object_lock_autorelease_error_on_object_ptr),
            cmocka_unit_test(check_object_lock_autorelease_error_on_object_unavailable),
            cmocka_unit_test(check_object_lock_error_on_null_object_ptr),
            cmocka_unit_test(check_object_lock_error_on_object_is_uninitialized),
            cmocka_unit_test(check_object_lock),
            cmocka_unit_test(check_object_unlock_error_on_null_object_ptr),
            cmocka_unit_test(check_object_unlock_error_on_object_is_uninitialized),
            cmocka_unit_test(check_object_unlock_error_on_object_unavailable),
            cmocka_unit_test(check_object_unlock),
            cmocka_unit_test(check_object_new_condition_error_on_null_object_ptr),
            cmocka_unit_test(check_object_new_condition_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_new_condition),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}