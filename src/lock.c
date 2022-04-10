#include <stdlib.h>
#include <errno.h>
#include <coral.h>

#include "private/coral.h"
#include "private/lock.h"
#include "test/cmocka.h"

#pragma mark private

bool coral$lock_alloc(struct coral_lock **out) {
    return coral_object_alloc(sizeof(struct coral_lock), (void **) out);
}

bool coral$lock_init(struct coral_lock *lock) {
    coral_required(lock);
    pthread_mutex_t *mutex = malloc(sizeof(pthread_mutex_t));
    if (!mutex) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    pthread_mutexattr_t attr;
    if (pthread_mutexattr_init(&attr)
        || pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK)
        || pthread_mutex_init(mutex, &attr)) {
        coral_error = CORAL_ERROR_INITIALIZATION_FAILED;
        free(mutex);
        return false;
    }
    void (*on_destroy)(void *) = (void (*)(void *)) coral$lock_destroy;
    if (!coral_object_init(lock, on_destroy)) {
        return false;
    }
    lock->mutex = mutex;
    return true;
}

void coral$lock_destroy(struct coral_lock *lock) {
    coral_required(lock);
    for (uint8_t state = 0; EBUSY == pthread_mutex_destroy(lock->mutex);
         coral_exponential_usleep(&state, MAXIMUM_USLEEP));
    free(lock->mutex);
    coral_required_true(coral_object_destroy(lock));
}

bool coral$lock_acquire_lock(struct coral_lock *object) {
    coral_required(object);
    const int result = pthread_mutex_lock(object->mutex);
    if (result) {
        coral_error = CORAL_ERROR_OBJECT_UNAVAILABLE;
    }
    return !result;
}

bool coral$lock_release_lock(struct coral_lock *object) {
    coral_required(object);
    const int result = pthread_mutex_unlock(object->mutex);
    if (result) {
        coral_error = CORAL_ERROR_OBJECT_UNAVAILABLE;
    }
    return !result;
}

bool coral$acquire_lock(const size_t count, struct coral_lock **locks,
                        struct coral_lock **out) {
    coral_required_true(count > 0);
    coral_required(locks);
    coral_required(out);
    /* use a strict order in which we will obtain the locks */
    qsort(locks, count, sizeof(struct coral_lock *),
          coral_compare_void_ptr);
    for (size_t i = 0; i < count; i++) {
        struct coral_lock *item = locks[i];
        if (!item) {
            continue;
        }
        if (i > 0) {
            /* ignore duplicates */
            const struct coral_lock *prev = locks[i - 1];
            if (item == prev) {
                continue;
            }
        }
        if (!coral$lock_acquire_lock(item)) {
            *out = item;
            coral_required_true(coral$release_lock(1 + i, locks));
            return false;
        }
    }
    return true;
}

bool coral$release_lock(const size_t count, struct coral_lock **locks) {
    coral_required_true(count > 0);
    coral_required(locks);
    /* unlock in the opposite order that is used to obtain the locks */
    qsort(locks, count, sizeof(struct coral_lock *),
          coral_compare_void_ptr);
    for (size_t i = 0, limit = count - 1; i <= limit; i++) {
        struct coral_lock *item = locks[limit - i];
        if (!item) {
            continue;
        }
        if (i > 0) {
            /* ignore duplicates */
            const struct coral_lock *prev = locks[limit - (i - 1)];
            if (item == prev) {
                continue;
            }
        }
        coral_required_true(coral$lock_release_lock(item));
    }
    return true;
}

#pragma mark public

bool coral_lock_alloc(struct coral_lock **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral$lock_alloc(out);
}

bool coral_lock_init(struct coral_lock *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral$lock_init(object);
}

bool coral_lock_destroy(struct coral_lock *object) {
    return coral_object_destroy(object);
}

bool coral_lock_retain(struct coral_lock *object) {
    return coral_object_retain(object);
}

bool coral_lock_release(struct coral_lock *object) {
    return coral_object_release(object);
}

bool coral_lock_autorelease(struct coral_lock *object) {
    return coral_object_autorelease(object);
}

bool coral_acquire_lock(const size_t count, struct coral_lock **locks,
                        struct coral_lock **out) {
    if (!count) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    }
    if (!locks) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral$acquire_lock(count, locks, out);
}

bool coral_release_lock(const size_t count, struct coral_lock **locks) {
    if (!count) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    }
    if (!locks) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral$release_lock(count, locks);
}
