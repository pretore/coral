#ifndef _CORAL_PRIVATE_LOCK_CONDITION_H_
#define _CORAL_PRIVATE_LOCK_CONDITION_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

#include "reference.h"

#define CORAL_CLASS_LOAD_PRIORITY_LOCK_CONDITION \
    (1 + CORAL_CLASS_LOAD_PRIORITY_REFERENCE)

struct coral$lock_condition {
    struct coral$reference lock_ref;
    pthread_cond_t cond;
};

/**
 * @brief Initialize the lock condition.
 * @param [in] object instance to be initialized.
 * @param [in] lock used to wait on for a signal.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral$lock_condition$init(struct coral$lock_condition *object,
                               struct coral_lock *lock);

/**
 * @brief Invalidate the lock condition.
 * <p>The actual <u>lock condition is not deallocated</u> since it may have been
 * embedded in a larger structure.
 * @param [in] object instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral$lock_condition$invalidate(struct coral$lock_condition *object);

/**
 * @brief Wait on signal.
 * @param [in] object lock condition instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if the lock is invalid.
 * @throws CORAL_ERROR_OBJECT_UNAVAILABLE if the thread attempting to regain
 * the lock does not own it.
 */
bool coral$lock_condition$await(struct coral$lock_condition *object);

/**
 * @brief Wake up a single thread.
 * @param [in] object lock condition instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if the lock condition is invalid.
 */
bool coral$lock_condition$signal(struct coral$lock_condition *object);

/**
 * @brief Wake up all waiting threads.
 * @param [in] object lock condition instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if the lock condition is invalid.
 */
bool coral$lock_condition$signal_all(struct coral$lock_condition *object);

#endif /* _CORAL_PRIVATE_LOCK_CONDITION_H_ */