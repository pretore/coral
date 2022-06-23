#include <string.h>
#include <coral.h>

#include "private/weak_reference.h"
#include "private/class.h"
#include "test/cmocka.h"

#pragma mark private -

static struct coral_class *$class;

#pragma mark + invokables

static bool $weak_reference_destroy(void *this,
                                    struct coral_weak_reference *data,
                                    void *args);

static bool $weak_reference_hash_code(void *this,
                                      struct coral_weak_reference *data,
                                      struct hash_code_args *args);

static bool $weak_reference_is_equal(void *this,
                                     struct coral_weak_reference *data,
                                     struct is_equal_args *args);

static bool $weak_reference_copy(void *this,
                                 struct coral_weak_reference *data,
                                 struct copy_args *args);

__attribute__((constructor(CORAL_CLASS_LOAD_PRIORITY_WEAK_REFERENCE)))
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
            (coral_invokable_t) $weak_reference_destroy));
    /* hash_code */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[1],
            (coral_invokable_t) $weak_reference_hash_code));
    /* is_equal */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[2],
            (coral_invokable_t) $weak_reference_is_equal));
    /* copy */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[3],
            (coral_invokable_t) $weak_reference_copy));

    coral_autorelease_pool_drain();
}

__attribute__((destructor(CORAL_CLASS_LOAD_PRIORITY_WEAK_REFERENCE)))
static void $on_unload() {
    coral_required_true(coral_class_destroy($class));
    coral_autorelease_pool_drain();
}

bool coral$weak_reference$init(struct coral$weak_reference *object,
                               void *instance) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!instance) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    object->object = instance;
    // TODO: Add OBSERVER for OBJECT_DESTROYED ... to clear object->object
    return true;
}

bool coral$weak_reference$invalidate(struct coral$weak_reference *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    // TODO: remove OBSERVER for OBJECT_DESTROYED
    object->object = NULL;
    return true;
}

bool coral$weak_reference$get(struct coral$weak_reference *object,
                              void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    const bool result = coral_object_autorelease(object->object);
    if (!result) {
        coral_error = CORAL_ERROR_OBJECT_NOT_FOUND;
    } else {
        *out = object->object;
    }
    return result;
}

struct coral_weak_reference {
    struct coral$weak_reference weak_reference;
};

static bool $weak_reference_destroy(void *this,
                                    struct coral_weak_reference *data,
                                    void *args) {
    coral_required(data);
    return coral$weak_reference$invalidate(&data->weak_reference);
}

static bool $weak_reference_copy(void *this,
                                 struct coral_weak_reference *data,
                                 struct copy_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->src);
    struct coral_weak_reference *src = args->src;
    return coral$weak_reference$init(&data->weak_reference,
                                     src->weak_reference.object);
}

static bool $weak_reference_hash_code(void *this,
                                      struct coral_weak_reference *data,
                                      struct hash_code_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    bool result;
    coral_required_true(coral_object_instance_of(this,
                                                 $class,
                                                 &result));
    result = coral_object_dispatch(data->weak_reference.object,
                                        true,
                                        hash_code,
                                        args);
    if (!result) {
        coral_required_true(CORAL_ERROR_METHOD_NOT_FOUND != coral_error);
    }
    return result;
}

static bool $weak_reference_is_equal(void *this,
                                     struct coral_weak_reference *data,
                                     struct is_equal_args *args) {
    coral_required(this);
    coral_required(data);
    coral_required(args);
    coral_required(args->other);
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
    struct coral_weak_reference *object = data;
    struct coral_weak_reference *other = args->other;
    args->other = other->weak_reference.object;
    bool result = coral_object_dispatch(object->weak_reference.object,
                                        true,
                                        is_equal,
                                        args);
    if (!result) {
        coral_required_true(CORAL_ERROR_METHOD_NOT_FOUND != coral_error);
    }
    return result;
}

struct $weak_reference_get_args {
    void **out;
};

static bool $weak_reference_get(void *this,
                                struct coral_weak_reference *data,
                                struct $weak_reference_get_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    return coral$weak_reference$get(&data->weak_reference, args->out)
           && coral_object_autorelease(*args->out);
}

#pragma mark public -

bool coral_weak_reference_of(struct coral_weak_reference **out,
                             void *instance) {
    if (coral_weak_reference_alloc(out)) {
        if (coral_weak_reference_init(*out, instance)) {
            return true;
        }
        const size_t error = coral_error;
        coral_required_true(coral_weak_reference_destroy(*out));
        coral_error = error;
    }
    return false;
}

bool coral_weak_reference_class(struct coral_class **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral_object_copy($class, (void **) out);
}

bool coral_weak_reference_alloc(struct coral_weak_reference **out) {
    return coral_object_alloc(sizeof(**out), (void **) out);
}

bool coral_weak_reference_init(struct coral_weak_reference *object,
        void *instance) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!instance) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    bool result = false;
    if (coral_object_init(object, $class)) {
        if (coral$weak_reference$init(&object->weak_reference, instance)) {
            result = true;
        }
    }
    return result;
}

bool coral_weak_reference_destroy(struct coral_weak_reference *object) {
    return coral_object_destroy(object);
}

bool coral_weak_reference_hash_code(struct coral_weak_reference *object,
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
            (coral_invokable_t) $weak_reference_hash_code,
            &args);
}

bool coral_weak_reference_is_equal(struct coral_weak_reference *object,
                                   struct coral_weak_reference *other,
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
            (coral_invokable_t) $weak_reference_is_equal,
            &args);
}

bool coral_weak_reference_copy(struct coral_weak_reference *object,
                               struct coral_weak_reference **out) {
    return coral_object_copy(object, (void **) out);
}

bool coral_weak_reference_retain(struct coral_weak_reference *object) {
    return coral_object_retain(object);
}

bool coral_weak_reference_release(struct coral_weak_reference *object) {
    return coral_object_release(object);
}

bool coral_weak_reference_autorelease(struct coral_weak_reference *object) {
    return coral_object_autorelease(object);
}

bool coral_weak_reference_get(struct coral_weak_reference *object, void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $weak_reference_get_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            true,
            (coral_invokable_t) $weak_reference_get,
            &args);
}
