#ifndef _CORAL_LOCK_H_
#define _CORAL_LOCK_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

struct coral_lock;

bool coral_lock_alloc(struct coral_lock **out);

bool coral_lock_init(struct coral_lock *object);

bool coral_lock_destroy(struct coral_lock *object);

bool coral_lock_retain(struct coral_lock *object);

bool coral_lock_release(struct coral_lock *object);

bool coral_lock_autorelease(struct coral_lock *object);

// FIXME: change to lock_group / lock_hierarchy
bool coral_acquire_lock(size_t count, struct coral_lock **locks,
                        struct coral_lock **out);

bool coral_release_lock(size_t count, struct coral_lock **locks);

#endif /* _CORAL_LOCK_H_ */