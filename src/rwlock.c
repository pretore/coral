#include <stdlib.h>
#include <string.h>
#include <coral.h>

#include "private/object.h"
#include "private/rwlock.h"
#include "private/class.h"
#include "test/cmocka.h"

#pragma mark private -

static struct coral_class *$class;

#pragma mark + invokables

static bool $rwlock_destroy(void *this,
                            struct coral_rwlock *data,
                            void *args);

static bool $rwlock_hash_code(void *this,
                              struct coral_rwlock *data,
                              struct hash_code_args *args);

static bool $rwlock_is_equal(void *this,
                             struct coral_rwlock *data,
                             struct is_equal_args *args);

__attribute__((constructor(CORAL_CLASS_LOAD_PRIORITY_RWLOCK)))
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
            (coral_invokable_t) $rwlock_destroy));
    /* hash_code */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[1],
            (coral_invokable_t) $rwlock_hash_code));
    /* is_equal */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[2],
            (coral_invokable_t) $rwlock_is_equal));
    /* copy */

    coral_autorelease_pool_drain();
}

__attribute__((destructor(CORAL_CLASS_LOAD_PRIORITY_RWLOCK)))
static void $on_unload() {
    coral_required_true(coral_class_destroy($class));

    coral_autorelease_pool_drain();
}

struct coral_rwlock {
    struct coral$rwlock rwlock;
};

static bool $rwlock_instance_of(void *instance) {
    coral_required(instance);
    bool result;
    coral_required_true(coral_object_instance_of(instance,
                                                 $class,
                                                 &result));
    if (!result) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
    }
    return result;
}

static bool $rwlock_destroy(void *this,
                            struct coral_rwlock *data,
                            void *args) {
    coral_required(data);
    return coral$rwlock$invalidate(&data->rwlock);
}

static bool $rwlock_hash_code(void *this,
                              struct coral_rwlock *data,
                              struct hash_code_args *args) {
    coral_required(this);
    coral_required(args);
    coral_required(args->out);
    if (!$rwlock_instance_of(this)) {
        return false;
    }
    return coral_object_hash_code(this, args->out);
}

static bool $rwlock_is_equal(void *this,
                             struct coral_rwlock *data,
                             struct is_equal_args *args) {
    coral_required(this);
    coral_required(args);
    coral_required(args->out);
    void *objects[2] = {
            this, args->other
    };
    for (size_t i = 0; i < 2; i++) {
        bool result;
        coral_required_true(coral_object_instance_of(objects[i],
                                                     $class,
                                                     &result));
        if (!result) {
            *args->out = false;
            return true;
        }
    }
    return coral_object_is_equal(this, args->other, args->out);
}

static bool $rwlock_read_lock(void *this,
                              struct coral_rwlock *data,
                              void *args) {
    coral_required(data);
    if (!$rwlock_instance_of(this)) {
        return false;
    }
    return coral$rwlock$read_lock(&data->rwlock);
}

static bool $rwlock_write_lock(void *this,
                               struct coral_rwlock *data,
                               void *args) {
    coral_required(data);
    if (!$rwlock_instance_of(this)) {
        return false;
    }
    return coral$rwlock$write_lock(&data->rwlock);
}

#pragma mark public -

bool coral_rwlock_class(struct coral_class **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral_object_copy($class, (void **) out);
}

bool coral_rwlock_alloc(struct coral_rwlock **out) {
    return coral_object_alloc(sizeof(**out), (void **) out);
}

bool coral_rwlock_init(struct coral_rwlock *object) {
    return coral_object_init(object, $class)
           && coral$rwlock$init(&object->rwlock);
}

bool coral_rwlock_destroy(struct coral_rwlock *object) {
    return coral_object_destroy(object);
}

bool coral_rwlock_hash_code(struct coral_rwlock *object,
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
            true,
            (coral_invokable_t) $rwlock_hash_code,
            &args);
}

bool coral_rwlock_is_equal(struct coral_rwlock *object,
                           struct coral_rwlock *other,
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
    return coral_object_invoke(
            object,
            true,
            (coral_invokable_t) $rwlock_is_equal,
            &args);
}

bool coral_rwlock_retain(struct coral_rwlock *object) {
    return coral_object_retain(object);
}

bool coral_rwlock_release(struct coral_rwlock *object) {
    return coral_object_release(object);
}

bool coral_rwlock_autorelease(struct coral_rwlock *object) {
    return coral_object_autorelease(object);
}

bool coral_rwlock_read_lock(struct coral_rwlock *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral_object_invoke(
            object,
            true,
            (coral_invokable_t) $rwlock_read_lock,
            NULL);
}

bool coral_rwlock_write_lock(struct coral_rwlock *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral_object_invoke(
            object,
            true,
            (coral_invokable_t) $rwlock_write_lock,
            NULL);
}

