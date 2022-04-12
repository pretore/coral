#ifndef _CORAL_PRIVATE_OBJECT_POOL_H_
#define _CORAL_PRIVATE_OBJECT_POOL_H_

#include <stddef.h>

struct coral_object;

struct coral$autorelease_pool_entry {
    struct coral$autorelease_pool_entry *next;
    void *object;
};

void coral$autorelease_pool_start();

void coral$autorelease_pool_end();

void coral$autorelease_pool_add(void *object);

void coral$autorelease_pool_add_object_init(void *object);

#endif /* _CORAL_PRIVATE_OBJECT_POOL_H_ */