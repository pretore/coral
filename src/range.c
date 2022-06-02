#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <coral.h>

#include "private/range.h"
#include "private/class.h"
#include "test/cmocka.h"

#pragma mark private -

static struct coral_class *$class;

#pragma mark + invokables

static bool $range_destroy(void *this,
                           struct coral_range *data,
                           void *args);

static bool $range_hash_code(void *this,
                             struct coral_range *data,
                             struct hash_code_args *args);

static bool $range_is_equal(void *this,
                            struct coral_range *data,
                            struct is_equal_args *args);

__attribute__((constructor(CORAL_CLASS_LOAD_PRIORITY_RANGE)))
static void $on_load() {
    struct coral_class_method_name $method_names[] = {
            {destroy,   strlen(destroy)},
            {hash_code, strlen(hash_code)},
            {is_equal,  strlen(is_equal)}
    };
    coral_required_true(coral_class_alloc(&$class));
    coral_required_true(coral_class_init($class));
    coral_required_true(coral_class_retain($class));
    /* destroy */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[0],
            (coral_invokable_t) $range_destroy));
    /* hash_code */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[1],
            (coral_invokable_t) $range_hash_code));
    /* is_equal */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[2],
            (coral_invokable_t) $range_is_equal));

    coral_autorelease_pool_drain();
}

__attribute__((destructor(CORAL_CLASS_LOAD_PRIORITY_RANGE)))
static void $on_unload() {
    coral_required_true(coral_class_destroy($class));

    coral_autorelease_pool_drain();
}

bool coral$range$init(struct coral$range *object,
                      const struct coral_range_values values,
                      coral_range_step_func step_func,
                      struct coral_context *context) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!step_func) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (context && !coral_context_retain(context)) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    }
    object->values = values;
    object->step_func = step_func;
    object->context = context;
    return true;
}

bool coral$range$invalidate(struct coral$range *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (object->context) {
        coral_required_true(coral_context_release(object->context));
    }
    object->context = NULL;
    object->step_func = NULL;
    object->values.first = 0;
    object->values.last = 0;
    return true;
}

bool coral$range$get_first(struct coral$range *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    *out = object->values.first;
    return true;
}

bool coral$range$get_last(struct coral$range *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    *out = object->values.last;
    return true;
}

bool coral$range$get_minimum(struct coral$range *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral_minimum_size_t(object->values.first, object->values.last, out);
}

bool coral$range$get_maximum(struct coral$range *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral_maximum_size_t(object->values.first, object->values.last, out);
}

bool coral$range$is_inclusive(struct coral$range *object, size_t current,
                              bool *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral_inclusive_size_t(object->values.first, object->values.last,
                                  current, out);
}

static bool coral$range$get(struct coral$range *object, const size_t current,
                            const bool previous, size_t *out) {
    coral_required(object);
    coral_required(out);
    bool is_inclusive;
    coral_required_true(coral$range$is_inclusive(
            object, current, &is_inclusive));
    if (!is_inclusive) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    }
    void *context = NULL;
    if (object->context && !coral_context_get(object->context, &context)) {
        coral_error = CORAL_ERROR_OBJECT_UNAVAILABLE;
        return false;
    }
    bool result = object->step_func(current, previous, object->values,
                                    context, out);
    if (result) {
        coral_required_true(coral$range$is_inclusive(
                object, *out, &is_inclusive));
        if (!is_inclusive) {
            coral_error = CORAL_ERROR_END_OF_SEQUENCE;
            return false;
        }
    }
    return result;
}

bool coral$range$get_next(struct coral$range *object, const size_t current,
                          size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral$range$get(object, current, false, out);
}

bool coral$range$get_prev(struct coral$range *object, const size_t current,
                          size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral$range$get(object, current, true, out);
}

static bool $range_step_func_delta(const size_t current,
                                   const bool previous,
                                   const struct coral_range_values values,
                                   void *context,
                                   size_t *out) {
    coral_required(context);
    const ssize_t delta = **(ssize_t **) context;
    *out = previous ? current - delta : delta + current;
    return true;
}

static bool $range_step_func_rate(const size_t current,
                                  const bool previous,
                                  const struct coral_range_values values,
                                  void *context,
                                  size_t *out) {
    coral_required(context);
    double rate = **(double **) context;
    bool result;
    if (previous) {
        coral_required_true(coral_double_is_equal(0, rate, &result));
        if (result) {
            return false;
        } else {
            rate = 1 / rate;
        }
    }
    double c;
    if (!coral_multiply_double((double) current, rate, &c)) {
        return false;
    }
    *out = (size_t) floor(c);
    result = previous ? *out <= current : *out >= current;
    if (!result) {
        coral_error = CORAL_ERROR_LOSS_OF_PRECISION;
    }
    return result;
}

static void $context_on_destroy(void *ptr) {
    free(ptr);
}

bool coral_range_of_delta(struct coral_range **out,
                          struct coral_range_values values,
                          ssize_t delta) {
    if (!delta) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    }
    if (coral_range_alloc(out)) {
        struct coral_context *context;
        ssize_t **address = NULL;
        if (coral_context_of(&context, $context_on_destroy)
            && coral_context_get(context, (void **) &address)
            && (*address = malloc(sizeof(**address)))
            && coral_range_init(*out,
                                values,
                                $range_step_func_delta,
                                context)) {
            **address = delta;
            return true;
        }
        if (address && !(*address)) {
            coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        }
    }
    return false;
}

