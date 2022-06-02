#ifndef _CORAL_RANGE_H_
#define _CORAL_RANGE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

struct coral_range;
struct coral_range_values {
    size_t first;
    size_t last;
};

/**
 * @brief Create a range instance using the given values and delta.
 * @param [out] out receive the created range instance.
 * @param [in] values results must be within the values given.
 * @param [in] delta by how much do we change from current value to the new
 * value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the range instance.
 * @throws CORAL_ERROR_INVALID_VALUE if delta is <i>ZERO</i>.
 */
bool coral_range_of_delta(struct coral_range **out,
                          struct coral_range_values values,
                          ssize_t delta);

/**
 * @brief Create a range instance using the given values and multiplier.
 * @param [out] out receive the created range instance.
 * @param [in] values results must be within the values given.
 * @param [in] multiplier by how much do we change from current value to the
 * new value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the range instance.
 * @throws CORAL_ERROR_INVALID_VALUE if multiplier is <i>ZERO</i>.
 */
bool coral_range_of_rate(struct coral_range **out,
                         struct coral_range_values values,
                         double multiplier);

#pragma mark coral_object

struct coral_class;
/**
 * @brief Retrieve the class for range.
 * @param [out] out receive class.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral_range_class(struct coral_class **out);

/**
 * @brief Allocate memory for range instance.
 * @param [out] out receive the allocated range instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate for the range instance.
 */
bool coral_range_alloc(struct coral_range **out);

typedef bool (*coral_range_step_func)(const size_t current,
                                      const bool previous,
                                      const struct coral_range_values values,
                                      void *context,
                                      size_t *out);

/**
 * @brief Initialize the range instance.
 * @param [in] object range instance.
 * @param [in] values specify the first and last value within the range.
 * @param [in] step_func used to provide the next or previous value.
 * @param [in] context auxiliary data used within step_func.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if step_func is <i>NULL</i>.
 */
bool coral_range_init(struct coral_range *object,
                      struct coral_range_values values,
                      coral_range_step_func step_func,
                      struct coral_context *context);

/**
 * @brief Destroy the range instance.
 * @param [in] object instance to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_range_destroy(struct coral_range *object);

extern const char *destroy;

/**
 * @brief Retrieve the hash code of the range.
 * @param [in] object instance whose hash code we would like to retrieve.
 * @param [out] out receive the hash code.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_range_hash_code(struct coral_range *object, size_t *out);

extern const char *hash_code;

/**
 * @brief Check if these two ranges are equal.
 * @param [in] object first range.
 * @param [in] other second range.
 * @param [out] out true if <b>object</b> and <b>other</b> are equal,
 * otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other or out is <i>NULL</i>.
 */
bool coral_range_is_equal(struct coral_range *object,
                          struct coral_range *other,
                          bool *out);

extern const char *is_equal;

/**
 * @brief Return a copy of the given range.
 * @param [in] object that we would like to copy.
 * @param [out] out receive the copy.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object and *out are the same object.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_range_copy(struct coral_range *object, struct coral_range **out);

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
bool coral_range_retain(struct coral_range *object);

/**
 * @brief Release and possibly free object resources.
 * @param [in] object whose reference count we are to decrease and upon zero
 * where we will free the memory used by <b>range</b> instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_range_release(struct coral_range *object);

/**
 * @brief Allow object to return from current function.
 * @param [in] object whose reference count will be retained and released in
 * such a manner so that the instance may return from the current function.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_range_autorelease(struct coral_range *object);

#pragma mark coral_range

/**
 * @brief Retrieve the first range value.
 * @param [in] object range instance.
 * @param [out] out receive first range value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_range_get_first(struct coral_range *object, size_t *out);

/**
 * @brief Retrieve the last range value.
 * @param [in] object range instance.
 * @param [out] out receive last range value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_range_get_last(struct coral_range *object, size_t *out);

/**
 * @brief Retrieve the minimum value.
 * @param [in] object range instance.
 * @param [out] out receive the minimum value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_range_get_minimum(struct coral_range *object, size_t *out);

/**
 * @brief Retrieve the maximum value.
 * @param [in] object range instance.
 * @param [out] out receive the maximum value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_range_get_maximum(struct coral_range *object, size_t *out);

/**
 * @brief Check if value is inclusively between the range values.
 * @param [in] object range instance.
 * @param [in] value to be checked.
 * @param [out] out true if included in range values, otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_range_is_inclusive(struct coral_range *object, size_t value,
                              bool *out);

/**
 * @brief Retrieve the next value in the sequence.
 * @param [in] object range instance.
 * @param [in] current value from which next is to be determined.
 * @param [out] out receive next value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if current is not inclusively between
 * first and last values in range.
 * @throws CORAL_ERROR_END_OF_SEQUENCE if we have reached the end of the
 * sequence in the next direction.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_range_get_next(struct coral_range *object, size_t current,
                          size_t *out);

/**
 * @brief Retrieve the previous value in the sequence.
 * @param [in] object range instance.
 * @param [in] current value from which previous is to be determined.
 * @param [out] out receive previous value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if current is not inclusively between
 * first and last values in range.
 * @throws CORAL_ERROR_END_OF_SEQUENCE if we have reached the end of the
 * sequence in the previous direction.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_range_get_prev(struct coral_range *object, size_t current,
                          size_t *out);

#endif /* _CORAL_RANGE_H_ */