#ifndef _CORAL_REFERENCE_H_
#define _CORAL_REFERENCE_H_

#include <stddef.h>
#include <stdbool.h>

struct coral_ref;

/**
 * @brief Create reference instance.
 * @param [out] out receive the created reference instance.
 * @param [in] instance for which we will be a strong reference.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the reference instance.
 */
bool coral_ref_of(struct coral_ref **out, void *instance);

/**
 * @brief Allocate memory for reference instance.
 * @param [out] out receive the allocated reference instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the reference instance.
 */
bool coral_ref_alloc(struct coral_ref **out);

/**
 * @brief Initialize the reference instance.
 * @param [in] object reference to be initialized.
 * @param [in] instance for which we will be a strong reference.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_ref_init(struct coral_ref *object, void *instance);

/**
 * @brief Destroy the reference instance.
 * @param [in] object instance to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_ref_destroy(struct coral_ref *object);

/**
 * @brief Retain the referenced counted object.
 * @param [in] object whose reference count is to be increase to prevent early
 * collection.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_ref_retain(struct coral_ref *object);

/**
 * @brief Release and possibly free object resources.
 * @param [in] object whose reference count we are to decrease and upon zero
 * where we will free the memory used by <b>object</b> instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_ref_release(struct coral_ref *object);

/**
 * @brief Allow object to return from current function.
 * @param [in] object whose reference count will be retained and released in
 * such a manner so that the instance may return from the current function.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_ref_autorelease(struct coral_ref *object);

/**
 * @brief Retrieve the <i>void*</i> address.
 * @param [in] object reference instance.
 * @param [out] out receive instance for which we are a strong reference for.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_ref_get(struct coral_ref *object, void **out);

#endif /* _CORAL_REFERENCE_H_ */