bool coral_range_of_rate(struct coral_range **out,
                         struct coral_range_values values,
                         double multiplier) {
    if (!multiplier) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    }
    if (coral_range_alloc(out)) {
        struct coral_context *context;
        double **address = NULL;
        if (coral_context_of(&context, $context_on_destroy)
            && coral_context_get(context, (void **) &address)
            && (*address = malloc(sizeof(**address)))
            && coral_range_init(*out,
                                values,
                                $range_step_func_rate,
                                context)) {
            **address = multiplier;
            return true;
        }
        if (address && !(*address)) {
            coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        }
    }
    return false;
}

struct coral_range {
    struct coral$range range;
};

static bool $range_destroy(void *this, struct coral_range *data, void *args) {
    coral_required(this);
    coral_required(data);
    return coral$range$invalidate(&data->range);
}

static bool $range_hash_code(void *this,
                             struct coral_range *data,
                             struct hash_code_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    const size_t first = data->range.values.first >> 1;
    const size_t last = data->range.values.last >> 1;
    *args->out = first + last;
    return true;
}

static bool $range_is_equal(void *this,
                            struct coral_range *data,
                            struct is_equal_args *args) {
    coral_required(args);
    coral_required(args->other);
    coral_required(args->out);
    *args->out = !memcmp(data, args->other, sizeof(*data));
    return true;
}

struct $range_get_first_args {
    size_t *out;
};

static bool $range_get_first(void *this,
                             struct coral_range *data,
                             struct $range_get_first_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    return coral$range$get_first(&data->range, args->out);
}

struct $range_get_last_args {
    size_t *out;
};

static bool $range_get_last(void *this,
                            struct coral_range *data,
                            struct $range_get_last_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    return coral$range$get_last(&data->range, args->out);
}

struct $range_get_minimum_args {
    size_t *out;
};

static bool $range_get_minimum(void *this,
                               struct coral_range *data,
                               struct $range_get_minimum_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    return coral$range$get_minimum(&data->range, args->out);
}

struct $range_get_maximum_args {
    size_t *out;
};

static bool $range_get_maximum(void *this,
                               struct coral_range *data,
                               struct $range_get_maximum_args *args) {
    coral_required(this);
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    return coral$range$get_maximum(&data->range, args->out);
}

struct $range_is_inclusive_args {
    size_t value;
    bool *out;
};

static bool $range_is_inclusive(void *this,
                                struct coral_range *data,
                                struct $range_is_inclusive_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    return coral$range$is_inclusive(&data->range, args->value, args->out);
}

struct $range_get_next_args {
    size_t current;
    size_t *out;
};

static bool $range_get_next(void *this,
                            struct coral_range *data,
                            struct $range_get_next_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    return coral$range$get_next(&data->range, args->current, args->out);
}

struct $range_get_prev_args {
    size_t current;
    size_t *out;
};

static bool $range_get_prev(void *this,
                            struct coral_range *data,
                            struct $range_get_prev_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    return coral$range$get_prev(&data->range, args->current, args->out);
}

#pragma mark public

bool coral_range_class(struct coral_class **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral_object_copy($class, (void **) out);
}

bool coral_range_alloc(struct coral_range **out) {
    return coral_object_alloc(sizeof(**out), (void **) out);
}

bool coral_range_init(struct coral_range *object,
                      struct coral_range_values values,
                      coral_range_step_func step_func,
                      struct coral_context *context) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!step_func) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral_object_init(object, $class)
           && coral$range$init(&object->range, values, step_func, context);
}

bool coral_range_destroy(struct coral_range *object) {
    return coral_object_destroy(object);
}

bool coral_range_hash_code(struct coral_range *object, size_t *out) {
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
            (coral_invokable_t) $range_hash_code,
            &args);
}

bool coral_range_is_equal(struct coral_range *object,
                          struct coral_range *other,
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
            (coral_invokable_t) $range_is_equal,
            &args);
}

bool coral_range_copy(struct coral_range *object, struct coral_range **out) {
    return coral_object_copy(object, (void **) out);
}

bool coral_range_retain(struct coral_range *object) {
    return coral_object_retain(object);
}

bool coral_range_release(struct coral_range *object) {
    return coral_object_release(object);
}

bool coral_range_autorelease(struct coral_range *object) {
    return coral_object_autorelease(object);
}

bool coral_range_get_first(struct coral_range *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $range_get_first_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $range_get_first,
            &args);
}

bool coral_range_get_last(struct coral_range *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $range_get_last_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $range_get_last,
            &args);
}

bool coral_range_get_minimum(struct coral_range *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $range_get_minimum_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $range_get_minimum,
            &args);
}

bool coral_range_get_maximum(struct coral_range *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $range_get_maximum_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $range_get_maximum,
            &args);
}

bool coral_range_is_inclusive(struct coral_range *object, const size_t value,
                              bool *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $range_is_inclusive_args args = {
            .value = value,
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $range_is_inclusive,
            &args);
}

bool coral_range_get_next(struct coral_range *object, size_t current,
                          size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $range_get_next_args args = {
            .current = current,
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $range_get_next,
            &args);
}

bool coral_range_get_prev(struct coral_range *object, size_t current,
                          size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $range_get_prev_args args = {
            .current = current,
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $range_get_prev,
            &args);
}
