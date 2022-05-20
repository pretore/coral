#ifndef _CORAL_CONTEXT_H_
#define _CORAL_CONTEXT_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct coral_context;

/**
 * @brief Create context instance.
 * @param [out] out receive the created context instance.
 * @param [in] on_destroy called when instance should be destroyed.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the context instance.
 */
bool coral_context_of(struct coral_context **out, void (*on_destroy)(void *));

#pragma mark coral_object

/**
 * @brief Retrieve the class for context.
 * @param [out] out receive class.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral_context_class(struct coral_class **out);

/**
 * @brief Allocate memory for context instance.
 * @param [out] out receive the allocated context instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the context instance.
 */
bool coral_context_alloc(struct coral_context **out);

/**
 * @brief Initialize the context instance.
 * @param [in] object instance to be initialized.
 * @param [in] on_destroy called when instance should be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_context_init(struct coral_context *object,
                        void (*on_destroy)(void *));

/**
 * @brief Destroy the context instance.
 * @param [in] object instance to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_context_destroy(struct coral_context *object);

extern const char *destroy;

/**
 * @brief Retrieve the hash code of context.
 * @param [in] object instance whose hash code we would like to retrieve.
 * @param [out] out receive the hash code.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_context_hash_code(struct coral_context *object, size_t *out);

extern const char *hash_code;

/**
 * @brief Check if these two contexts are equal.
 * @param [in] object first context.
 * @param [in] other second context.
 * @param [out] out true if <b>object</b> and <b>other</b> are equal,
 * otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other or out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_context_is_equal(void *object, void *other, bool *out);

extern const char *is_equal;

/**
 * @brief Return a copy of the given context.
 * @param [in] object that we would like to copy.
 * @param [out] out receive the copy.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object and *out are the same object.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_context_copy(void *object, void **out);

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
bool coral_context_retain(struct coral_context *object);

/**
 * @brief Release and possibly free object resources.
 * @param [in] object whose reference count we are to decrease and upon zero
 * where we will free the memory used by <b>context</b> instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_context_release(struct coral_context *object);

/**
 * @brief Allow object to return from current function.
 * @param [in] object whose reference count will be retained and released in
 * such a manner so that the instance may return from the current function.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_context_autorelease(struct coral_context *object);

#pragma mark coral_context

/**
 * @brief Retrieve the <i>void*</i> address.
 * @param [in] object context instance.
 * @param [out] out receive the <i>void*</i> address.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_context_get(struct coral_context *object, void **out);

#endif /* _CORAL_CONTEXT_H_ */