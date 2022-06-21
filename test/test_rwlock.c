#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <errno.h>
#include <coral.h>

#include "private/rwlock.h"
#include "test/cmocka.h"
#include "test/wrap.h"

static void check_invalidate_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$rwlock$invalidate(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_invalidate(void **state) {
    pthread_rwlock_destroy_is_overridden = true;
    will_return(__wrap_pthread_rwlock_destroy, 0);
    coral_error = CORAL_ERROR_NONE;
    struct coral$rwlock object = {};
    assert_true(coral$rwlock$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
    pthread_rwlock_destroy_is_overridden = false;
}

static void check_invalidate_with_exponential_usleep(void **state) {
    pthread_rwlock_destroy_is_overridden = true;
    will_return_count(__wrap_pthread_rwlock_destroy, EBUSY, 5);
    will_return(__wrap_pthread_rwlock_destroy, 0);
    coral_error = CORAL_ERROR_NONE;
    struct coral$rwlock object = {};
    assert_true(coral$rwlock$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
    pthread_rwlock_destroy_is_overridden = false;
}

static void check_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$rwlock$init(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$rwlock object;
    assert_true(coral$rwlock$init(&object));
    assert_true(coral$rwlock$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_read_lock_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$rwlock$read_lock(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_read_lock_error_on_object_unavailable(void **state) {
    pthread_rwlock_rdlock_is_overridden = true;
    will_return(__wrap_pthread_rwlock_rdlock, EDEADLK);
    coral_error = CORAL_ERROR_NONE;
    struct coral$rwlock object = {};
    assert_false(coral$rwlock$read_lock(&object));
    assert_int_equal(CORAL_ERROR_OBJECT_UNAVAILABLE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    pthread_rwlock_rdlock_is_overridden = false;
}

static void check_read_lock_error_on_invalid_value(void **state) {
    pthread_rwlock_rdlock_is_overridden = true;
    will_return(__wrap_pthread_rwlock_rdlock, EINVAL);
    coral_error = CORAL_ERROR_NONE;
    struct coral$rwlock object = {};
    assert_false(coral$rwlock$read_lock(&object));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    will_return(__wrap_pthread_rwlock_rdlock, EAGAIN);
    assert_false(coral$rwlock$read_lock(&object));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    pthread_rwlock_rdlock_is_overridden = false;
}

static void check_read_lock(void **state) {
    pthread_rwlock_rdlock_is_overridden = true;
    will_return(__wrap_pthread_rwlock_rdlock, 0);
    coral_error = CORAL_ERROR_NONE;
    struct coral$rwlock object = {};
    assert_true(coral$rwlock$read_lock(&object));
    coral_error = CORAL_ERROR_NONE;
    pthread_rwlock_rdlock_is_overridden = false;
}

static void check_write_lock_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$rwlock$write_lock(NULL));
    coral_error = CORAL_ERROR_NONE;
}

static void check_write_lock_error_on_object_unavailable(void **state) {
    pthread_rwlock_wrlock_is_overridden = true;
    will_return(__wrap_pthread_rwlock_wrlock, EDEADLK);
    coral_error = CORAL_ERROR_NONE;
    struct coral$rwlock object = {};
    assert_false(coral$rwlock$write_lock(&object));
    assert_int_equal(CORAL_ERROR_OBJECT_UNAVAILABLE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    pthread_rwlock_wrlock_is_overridden = false;
}

static void check_write_lock_error_on_invalid_value(void **state) {
    pthread_rwlock_wrlock_is_overridden = true;
    will_return(__wrap_pthread_rwlock_wrlock, EINVAL);
    coral_error = CORAL_ERROR_NONE;
    struct coral$rwlock object = {};
    assert_false(coral$rwlock$write_lock(&object));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    pthread_rwlock_wrlock_is_overridden = false;
}

static void check_write_lock(void **state) {
    pthread_rwlock_wrlock_is_overridden = true;
    will_return(__wrap_pthread_rwlock_wrlock, 0);
    coral_error = CORAL_ERROR_NONE;
    struct coral$rwlock object = {};
    assert_true(coral$rwlock$write_lock(&object));
    coral_error = CORAL_ERROR_NONE;
    pthread_rwlock_wrlock_is_overridden = false;
}

static void check_unlock_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$rwlock$unlock(NULL));
    coral_error = CORAL_ERROR_NONE;
}

static void check_unlock_error_on_object_unavailable(void **state) {
    pthread_rwlock_unlock_is_overridden = true;
    will_return(__wrap_pthread_rwlock_unlock, EPERM);
    coral_error = CORAL_ERROR_NONE;
    struct coral$rwlock object = {};
    assert_false(coral$rwlock$unlock(&object));
    assert_int_equal(CORAL_ERROR_OBJECT_UNAVAILABLE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    pthread_rwlock_unlock_is_overridden = false;
}

static void check_unlock_error_on_invalid_value(void **state) {
    pthread_rwlock_unlock_is_overridden = true;
    will_return(__wrap_pthread_rwlock_unlock, EINVAL);
    coral_error = CORAL_ERROR_NONE;
    struct coral$rwlock object = {};
    assert_false(coral$rwlock$unlock(&object));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    pthread_rwlock_unlock_is_overridden = false;
}

static void check_unlock(void **state) {
    pthread_rwlock_unlock_is_overridden = true;
    will_return(__wrap_pthread_rwlock_unlock, 0);
    coral_error = CORAL_ERROR_NONE;
    struct coral$rwlock object = {};
    assert_true(coral$rwlock$unlock(&object));
    coral_error = CORAL_ERROR_NONE;
    pthread_rwlock_unlock_is_overridden = false;
}

static void check_class_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_rwlock_class(NULL));
    coral_error = CORAL_ERROR_NONE;
}

static void check_class(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_rwlock_class(&class));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_destroy_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_rwlock_destroy(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_alloc_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_rwlock_alloc(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_alloc(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_rwlock *object = NULL;
    assert_true(coral_rwlock_alloc(&object));
    assert_non_null(object);
    assert_true(coral_rwlock_destroy(object));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_rwlock_init(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_rwlock *object;
    assert_true(coral_rwlock_alloc(&object));
    assert_true(coral_rwlock_init(object));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_rwlock_hash_code(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_rwlock_hash_code((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_rwlock_class(&class));
    assert_false(coral_rwlock_hash_code(
            (struct coral_rwlock *)class, (void *)1));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_rwlock *object;
    assert_true(coral_rwlock_alloc(&object));
    size_t code;
    assert_false(coral_rwlock_hash_code(object, &code));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_rwlock_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_rwlock *object;
    assert_true(coral_rwlock_alloc(&object));
    assert_true(coral_rwlock_init(object));
    size_t code;
    assert_true(coral_rwlock_hash_code(object, &code));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_rwlock_is_equal(NULL, (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_rwlock_is_equal((void *) 1, (void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_rwlock_class(&class));
    struct coral_rwlock *i;
    assert_true(coral_rwlock_alloc(&i));
    assert_true(coral_rwlock_init(i));
    bool result;
    assert_true(coral_rwlock_is_equal(
            (struct coral_rwlock *)class, i, &result));
    assert_false(result);
    assert_true(coral_rwlock_is_equal(
            i, (struct coral_rwlock *)class, &result));
    assert_false(result);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_rwlock *i, *o;
    assert_true(coral_rwlock_alloc(&i));
    assert_true(coral_rwlock_init(i));
    assert_true(coral_rwlock_alloc(&o));
    assert_true(coral_rwlock_init(o));
    bool result;
    assert_true(coral_rwlock_is_equal(i, o, &result));
    assert_false(result);
    assert_true(coral_rwlock_is_equal(i, i, &result));
    assert_true(result);
    assert_true(coral_rwlock_is_equal(o, i, &result));
    assert_false(result);
    assert_true(coral_rwlock_is_equal(o, o, &result));
    assert_true(result);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_retain_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_rwlock_retain(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_retain_error_on_object_unavailable(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_rwlock *object;
    assert_true(coral_rwlock_alloc(&object));
    assert_false(coral_rwlock_retain(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_rwlock_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_release_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_rwlock_release(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_release_error_on_object_unavailable(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_rwlock *object;
    assert_true(coral_rwlock_alloc(&object));
    assert_false(coral_rwlock_release(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_rwlock_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_autorelease_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_rwlock_autorelease(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_autorelease_error_on_object_unavailable(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_rwlock *object;
    assert_true(coral_rwlock_alloc(&object));
    assert_false(coral_rwlock_autorelease(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_rwlock_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_read_lock_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_rwlock_read_lock(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_read_lock_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_rwlock_class(&class));
    assert_false(coral_rwlock_read_lock((struct coral_rwlock *)class));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_read_lock_error_on_object_unavailable(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_rwlock *object;
    assert_true(coral_rwlock_alloc(&object));
    assert_true(coral_rwlock_init(object));
    pthread_rwlock_rdlock_is_overridden = true;
    will_return(__wrap_pthread_rwlock_rdlock, EDEADLK);
    assert_false(coral_rwlock_read_lock(object));
    assert_int_equal(CORAL_ERROR_OBJECT_UNAVAILABLE, coral_error);
    pthread_rwlock_rdlock_is_overridden = false;
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_read_lock_error_on_object_is_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_rwlock *i;
    assert_true(coral_rwlock_alloc(&i));
    assert_false(coral_rwlock_read_lock(i));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_rwlock_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_read_lock(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_rwlock *object;
    assert_true(coral_rwlock_alloc(&object));
    assert_true(coral_rwlock_init(object));
    pthread_rwlock_rdlock_is_overridden = true;
    will_return(__wrap_pthread_rwlock_rdlock, 0);
    assert_true(coral_rwlock_read_lock(object));
    pthread_rwlock_rdlock_is_overridden = false;
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_write_lock_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_rwlock_write_lock(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_write_lock_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_rwlock_class(&class));
    assert_false(coral_rwlock_write_lock((struct coral_rwlock *)class));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_write_lock_error_on_object_unavailable(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_rwlock *object;
    assert_true(coral_rwlock_alloc(&object));
    assert_true(coral_rwlock_init(object));
    pthread_rwlock_wrlock_is_overridden = true;
    will_return(__wrap_pthread_rwlock_wrlock, EDEADLK);
    assert_false(coral_rwlock_write_lock(object));
    assert_int_equal(CORAL_ERROR_OBJECT_UNAVAILABLE, coral_error);
    pthread_rwlock_wrlock_is_overridden = false;
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_write_lock_error_on_object_is_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_rwlock *i;
    assert_true(coral_rwlock_alloc(&i));
    assert_false(coral_rwlock_write_lock(i));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_rwlock_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_write_lock(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_rwlock *object;
    assert_true(coral_rwlock_alloc(&object));
    assert_true(coral_rwlock_init(object));
    pthread_rwlock_wrlock_is_overridden = true;
    will_return(__wrap_pthread_rwlock_wrlock, 0);
    assert_true(coral_rwlock_write_lock(object));
    pthread_rwlock_wrlock_is_overridden = false;
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
            cmocka_unit_test(check_read_lock_error_on_null_object_ptr),
            cmocka_unit_test(check_read_lock_error_on_object_unavailable),
            cmocka_unit_test(check_read_lock_error_on_invalid_value),
            cmocka_unit_test(check_read_lock),
            cmocka_unit_test(check_write_lock_error_on_null_object_ptr),
            cmocka_unit_test(check_write_lock_error_on_object_unavailable),
            cmocka_unit_test(check_write_lock_error_on_invalid_value),
            cmocka_unit_test(check_write_lock),
            cmocka_unit_test(check_unlock_error_on_null_object_ptr),
            cmocka_unit_test(check_unlock_error_on_object_unavailable),
            cmocka_unit_test(check_unlock_error_on_invalid_value),
            cmocka_unit_test(check_unlock),
            cmocka_unit_test(check_class_error_on_null_argument_ptr),
            cmocka_unit_test(check_class),
            cmocka_unit_test(check_object_destroy_error_on_null_object_ptr),
            cmocka_unit_test(check_object_alloc_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_alloc),
            cmocka_unit_test(check_object_init_error_on_null_object_ptr),
            cmocka_unit_test(check_object_init),
            cmocka_unit_test(check_object_retain_error_on_null_object_ptr),
            cmocka_unit_test(check_object_retain_error_on_object_unavailable),
            cmocka_unit_test(check_object_release_error_on_null_object_ptr),
            cmocka_unit_test(check_object_release_error_on_object_unavailable),
            cmocka_unit_test(check_object_autorelease_error_on_null_object_ptr),
            cmocka_unit_test(check_object_autorelease_error_on_object_unavailable),
            cmocka_unit_test(check_object_read_lock_error_on_null_object_ptr),
            cmocka_unit_test(check_object_read_lock_error_on_invalid_value),
            cmocka_unit_test(check_object_read_lock_error_on_object_unavailable),
            cmocka_unit_test(check_object_read_lock_error_on_object_is_uninitialized),
            cmocka_unit_test(check_object_read_lock),
            cmocka_unit_test(check_object_write_lock_error_on_null_object_ptr),
            cmocka_unit_test(check_object_write_lock_error_on_invalid_value),
            cmocka_unit_test(check_object_write_lock_error_on_object_unavailable),
            cmocka_unit_test(check_object_write_lock_error_on_object_is_uninitialized),
            cmocka_unit_test(check_object_write_lock),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
