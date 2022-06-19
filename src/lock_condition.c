#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <coral.h>

#include "private/coral.h"
#include "private/class.h"
#include "private/lock_condition.h"
#include "test/cmocka.h"

#pragma mark private -

static struct coral_class *$class;

#pragma mark + invokables

static bool $lock_condition_destroy(void *this,
                                    struct coral_lock_condition *data,
                                    void *args);

static bool $lock_condition_hash_code(void *this,
                                      struct coral_lock_condition *data,
                                      struct hash_code_args *args);

static bool $lock_condition_is_equal(void *this,
                                     struct coral_lock_condition *data,
                                     struct is_equal_args *args);

static bool $lock_condition_copy(void *this,
                                 void *data,
                                 void *args);

__attribute__((constructor(CORAL_CLASS_LOAD_PRIORITY_LOCK_CONDITION)))
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
            (coral_invokable_t) $lock_condition_destroy));
    /* hash_code */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[1],
            (coral_invokable_t) $lock_condition_hash_code));
    /* is_equal */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[2],
            (coral_invokable_t) $lock_condition_is_equal));
    /* copy */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[3],
            (coral_invokable_t) $lock_condition_copy));

    coral_autorelease_pool_drain();
}

__attribute__((destructor(CORAL_CLASS_LOAD_PRIORITY_LOCK_CONDITION)))
static void $on_unload() {
    coral_required_true(coral_class_destroy($class));

    coral_autorelease_pool_drain();
}

bool coral$lock_condition$init(struct coral$lock_condition *object,
                               struct coral_lock *lock) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!lock) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    bool result = !pthread_cond_init(&object->cond, NULL)
                  && coral$reference$init(&object->lock_ref, lock);
    if (!result) {
        coral_error = CORAL_ERROR_INITIALIZATION_FAILED;
    }
    return result;
}

bool coral$lock_condition$invalidate(struct coral$lock_condition *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    for (uint8_t state = 0;
         EBUSY == pthread_cond_destroy(&object->cond);
         coral_exponential_usleep(&state, MAXIMUM_USLEEP));
    return coral$reference$invalidate(&object->lock_ref);
}

bool coral$lock_condition$await(struct coral$lock_condition *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    struct coral_lock *lock = NULL;
    coral_required_true(coral$reference$get(&object->lock_ref,
                                            (void **) &lock));
    const int error = pthread_cond_wait(&object->cond,
                                        (pthread_mutex_t *) lock);
    switch (error) {
        case EINVAL: {
            coral_error = CORAL_ERROR_INVALID_VALUE;
            break;
        }
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

bool coral$lock_condition$signal(struct coral$lock_condition *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    const int error = pthread_cond_signal(&object->cond);
    if (EINVAL == error) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
    }
    return !error;
}

bool coral$lock_condition$signal_all(struct coral$lock_condition *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    const int error = pthread_cond_broadcast(&object->cond);
    if (EINVAL == error) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
    }
    return !error;
}

struct coral_lock_condition {
    struct coral$lock_condition condition;
};

static bool $lock_condition_destroy(void *this,
                                    struct coral_lock_condition *data,
                                    void *args) {
    coral_required(data);
    return coral$lock_condition$invalidate(&data->condition);
}

static bool $lock_condition_hash_code(void *this,
                                      struct coral_lock_condition *data,
                                      struct hash_code_args *args) {
    coral_required(this);
    coral_required(args);
    coral_required(args->out);
    return coral_object_hash_code(this, args->out);
}

static bool $lock_condition_is_equal(void *this,
                                     struct coral_lock_condition *data,
                                     struct is_equal_args *args) {
    coral_required(this);
    coral_required(args);
    coral_required(args->out);
    return coral_object_is_equal(this, args->other, args->out);
}

static bool $lock_condition_copy(void *this, void *data, void *args) {
    coral_error = CORAL_ERROR_OBJECT_UNAVAILABLE;
    return false; /* copying is not allowed */
}

static bool $lock_condition_await(void *this,
                                  struct coral_lock_condition *data,
                                  void *args) {
    coral_required(data);
    return coral$lock_condition$await(&data->condition);
}

static bool $lock_condition_signal(void *this,
                                   struct coral_lock_condition *data,
                                   void *args) {
    coral_required(data);
    return coral$lock_condition$signal(&data->condition);
}

static bool $lock_condition_signal_all(void *this,
                                       struct coral_lock_condition *data,
                                       void *args) {
    coral_required(data);
    return coral$lock_condition$signal_all(&data->condition);
}

#pragma mark public -

bool coral_lock_condition_class(struct coral_class **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral_object_copy($class, (void **) out);
}

bool coral_lock_condition_alloc(struct coral_lock_condition **out) {
    return coral_object_alloc(sizeof(**out), (void **) out);
}

bool coral_lock_condition_init(struct coral_lock_condition *object,
                               struct coral_lock *lock) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!lock) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral_object_init(object, $class)
           && coral$lock_condition$init(&object->condition, lock);
}

bool coral_lock_condition_destroy(struct coral_lock_condition *object) {
    return coral_object_destroy(object);
}

bool coral_lock_condition_hash_code(struct coral_lock_condition *object,
                                    size_t *out) {
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
            (coral_invokable_t) $lock_condition_hash_code,
            &args);
}

bool coral_lock_condition_is_equal(struct coral_lock_condition *object,
                                   struct coral_lock_condition *other,
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
            (coral_invokable_t) $lock_condition_is_equal,
            &args);
}

bool coral_lock_condition_retain(struct coral_lock_condition *object) {
    return coral_object_retain(object);
}

bool coral_lock_condition_release(struct coral_lock_condition *object) {
    return coral_object_release(object);
}

bool coral_lock_condition_autorelease(struct coral_lock_condition *object) {
    return coral_object_autorelease(object);
}

bool coral_lock_condition_await(struct coral_lock_condition *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral_object_invoke(
            object,
            (coral_invokable_t) $lock_condition_await,
            NULL);
}

bool coral_lock_condition_signal(struct coral_lock_condition *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral_object_invoke(
            object,
            (coral_invokable_t) $lock_condition_signal,
            NULL);
}

bool coral_lock_condition_signal_all(struct coral_lock_condition *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral_object_invoke(
            object,
            (coral_invokable_t) $lock_condition_signal_all,
            NULL);
}
