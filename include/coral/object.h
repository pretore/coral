#ifndef _CORAL_OBJECT_H_
#define _CORAL_OBJECT_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct coral_object;

#pragma mark coral_object

struct coral_class;
/**
 * @brief Retrieve the class for object.
 * @param [out] out receive class.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral_object_class(struct coral_class **out);

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
 * @param [in] class instance type.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_object_init(void *object, struct coral_class *class);

/**
 * @brief Destroy the object instance.
 * @param [in] object instance to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_object_destroy(void *object);

extern const char *destroy;

/**
 * @brief Retrieve the hash code of the object.
 * @param [in] object instance whose hash code we would like to retrieve.
 * @param [out] out receive the hash code.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_object_hash_code(void *object, size_t *out);

struct hash_code_args {
    size_t *out;
};
extern const char *hash_code;

/**
 * @brief Check if these two objects are equal.
 * @param [in] object first object.
 * @param [in] other second object.
 * @param [out] out true if <b>object</b> and <b>other</b> are equal,
 * otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other or out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_object_is_equal(void *object, void *other, bool *out);

struct is_equal_args {
    void *other;
    bool *out;
};
extern const char *is_equal;

/**
 * @brief Return a copy of the given object.
 * @param [in] object that we would like to copy.
 * @param [out] out receive the copy.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object and *out are the same object.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_object_copy(void *object, void **out);

struct copy_args {
    void *src;
};
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

/**
 * @brief Check if object is an instance of class.
 * @param [in] object instance whose class membership will be checked.
 * @param [in] class of which object will be checked for membership.
 * @param [out] out receive the result of the membership check.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if class or out is <i>NULL</i>.
 */
bool coral_object_instance_of(void *object, struct coral_class *class,
                              bool *out);

typedef bool (*coral_invokable_t)(void *this, void *data, void *args);
/**
 * @brief Invoke methods on objects.
 * <p>It is required, <u>when implementing calls to objects</u>, that it
 * would be done through this method so that services such as memory management
 * can be performed.</p>
 * @param [in] object instance.
 * @param [in] function perform the desired operations.
 * @param [in, out] args used by function to modify object and to provide
 * results.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is not a valid
 * instance.
 * @note If object or function is <i>NULL</i> we will call abort(3).
 */
bool coral_object_invoke(void *object, coral_invokable_t function, void *args);

/**
 * @brief Lookup method on object and then invoke it.
 * @param [in] object instance.
 * @param [in] method name of the function.
 * @param [in, out] args used by function to modify object and to provide
 * results.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_METHOD_NOT_FOUND if the given method could not be found.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is not a valid
 * instance.
 * @note If object or method is <i>NULL</i> we will call abort(3).
 */
bool coral_object_dispatch(void *object, const char *method, void *args);

#endif /* _CORAL_OBJECT_H_ */
