#ifndef _CORAL_LOCK_CONDITION_H_
#define _CORAL_LOCK_CONDITION_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

struct coral_lock_condition;

#pragma mark coral_object

struct coral_class;
/**
 * @brief Retrieve the class for lock condition.
 * @param [out] out receive class.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral_lock_condition_class(struct coral_class **out);

/**
 * @brief Allocate memory for lock condition instance.
 * @param [in] size bytes needed to be allocated for lock condition instance.
 * @param [out] out receive the allocated lock instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the lock condition instance.
 */
bool coral_lock_condition_alloc(struct coral_lock_condition **out);

/**
 * @brief Initialize the lock condition instance.
 * @param [in] object instance to be initialized.
 * @param [in] lock used to wait on for a signal.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_lock_condition_init(struct coral_lock_condition *object,
                               struct coral_lock *lock);

/**
 * @brief Destroy the lock condition instance.
 * @param [in] object instance to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_lock_condition_destroy(struct coral_lock_condition *object);

extern const char *destroy;

/**
 * @brief Retrieve the hash code of the lock condition.
 * @param [in] object instance whose hash code we would like to retrieve.
 * @param [out] out receive the hash code.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_lock_condition_hash_code(struct coral_lock_condition *object,
                                    size_t *out);

extern const char *hash_code;

/**
 * @brief Check if these two lock conditions are equal.
 * @param [in] object first lock condition.
 * @param [in] other second lock condition.
 * @param [out] out true if <b>object</b> and <b>other</b> are equal,
 * otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other or out is <i>NULL</i>.
 */
bool coral_lock_condition_is_equal(struct coral_lock_condition *object,
                                   struct coral_lock_condition *other,
                                   bool *out);

extern const char *is_equal;

/**
 * @brief Lock condition is not copyable.
 * @return false.
 * @throws CORAL_ERROR_OBJECT_UNAVAILABLE since lock condition is not copyable.
 */
extern const char *copy;

/**
 * @brief Retain the referenced counted object.
 * @param [in] object whose reference count is to be increase to prevent early
 * collection.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_lock_condition_retain(struct coral_lock_condition *object);

/**
 * @brief Release and possibly free object resources.
 * @param [in] object whose reference count we are to decrease and upon zero
 * where we will free the memory used by <b>object</b> instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_lock_condition_release(struct coral_lock_condition *object);

/**
 * @brief Allow object to return from current function.
 * @param [in] object whose reference count will be retained and released in
 * such a manner so that the instance may return from the current function.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_lock_condition_autorelease(struct coral_lock_condition *object);

#pragma mark coral_lock_condition

/**
 * @brief Wait on signal.
 * @param [in] object lock condition instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if the lock is invalid.
 * @throws CORAL_ERROR_OBJECT_UNAVAILABLE if the thread attempting to regain
 * the lock does not own it.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_lock_condition_await(struct coral_lock_condition *object);

/**
 * @brief Wake up a single thread.
 * @param [in] object lock condition instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if the lock condition is invalid.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_lock_condition_signal(struct coral_lock_condition *object);

/**
 * @brief Wake up all waiting threads.
 * @param [in] object lock condition instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if the lock condition is invalid.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_lock_condition_signal_all(struct coral_lock_condition *object);

#endif /* _CORAL_LOCK_CONDITION_H_ */