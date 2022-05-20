#ifndef _CORAL_LOCK_H_
#define _CORAL_LOCK_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

struct coral_lock;

#pragma mark coral_object

/**
 * @brief Retrieve the class for lock.
 * @param [out] out receive class.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral_lock_class(struct coral_class **out);

/**
 * @brief Allocate memory for lock instance.
 * @param [in] size bytes needed to be allocated for lock instance.
 * @param [out] out receive the allocated lock instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the lock instance.
 */
bool coral_lock_alloc(struct coral_lock **out);

/**
 * @brief Initialize the lock instance.
 * @param [in] object instance to be initialized.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_lock_init(struct coral_lock *object);

/**
 * @brief Destroy the lock instance.
 * @param [in] object instance to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_lock_destroy(struct coral_lock *object);

extern const char *destroy;

/**
 * @brief Retrieve the hash code of the lock.
 * @param [in] object instance whose hash code we would like to retrieve.
 * @param [out] out receive the hash code.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_lock_hash_code(struct coral_lock *object, size_t *out);

extern const char *hash_code;

/**
 * @brief Check if these two locks are equal.
 * @param [in] object first lock.
 * @param [in] other second lock.
 * @param [out] out true if <b>object</b> and <b>other</b> are equal,
 * otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other or out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_lock_is_equal(struct coral_lock *object,
                         struct coral_lock *other,
                         bool *out);

extern const char *is_equal;

/**
 * @brief Lock is not copyable.
 * @return false.
 * @throws CORAL_ERROR_OBJECT_UNAVAILABLE since lock is not copyable.
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
bool coral_lock_retain(struct coral_lock *object);

/**
 * @brief Release and possibly free object resources.
 * @param [in] object whose reference count we are to decrease and upon zero
 * where we will free the memory used by <b>object</b> instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_lock_release(struct coral_lock *object);

/**
 * @brief Allow object to return from current function.
 * @param [in] object whose reference count will be retained and released in
 * such a manner so that the instance may return from the current function.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_lock_autorelease(struct coral_lock *object);

#pragma mark coral_lock

/**
 * @brief Acquire the lock.
 * @param [in] object lock we would like to acquire.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_lock_lock(struct coral_lock *object);

/**
 * @brief Release the lock.
 * @param [in] object lock we own and we are to release.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_UNAVAILABLE if the thread releasing the
 * object's lock does not own it.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_lock_unlock(struct coral_lock *object);

struct coral_lock_condition;

/**
 * @brief Create new lock condition bound to the given lock.
 * @param [in] object lock instance.
 * @param [out] out receive new lock condition.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the lock condition instance.
 */
bool coral_lock_new_condition(struct coral_lock *object,
                              struct coral_lock_condition **out);

#endif /* _CORAL_LOCK_H_ */