#ifndef _CORAL_CLASS_H_
#define _CORAL_CLASS_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct coral_class;

#pragma mark coral_object

/**
 * @brief Allocate memory for the class.
 * @param [out] out receive the allocated class.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate for the class.
 */
bool coral_class_alloc(struct coral_class **out);

/**
 * @brief Initialize the class.
 * @param [in] object class to be initialized.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_class_init(struct coral_class *object);

/**
 * @brief Destroy the class.
 * @param [in] object class to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_class_destroy(struct coral_class *object);

extern const char *destroy;

/**
 * @brief Retrieve the hash code of the class.
 * @param [in] object instance whose hash code we would like to retrieve.
 * @param [out] out receive the hash code.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_class_hash_code(struct coral_class *object, size_t *out);

extern const char *hash_code;

/**
 * @brief Check if these two classes are equal.
 * @param [in] object first class.
 * @param [in] other second class.
 * @param [out] out true if <b>object</b> and <b>other</b> are equal,
 * otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other or out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_class_is_equal(struct coral_class *object, void *other, bool *out);

extern const char *is_equal;

/**
 * @brief Return a copy of the given class.
 * @param [in] object that we would like to copy.
 * @param [out] out receive the copy.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_class_copy(struct coral_class *object, struct coral_class *out);

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
bool coral_class_retain(struct coral_class *object);

/**
 * @brief Release and possibly free object resources.
 * @param [in] object whose reference count we are to decrease and upon zero
 * where we will free the memory used by <b>object</b> instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_class_release(struct coral_class *object);

/**
 * @brief Allow object to return from current function.
 * @param [in] object whose reference count will be retained and released in
 * such a manner so that the instance may return from the current function.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_class_autorelease(struct coral_class *object);

#pragma mark coral_class

struct coral_class_method_name {
    const char *data;
    size_t size;
};

/**
 * @brief Add method to the class.
 * @param [in] object class which will have a new method added.
 * @param [in] name method for which the invokable will be mapped to.
 * @param [in] invokable runnable code for the associated method name.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if name or invokable is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if method name is invalid by having either
 * <i>ZERO</i> size or <i>NULL</i> data.
 * @throws CORAL_ERROR_METHOD_ALREADY_EXISTS if method is already in the class.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_class_method_add(struct coral_class *object,
                            struct coral_class_method_name *name,
                            coral_invokable_t invokable);

/**
 * @brief Remove method from the class.
 * @param [in] object class which the method will be removed from.
 * @param [in] name method that will be removed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if name <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if method name is invalid by having either
 * <i>ZERO</i> size or <i>NULL</i> data.
 * @throws CORAL_ERROR_METHOD_NOT_FOUND if method is not in the class.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_class_method_remove(struct coral_class *object,
                               struct coral_class_method_name *name);

/**
 * @brief Retrieve the invokable for the given method.
 * @param [in] object class which we will look up the method by name.
 * @param [in] name of method whose invokable we would like to retrieve.
 * @param [out] out receive the invokable.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if name or out is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if method name is invalid by having either
 * <i>ZERO</i> size or <i>NULL</i> data.
 * @throws CORAL_ERROR_METHOD_NOT_FOUND if method is not in the class.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_class_method_get(struct coral_class *object,
                            struct coral_class_method_name *name,
                            coral_invokable_t *out);

#endif /* _CORAL_CLASS_H_ */
