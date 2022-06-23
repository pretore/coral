#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <coral.h>

#include "coral.h"
#include "rwlock.h"

bool coral$rwlock$init(struct coral$rwlock *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (pthread_rwlock_init(&object->rwlock, NULL)) {
        coral_error = CORAL_ERROR_INITIALIZATION_FAILED;
        return false;
    }
    return true;
}

bool coral$rwlock$invalidate(struct coral$rwlock *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    for (uint8_t state = 0;
         EBUSY == pthread_rwlock_destroy(&object->rwlock);
         coral_exponential_usleep(&state, MAXIMUM_USLEEP));
    return true;
}

static bool $rwlock$action(struct coral$rwlock *object,
                           int (*const func)(pthread_rwlock_t *)) {
    coral_required(object);
    coral_required(func);
    const int error = func(&object->rwlock);
    switch (error) {
        case EAGAIN:
        case EINVAL: {
            coral_error = CORAL_ERROR_INVALID_VALUE;
            break;
        }
        case EBUSY:
        case EDEADLK:
        case EPERM: {
            coral_error = CORAL_ERROR_OBJECT_UNAVAILABLE;
            break;
        }
        default: {
            break;
        }
    }
    return !error;
}

bool coral$rwlock$read_lock(struct coral$rwlock *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return $rwlock$action(object, pthread_rwlock_rdlock);
}

bool coral$rwlock$write_lock(struct coral$rwlock *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return $rwlock$action(object, pthread_rwlock_wrlock);
}

bool coral$rwlock$unlock(struct coral$rwlock *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return $rwlock$action(object, pthread_rwlock_unlock);
}
