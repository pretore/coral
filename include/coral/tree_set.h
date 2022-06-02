#ifndef _CORAL_TREE_SET_H_
#define _CORAL_TREE_SET_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct coral_tree_set;

#pragma mark coral_object

struct coral_class;
/**
 * @brief Retrieve the class for tree set.
 * @param [out] out receive class.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral_tree_set_class(struct coral_class **out);

/**
 * @brief Allocate memory for tree set instance.
 * @param [out] out receive the allocated tree set instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate space for the tree set instance.
 */
bool coral_tree_set_alloc(struct coral_tree_set **out);

struct coral_range_values;

/**
 * @brief Initialize the tree set instance.
 * @param [in] object instance to be initialized.
 * @param [in] limit range values used to provide a lower and upper limit to
 * the number of items contained within the tree set.
 * @param [in] compare comparison which must return an integer less than,
 * equal to, or greater than zero if <u>first</u> is considered to be
 * respectively less than, equal to, or greater than the <u>second</u>.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if compare is <i>NULL</i>.
 */
bool coral_tree_set_init(struct coral_tree_set *object,
                         struct coral_range_values *limit,
                         int (*compare)(const void *first,
                                        const void *second));

/**
 * @brief Destroy the tree set instance.
 * @param [in] object instance to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_tree_set_destroy(struct coral_tree_set *object);

extern const char *destroy;

/**
 * @brief Retrieve the hash code of the tree set instance.
 * @param [in] object instance whose hash code we would like to retrieve.
 * @param [out] out receive the hash code.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_set_hash_code(struct coral_tree_set *object, size_t *out);

extern const char *hash_code;

/**
 * @brief Check if these two tree sets are equal.
 * @param [in] object first tree set.
 * @param [in] other second tree set.
 * @param [out] out true if <b>object</b> and <b>other</b> are equal,
 * otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other or out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_set_is_equal(struct coral_tree_set *object,
                             struct coral_tree_set *other,
                             bool *out);

extern const char *is_equal;

/**
 * @brief Return a copy of the given tree set.
 * @param [in] object that we would like to copy.
 * @param [out] out receive the copy.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object and *out are the same object.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_set_copy(struct coral_tree_set *object,
                         struct coral_tree_set **out);

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
bool coral_tree_set_retain(struct coral_tree_set *object);

/**
 * @brief Release and possibly free object resources.
 * @param [in] object whose reference count we are to decrease and upon zero
 * where we will free the memory used by <b>object</b> instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_set_release(struct coral_tree_set *object);

/**
 * @brief Allow object to return from current function.
 * @param [in] object whose reference count will be retained and released in
 * such a manner so that the instance may return from the current function.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_set_autorelease(struct coral_tree_set *object);

#pragma mark coral_tree_set

/**
 * @brief Retrieve the number of the instances in the tree set.
 * @param [in] object instance whose count is to be retrieved.
 * @param [out] out receive the count.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_set_get_count(struct coral_tree_set *object, size_t *out);

/**
 * @brief Insert instance into tree set.
 * @param [in] object where we would like to insert instance into.
 * @param [in] instance to insert into tree set.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if instance is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_ALREADY_EXISTS if instance is already in the
 * tree set.
 * @throws CORAL_ERROR_OBJECT_UNAVAILABLE if inserting the instance will exceed
 * the upper limit on the total number of instances allowed.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_set_insert(struct coral_tree_set *object, void *instance);

/**
 * @brief Delete instance from tree set.
 * @param [in] object where we would like to delete the instance.
 * @param [in] instance to delete from the tree set.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if instance is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if instance was not in the tree set.
 * @throws CORAL_ERROR_OBJECT_UNAVAILABLE if deleting the instance will
 * reduce the total number of instances to below the lower limit.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_set_delete(struct coral_tree_set *object, void *instance);

/**
 * @brief Check if instance is contained in the tree set.
 * @param [in] object where we will check the presence of instance.
 * @param [in] instance that needs to be checked for membership with tree set.
 * @param [out] out receive true if instance is present, otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if instance or out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_set_contains(struct coral_tree_set *object, void *instance,
                             bool *out);

/**
 * @brief Retrieve the first instance in the tree set.
 * @param [in] object from which we are to fetch the first instance.
 * @param [out] out receive the first instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if the tree set it empty.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_set_get_first(struct coral_tree_set *object, void **out);

/**
 * @brief Retrieve the last instance in the tree set.
 * @param [in] object  from which we are to fetch the last instance.
 * @param [out] out receive the last instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if the tree set it empty.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_set_get_last(struct coral_tree_set *object, void **out);

/**
 * @brief Retrieve the next instance.
 * @param [in] object from which we are to retrieve the next instance.
 * @param [in] instance whose successor instance we want.
 * @param [out] out receive the next instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if instance or out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if the tree set is empty.
 * @throws CORAL_ERROR_END_OF_SEQUENCE if there are is no successor.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_set_get_next(struct coral_tree_set *object, void *instance,
                             void **out);

/**
 * @brief Retrieve the previous instance.
 * @param [in] object from which we are to retrieve the previous instance.
 * @param [in] instance whose predecessor we want.
 * @param [out] out receive the previous instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if instance or out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if the tree set is empty.
 * @throws CORAL_ERROR_END_OF_SEQUENCE if there is no predecessor.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_set_get_prev(struct coral_tree_set *object, void *instance,
                             void **out);

#endif /* _CORAL_TREE_SET_H_ */
