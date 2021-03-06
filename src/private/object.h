#ifndef _CORAL_PRIVATE_OBJECT_H_
#define _CORAL_PRIVATE_OBJECT_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdatomic.h>

#include "class.h"

#define CORAL_CLASS_LOAD_PRIORITY_OBJECT \
    (1 + CORAL_CLASS_LOAD_PRIORITY_CLASS)

struct coral_object;

struct coral_object *coral$object_from(void *object);

void *coral$object_to(struct coral_object *object);

bool coral$object$get_class(struct coral_object *object,
                            struct coral_class **out);

bool coral$object$get_ref_count(struct coral_object *object, size_t *out);

void
coral$object_add_observer(struct coral_object *object, void *observer,
                          const char *event,
                          void(*on_event)(void *sender, const char *event));

void
coral$object_remove_observer(struct coral_object *object, void *observer,
                             const char *event);

void coral$object_post_notification(void *object, const char *notification);

#endif /* _CORAL_PRIVATE_OBJECT_H_ */