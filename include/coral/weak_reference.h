#ifndef _CORAL_WEAK_REFERENCE_H_
#define _CORAL_WEAK_REFERENCE_H_

#include <stddef.h>
#include <stdbool.h>

struct coral_weak_reference;

/**
 * @brief Create weak reference instance.
 * @param [out] out receive the created weak reference instance.
 * @param [in] instance for which we will be a weak reference.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out or instance is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the weak reference instance.
 */
bool coral_weak_reference_of(struct coral_weak_reference **out, void *instance);

#pragma mark coral_object

/**
 * @brief Retrieve the class for weak reference.
 * @param [out] out receive class.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral_weak_reference_class(struct coral_class **out);

/**
 * @brief Allocate memory for weak reference instance.
 * @param [out] out receive the allocated weak reference instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the weak reference instance.
 */
bool coral_weak_reference_alloc(struct coral_weak_reference **out);

/**
 * @brief Initialize the weak reference instance.
 * @param [in] object weak reference to be initialized.
 * @param [in] instance for which we will be a weak reference.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if instance is <i>NULL</i>.
 */
bool coral_weak_reference_init(struct coral_weak_reference *object,
                               void *instance);

/**
 * @brief Destroy the weak reference instance.
 * @param [in] object instance to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_weak_reference_destroy(struct coral_weak_reference *object);

extern const char* destroy;

/**
 * @brief Retrieve the hash code of the weak reference.
 * @param [in] object instance whose hash code we would like to retrieve.
 * @param [out] out receive the hash code.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_weak_reference_hash_code(struct coral_weak_reference *object,
                                    size_t *out);

extern const char *hash_code;

/**
 * @brief Check if these two weak references are equal.
 * @param [in] object first weak reference.
 * @param [in] other second weak reference.
 * @param [out] out true if <b>object</b> and <b>other</b> are equal,
 * otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other or out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_weak_reference_is_equal(struct coral_weak_reference *object,
                                   struct coral_weak_reference *other,
                                   bool *out);

extern const char *is_equal;

/**
 * @brief Return a copy of the given weak reference.
 * @param [in] object that we would like to copy.
 * @param [out] out receive the copy.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object and *out are the same object.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_weak_reference_copy(struct coral_weak_reference *object,
                               struct coral_weak_reference **out);

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
bool coral_weak_reference_retain(struct coral_weak_reference *object);

/**
 * @brief Release and possibly free object resources.
 * @param [in] object whose reference count we are to decrease and upon zero
 * where we will free the memory used by <b>object</b> instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_weak_reference_release(struct coral_weak_reference *object);

/**
 * @brief Allow object to return from current function.
 * @param [in] object whose reference count will be retained and released in
 * such a manner so that the instance may return from the current function.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_weak_reference_autorelease(struct coral_weak_reference *object);

#pragma mark coral_weak_reference

/**
 * @brief Retrieve the <i>void*</i> address.
 * @param [in] object weak reference instance.
 * @param [out] out receive instance for which we are a weak reference for.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if there is no instance contained in
 * the weak reference.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_weak_reference_get(struct coral_weak_reference *object, void **out);

#endif /* _CORAL_WEAK_REFERENCE_H_ */