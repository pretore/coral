#include <coral.h>

#include "private/range.h"

#pragma mark private

struct coral_range_values $values = {0, SIZE_MAX};

bool
coral$range_step_function_delta(const size_t current, const bool prev,
                                const struct coral_range_values values,
                                void *context, size_t *out) {
    if (!context) {
        return false;
    }
    const size_t *delta = context;
    size_t value;
    if (prev) {
        value = current - (*delta);
        if (current < value && !coral_minimum_size_t(values.first,
                                                     value,
                                                     &value)) {
            return false;
        }
    } else {
        value = current + (*delta);
        if (current > value && !coral_maximum_size_t(values.last,
                                                     value,
                                                     &value)) {
            return false;
        }
    }
    *out = value;
    return current != *out;
}

bool
coral$range_step_function_rate(const size_t current, const bool prev,
                               const struct coral_range_values values,
                               void *context, size_t *out) {
    if (!context) {
        return false;
    }
    const double *rate = context;
    size_t value;
    if (prev) {
        value = current / (*rate);
    } else {
        value = current * (*rate);
        if (current > value && !coral_maximum_size_t(values.last,
                                                     value,
                                                     &value)) {
            return false;
        }
    }
    *out = value;
    return current != *out;
}

bool coral$range_init(struct coral_range *object,
                      struct coral_range_values *values,
                      coral_range_step_func_t step_func,
                      struct coral_context *context) {
    coral_required(object);
    coral_required(step_func);
    if (context && !coral_set_ref(&object->context_ref, context)) {
        return false;
    }
    void (*on_destroy)(void *) = (void (*)(void *)) coral$range_destroy;
    if (!coral_object_init(object, on_destroy)) {
        return false;
    }
    if (!values) {
        values = &$values;
    }
    if (!coral_minimum_size_t(values->first, values->last, &object->min)
        || !coral_maximum_size_t(values->first, values->last, &object->max)) {
        return false;
    }
    object->step_func = step_func;
    return true;
}

void coral$range_destroy(struct coral_range *object) {
    coral_required(object);
    coral_required_true(coral_clear_ref(&object->context_ref));
    coral_required_true(coral_object_destroy(object));
}

bool coral$range_get_min(struct coral_range *object,
                         struct coral$range_get_min_args *args) {
    coral_required(object);
    coral_required(args);
    *args->out = object->min;
    return true;
}

bool coral$range_get_max(struct coral_range *object,
                         struct coral$range_get_max_args *args) {
    coral_required(object);
    coral_required(args);
    *args->out = object->max;
    return true;
}

bool coral$range_get(struct coral_range *object,
                     struct coral$range_get_args *args) {
    coral_required(object);
    coral_required(args);
    coral_required(args->out);
    struct coral_range_values *values = args->out;
    values->first = object->min;
    values->last = object->max;
    return true;
}

bool coral$range_get_next(struct coral_range *object,
                          struct coral$range_get_next_args *args) {
    coral_required(object);
    coral_required(args);
    coral_required(args->out);
    coral_range_step_func_t step_func = object->step_func;
    void *data = NULL;
    struct coral_context *context;
    if (object->context_ref
        && (!coral_ref_get(object->context_ref, (void **) &context)
            || !coral_context_get(context, &data))) {
        coral_error = CORAL_ERROR_ARGUMENT_UNAVAILABLE;
        return false;
    }
    struct coral_range_values values = {
            .first = object->min,
            .last = object->max
    };
    if (!step_func(args->current, false, values, data, args->out)
        || values.first > *args->out
        || values.last < *args->out) {
        coral_error = CORAL_ERROR_END_OF_SEQUENCE;
        return false;
    }
    return true;
}

bool coral$range_get_prev(struct coral_range *object,
                          struct coral$range_get_prev_args *args) {
    coral_required(object);
    coral_required(args);
    coral_required(args->out);
    coral_range_step_func_t step_func = object->step_func;
    void *data = NULL;
    struct coral_context *context;
    if (object->context_ref
        && (!coral_ref_get(object->context_ref, (void **) &context)
            || !coral_context_get(context, &data))) {
        coral_error = CORAL_ERROR_ARGUMENT_UNAVAILABLE;
        return false;
    }
    struct coral_range_values values = {
            .first = object->min,
            .last = object->max
    };
    if (!step_func(args->current, true, values, data, args->out)
        || values.first > *args->out
        || values.last < *args->out) {
        coral_error = CORAL_ERROR_END_OF_SEQUENCE;
        return false;
    }
    return true;
}

#pragma mark public

bool coral_range_of_delta(struct coral_range **out,
                          struct coral_range_values *values,
                          const size_t delta) {
    if (coral_range_alloc(out)) {
        struct coral_context *context;
        size_t *delta_;
        if (!coral_context_of(&context, NULL)
            || !coral_context_get(context, (void **) &delta_)) {
            return false;
        }
        *delta_ = delta;
        struct coral_range_values v_;
        if (coral_range_init(*out,
                             values,
                             coral$range_step_function_delta,
                             context)
            && coral_range_get(*out, &v_)) {
            if (!delta || v_.last - v_.first < delta) {
                coral_error = CORAL_ERROR_INVALID_VALUE;
            } else {
                return true;
            }
        }
        coral_range_destroy(*out);
    }
    return false;
}

bool coral_range_of_rate(struct coral_range **out,
                         struct coral_range_values *values,
                         const double multiplier) {
    if (multiplier <= 0 || 1 == multiplier) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
    } else if (coral_range_alloc(out)) {
        struct coral_context *context;
        double *multiplier_;
        if (!coral_context_of(&context, NULL)
            || !coral_context_get(context, (void **) &multiplier_)) {
            return false;
        }
        *multiplier_ = multiplier;
        struct coral_range_values v_;
        if (coral_range_init(*out,
                             values,
                             coral$range_step_function_rate,
                             context)
            && coral_range_get(*out, &v_)) {
            if (v_.first * multiplier > v_.last) {
                coral_error = CORAL_ERROR_INVALID_VALUE;
            } else {
                return true;
            }
        }
        coral_range_destroy(*out);
    }
    return false;
}

bool coral_range_alloc(struct coral_range **out) {
    return coral_object_alloc(sizeof(struct coral_range), (void **) out);
}

bool coral_range_init(struct coral_range *object,
                      struct coral_range_values *values,
                      coral_range_step_func_t step_func,
                      struct coral_context *context) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!step_func) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral$range_init(object, values, step_func, context);
}

bool coral_range_destroy(struct coral_range *object) {
    return coral_object_destroy(object);
}

bool coral_range_retain(struct coral_range *object) {
    return coral_object_retain(object);
}

bool coral_range_autorelease(struct coral_range *object) {
    return coral_object_autorelease(object);
}

bool coral_range_release(struct coral_range *object) {
    return coral_object_release(object);
}

bool coral_range_get_min(struct coral_range *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct coral$range_get_min_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$range_get_min,
            &args);
}

bool coral_range_get_max(struct coral_range *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct coral$range_get_max_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$range_get_max,
            &args);
}

bool coral_range_get(struct coral_range *object,
                     struct coral_range_values *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct coral$range_get_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$range_get,
            &args);
}

bool coral_range_get_next(struct coral_range *object, const size_t current,
                          size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct coral$range_get_next_args args = {
            .current = current,
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$range_get_next,
            &args);
}

bool coral_range_get_prev(struct coral_range *object, const size_t current,
                          size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct coral$range_get_prev_args args = {
            .current = current,
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$range_get_prev,
            &args);
}
