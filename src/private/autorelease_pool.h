#ifndef _CORAL_PRIVATE_AUTORELEASE_POOL_H_
#define _CORAL_PRIVATE_AUTORELEASE_POOL_H_

#include <stddef.h>

void coral$autorelease_pool_start();

void coral$autorelease_pool_end();

void coral$autorelease_pool_add(void *object);

void coral$autorelease_pool_add_object_init(void *object);

#endif /* _CORAL_PRIVATE_AUTORELEASE_POOL_H_ */