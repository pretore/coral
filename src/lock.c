#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <coral.h>

#include "private/coral.h"
#include "private/lock.h"
#include "private/class.h"
#include "test/cmocka.h"

#pragma mark private -

static struct coral_class *$class;

#pragma mark + invokables

static bool $lock_destroy(void *this,
                          struct coral_lock *data,
                          void *args);

static bool $lock_hash_code(void *this,
                            struct coral_lock *data,
                            struct hash_code_args *args);

static bool $lock_is_equal(void *this,
                           struct coral_lock *data,
                           struct is_equal_args *args);

static bool $lock_copy(void *this,
                       void *data,
                       void *args);

__attribute__((constructor(CORAL_CLASS_LOAD_PRIORITY_LOCK)))
static void $on_load() {
    struct coral_class_method_name $method_names[] = {
            {destroy,   strlen(destroy)},
            {hash_code, strlen(hash_code)},
            {is_equal,  strlen(is_equal)},
            {copy,      strlen(copy)},
    };
    coral_required_true(coral_class_alloc(&$class));
    coral_required_true(coral_class_init($class));
    coral_required_true(coral_class_retain($class));
    /* destroy */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[0],
            (coral_invokable_t) $lock_destroy));
    /* hash_code */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[1],
            (coral_invokable_t) $lock_hash_code));
    /* is_equal */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[2],
            (coral_invokable_t) $lock_is_equal));
    /* copy */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[3],
            (coral_invokable_t) $lock_copy));

    coral_autorelease_pool_drain();
}

__attribute__((destructor(CORAL_CLASS_LOAD_PRIORITY_LOCK)))
static void $on_unload() {
    coral_required_true(coral_class_destroy($class));

    coral_autorelease_pool_drain();
}

bool coral$lock$init(struct coral$lock *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    pthread_mutexattr_t attr;
    if (pthread_mutexattr_init(&attr)
        || pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL)
        || pthread_mutex_init(&object->mutex, &attr)) {
        coral_required_true(EINVAL != errno);
        coral_error = CORAL_ERROR_INITIALIZATION_FAILED;
        return false;
    }
    return true;
}

bool coral$lock$invalidate(struct coral$lock *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    for (uint8_t state = 0;
         EBUSY == pthread_mutex_destroy(&object->mutex);
         coral_exponential_usleep(&state, MAXIMUM_USLEEP));
    return true;
}

static bool $lock$lock_or_unlock(struct coral$lock *object,
                                 int (*const func)(pthread_mutex_t *)) {
    coral_required(object);
    coral_required(func);
    int error;
    switch (error = func(&object->mutex)) {
        case EINVAL: {
            coral_error = CORAL_ERROR_INVALID_VALUE;
            break;
        }
        case EDEADLK:
        case EPERM: {
            coral_error = CORAL_ERROR_OBJECT_UNAVAILABLE;
            break;
        }
        default: {
            break;
        }
    }
    return !error;
}

bool coral$lock$lock(struct coral$lock *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return $lock$lock_or_unlock(object, pthread_mutex_lock);
}

bool coral$lock$unlock(struct coral$lock *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return $lock$lock_or_unlock(object, pthread_mutex_unlock);
}

struct coral_lock {
    struct coral$lock lock;
};

static bool $lock_destroy(void *this,
                          struct coral_lock *data,
                          void *args) {
    coral_required(data);
    return coral$lock$invalidate(&data->lock);
}

static bool $lock_hash_code(void *this,
                            struct coral_lock *data,
                            struct hash_code_args *args) {
    coral_required(this);
    coral_required(args);
    coral_required(args->out);
    return coral_object_hash_code(this, args->out);
}

static bool $lock_is_equal(void *this,
                           struct coral_lock *data,
                           struct is_equal_args *args) {
    coral_required(this);
    coral_required(args);
    coral_required(args->out);
    return coral_object_is_equal(this, args->other, args->out);
}

static bool $lock_copy(void *this, void *data, void *args) {
    coral_error = CORAL_ERROR_OBJECT_UNAVAILABLE;
    return false; /* copying is not allowed */
}

static bool $lock_lock(void *this, struct coral_lock *data, void *args) {
    coral_required(data);
    return coral$lock$lock(&data->lock);
}

static bool $lock_unlock(void *this, struct coral_lock *data, void *args) {
    coral_required(data);
    return coral$lock$unlock(&data->lock);
}

#pragma mark public -

bool coral_lock_class(struct coral_class **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral_object_copy($class, (void **) out);
}

bool coral_lock_alloc(struct coral_lock **out) {
    return coral_object_alloc(sizeof(**out), (void **) out);
}

bool coral_lock_init(struct coral_lock *object) {
    return coral_object_init(object, $class)
           && coral$lock$init(&object->lock);
}

bool coral_lock_destroy(struct coral_lock *object) {
    return coral_object_destroy(object);
}

bool coral_lock_hash_code(struct coral_lock *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct hash_code_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $lock_hash_code,
            &args);
}

bool coral_lock_is_equal(struct coral_lock *object,
                         struct coral_lock *other,
                         bool *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!other || !out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct is_equal_args args = {
            .other = other,
            .out = out
    };
    return coral_object_instance_of(object, $class, out)
           && *out
           && coral_object_instance_of(other, $class, out)
           && *out
           && coral_object_invoke(
            object,
            (coral_invokable_t) $lock_is_equal,
            &args);
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

bool coral_lock_lock(struct coral_lock *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral_object_invoke(
            object,
            (coral_invokable_t) $lock_lock,
            NULL);
}

bool coral_lock_unlock(struct coral_lock *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral_object_invoke(
            object,
            (coral_invokable_t) $lock_unlock,
            NULL);
}

bool coral_lock_new_condition(struct coral_lock *object,
                              struct coral_lock_condition **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral_lock_condition_alloc(out)
           && coral_lock_condition_init(*out, object);
}
