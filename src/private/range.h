#ifndef _CORAL_PRIVATE_RANGE_H_
#define _CORAL_PRIVATE_RANGE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct coral_range_values;
struct coral_context;

struct coral$range {
    struct coral_range_values values;
    struct coral_context *context;
    coral_range_step_func step_func;
};

/**
 * @brief Initialize the range.
 * @param [in] object instance.
 * @param [in] values specify the first and last value within the range.
 * @param [in] step_func used to provide the next or previous value.
 * @param [in] context auxiliary data used within step_func.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if step_func is <i>NULL</i>.
 */
bool coral$range$init(struct coral$range *object,
                      struct coral_range_values values,
                      coral_range_step_func step_func,
                      struct coral_context *context);

/**
 * @brief Invalidate the range.
 * <p>The actual <u>range is not deallocated</u> since it may have been
 * embedded in a larger structure.
 * @param [in] object instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral$range$invalidate(struct coral$range *object);

/**
 * @brief Retrieve the first range value.
 * @param [in] object instance.
 * @param [out] out receive first range value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral$range$get_first(struct coral$range *object, size_t *out);

/**
 * @brief Retrieve the last range value.
 * @param [in] object instance.
 * @param [out] out receive last range value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral$range$get_last(struct coral$range *object, size_t *out);

/**
 * @brief Retrieve the minimum value.
 * @param [in] object instance.
 * @param [out] out receive the minimum value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral$range$get_minimum(struct coral$range *object, size_t *out);

/**
 * @brief Retrieve the maximum value.
 * @param [in] object instance.
 * @param [out] out receive the maximum value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral$range$get_maximum(struct coral$range *object, size_t *out);

/**
 * @brief Check if value is inclusively between the range values.
 * @param [in] object instance.
 * @param [in] value to be checked.
 * @param [out] out true if included in range values, otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral$range$is_inclusive(struct coral$range *object, size_t value,
                              bool *out);

/**
 * @brief Retrieve the next value in the sequence.
 * @param [in] object instance.
 * @param [in] current value from which next is to be determined.
 * @param [out] out receive next value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if current is not inclusively between
 * first and last values in range.
 * @throws CORAL_ERROR_END_OF_SEQUENCE if we have reached the end of the
 * sequence in the next direction.
 */
bool coral$range$get_next(struct coral$range *object, size_t current,
                          size_t *out);

/**
 * @brief Retrieve the previous value in the sequence.
 * @param [in] object instance.
 * @param [in] current value from which previous is to be determined.
 * @param [out] out receive previous value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if current is not inclusively between
 * first and last values in range.
 * @throws CORAL_ERROR_END_OF_SEQUENCE if we have reached the end of the
 * sequence in the previous direction.
 */
bool coral$range$get_prev(struct coral$range *object, size_t current,
                          size_t *out);

#endif /* _CORAL_PRIVATE_RANGE_H_ */