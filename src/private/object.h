#ifndef _CORAL_PRIVATE_OBJECT_H_
#define _CORAL_PRIVATE_OBJECT_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdatomic.h>

struct coral_object {
    atomic_size_t ref_count;
    // FIXME: Implement observer pattern for events ...
    void (*on_destroy)(void *);
};

bool coral$object_alloc(size_t size, void **out);

bool coral$object_init(void *object, void (*on_destroy)(void *));

void coral$object_destroy(void *object);

bool coral$object_retain(void *object, void *);

bool coral$object_release(void *object, void *);

struct coral_object *coral$object_from(void *object);

void *coral$object_to(struct coral_object *object);

void (*coral$object_get_on_destroy(struct coral_object *object))(void *);

void
coral$object_add_observer(struct coral_object *object, void *observer,
                          const char *event,
                          void(*on_event)(void *sender, const char *event));

void
coral$object_remove_observer(struct coral_object *object, void *observer,
                             const char *event);

void coral$object_post_notification(void *object, const char *notification);

#endif /* _CORAL_PRIVATE_OBJECT_H_ */