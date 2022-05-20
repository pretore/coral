#ifndef _CORAL_ARRAY_H_
#define _CORAL_ARRAY_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct coral_array;

#pragma mark coral_object

/**
 * @brief Allocate memory for array instance.
 * @param [out] out receive the allocated array instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate space for the array instance.
 */
bool coral_array_alloc(struct coral_array **out);

/**
 * @brief Initialize the array instance.
 * @param [in] object array instance.
 * @param [in] limit range instance used to specify the <u>capacity limits</u>
 * and also provide a <u>capacity function</u> which is used to calculate the
 * capacity when a given <b>count</b> is provided. In the event that this is
 * <i>NULL</i>, a default of no limits set and a <u>capacity function</u>
 * which will increase the capacity at a rate of <u>1.5</u> times.
 * @param [in] count initial number of <i>NULL</i> members.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the backing data structure.
 * @throws CORAL_ERROR_INVALID_VALUE if the count provided is larger than the
 * given maximum capacity limit.
 */
bool coral_array_init(struct coral_array *object,
                      struct coral_range *limit,
                      size_t count);

/**
 * @brief Destroy the array instance.
 * @param [in] object instance to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_array_destroy(struct coral_array *object);

extern const char *destroy;

/**
 * @brief Retrieve the hash code of the array instance.
 * @param [in] object instance whose hash code we would like to retrieve.
 * @param [out] out receive the hash code.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_hash_code(struct coral_array *object, size_t *out);

extern const char *hash_code;

/**
 * @brief Check if these two arrays are equal.
 * @param [in] object first array.
 * @param [in] other second array.
 * @param [out] out true if <b>object</b> and <b>other</b> are equal,
 * otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other or out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_is_equal(struct coral_array *object,
                          struct coral_array *other,
                          bool *out);

extern const char *is_equal;

/**
 * @brief Return a copy of the given array.
 * @param [in] object that we would like to copy.
 * @param [out] out receive the copy.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object and *out are the same object.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_copy(struct coral_array *object, struct coral_array **out);

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
bool coral_array_retain(struct coral_array *object);

/**
 * @brief Release and possibly free object resources.
 * @param [in] object whose reference count we are to decrease and upon zero
 * where we will free the memory used by <b>object</b> instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_release(struct coral_array *object);

/**
 * @brief Allow object to return from current function.
 * @param [in] object whose reference count will be retained and released in
 * such a manner so that the instance may return from the current function.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_autorelease(struct coral_array *object);

#pragma mark coral_array

/**
 * @brief Retrieve the capacity of the array instance.
 * @param [in] object array instance.
 * @param [out] out receive capacity.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_get_capacity(struct coral_array *object, size_t *out);

/**
 * @brief Set the capacity of the array instance.
 * @param [in] object array instance.
 * @param [in] capacity value to set the capacity to.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if capacity is not within limits.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the backing data structure.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_set_capacity(struct coral_array *object, size_t capacity);

/**
 * @brief Retrieve the count of items held in the array instance.
 * @param [in] object array instance.
 * @param [out] out receive count of items.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_get_count(struct coral_array *object, size_t *out);

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
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_set_count(struct coral_array *object, size_t count);

/**
 * @brief Retrieve the instance at the given index in the array.
 * @param [in] object array instance.
 * @param [in] at zero based index of instance in array.
 * @param [out] out receive instance from array.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_INDEX_OUT_OF_BOUNDS if at is referring to an instance
 * not contained within the array.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_get(struct coral_array *object, size_t at, void **out);

/**
 * @brief Set the instance at the given index in the array.
 * @param [in] object array instance.
 * @param [in] at zero based index of instance in array.
 * @param [in] instance to be contained by the array.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INDEX_OUT_OF_BOUNDS if at is referring to an instance
 * not contained within the array.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_set(struct coral_array *object, size_t at, void *instance);

/**
 * @brief Add instance at the end of the array.
 * @param [in] object array instance.
 * @param [in] instance to be added at the end of the array.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the backing data structure.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_add(struct coral_array *object, void *instance);

/**
 * @brief Remove last instance from the array.
 * @param [in] object array instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if array is empty.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_remove(struct coral_array *object);

/**
 * @brief Insert instance at the given index in the array.
 * @param [in] object array instance.
 * @param [in] at zero based index of instance.
 * @param [in] instance to be inserted into the array.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INDEX_OUT_OF_BOUNDS if at is referring to an instance
 * not contained within the array.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the backing data structure.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_insert(struct coral_array *object, size_t at, void *instance);

/**
 * @brief Delete instance at the given index in the array.
 * @param [in] object array instance.
 * @param [in] at zero based index of instance within the array.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INDEX_OUT_OF_BOUNDS if at is referring to an instance
 * not contained within the array.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_delete(struct coral_array *object, size_t at);

/**
 * @brief Sort the contents of the array.
 * @param [in] object array instance.
 * @param [in] values if provided, will limit the sort to only affect between
 * the given indexes inclusively.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_array_sort(struct coral_array *object,
                      struct coral_range_values *values);

struct coral_array_search_pattern;

/**
 * @brief Create a linear search pattern instance.
 * @param [out] out receive linear search pattern instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the context for the linear search pattern instance.
 * @see <a href="https://en.wikipedia.org/wiki/Linear_search" />
 * Linear search algorithm
 */
bool coral_array_search_pattern_of_linear(
        struct coral_array_search_pattern **out);

/**
 * @brief Create a binary search pattern instance.
 * <p>The binary search pattern instance will find the <u>any</u> duplicate
 * target member in the <u>sorted</u> array.</p>
 * @param [out] out receive binary search pattern instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the context for the binary search pattern instance.
 * @see <a href="https://en.wikipedia.org/wiki/Binary_search_algorithm" />
 * Binary search algorithm
 */
bool coral_array_search_pattern_of_binary(
        struct coral_array_search_pattern **out);

/**
 * @brief Destroy the search pattern instance.
 * @param [in] object search pattern instance to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_array_search_pattern_destroy(
        struct coral_array_search_pattern *object);

#endif /* _CORAL_ARRAY_H_ */
