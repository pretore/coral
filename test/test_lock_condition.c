#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <errno.h>
#include <coral.h>

#include "private/lock_condition.h"
#include "test/cmocka.h"
#include "test/wrap.h"

static void check_invalidate_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$lock_condition$invalidate(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_invalidate(void **state) {
    struct coral_class *class = NULL;
    assert_true(coral_lock_condition_class(&class));
    coral_error = CORAL_ERROR_NONE;
    pthread_cond_destroy_is_overridden = true;
    will_return(__wrap_pthread_cond_destroy, 0);
    struct coral$lock_condition object = {};
    assert_true(coral$reference$init(&object.lock_ref, class));
    assert_true(coral$lock_condition$invalidate(&object));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
    pthread_cond_destroy_is_overridden = false;
}

static void check_invalidate_with_exponential_usleep(void **state) {
    struct coral_class *class = NULL;
    assert_true(coral_lock_condition_class(&class));
    pthread_cond_destroy_is_overridden = true;
    will_return_count(__wrap_pthread_cond_destroy, EBUSY, 5);
    will_return(__wrap_pthread_cond_destroy, 0);
    coral_error = CORAL_ERROR_NONE;
    struct coral$lock_condition object = {};
    assert_true(coral$reference$init(&object.lock_ref, class));
    assert_true(coral$lock_condition$invalidate(&object));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
    pthread_cond_destroy_is_overridden = false;
}

static void check_await_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$lock_condition$await(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_await_error_on_invalid_value(void **state) {
    struct coral_class *class = NULL;
    assert_true(coral_lock_condition_class(&class));
    pthread_cond_wait_is_overridden = true;
    will_return(__wrap_pthread_cond_wait, EINVAL);
    coral_error = CORAL_ERROR_NONE;
    struct coral$lock_condition i = {};
    assert_true(coral$reference$init(&i.lock_ref, class));
    assert_false(coral$lock_condition$await(&i));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    pthread_cond_wait_is_overridden = false;
    assert_true(coral$lock_condition$invalidate(&i));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_await_error_on_object_unavailable(void **state) {
    struct coral_class *class = NULL;
    assert_true(coral_lock_condition_class(&class));
    pthread_cond_wait_is_overridden = true;
    will_return(__wrap_pthread_cond_wait, EPERM);
    coral_error = CORAL_ERROR_NONE;
    struct coral$lock_condition i = {};
    assert_true(coral$reference$init(&i.lock_ref, class));
    assert_false(coral$lock_condition$await(&i));
    assert_int_equal(CORAL_ERROR_OBJECT_UNAVAILABLE, coral_error);
    pthread_cond_wait_is_overridden = false;
    assert_true(coral$lock_condition$invalidate(&i));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_await(void **state) {
    struct coral_class *class = NULL;
    assert_true(coral_lock_condition_class(&class));
    pthread_cond_wait_is_overridden = true;
    will_return(__wrap_pthread_cond_wait, 0);
    coral_error = CORAL_ERROR_NONE;
    struct coral$lock_condition i = {};
    assert_true(coral$reference$init(&i.lock_ref, class));
    assert_true(coral$lock_condition$await(&i));
    pthread_cond_wait_is_overridden = false;
    assert_true(coral$lock_condition$invalidate(&i));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_signal_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$lock_condition$signal(NULL));
    coral_error = CORAL_ERROR_NONE;
}

static void check_signal_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    pthread_cond_signal_is_overridden = true;
    will_return(__wrap_pthread_cond_signal, EINVAL);
    assert_false(coral$lock_condition$signal((void *)1));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    pthread_cond_signal_is_overridden = false;
    coral_error = CORAL_ERROR_NONE;
}

static void check_signal(void **state) {
    coral_error = CORAL_ERROR_NONE;
    pthread_cond_signal_is_overridden = true;
    will_return(__wrap_pthread_cond_signal, 0);
    assert_true(coral$lock_condition$signal((void *)1));
    pthread_cond_signal_is_overridden = false;
    coral_error = CORAL_ERROR_NONE;
}

static void check_signal_all_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$lock_condition$signal_all(NULL));
    coral_error = CORAL_ERROR_NONE;
}

