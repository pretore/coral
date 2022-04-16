#ifndef _CORAL_ARRAY_H_
#define _CORAL_ARRAY_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct coral_array;
struct coral_array_item {
    size_t size;
    void *data;
};
struct coral_range;

/**
 * @brief Create an array of objects.
 * @param [out] out receive the created array of objects.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the array instance.
 */
bool coral_array_of_objects(struct coral_array **out);

/**
 * @brief Allocate memory for array instance.
 * @param [out] out receive the allocated array instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the array instance.
 */
bool coral_array_alloc(struct coral_array **out);

/**
 * @brief Initialize the array instance.
 * @param [in] object instance to be initialized.
 * @param [in] count number of elements in the array.
 * @param [in] size number of bytes that each element consists of.
 * @param [in] capacity_range limit the capacity of the array to a specified
 * minimum and/or maximum while also specifying how to increase the capacity.
 * If unspecified an instance of coral_range_of_rate with a minimum capacity of
 * <i>16</i> and the capacity will be increased in by a factor of <i>1.5</i>.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the data buffer.
 * @throws CORAL_ERROR_INVALID_VALUE if count is less than the minimum value
 * or count is greater than the maximum value.
 * @throws CORAL_ERROR_CAPACITY_LIMIT_REACHED if we are unable to determine
 * the capacity needed.
 */
bool coral_array_init(struct coral_array *object,
                      size_t count,
                      size_t size,
                      struct coral_range *capacity_range);

/**
 * @brief Destroy the array instance.
 * @param [in] object instance to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_array_destroy(struct coral_array *object);

/**
 * @brief Retain the referenced counted array.
 * @param [in] object whose reference count is to be increase to prevent early
 * memory collection.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_retain(struct coral_array *object);

/**
 * @brief Release and possibly free array resources.
 * @param [in] object whose reference count we are to decrease and upon zero
 * where we will free the resources used by <b>array</b> instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_release(struct coral_array *object);

/**
 * @brief Retain the referenced counted object.
 * @param [in] object whose reference count is to be increase to prevent early
 * collection.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_autorelease(struct coral_array *object);

/**
 * @brief Retrieve the capacity limits of the array.
 * @param [in] object array instance whose capacity limits we want to retrieve.
 * @param [out] out receive the capacity limits of the array instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_get_capacity(struct coral_array *object,
                              struct coral_range_values *out);

/**
 * @brief Retrieve the count of the array.
 * @param [in] object array instance whose count we want to retrieve.
 * @param [out] out receive the count of the array instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_get_count(struct coral_array *object, size_t *out);

/**
 * @brief Set the count of the array.
 * @param [in] object array instance whose count we want to set.
 * @param [in] count capacity of the array instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_set_count(struct coral_array *object, size_t count);

/**
 * @brief Retrieve the size of an item in the array.
 * @param [in] object array instance whose item size we want to retrieve.
 * @param [out] out receive the size of the items in the array.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_get_size(struct coral_array *object, size_t *out);

/**
 * @brief Set the contents of the item at the given index.
 * @param [in] object array instance where the targeted item resides in.
 * @param [in] at index of the item in the array instance.
 * @param [in] item holder of the contents for setting the item at the given
 * index.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if given item does not have size nor
 * data defined.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 * @throws CORAL_ERROR_INDEX_OUT_OF_BOUNDS if the given index is not
 * contained within the array.
 */
bool coral_array_set(struct coral_array *object, size_t at,
                     const struct coral_array_item *item);

/**
 * @brief Retrieve the contents of the item at the given index.
 * @param [in] object array instance where the targeted item resides in.
 * @param [in] at index of the item in the array instance.
 * @param [out] item holder of the contents of the item at the given index.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 * @throws CORAL_ERROR_INDEX_OUT_OF_BOUNDS if the given index is not
 * contained within the array.
 */
bool coral_array_get(struct coral_array *object, size_t at,
                     struct coral_array_item *item);

/**
 * @brief Sort the array using the provided compare function.
 * @param [in] object array instance whose contents will be sorted.
 * @param [in] compare comparison which must return an integer less than,
 * equal to, or greater than zero if the <u>min argument</u> is considered
 * to be respectively less than, equal to, or greater than the <u>second</u>.
 * If two members compare as equal, their order in the sorted array is
 * undefined.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if compare is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_sort(struct coral_array *object,
                      int (*compare)(const struct coral_array_item *,
                                     const struct coral_array_item *));

/**
 * @brief Search the array for needle using the given equality function.
 * @param [in] object array instance whose contents will be searched.
 * @param [in] range limit search to the indexes given.
 * @param [in] needle item to look for in the array.
 * @param [in] is_equals function to provide equality check between two items.
 * @param [out] out receive the index of the matching item.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if needle, is_equals or out is
 * <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if no object matching needle was not
 * found.
 */
bool coral_array_find(struct coral_array *object,
                      const struct coral_range *range,
                      const struct coral_array_item *needle,
                      bool (*is_equals)(const struct coral_array_item *,
                                        const struct coral_array_item *),
                      size_t *out);

/**
 * @brief Insert contents of the item at the given index.
 * @param [in] object array instance which will contain item.
 * @param [in] at index of where the item will be inserted.
 * @param [in] item holder of the contents for inserting the item at the given
 * index.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if given item does not have size nor
 * data defined.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 * @throws CORAL_ERROR_CAPACITY_LIMIT_REACHED if could not insert item
 * because we have have reached the maximum capacity limit.
 */
bool coral_array_insert(struct coral_array *object, size_t at,
                        const struct coral_array_item *item);

/**
 * @brief Delete the item at the given index.
 * @param [in] object array instance of which to delete items from.
 * @param [in] at index of item that we would like to erase.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if given item does not have size nor
 * data defined.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 * @throws CORAL_ERROR_INDEX_OUT_OF_BOUNDS if the given index is not
 * contained within the array.
 */
bool coral_array_delete(struct coral_array *object, size_t at);

/**
 * @brief Add contents of the item at the end of the array.
 * @param [in] object array instance which will contain item.
 * @param [in] item holder of the contents for adding the item at the end of
 * the array.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if given item does not have size nor
 * data defined.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 * @throws CORAL_ERROR_CAPACITY_LIMIT_REACHED if could not insert item
 * because we have have reached the maximum capacity limit.
 */
bool coral_array_add(struct coral_array *object,
                     const struct coral_array_item *item);

/**
 * @brief Remove the last item in the array.
 * @param [in] object array instance from which the last item will be removed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if given item does not have size nor
 * data defined.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if the array is empty.
 */
bool coral_array_remove(struct coral_array *object);

#endif /* _CORAL_ARRAY_H_ */