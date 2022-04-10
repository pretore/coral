#ifndef _CORAL_PRIVATE_LOCK_H_
#define _CORAL_PRIVATE_LOCK_H_

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>

struct coral_lock {
    pthread_mutex_t *mutex;
};

bool coral$lock_alloc(struct coral_lock **out);

bool coral$lock_init(struct coral_lock *object);

void coral$lock_destroy(struct coral_lock *object);

bool coral$lock_acquire_lock(struct coral_lock *object);

bool coral$lock_release_lock(struct coral_lock *object);

bool coral$acquire_lock(size_t count, struct coral_lock **locks,
                        struct coral_lock **lock);

bool coral$release_lock(size_t count, struct coral_lock **locks);

#endif /* _CORAL_PRIVATE_LOCK_H_ */