static void check_signal_all_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    pthread_cond_broadcast_is_overridden = true;
    will_return(__wrap_pthread_cond_broadcast, EINVAL);
    assert_false(coral$lock_condition$signal_all((void *)1));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    pthread_cond_broadcast_is_overridden = false;
    coral_error = CORAL_ERROR_NONE;
}

static void check_signal_all(void **state) {
    coral_error = CORAL_ERROR_NONE;
    pthread_cond_broadcast_is_overridden = true;
    will_return(__wrap_pthread_cond_broadcast, 0);
    assert_true(coral$lock_condition$signal_all((void *)1));
    pthread_cond_broadcast_is_overridden = false;
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_class_error_on_null_argument(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_condition_class(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_class(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_lock_condition_class(&class));
    assert_non_null(class);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_destroy_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_condition_destroy(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_alloc_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_condition_alloc(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_alloc(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock_condition *object = NULL;
    assert_true(coral_lock_condition_alloc(&object));
    assert_non_null(object);
    assert_true(coral_lock_condition_destroy(object));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_condition_init(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_condition_init((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *lock;
    assert_true(coral_lock_alloc(&lock));
    assert_true(coral_lock_init(lock));
    struct coral_lock_condition *object;
    assert_true(coral_lock_condition_alloc(&object));
    assert_true(coral_lock_condition_init(object, lock));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_condition_hash_code(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_condition_hash_code((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_hash_code_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock_condition *i;
    assert_true(coral_lock_condition_alloc(&i));
    size_t code;
    assert_false(coral_lock_condition_hash_code(i, &code));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_lock_condition_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *i;
    assert_true(coral_lock_alloc(&i));
    assert_true(coral_lock_init(i));
    struct coral_lock_condition *o;
    assert_true(coral_lock_condition_alloc(&o));
    assert_true(coral_lock_condition_init(o, i));
    size_t code;
    assert_true(coral_lock_condition_hash_code(o, &code));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_condition_is_equal(NULL, (void *)1, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_condition_is_equal((void *)1, NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    assert_false(coral_lock_condition_is_equal((void *)1, (void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *i;
    assert_true(coral_lock_alloc(&i));
    assert_true(coral_lock_init(i));
    struct coral_lock_condition *o;
    assert_true(coral_lock_condition_alloc(&o));
    assert_true(coral_lock_condition_init(o, i));
    struct coral_lock_condition *p;
    assert_true(coral_lock_condition_alloc(&p));
    assert_true(coral_lock_condition_init(p, i));
    bool result;
    assert_true(coral_lock_condition_is_equal(o, o, &result));
    assert_true(result);
    assert_true(coral_lock_condition_is_equal(o, p, &result));
    assert_false(result);
    assert_true(coral_lock_condition_is_equal(p, o, &result));
    assert_false(result);
    assert_true(coral_lock_condition_is_equal(p, p, &result));
    assert_true(result);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_copy_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_object_copy(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_copy_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_object_copy((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_copy(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *i;
    assert_true(coral_lock_alloc(&i));
    assert_true(coral_lock_init(i));
    struct coral_lock_condition *o;
    assert_true(coral_lock_condition_alloc(&o));
    assert_true(coral_lock_condition_init(o, i));
    struct coral_lock_condition *p = NULL;
    assert_false(coral_object_copy(i, (void **)&p));
    assert_int_equal(CORAL_ERROR_OBJECT_UNAVAILABLE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_retain_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_condition_retain(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_retain_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock_condition *i;
    assert_true(coral_lock_condition_alloc(&i));
    assert_false(coral_lock_condition_retain(i));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_lock_condition_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_release_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_condition_release(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_release_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock_condition *i;
    assert_true(coral_lock_condition_alloc(&i));
    assert_false(coral_lock_condition_release(i));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_lock_condition_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_autorelease_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_condition_autorelease(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_autorelease_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock_condition *i;
    assert_true(coral_lock_condition_alloc(&i));
    assert_false(coral_lock_condition_autorelease(i));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_lock_condition_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_await_error_on_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_condition_await(NULL));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_await_error_on_invalid_value(void **state) {
    struct coral_class *class = NULL;
    assert_true(coral_lock_condition_class(&class));
    pthread_cond_wait_is_overridden = true;
    will_return(__wrap_pthread_cond_wait, EINVAL);
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *i;
    assert_true(coral_lock_alloc(&i));
    assert_true(coral_lock_init(i));
    struct coral_lock_condition *o;
    assert_true(coral_lock_condition_alloc(&o));
    assert_true(coral_lock_condition_init(o, i));
    assert_false(coral_lock_condition_await(o));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    pthread_cond_wait_is_overridden = false;
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_await_error_on_object_unavailable(void **state) {
    struct coral_class *class = NULL;
    assert_true(coral_lock_condition_class(&class));
    pthread_cond_wait_is_overridden = true;
    will_return(__wrap_pthread_cond_wait, EPERM);
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *i;
    assert_true(coral_lock_alloc(&i));
    assert_true(coral_lock_init(i));
    struct coral_lock_condition *o;
    assert_true(coral_lock_condition_alloc(&o));
    assert_true(coral_lock_condition_init(o, i));
    assert_false(coral_lock_condition_await(o));
    assert_int_equal(CORAL_ERROR_OBJECT_UNAVAILABLE, coral_error);
    pthread_cond_wait_is_overridden = false;
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_await(void **state) {
    struct coral_class *class = NULL;
    assert_true(coral_lock_condition_class(&class));
    pthread_cond_wait_is_overridden = true;
    will_return(__wrap_pthread_cond_wait, 0);
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *i;
    assert_true(coral_lock_alloc(&i));
    assert_true(coral_lock_init(i));
    struct coral_lock_condition *o;
    assert_true(coral_lock_condition_alloc(&o));
    assert_true(coral_lock_condition_init(o, i));
    assert_true(coral_lock_condition_await(o));
    pthread_cond_wait_is_overridden = false;
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_signal_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_condition_signal(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_signal_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock_condition *i;
    assert_true(coral_lock_condition_alloc(&i));
    assert_false(coral_lock_condition_signal(i));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_lock_condition_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_signal_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *i;
    assert_true(coral_lock_alloc(&i));
    assert_true(coral_lock_init(i));
    struct coral_lock_condition *o;
    assert_true(coral_lock_condition_alloc(&o));
    assert_true(coral_lock_condition_init(o, i));
    pthread_cond_signal_is_overridden = true;
    will_return(__wrap_pthread_cond_signal, EINVAL);
    assert_false(coral_lock_condition_signal(o));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    pthread_cond_signal_is_overridden = false;
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_signal(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *i;
    assert_true(coral_lock_alloc(&i));
    assert_true(coral_lock_init(i));
    struct coral_lock_condition *o;
    assert_true(coral_lock_condition_alloc(&o));
    assert_true(coral_lock_condition_init(o, i));
    pthread_cond_signal_is_overridden = true;
    will_return(__wrap_pthread_cond_signal, 0);
    assert_true(coral_lock_condition_signal(o));
    pthread_cond_signal_is_overridden = false;
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_signal_all_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_lock_condition_signal_all(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_signal_all_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock_condition *i;
    assert_true(coral_lock_condition_alloc(&i));
    assert_false(coral_lock_condition_signal_all(i));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_lock_condition_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_signal_all_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *i;
    assert_true(coral_lock_alloc(&i));
    assert_true(coral_lock_init(i));
    struct coral_lock_condition *o;
    assert_true(coral_lock_condition_alloc(&o));
    assert_true(coral_lock_condition_init(o, i));
    pthread_cond_broadcast_is_overridden = true;
    will_return(__wrap_pthread_cond_broadcast, EINVAL);
    assert_false(coral_lock_condition_signal_all(o));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    pthread_cond_broadcast_is_overridden = false;
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_signal_all(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_lock *i;
    assert_true(coral_lock_alloc(&i));
    assert_true(coral_lock_init(i));
    struct coral_lock_condition *o;
    assert_true(coral_lock_condition_alloc(&o));
    assert_true(coral_lock_condition_init(o, i));
    pthread_cond_broadcast_is_overridden = true;
    will_return(__wrap_pthread_cond_broadcast, 0);
    assert_true(coral_lock_condition_signal_all(o));
    pthread_cond_broadcast_is_overridden = false;
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_invalidate_error_on_null_object_ptr),
            cmocka_unit_test(check_invalidate),
            cmocka_unit_test(check_invalidate_with_exponential_usleep),
            cmocka_unit_test(check_await_error_on_null_object_ptr),
            cmocka_unit_test(check_await_error_on_invalid_value),
            cmocka_unit_test(check_await_error_on_object_unavailable),
            cmocka_unit_test(check_await),
            cmocka_unit_test(check_signal_error_on_null_object_ptr),
            cmocka_unit_test(check_signal_error_on_invalid_value),
            cmocka_unit_test(check_signal),
            cmocka_unit_test(check_signal_all_error_on_null_object_ptr),
            cmocka_unit_test(check_signal_all_error_on_invalid_value),
            cmocka_unit_test(check_signal_all),
            cmocka_unit_test(check_object_class_error_on_null_argument),
            cmocka_unit_test(check_object_class),
            cmocka_unit_test(check_object_destroy_error_on_null_object_ptr),
            cmocka_unit_test(check_object_alloc_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_alloc),
            cmocka_unit_test(check_object_init_error_on_null_object_ptr),
            cmocka_unit_test(check_object_init_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_init),
            cmocka_unit_test(check_object_hash_code_error_on_null_object_ptr),
            cmocka_unit_test(check_object_hash_code_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_hash_code_error_on_object_uninitialized),
            cmocka_unit_test(check_object_hash_code),
            cmocka_unit_test(check_object_is_equal_error_on_null_object_ptr),
            cmocka_unit_test(check_object_is_equal_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_is_equal),
            cmocka_unit_test(check_object_copy_error_on_null_object_ptr),
            cmocka_unit_test(check_object_copy_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_copy),
            cmocka_unit_test(check_object_retain_error_on_null_object_ptr),
            cmocka_unit_test(check_object_retain_error_on_object_uninitialized),
            cmocka_unit_test(check_object_release_error_on_null_object_ptr),
            cmocka_unit_test(check_object_release_error_on_object_uninitialized),
            cmocka_unit_test(check_object_autorelease_error_on_null_object_ptr),
            cmocka_unit_test(check_object_autorelease_error_on_object_uninitialized),
            cmocka_unit_test(check_object_await_error_on_object_ptr),
            cmocka_unit_test(check_object_await_error_on_invalid_value),
            cmocka_unit_test(check_object_await_error_on_object_unavailable),
            cmocka_unit_test(check_object_await),
            cmocka_unit_test(check_object_signal_error_on_null_object_ptr),
            cmocka_unit_test(check_object_signal_error_on_object_uninitialized),
            cmocka_unit_test(check_object_signal_error_on_invalid_value),
            cmocka_unit_test(check_object_signal),
            cmocka_unit_test(check_object_signal_all_error_on_null_object_ptr),
            cmocka_unit_test(check_object_signal_all_error_on_object_uninitialized),
            cmocka_unit_test(check_object_signal_all_error_on_invalid_value),
            cmocka_unit_test(check_object_signal_all),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}