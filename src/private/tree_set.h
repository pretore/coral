#ifndef _CORAL_PRIVATE_TREE_SET_H_
#define _CORAL_PRIVATE_TREE_SET_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdatomic.h>

#include "red_black_tree.h"
#include "range.h"

#define CORAL_CLASS_LOAD_PRIORITY_TREE_SET \
    (1 + CORAL_CLASS_LOAD_PRIORITY_RANGE)

/* Set: https://en.wikipedia.org/wiki/Set_(abstract_data_type) */

struct coral$tree_set {
    atomic_size_t id;
    struct coral_range_values limit;
    size_t count;
    size_t size;
    struct coral$red_black_tree tree;
};

/**
 * @brief Allocate memory for tree set instance.
 * @param [out] out receive the allocated tree set instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate space for the tree set instance.
 */
bool coral$tree_set$alloc(struct coral$tree_set **out);

/**
 * @brief Initialize the tree set instance.
 * @param [in] object instance to be initialized.
 * @param [in] limit range values used to provide a lower and upper limit to
 * the number of items contained within the tree set.
 * @param [in] size in bytes of the members of the set.
 * @param [in] compare comparison which must return an integer less than,
 * equal to, or greater than zero if <u>first</u> is considered to be
 * respectively less than, equal to, or greater than the <u>second</u>.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if compare is <i>NULL</i>.
 */
bool coral$tree_set$init(struct coral$tree_set *object,
                         struct coral_range_values *limit,
                         size_t size,
                         int (*compare)(const void *, const void *));

/**
 * @brief Invalidate the tree set instance.
 * <p>The items in the tree set are destroyed and each will invoke the
 * provided <i>on destroy</i> callback. The actual <u>tree set instance
 * is not deallocated</u> since it may have been embedded in a larger structure.
 * @param [in] object instance to be invalidated.
 * @param [in] on_destroy called just before the set member is to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral$tree_set$invalidate(struct coral$tree_set *object,
                               void (*on_destroy)(void *));

/**
 * @brief Retrieve the count of items in the tree set.
 * @param [in] object instance whose count is to be retrieved.
 * @param [out] out receive the count.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral$tree_set$get_count(struct coral$tree_set *object, size_t *out);

/**
 * @brief Retrieve the size of a member of the set.
 * @param [in] object instance whose member size we want to retrieve.
 * @param [out] out receive the member size.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral$tree_set$get_size(struct coral$tree_set *object, size_t *out);

/**
 * @brief Insert item into tree set.
 * @param [in] object instance where we would like to insert item into.
 * @param [in] item to insert into tree set.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if item is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_ALREADY_EXISTS if item is already in the set.
 * @throws CORAL_ERROR_OBJECT_UNAVAILABLE if inserting the instance will exceed
 * the upper limit on the total number of instances allowed.
 */
bool coral$tree_set$insert(struct coral$tree_set *object, const void *item);

/**
 * @brief Delete item from tree set.
 * @param [in] object instance where we would like to delete the item.
 * @param [in] item to delete from the tree set.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if item is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if item was not in the tree set.
 * @throws CORAL_ERROR_OBJECT_UNAVAILABLE if deleting the instance will
 * reduce the total number of instances to below the lower limit.
 */
bool coral$tree_set$delete(struct coral$tree_set *object, const void *item);

/**
 * @brief Check if item is contained in the tree set.
 * @param [in] object instance where we will check the presence of item.
 * @param [in] item that needs to be checked for membership with tree set.
 * @param [out] out receive true if item is present, otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if item or out is <i>NULL</i>.
 */
bool coral$tree_set$contains(struct coral$tree_set *object, const void *item,
                             bool *out);

/**
 * @brief Retrieve the first value in the tree set.
 * @param [in] object instance from which we are to fetch the first value.
 * @param [out] out receive the first value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if the tree set it empty.
 */
bool coral$tree_set$get_first(struct coral$tree_set *object, void **out);

/**
 * @brief Retrieve the last value in the tree set.
 * @param [in] object instance from which we are to fetch the last value.
 * @param [out] out receive the last value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if the tree set it empty.
 */
bool coral$tree_set$get_last(struct coral$tree_set *object, void **out);

/**
 * @brief Retrieve the next value.
 * @param [in] object instance from which we are to retrieve the next value.
 * @param [in] value whose successor value we want.
 * @param [out] out receive the next value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if item or out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if the tree set is empty.
 * @throws CORAL_ERROR_END_OF_SEQUENCE if there are no more values.
 */
bool coral$tree_set$get_next(struct coral$tree_set *object, const void *value,
                             void **out);

/**
 * @brief Retrieve the previous value.
 * @param [in] object instance from which we are to retrieve the previous value.
 * @param [in] value whose predecessor value we want.
 * @param [out] out receive the previous value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if item or out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if the tree set is empty.
 * @throws CORAL_ERROR_END_OF_SEQUENCE if there are no more values.
 */
bool coral$tree_set$get_prev(struct coral$tree_set *object, const void *value,
                             void **out);

#endif /* _CORAL_PRIVATE_TREE_SET_H_ */