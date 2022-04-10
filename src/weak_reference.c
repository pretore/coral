#include <coral.h>

#include "private/weak_reference.h"

#pragma mark private

bool coral$weak_ref_alloc(struct coral_weak_ref **out) {
    return coral_object_alloc(sizeof(struct coral_weak_ref), (void **) out);
}

bool coral$weak_ref_init(struct coral_weak_ref *object, void *instance) {
    coral_required(object);
    coral_required(instance);
    void (*on_destroy)(void *) = (void (*)(void *)) coral$weak_ref_destroy;
    if (!coral_object_init(object, on_destroy)) {
        return false;
    }
    object->object = instance;
    // TODO: Add OBSERVER for OBJECT_DESTROYED ... to clear object->object
    return true;
}

void coral$weak_ref_destroy(struct coral_weak_ref *object) {
    coral_required(object);
    // TODO: remove OBSERVER for OBJECT_DESTROYED
    coral_required_true(coral_object_destroy(object));
}

bool coral$weak_ref_get(struct coral_weak_ref *object,
                        struct coral$weak_ref_get_args *args) {
    coral_required(object);
    coral_required(args);
    void *instance = object->object;
    *args->out = instance;
    const bool result = coral_object_autorelease(instance);
    if (!result && CORAL_ERROR_OBJECT_IS_UNINITIALIZED == coral_error) {
        coral_error = CORAL_ERROR_ARGUMENT_UNAVAILABLE;
    }
    return result;
}

#pragma mark public

bool coral_weak_ref_of(struct coral_weak_ref **out, void *instance) {
    if (coral_weak_ref_alloc(out)) {
        if (coral_weak_ref_init(*out, instance)) {
            return true;
        }
        coral_weak_ref_destroy(*out);
    }
    return false;
}

bool coral_weak_ref_alloc(struct coral_weak_ref **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral$weak_ref_alloc(out);
}

bool coral_weak_ref_init(struct coral_weak_ref *object, void *instance) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!instance) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral$weak_ref_init(object, instance);
}

bool coral_weak_ref_destroy(struct coral_weak_ref *object) {
    return coral_object_destroy(object);
}

bool coral_weak_ref_retain(struct coral_weak_ref *object) {
    return coral_object_retain(object);
}

bool coral_weak_ref_release(struct coral_weak_ref *object) {
    return coral_object_release(object);
}

bool coral_weak_ref_autorelease(struct coral_weak_ref *object) {
    return coral_object_autorelease(object);
}

bool coral_weak_ref_get(struct coral_weak_ref *object, void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct coral$weak_ref_get_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$weak_ref_get,
            &args);
}
