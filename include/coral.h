#ifndef _CORAL_CORAL_H_
#define _CORAL_CORAL_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "coral/error.h"
#include "coral/notification.h"
#include "coral/autorelease_pool.h"
#include "coral/object.h"
#include "coral/context.h"
#include "coral/reference.h"
#include "coral/weak_reference.h"
#include "coral/range.h"
#include "coral/array.h"
#include "coral/lock.h"

/**
 * @brief Require that object be non-<i>NULL</i>.
 * @param [in] obj to check if it is non-<i>NULL</i>.
 * @note Will call abort(3) if object is <i>NULL</i>.
 */
void coral_required(const void *obj);

/**
 * @brief Require true evaluation.
 * @param [in] check to see if it is true.
 * @note Will call abort(3) if check is false.
 */
void coral_required_true(int check);

/**
 * @brief Comparison function for sorting void* data.
 * @param [in] a min void* to compare.
 * @param [in] b second void* to compare.
 * @return <i>-1</i> if <b>a</b> is <u>less than</u> <b>b</b>,
 * <i>0</i> if <b>a</b> is <u>equal to</u> <b>b</b> or <i>1</i> if <b>a</b> is
 * <u>greater than</u> <b>b</b>.
 */
int coral_compare_void_ptr(const void *a, const void *b);

/**
 * @brief Comparison function for sorting size_t data.
 * @param [in] a min size_t to compare.
 * @param [in] b second size_t to compare.
 * @return <i>-1</i> if <b>a</b> is <u>less than</u> <b>b</b>,
 * <i>0</i> if <b>a</b> is <u>equal to</u> <b>b</b> or <i>1</i> if <b>a</b> is
 * <u>greater than</u> <b>b</b>.
 */
int coral_compare_size_t(const void *a, const void *b);

/**
 * @brief Add two size_t together and check for overflow.
 * @param [in] a min size_t.
 * @param [in] b second size_t.
 * @param [out] out where the result is stored if successful.
 * @return On success true otherwise false if an error occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if the out argument is
 * <i>NULL</i>.
 * @throws CORAL_ERROR_OVERFLOW_OCCURRED if an integer overflow has occurred.
 */
bool coral_add_size_t(size_t a, size_t b, size_t *out);

/**
 * @brief Multiply two size_ts and check for overflow.
 * @param [in] a min integer.
 * @param [in] b second integer.
 * @param [out] out where the result is stored if successful.
 * @return On success true otherwise false if an error occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if the out argument is
 * <i>NULL</i>.
 * @throws CORAL_ERROR_OVERFLOW_OCCURRED if an integer overflow has occurred.
 */
bool coral_multiply_size_t(size_t a, size_t b, size_t *out);

bool coral_minimum_size_t(size_t a, size_t b, size_t *out);

bool coral_maximum_size_t(size_t a, size_t b, size_t *out);

bool coral_set_ref(struct coral_ref **out, void *object);

bool coral_clear_ref(struct coral_ref **ref);

bool coral_set_weak_ref(struct coral_weak_ref **out, void *object);

bool coral_clear_weak_ref(struct coral_weak_ref **ref);

/**s
 * @brief Exponentially puts the current thread to sleep.
 * <p>Puts the current thread asleep for the duration calculated in microseconds
 * from the exponential function and given state. The lesser between 999999 and
 * <b>maximum</b> will be used as the upper bound limit.
 * @param [in,out] state keep track of which iteration we are in.
 * @param [in] maximum upper bound value of the exponential calculation.
 * @return State value to use in next iteration.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if the state argument is
 * <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if maximum is zero or if state value is
 * too large then the state value will be reset.
 * @throws CORAL_ERROR_SYSCALL_FAILED if nanosleep(2) encountered an
 * error.
 */
bool coral_exponential_usleep(uint8_t *state, uint32_t maximum);

#endif /* _CORAL_CORAL_H_ */
