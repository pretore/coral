#ifndef _CORAL_PRIVATE_RWLOCK_H_
#define _CORAL_PRIVATE_RWLOCK_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>

#include "object.h"

#define CORAL_CLASS_LOAD_PRIORITY_RWLOCK  \
    (1 + CORAL_CLASS_LOAD_PRIORITY_OBJECT)

struct coral$rwlock {
    pthread_rwlock_t rwlock;
};

/**
 * @brief Initialize the read-write lock.
 * @param [in] object instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INITIALIZATION_FAILED if the read-write instance failed
 * to initialize, more details can be obtained from errno(3).
 */
bool coral$rwlock$init(struct coral$rwlock *object);

/**
 * @brief Invalidate the read-write lock.
 * <p>The actual <u>read-write lock is not deallocated</u> since it may have
 * been embedded in a larger structure.</p>
 * @param [in] object instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral$rwlock$invalidate(struct coral$rwlock *object);

/**
 * @brief Acquire the read lock.
 * @param [in] object instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_UNAVAILABLE if the thread already owns the
 * write lock.
 * @throws CORAL_ERROR_INVALID_VALUE if read-write lock is invalid because it
 * was not initialized or the maximum number of read locks has been exceeded.
 */
bool coral$rwlock$read_lock(struct coral$rwlock *object);

/**
 * @brief Acquire the write lock.
 * @param [in] object instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_UNAVAILABLE if the thread already owns the
 * lock for reading or writing.
 * @throws CORAL_ERROR_INVALID_VALUE if read-write lock is invalid because it
 * was not initialized.
 */
bool coral$rwlock$write_lock(struct coral$rwlock *object);

/**
 * @brief Release the lock.
 * @param [in] object instance
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_UNAVAILABLE if the thread releasing the lock does
 * not own it.
 * @throws CORAL_ERROR_INVALID_VALUE if read-write lock is invalid because it
 * was not initialized.
 */
bool coral$rwlock$unlock(struct coral$rwlock *object);

#endif /* _CORAL_PRIVATE_RWLOCK_H_ */
