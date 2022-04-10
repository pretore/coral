#ifndef _CORAL_OBJECT_H_
#define _CORAL_OBJECT_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct coral_object;

/**
 * @brief Allocate memory for object instance.
 * @param [in] size bytes needed to be allocated for object instance.
 * @param [out] out receive the allocated object instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the object instance.
 */
bool coral_object_alloc(size_t size, void **out);

/**
 * @brief Initialize the object instance.
 * @param [in] object instance to be initialized.
 * @param [in] on_destroy called when instance should be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_object_init(void *object, void(*on_destroy)(void *));

/**
 * @brief Destroy the object instance.
 * @param [in] object instance to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_object_destroy(void *object);

/**
 * @brief Retain the referenced counted object.
 * @param [in] object whose reference count is to be increase to prevent early
 * collection.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_object_retain(void *object);

/**
 * @brief Release and possibly free object resources.
 * @param [in] object whose reference count we are to decrease and upon zero
 * where we will free the memory used by <b>object</b> instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_object_release(void *object);

/**
 * @brief Allow object to return from current function.
 * @param [in] object whose reference count will be retained and released in
 * such a manner so that the instance may return from the current function.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_object_autorelease(void *object);

typedef bool (*coral_invokable_t)(void *, void *);
/**
 * @brief Invoke methods on objects.
 * <p>It is required, <u>when implementing calls to objects</u>, that it
 * would be done through this method so that services such as memory management
 * can be performed.</p>
 * @param [in] object state of the instance.
 * @param [in] function invoked to perform actions on object with the given
 * arguments.
 * @param [in, out] args used by function to modify object and to provide
 * results.
 * @return On success true, otherwise false if an error has occurred.
 * @note If object or function is <i>NULL</i> we will call abort(3).
 */
bool coral_object_invoke(void *object, coral_invokable_t function, void *args);

#endif /* _CORAL_OBJECT_H_ */
