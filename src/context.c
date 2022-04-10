#include <coral.h>

#include "private/context.h"

#pragma mark private

bool coral$context_get(struct coral_context *object,
                       struct coral$context_get_args *args) {
    coral_required(object);
    coral_required(args);
    *args->out = &(*object).data;
    return true;
}

#pragma mark public

bool coral_context_of(struct coral_context **out, void (*on_destroy)(void *)) {
    if (coral_context_alloc(out)) {
        if (coral_context_init(*out, on_destroy)) {
            return true;
        }
        coral_context_destroy(*out);
    }
    return false;
}

bool coral_context_alloc(struct coral_context **out) {
    return coral_object_alloc(sizeof(struct coral_context), (void **) out);
}

bool
coral_context_init(struct coral_context *object, void (*on_destroy)(void *)) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!coral_object_init(object, on_destroy)) {
        return false;
    }
    return true;
}

bool coral_context_destroy(struct coral_context *object) {
    return coral_object_destroy(object);
}

bool coral_context_retain(struct coral_context *object) {
    return coral_object_retain(object);
}

bool coral_context_release(struct coral_context *object) {
    return coral_object_release(object);
}

bool coral_context_autorelease(struct coral_context *object) {
    return coral_object_autorelease(object);
}

bool coral_context_get(struct coral_context *object, void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct coral$context_get_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$context_get,
            &args);
}