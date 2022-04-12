#ifndef _CORAL_PRIVATE_REFERENCE_H_
#define _CORAL_PRIVATE_REFERENCE_H_

#include <stddef.h>
#include <stdbool.h>

struct coral_ref {
    void* object;
};

bool coral$ref_init(struct coral_ref *object, void *instance);

void coral$ref_destroy(struct coral_ref *object);

struct coral$ref_get_args {
    void **out;
};

bool coral$ref_get(struct coral_ref *object, struct coral$ref_get_args *args);

#endif /* _CORAL_PRIVATE_REFERENCE_H_ */