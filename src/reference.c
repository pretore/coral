#include <string.h>
#include <coral.h>

#include "private/reference.h"
#include "private/class.h"
#include "test/cmocka.h"

#pragma mark private -

static struct coral_class *$class;

#pragma mark + invokables

static bool $reference_destroy(void *this,
                               struct coral_reference *data,
                               void *args);

static bool $reference_hash_code(void *this,
                                 struct coral_reference *data,
                                 struct hash_code_args *args);

static bool $reference_is_equal(void *this,
                                struct coral_reference *data,
                                struct is_equal_args *args);

static bool $reference_copy(void *this,
                            struct coral_reference *data,
                            struct copy_args *args);

__attribute__((constructor(CORAL_CLASS_LOAD_PRIORITY)))
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
            (coral_invokable_t) $reference_destroy));
    /* hash_code */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[1],
            (coral_invokable_t) $reference_hash_code));
    /* is_equal */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[2],
            (coral_invokable_t) $reference_is_equal));
    /* copy */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[3],
            (coral_invokable_t) $reference_copy));

    coral_autorelease_pool_drain();
}

__attribute__((destructor(CORAL_CLASS_LOAD_PRIORITY)))
static void $on_unload() {
    coral_required_true(coral_object_destroy($class));
    coral_autorelease_pool_drain();
}

bool coral$reference$init(struct coral$reference *object, void *instance) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!instance) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    const bool result = coral_object_retain(instance);
    if (result) {
        object->object = instance;
        // TODO: Add OBSERVER for OBJECT_DESTROYED ... (this is a fatal error event)
        // we have a strong ownership to instance; it should not be collected.
    }
    return result;
}

bool coral$reference$invalidate(struct coral$reference *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!object->object) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    }
    const bool result = coral_object_release(object->object);
    if (result) {
        // TODO: remove OBSERVER for OBJECT_DESTROYED
        object->object = NULL;
    }
    return result;
}

bool coral$reference$get(struct coral$reference *object, void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    *out = object->object;
    return true;
}

bool coral_reference_of(struct coral_reference **out, void *instance) {
    if (coral_reference_alloc(out)) {
        if (coral_reference_init(*out, instance)) {
            return true;
        }
        const size_t error = coral_error;
        coral_reference_destroy(*out);
        coral_error = error;
    }
    return false;
}

struct coral_reference {
    struct coral$reference reference;
};

static bool $reference_destroy(void *this,
                               struct coral_reference *data,
                               void *args) {
    coral_required(data);
    return coral$reference$invalidate(&data->reference);
}

static bool $reference_copy(void *this,
                            struct coral_reference *data,
                            struct copy_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->src);
    struct coral_reference *src = args->src;
    return coral$reference$init(&data->reference,
                                src->reference.object);
}

static bool $reference_hash_code(void *this,
                                 struct coral_reference *data,
                                 struct hash_code_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    bool result = coral_object_dispatch(data->reference.object,
                                        hash_code,
                                        args);
    if (!result) {
        coral_required_true(CORAL_ERROR_METHOD_NOT_FOUND != coral_error);
    }
    return result;
}

static bool $reference_is_equal(void *this,
                                struct coral_reference *data,
                                struct is_equal_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    struct coral_reference *object = data;
    struct coral_reference *other = args->other;
    if ((coral_object_instance_of(object, $class, args->out)
         && !*args->out)
        || (coral_object_instance_of(other, $class, args->out)
            && !*args->out)) {
        return true;
    }
    args->other = other->reference.object;
    bool result = coral_object_dispatch(object->reference.object,
                                        is_equal,
                                        args);
    if (!result) {
        coral_required_true(CORAL_ERROR_METHOD_NOT_FOUND != coral_error);
    }
    return result;
}

struct $reference_get_args {
    void **out;
};

static bool $reference_get(void *this,
                           struct coral_reference *data,
                           struct $reference_get_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    return coral$reference$get(&data->reference, args->out)
           && coral_object_autorelease(*args->out);
}

#pragma mark public -

bool coral_reference_class(struct coral_class **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral_object_copy($class, (void **) out);
}

bool coral_reference_alloc(struct coral_reference **out) {
    return coral_object_alloc(sizeof(**out), (void **) out);
}

bool coral_reference_init(struct coral_reference *object, void *instance) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!instance) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral_object_init(object, $class)
           && coral$reference$init(&object->reference, instance);
}

bool coral_reference_destroy(struct coral_reference *object) {
    return coral_object_destroy(object);
}

bool coral_reference_hash_code(struct coral_reference *object, size_t *out) {
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
            (coral_invokable_t) $reference_hash_code,
            &args);
}

bool coral_reference_is_equal(struct coral_reference *object,
                              struct coral_reference *other,
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
            (coral_invokable_t) $reference_is_equal,
            &args);
}

bool coral_reference_copy(struct coral_reference *object,
                          struct coral_reference **out) {
    return coral_object_copy(object, (void **) out);
}

bool coral_reference_retain(struct coral_reference *object) {
    return coral_object_retain(object);
}

bool coral_reference_release(struct coral_reference *object) {
    return coral_object_release(object);
}

bool coral_reference_autorelease(struct coral_reference *object) {
    return coral_object_autorelease(object);
}

bool coral_reference_get(struct coral_reference *object, void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $reference_get_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $reference_get,
            &args);
}
