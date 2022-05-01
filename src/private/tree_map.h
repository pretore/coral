#ifndef _CORAL_PRIVATE_TREE_MAP_H_
#define _CORAL_PRIVATE_TREE_MAP_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdatomic.h>

#include "tree_set.h"

/* Map : https://en.wikipedia.org/wiki/Associative_array */

struct coral$tree_map {
    size_t key_size;
    size_t value_size;
    struct coral$tree_set tree_set;
};

/**
 * @brief Allocate memory for tree map instance.
 * @param [out] out receive the allocated tree map instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral$tree_map$alloc(struct coral$tree_map **out);

/**
 * @brief Initialise the tree map instance.
 * @param [in] object instance to be initialised.
 * @param [in] key_size length in bytes of the key.
 * @param [in] value_size length in bytes of the value.
 * @param [in] compare comparison which must return an integer less than,
 * equal to, or greater than zero if the <u>first entry</u> is considered
 * to be respectively less than, equal to, or greater than the <u>second
 * entry</u>.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if compare is <i>NULL</i>.
 */
bool coral$tree_map$init(struct coral$tree_map *object,
                         size_t key_size,
                         size_t value_size,
                         int (*compare)(const void *first,
                                        const void *second));

/**
 * @brief Invalidate the tree map instance.
 * <p>The entries in the tree map are destroyed and each will invoke the
 * provided <i>on destroy</i> callback. The actual <u>tree map instance
 * is not deallocated</u> since it may have been embedded in a larger structure.
 * @param [in] object instance to be invalidated.
 * @param [in] on_destroy called just before the map entry is to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral$tree_map$invalidate(struct coral$tree_map *object,
                               void (*on_destroy)(void *));

/**
 * @brief Retrieve the count of entries in the tree map.
 * @param [in] object instance whose count is to be retrieved.
 * @param [out] out receive the count.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral$tree_map$get_count(struct coral$tree_map *object, size_t *out);

/**
 * @brief Retrieve the size of the entries in the tree map.
 * @param [in] object instance whose entry size it to be retrieved.
 * @param [out] out receive the entry size.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral$tree_map$get_size(struct coral$tree_map *object, size_t *out);

/**
 * @brief Retrieve the key size of the entries in the tree map.
 * @param [in] object instance whose key size it to be retrieved.
 * @param [out] out receive the key size.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral$tree_map$get_key_size(struct coral$tree_map *object, size_t *out);

/**
 * @brief Retrieve the value size of the entries in the tree map.
 * @param [in] object instance whose value size it to be retrieved.
 * @param [out] out receive the key size.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral$tree_map$get_value_size(struct coral$tree_map *object, size_t *out);

/**
 * @brief Insert entry into tree map.
 * @param [in] object instance where we would like to insert entry into.
 * @param [in] entry to insert into tree map.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_ALREADY_EXISTS if entry is already in the map.
 */
bool coral$tree_map$insert(struct coral$tree_map *object, const void *entry);

/**
 * @brief Delete entry from tree map.
 * @param [in] object instance where we would like to delete the entry.
 * @param [in] key of the entry that we would like to delete.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if key is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if entry with key was not in the tree
 * map.
 */
bool coral$tree_map$delete(struct coral$tree_map *object, const void *key);

/**
 * @brief Check if entry with key is contained in the tree map.
 * @param [in] object instance where we will check the presence of the entry
 * with the given key.
 * @param [in] key of the entry that we would like to check for presence in
 * the tree map.
 * @param [out] out receive true if entry is present, otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if key or out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if entry was not in the tree map.
 */
bool coral$tree_map$contains(struct coral$tree_map *object, const void *key,
                             bool *out);

/**
 * @brief Retrieve the entry with the given key in the tree map.
 * @param [in] object instance which contains the entry we would like to
 * retrieve.
 * @param [in] key of the entry we would like to retrieve.
 * @param [out] out receive the entry.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if key or out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if entry was not in the tree map.
 */
bool coral$tree_map$get(struct coral$tree_map *object, const void *key,
                        void **out);

/**
 * @brief Update an existing entry's value in the tree map.
 * @param [in] object instance whose matched entry should have its value set.
 * @param [in] entry that matching entry should be set to.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if entry is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if entry was not in the tree map.
 */
bool coral$tree_map$set(struct coral$tree_map *object, const void *entry);

/**
 * @brief Retrieve the first entry in the tree map.
 * @param [in] object instance of tree map whose first entry we would like to
 * retrieve.
 * @param [out] out receive the first entry.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if entry is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if tree map is empty.
 */
bool coral$tree_map$first(struct coral$tree_map *object, void **out);

/**
 * @brief Retrieve the last entry in the tree map.
 * @param [in] object instance of tree map whose last entry we would like to
 * retrieve.
 * @param [out] out receive the last entry.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if entry is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if tree map is empty.
 */
bool coral$tree_map$last(struct coral$tree_map *object, void **out);

/**
 * @brief Retrieve the next entry in the tree map.
 * @param [in] object instance from which we are to retrieve the next entry.
 * @param [in] entry whose successor entry we want.
 * @param [out] out receive the entry.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if entry or out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if the tree map is empty.
 * @throws CORAL_ERROR_END_OF_SEQUENCE if there are no more entries.
 */
bool coral$tree_map$next(struct coral$tree_map *object, void *entry,
                         void **out);

/**
 * @brief Retrieve the previous entry in the tree map.
 * @param [in] object instance from which we are to retrieve the next entry.
 * @param [in] entry whose predecessor entry we want.
 * @param [out] out receive the entry.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if entry or out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if the tree map is empty.
 * @throws CORAL_ERROR_END_OF_SEQUENCE if there are no more entries.
 */
bool coral$tree_map$prev(struct coral$tree_map *object, void *entry,
                         void **out);

struct coral$tree_map$iterator {
    struct coral$tree_set$iterator iterator;
};

/**
 * @brief Allocate memory for tree map iterator instance.
 * @param [out] out receive the allocated tree map iterator instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate space for a tree map iterator instance.
 */
bool coral$tree_map$iterator_alloc(struct coral$tree_map$iterator **out);

/**
 * @brief Initialize tree map iterator.
 * @param [in] object instance to be initialised.
 * @param [in] tree_map instance of which we would like to iterate.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if tree_map is <i>NULL</i>.
 */
bool coral$tree_map$iterator_init(struct coral$tree_map$iterator *object,
                                  struct coral$tree_map *tree_map);

/**
 * @brief Retrieve the next entry.
 * @param [in] object iterator instance.
 * @param [out] out receive the next entry.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_UNAVAILABLE if the map has been modified since
 * last use of the iterator.
 * @throws CORAL_ERROR_END_OF_SEQUENCE if there are no more entries to iterate
 * over.
 */
bool coral$tree_map$iterator_get_next(struct coral$tree_map$iterator *object,
                                      void **out);

/**
 * @brief Delete the current entry.
 * @param [in] object iterator instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if the iterator is not referring to any
 * entries.
 * @throws CORAL_ERROR_OBJECT_UNAVAILABLE if the map has been modified since
 * last use of the iterator.
 */
bool coral$tree_map$iterator_delete(struct coral$tree_map$iterator *object);

/**
 * @brief Invalidate the tree map instance.
 * @param [in] object iterator instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral$tree_map$iterator_invalidate(struct coral$tree_map$iterator *object);

#endif /* _CORAL_PRIVATE_TREE_MAP_H_ */
