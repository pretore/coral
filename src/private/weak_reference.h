#ifndef _CORAL_PRIVATE_WEAK_REFERENCE_H_
#define _CORAL_PRIVATE_WEAK_REFERENCE_H_

#include <stddef.h>
#include <stdbool.h>

struct coral_weak_ref {
    void *object;
};

bool coral$weak_ref_alloc(struct coral_weak_ref **out);

bool coral$weak_ref_init(struct coral_weak_ref *object, void *instance);

void coral$weak_ref_destroy(struct coral_weak_ref *object);

struct coral$weak_ref_get_args {
    void **out;
};

bool coral$weak_ref_get(struct coral_weak_ref *object,
                        struct coral$weak_ref_get_args *args);

#endif /* _CORAL_PRIVATE_WEAK_REFERENCE_H_ */