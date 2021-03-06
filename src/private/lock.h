#ifndef _CORAL_PRIVATE_LOCK_H_
#define _CORAL_PRIVATE_LOCK_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>

#include "object.h"

#define CORAL_CLASS_LOAD_PRIORITY_LOCK  \
    (1 + CORAL_CLASS_LOAD_PRIORITY_OBJECT)

struct coral$lock {
    pthread_mutex_t mutex;
};

/**
 * @brief Initialize the lock.
 * @param [in] object instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INITIALIZATION_FAILED if the lock instance failed to
 * initialize, more details can be obtained from errno(3).
 */
bool coral$lock$init(struct coral$lock *object);

/**
 * @brief Invalidate the lock.
 * <p>The actual <u>lock is not deallocated</u> since it may have been
 * embedded in a larger structure.</p>
 * @param [in] object instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral$lock$invalidate(struct coral$lock *object);

/**
 * @brief Acquire the lock.
 * @param [in] object instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_UNAVAILABLE if the thread already owns the lock.
 * @throws CORAL_ERROR_INVALID_VALUE if lock is invalid because it was not
 * initialized.
 */
bool coral$lock$lock(struct coral$lock *object);

/**
 * @brief Release the lock.
 * @param [in] object instance
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_UNAVAILABLE if the thread releasing the lock does
 * not own it.
 * @throws CORAL_ERROR_INVALID_VALUE if lock is invalid because it was not
 * initialized.
 */
bool coral$lock$unlock(struct coral$lock *object);

#endif /* _CORAL_PRIVATE_LOCK_H_ */