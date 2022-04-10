#ifndef _CORAL_PRIVATE_CONTEXT_H_
#define _CORAL_PRIVATE_CONTEXT_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct coral_context {
    void *data;
};

struct coral$context_get_args {
    void **out;
};

bool coral$context_get(struct coral_context *object,
                       struct coral$context_get_args *args);

#endif /* _CORAL_PRIVATE_CONTEXT_H_ */