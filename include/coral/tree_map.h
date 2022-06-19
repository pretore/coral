#ifndef _CORAL_TREE_MAP_H_
#define _CORAL_TREE_MAP_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct coral_tree_map;

struct coral_tree_map_entry {
    void *key;
    void *value;
};

#pragma mark coral_object

struct coral_class;
/**
 * @brief Retrieve the class for tree map.
 * @param [out] out receive class.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral_tree_map_class(struct coral_class **out);

/**
 * @brief Allocate memory for tree map instance.
 * @param [out] out receive the allocated tree map instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate space for the tree map instance.
 */
bool coral_tree_map_alloc(struct coral_tree_map **out);

struct coral_range_values;

/**
 * @brief Initialize the tree map instance.
 * @param [in] object instance to be initialized.
 * @param [in] limit range values used to provide a lower and upper limit to
 * the number of entries contained within the tree map.
 * @param [in] compare comparison which must return an integer less than,
 * equal to, or greater than zero if the <u>first key</u> is considered
 * to be respectively less than, equal to, or greater than the <u>second
 * key</u>.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if compare is <i>NULL</i>.
 */
bool coral_tree_map_init(struct coral_tree_map *object,
                         struct coral_range_values *limit,
                         int (*compare)(const void *first,
                                        const void *second));

/**
 * @brief Destroy the tree map instance.
 * @param [in] object instance to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_tree_map_destroy(struct coral_tree_map *object);

extern const char *destroy;

/**
 * @brief Retrieve the hash code of the tree map instance.
 * @param [in] object instance whose hash code we would like to retrieve.
 * @param [out] out receive the hash code.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_map_hash_code(struct coral_tree_map *object, size_t *out);

extern const char *hash_code;

/**
 * @brief Check if these two tree maps are equal.
 * @param [in] object first tree map.
 * @param [in] other second tree map.
 * @param [out] out true if <b>object</b> and <b>other</b> are equal,
 * otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other or out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_map_is_equal(struct coral_tree_map *object,
                             struct coral_tree_map *other,
                             bool *out);

extern const char *is_equal;

/**
 * @brief Return a copy of the given tree map.
 * @param [in] object that we would like to copy.
 * @param [out] out receive the copy.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object and *out are the same object.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_map_copy(struct coral_tree_map *object,
                         struct coral_tree_map **out);

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
bool coral_tree_map_retain(struct coral_tree_map *object);

/**
 * @brief Release and possibly free object resources.
 * @param [in] object whose reference count we are to decrease and upon zero
 * where we will free the memory used by <b>object</b> instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_map_release(struct coral_tree_map *object);

/**
 * @brief Allow object to return from current function.
 * @param [in] object whose reference count will be retained and released in
 * such a manner so that the instance may return from the current function.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_map_autorelease(struct coral_tree_map *object);

#pragma mark coral_tree_map

/**
 * @brief Retrieve the count of entries in the tree map.
 * @param [in] object instance whose count is to be retrieved.
 * @param [out] out receive the count.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_map_get_count(struct coral_tree_map *object, size_t *out);

/**
 * @brief Insert entry into tree map.
 * @param [in] object instance where we would like to insert entry into.
 * @param [in] entry to insert into tree map.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if entry is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if entry's key is <i>NULL<i>.
 * @throws CORAL_ERROR_OBJECT_ALREADY_EXISTS if entry with key is already in
 * the map.
 * @throws CORAL_ERROR_OBJECT_UNAVAILABLE if inserting the instance will exceed
 * the upper limit on the total number of instances allowed.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_map_insert(struct coral_tree_map *object,
                           struct coral_tree_map_entry *entry);

/**
 * @brief Delete entry from tree map.
 * @param [in] object instance where we would like to delete the entry.
 * @param [in] key of the entry that we would like to delete.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if key is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if entry with key was not in the tree
 * map.
 * @throws CORAL_ERROR_OBJECT_UNAVAILABLE if deleting the instance will
 * reduce the total number of instances to below the lower limit.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_map_delete(struct coral_tree_map *object, const void *key);

/**
 * @brief Check if entry with key is contained in the tree map.
 * @param [in] object instance where we will check the presence of the entry
 * with the given key.
 * @param [in] key of the entry that we would like to check for presence in
 * the tree map.
 * @param [out] out receive true if entry with key is present, otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if key or out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_map_contains(struct coral_tree_map *object, const void *key,
                             bool *out);

/**
 * @brief Retrieve the value with the given key in the tree map.
 * @param [in] object instance which contains the entry we would like to
 * retrieve.
 * @param [in] key of the value we would like to retrieve.
 * @param [out] out receive the value for the given key.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if key or out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if key is not present in the tree
 * map.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_map_get(struct coral_tree_map *object, const void *key,
                        void **out);

/**
 * @brief Update an existing entry's value in the tree map.
 * @param [in] object instance whose matched entry should have its value set.
 * @param [in] entry that matching entry should be set to.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if entry is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if entry's key is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if entry with key could not be found not
 * in the tree map.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_tree_map_set(struct coral_tree_map *object,
                        struct coral_tree_map_entry *entry);


#endif /* _CORAL_TREE_MAP_H_ */
