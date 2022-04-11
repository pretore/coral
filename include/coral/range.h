#ifndef _CORAL_RANGE_H_
#define _CORAL_RANGE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct coral_range;
struct coral_context;
struct coral_range_values {
    size_t first;
    size_t last;
};

/**
 * @brief Create a range instance using the given values and delta.
 * @param [out] out receive the created range instance.
 * @param [in] values results must be within the values given.
 * @param [in] delta how much change from current value to the new value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the range instance.
 * @throws CORAL_ERROR_INVALID_VALUE if delta is zero or when added to the
 * minimum value it will exceed the maximum value.
 */
bool coral_range_of_delta(struct coral_range **out,
                          struct coral_range_values *values,
                          size_t delta);

/**
 * @brief Create a range instance using the given values and multiplier.
 * @param [out] out receive the created range instance.
 * @param [in] values results must be within the values given.
 * @param [in] multiplier how much change from current value to the new value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the range instance.
 * @throws CORAL_ERROR_INVALID_VALUE if multiplier is negative, zero, one or
 * if multiplied with the minimum value give you a value that exceeds the
 * maximum value.
 */
bool coral_range_of_rate(struct coral_range **out,
                         struct coral_range_values *values,
                         double multiplier);

/**
 * @brief Allocate memory for range instance.
 * @param [out] out receive the allocated range instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the context instance.
 */
bool coral_range_alloc(struct coral_range **out);

typedef bool(*coral_range_step_func_t)(const size_t current, const bool prev,
                                       const struct coral_range_values values,
                                       void *context, size_t *out);

/**
 * @brief Initialize the range instance.
 * @param [in] object instance to be initialized.
 * @param [in] values results must be within the values given.
 * @param [in] step_func get the next or previous value as needed.
 * @param [in] context provide a <i>scratchpad</i> for step function
 * invocations.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if step_func is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to initialize the context instance.
 */
bool coral_range_init(struct coral_range *object,
                      struct coral_range_values *values,
                      coral_range_step_func_t step_func,
                      struct coral_context *context);

/**
 * @brief Destroy the range instance.
 * @param [in] object instance to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_range_destroy(struct coral_range *object);

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
 * where we will free the memory used by <b>object</b> instance.
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

/**
 * @brief Retrieve the minimum value.
 * @param [in] object range instance whose minimum value we would like to
 * retrieve.
 * @param [out] out receive the minimum value of the range instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_range_get_min(struct coral_range *object, size_t *out);

/**
 * @brief Retrieve the maximum value.
 * @param [in] object range instance whose maximum value we would like to
 * retrieve.
 * @param [out] out receive the maximum value of the range instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_range_get_max(struct coral_range *object, size_t *out);

/**
 * @brief Retrieve the values.
 * @param [in] object range instance whose values we would like to retrieve.
 * @param [out] out receive the values of the range instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_range_get(struct coral_range *object,
                     struct coral_range_values *out);

/**
 * @brief Retrieve the next value.
 * @param [in] object range instance whose next value we would like to retrieve.
 * @param [in] current from which the next value is to be calculated from.
 * @param [out] out receive the next value of the range instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 * @throws CORAL_ERROR_ARGUMENT_UNAVAILABLE if we failed to retrieve the data
 * from the provided context.
 * @throws CORAL_ERROR_END_OF_SEQUENCE if the next value could not be retrieved.
 */
bool coral_range_get_next(struct coral_range *object, size_t current,
                          size_t *out);

/**
 * @brief Retrieve the previous value.
 * @param [in] object range instance whose previous value we would like to
 * retrieve.
 * @param [in] current from which the previous value is to be calculated from.
 * @param [out] out receive the previous of the range instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 * @throws CORAL_ERROR_ARGUMENT_UNAVAILABLE if we failed to retrieve the data
 * from the provided context.
 * @throws CORAL_ERROR_END_OF_SEQUENCE if the previous value could not be
 * retrieved.
 */
bool coral_range_get_prev(struct coral_range *object, size_t current,
                          size_t *out);

#endif /* _CORAL_RANGE_H_ */