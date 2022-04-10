#ifndef _CORAL_PRIVATE_RANGE_H_
#define _CORAL_PRIVATE_RANGE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <coral/range.h>

struct coral_ref;

struct coral_range {
    size_t min;
    size_t max;
    coral_range_step_func_t step_func;
    struct coral_ref *context_ref;
};

bool
coral$range_step_function_delta(size_t current, bool prev,
                                struct coral_range_values values,
                                void *context, size_t *out);

bool
coral$range_step_function_rate(size_t current, bool prev,
                               struct coral_range_values values,
                               void *context, size_t *out);

bool coral$range_init(struct coral_range *object,
                      struct coral_range_values *values,
                      coral_range_step_func_t step_func,
                      struct coral_context *context);

void coral$range_destroy(struct coral_range *object);

struct coral$range_get_min_args {
    size_t *out;
};

bool coral$range_get_min(struct coral_range *object,
                         struct coral$range_get_min_args *args);

struct coral$range_get_max_args {
    size_t *out;
};

bool coral$range_get_max(struct coral_range *object,
                         struct coral$range_get_max_args *args);

struct coral$range_get_args {
    struct coral_range_values *out;
};

bool
coral$range_get(struct coral_range *object, struct coral$range_get_args *args);

struct coral$range_get_next_args {
    size_t current;
    size_t *out;
};

bool coral$range_get_next(struct coral_range *object,
                          struct coral$range_get_next_args *args);

struct coral$range_get_prev_args {
    size_t current;
    size_t *out;
};

bool coral$range_get_prev(struct coral_range *object,
                          struct coral$range_get_prev_args *args);

struct coral$range_get_context_args {
    struct coral_context **out;
};

bool coral$range_get_context(struct coral_range *object,
                             struct coral$range_get_context_args *args);

#endif /* _CORAL_PRIVATE_RANGE_H_ */