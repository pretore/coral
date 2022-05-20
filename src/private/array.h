#ifndef _CORAL_PRIVATE_ARRAY_H_
#define _CORAL_PRIVATE_ARRAY_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdatomic.h>

#include "range.h"

/* Array : https://en.wikipedia.org/wiki/Dynamic_array */

struct coral_reference;
struct coral$array {
    atomic_size_t id;
    struct coral_reference *limit_ref;
    size_t capacity;
    size_t count;
    size_t size;
    unsigned char *data;
};

/**
 * @brief Initialize the array.
 * @param [in] object instance.
 * @param [in] limit range used to specify the <u>capacity limits</u>
 * and also provide a <u>capacity function</u> which is used to calculate the
 * capacity when a given <b>count</b> is provided. In the event that this is
 * <i>NULL</i>, a default of no limits set and a <u>capacity function</u>
 * which will increase the capacity at a rate of <u>1.5</u> times.
 * @param [in] count initial number of <i>ZERO</i>ed out items.
 * @param [in] size initial length in bytes of every item.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate space for the array instance.
 * @throws CORAL_ERROR_INVALID_VALUE if the count provided is larger than the
 * given maximum capacity limit.
 */
bool coral$array$init(struct coral$array *object,
                      struct coral_range *limit,
                      size_t count,
                      size_t size);

/**
 * @brief Invalidate the array.
 * <p>The items in the array are destroyed and each will invoke the
 * provided <i>on destroy</i> callback. The actual <u>array is not
 * deallocated</u> since it may have been embedded in a larger structure.
 * @param [in] object instance.
 * @param [in] on_destroy called just before the array item is to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral$array$invalidate(struct coral$array *object,
                            void (*on_destroy)(void *, const size_t));

/**
 * @brief Retrieve the capacity of the array instance.
 * @param [in] object array instance.
 * @param [out] out receive capacity.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral$array$get_capacity(struct coral$array *object, size_t *out);

/**
 * @brief Set the capacity of the array instance.
 * @param [in] object array instance.
 * @param [in] capacity value to set the capacity to.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if capacity is not within limits.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the backing data structure.
 */
bool coral$array$set_capacity(struct coral$array *object, size_t capacity);

/**
 * @brief Retrieve the count of items held in the array instance.
 * @param [in] object array instance.
 * @param [out] out receive count of items.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral$array$get_count(struct coral$array *object, size_t *out);

/**
 * @brief Set the number of item.
 * @param [in] object array instance.
 * @param [in] count number of items to be held in the array instance. Any
 * added items will be <i>ZERO</i>ed out.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if count is not within limits.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the backing data structure.
 */
bool coral$array$set_count(struct coral$array *object, size_t count);

/**
 * @brief Retrieve the size of the items in the array.
 * @param [in] object array instance.
 * @param [out] out receive the size of the items.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral$array$get_size(struct coral$array *object, size_t *out);

struct coral$array$item {
    void *data;
    size_t size;
};

/**
 * @brief Retrieve the item at the given index in the array.
 * @param [in] object array instance.
 * @param [in] at zero based index of item.
 * @param [out] out receive item.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_INDEX_OUT_OF_BOUNDS if at is referring to an item not
 * contained within the array.
 */
bool coral$array$get(struct coral$array *object,
                     size_t at,
                     struct coral$array$item *out);

/**
 * @brief Set the item at the given index in the array.
 * @param [in] object array instance.
 * @param [in] at zero based index of item.
 * @param [in] item if provided, will be used to set the item at the given
 * index, otherwise the item at the given index will be a <i>ZERO</i>ed out.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if item.size is <i>0</i> or item.data is
 * <i>NULL</i>.
 * @throws CORAL_ERROR_INDEX_OUT_OF_BOUNDS if at is referring to an item not
 * contained within the array.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the backing data structure.
 */
bool coral$array$set(struct coral$array *object,
                     size_t at,
                     const struct coral$array$item *item);

