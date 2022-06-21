#ifndef _CORAL_RWLOCK_H_
#define _CORAL_RWLOCK_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

struct coral_rwlock;

#pragma mark coral_object

struct coral_class;
/**
 * @brief Retrieve the class for read-write lock.
 * @param [out] out receive class.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral_rwlock_class(struct coral_class **out);

/**
 * @brief Allocate memory for read-write lock instance.
 * @param [in] size bytes needed to be allocated for lock instance.
 * @param [out] out receive the allocated read-write lock instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the read-write lock instance.
 */
bool coral_rwlock_alloc(struct coral_rwlock **out);

/**
 * @brief Initialize the read-write lock instance.
 * @param [in] object instance to be initialized.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_rwlock_init(struct coral_rwlock *object);

/**
 * @brief Destroy the read-write lock instance.
 * @param [in] object instance to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_rwlock_destroy(struct coral_rwlock *object);

extern const char *destroy;

/**
 * @brief Retrieve the hash code of the read-write lock.
 * @param [in] object instance whose hash code we would like to retrieve.
 * @param [out] out receive the hash code.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object is not a read-write lock
 * instance.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_rwlock_hash_code(struct coral_rwlock *object,
                                     size_t *out);

extern const char *hash_code;

/**
 * @brief Check if these two read-write locks are equal.
 * @param [in] object first read-write lock.
 * @param [in] other second read-write lock.
 * @param [out] out true if <b>object</b> and <b>other</b> are equal,
 * otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other or out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_rwlock_is_equal(struct coral_rwlock *object,
                                    struct coral_rwlock *other,
                                    bool *out);

extern const char *is_equal;

/**
 * @brief Retain the referenced counted object.
 * @param [in] object whose reference count is to be increase to prevent early
 * collection.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_rwlock_retain(struct coral_rwlock *object);

/**
 * @brief Release and possibly free object resources.
 * @param [in] object whose reference count we are to decrease and upon zero
 * where we will free the memory used by <b>object</b> instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_rwlock_release(struct coral_rwlock *object);

/**
 * @brief Allow object to return from current function.
 * @param [in] object whose reference count will be retained and released in
 * such a manner so that the instance may return from the current function.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_rwlock_autorelease(struct coral_rwlock *object);

#pragma mark coral_rwlock

/**
 * @brief Acquire the read lock.
 * @param [in] object instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_UNAVAILABLE if the thread already owns the
 * write lock.
 * @throws CORAL_ERROR_INVALID_VALUE if object is not a lock instance or the
 * maximum number of read locks has been exceeded.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_rwlock_read_lock(struct coral_rwlock *object);

/**
 * @brief Acquire the write lock.
 * @param [in] object instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_UNAVAILABLE if the thread already owns the
 * lock for reading or writing.
 * @throws CORAL_ERROR_INVALID_VALUE if object is not a lock instance.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_rwlock_write_lock(struct coral_rwlock *object);

#endif /* _CORAL_RWLOCK_H_ */
