#include <coral.h>

#include "private/reference.h"

#pragma mark private

bool coral$ref_init(struct coral_ref *object, void *instance) {
    coral_required(object);
    coral_required(instance);
    if (!coral_object_retain(instance)) {
        return false;
    }
    object->object = instance;
    void (*on_destroy)(void *) = (void (*)(void *)) coral$ref_destroy;
    if (!coral_object_init(object, on_destroy)) {
        return false;
    }
    // TODO: Add OBSERVER for OBJECT_DESTROYED ... (this is a fatal error event)
    // we have a strong ownership to instance; it should not be collected.
    return true;
}

void coral$ref_destroy(struct coral_ref *object) {
    coral_required(object);
    // TODO: remove OBSERVER for OBJECT_DESTROYED
    coral_required_true(coral_object_release(object->object));
    coral_required_true(coral_object_destroy(object));
}

bool coral$ref_get(struct coral_ref *object, struct coral$ref_get_args *args) {
    coral_required(object);
    coral_required(args);
    void *instance = object->object;
    *args->out = instance;
    return coral_object_autorelease(instance);
}

#pragma mark public

bool coral_ref_of(struct coral_ref **out, void *instance) {
    if (coral_ref_alloc(out)) {
        if (coral_ref_init(*out, instance)) {
            return true;
        }
        coral_ref_destroy(*out);
    }
    return false;
}

bool coral_ref_alloc(struct coral_ref **out) {
    return coral_object_alloc(sizeof(struct coral_ref), (void **) out);
}

bool coral_ref_init(struct coral_ref *object, void *instance) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!instance) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral$ref_init(object, instance);
}

bool coral_ref_destroy(struct coral_ref *object) {
    return coral_object_destroy(object);
}

bool coral_ref_retain(struct coral_ref *object) {
    return coral_object_retain(object);
}

bool coral_ref_release(struct coral_ref *object) {
    return coral_object_release(object);
}

bool coral_ref_autorelease(struct coral_ref *object) {
    return coral_object_autorelease(object);
}

bool coral_ref_get(struct coral_ref *object, void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct coral$ref_get_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$ref_get,
            &args);
}