/**
 * @brief Add item at the end of the array.
 * @param [in] object array instance.
 * @param [in] item if provided, will be added at the end of the array,
 * otherwise a <i>ZERO</i>ed out item will be added.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if item.size is <i>0</i> or item.data is
 * <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the backing data structure.
 */
bool coral$array$add(struct coral$array *object,
                     const struct coral$array$item *item);

/**
 * @brief Remove last item from the array.
 * @param [in] object array instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if array is empty.
 */
bool coral$array$remove(struct coral$array *object);

/**
 * @brief Insert new item at the given index.
 * @param [in] object array instance.
 * @param [in] at zero based index of item.
 * @param [in] item if provided, will be inserted at the given index, otherwise
 * a <i>ZERO</i>ed out item will be inserted.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if item.size is <i>0</i> or item.data is
 * <i>NULL</i>.
 * @throws CORAL_ERROR_INDEX_OUT_OF_BOUNDS if at is referring to an item not
 * contained within the array.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the backing data structure.
 */
bool coral$array$insert(struct coral$array *object,
                        size_t at,
                        const struct coral$array$item *item);

/**
 * @brief Delete item at the given index.
 * @param [in] object array instance.
 * @param [in] at zero based index of item.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INDEX_OUT_OF_BOUNDS if at is referring to an item not
 * contained within the array.
 */
bool coral$array$delete(struct coral$array *object,
                        size_t at);

/**
 * @brief Sort the contents of the array using given compare function.
 * @param [in] object array instance.
 * @param [in] values if provided, will limit the sort to only affect between
 * the given indexes inclusively.
 * @param [in] compare comparison which must return an integer less than,
 * equal to, or greater than zero if the <u>first item</u> is considered
 * to be respectively less than, equal to, or greater than the <u>second
 * item</u>.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if compare is <i>NULL</i>.
 */
bool coral$array$sort(struct coral$array *object,
                      struct coral_range_values *values,
                      int (*compare)(const void *,
                                     const void *,
                                     const size_t));

struct coral$array$search_pattern {
    struct coral$range range;
};

/**
 * @brief Initialize the linear search pattern instance.
 * @param [in] object linear search pattern instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the context for the linear search pattern instance.
 * @see <a href="https://en.wikipedia.org/wiki/Linear_search" />
 * Linear search algorithm
 */
bool coral$array$search_pattern$init_linear(
        struct coral$array$search_pattern *object);

/**
 * @brief Initialize the binary search pattern instance.
 * <p>The binary search pattern instance will find the <u>any</u> duplicate
 * target member in the <u>sorted</u> array.</p>
 * @param [in] object binary search pattern instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the context for the binary search pattern instance.
 * @see <a href="https://en.wikipedia.org/wiki/Binary_search_algorithm" />
 * Binary search algorithm
 */
bool coral$array$search_pattern$init_binary(
        struct coral$array$search_pattern *object);

/**
 * @brief Invalidate the search pattern instance.
 * <p>The actual <u>search pattern instance is not deallocated</u> since it may
 * have been embedded in a larger structure.
 * @param [in] object search pattern instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral$array$search_pattern$invalidate(
        struct coral$array$search_pattern *object);

/**
 * @brief Find an item in the array.
 * @param [in] object array instance.
 * @param [in] values if provided, will limit the search to only between the
 * given indexes inclusively.
 * @param [in] search_pattern find the item using this search pattern.
 * @param [in] needle item that we are looking for.
 * @param [in] compare
 * @param [out] out
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral$array$find(struct coral$array *object,
                      const struct coral_range_values *values,
                      struct coral$array$search_pattern *search_pattern,
                      const struct coral$array$item *needle,
                      int (*compare)(const void *,
                                     const void *,
                                     const size_t),
                      size_t *out);

#endif /* _CORAL_PRIVATE_ARRAY_H_ */