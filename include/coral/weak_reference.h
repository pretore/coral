#ifndef _CORAL_WEAK_REFERENCE_H_
#define _CORAL_WEAK_REFERENCE_H_

#include <stddef.h>
#include <stdbool.h>

struct coral_weak_ref;

/**
 * @brief Create weak reference instance.
 * @param [out] out receive the created reference instance.
 * @param [in] instance for which we will be a weak reference.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the weak reference instance.
 */
bool coral_weak_ref_of(struct coral_weak_ref **out, void *instance);

/**
 * @brief Create weak reference instance.
 * @param [out] out receive the created weak reference instance.
 * @param [in] instance for which we will be a weak reference.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the weak reference instance.
 */
bool coral_weak_ref_alloc(struct coral_weak_ref **out);

/**
 * @brief Initialize the weak reference instance.
 * @param [in] object weak reference to be initialized.
 * @param [in] instance for which we will be a weak reference.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_weak_ref_init(struct coral_weak_ref *object, void *instance);

/**
 * @brief Destroy the weak reference instance.
 * @param [in] object instance to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_weak_ref_destroy(struct coral_weak_ref *object);

/**
 * @brief Retain the referenced counted object.
 * @param [in] object whose reference count is to be increase to prevent early
 * collection.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_weak_ref_retain(struct coral_weak_ref *object);

/**
 * @brief Release and possibly free object resources.
 * @param [in] object whose reference count we are to decrease and upon zero
 * where we will free the memory used by <b>object</b> instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_weak_ref_release(struct coral_weak_ref *object);

/**
 * @brief Allow object to return from current function.
 * @param [in] object whose reference count will be retained and released in
 * such a manner so that the instance may return from the current function.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_weak_ref_autorelease(struct coral_weak_ref *object);

/**
 * @brief Retrieve the <i>void*</i> address.
 * @param [in] object weak reference instance.
 * @param [out] out receive instance for which we are a weak reference for.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_weak_ref_get(struct coral_weak_ref *object, void **out);

#endif /* _CORAL_WEAK_REFERENCE_H_ */