#ifndef _CORAL_PRIVATE_OBJECT_H_
#define _CORAL_PRIVATE_OBJECT_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdatomic.h>

struct coral_object {
    struct coral_class *class;
    atomic_size_t ref_count;
    struct coral_object *copy_of;
    size_t size;
    size_t checksum;
    // FIXME: Implement observer pattern for events ...
};

struct coral_object *coral$object_from(void *object);

void *coral$object_to(struct coral_object *object);


void
coral$object_add_observer(struct coral_object *object, void *observer,
                          const char *event,
                          void(*on_event)(void *sender, const char *event));

void
coral$object_remove_observer(struct coral_object *object, void *observer,
                             const char *event);

void coral$object_post_notification(void *object, const char *notification);

#endif /* _CORAL_PRIVATE_OBJECT_H_ */