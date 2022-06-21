#ifndef _CORAL_PRIVATE_INTEGER_H_
#define _CORAL_PRIVATE_INTEGER_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <gmp.h>

#include "object.h"

#define CORAL_CLASS_LOAD_PRIORITY_INTEGER  \
    (1 + CORAL_CLASS_LOAD_PRIORITY_OBJECT)

struct coral$integer {
    mpz_t mpz;
};

/**
 * @brief Create integer from char pointer.
 * <p>
 * The char pointer's contents must be in one of the following formats:
 * <ul>
 * <li>hexadecimal number starts with 0x</li>
 * <li>binary number starts with 0b</li>
 * <li>octal number starts with 0</li>
 * <li>decimal otherwise</li>
 * </ul>
 * </p>
 * @param [out] out receive created integer instance.
 * @param [in] value number in one of the supported formats.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out or value is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if value was in a valid format.
 */
bool coral$integer$of_char_ptr(struct coral$integer *out,
                               const char *value);

/**
 * @brief Create integer from size_t.
 * @param [out] out receive created integer instance.
 * @param [in] value number that the integer should contain initially.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out or instance is <i>NULL</i>.
 */
bool coral$integer$of_size_t(struct coral$integer *out, size_t value);

/**
 * @brief Create integer from ssize_t.
 * @param [out] out receive created integer instance.
 * @param [in] value number that the integer should contain initially.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out or instance is <i>NULL</i>.
 */
bool coral$integer$of_ssize_t(struct coral$integer *out, ssize_t value);

/**
 * @brief Initialize the integer.
 * @param [in] object integer instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral$integer$init(struct coral$integer *object);

/**
 * @brief Invalidate the integer.
 * <p>The actual <u>integer is not deallocated</u> since it may have been
 * embedded in a larger structure.</p>
 * @param [in] object integer instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral$integer$invalidate(struct coral$integer *object);

/**
 * @brief Add other integer to this integer instance.
 * @param [in] object integer instance.
 * @param [in] other integer instance whose value will be added.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other is <i>NULL</i>.
 */
bool coral$integer$add(struct coral$integer *object,
                       struct coral$integer *other);

/**
 * @brief Subtract other integer from this integer instance.
 * @param [in] object integer instance.
 * @param [in] other integer instance whose value we will subtract from.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other is <i>NULL</i>.
 */
bool coral$integer$subtract(struct coral$integer *object,
                            struct coral$integer *other);

/**
 * @brief Multiply this integer by other integer instance.
 * @param [in] object integer instance.
 * @param [in] other integer instance whose value we will use to multiply.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other is <i>NULL</i>.
 */
bool coral$integer$multiply(struct coral$integer *object,
                            struct coral$integer *other);

/**
 * @brief Divide this integer by divisor integer instance.
 * @param [in] object integer instance.
 * @param [in] divisor integer instance whose value we will use to divide.
 * @param [out] remainder receive remainder if provided.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if divisor is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE is divisor is <i>ZERO</i>.
 */
bool coral$integer$divide(struct coral$integer *object,
                          struct coral$integer *divisor,
                          struct coral$integer *remainder);

/**
 * @brief Set this integer instance to its absolute value.
 * @param [in] object integer instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral$integer$absolute(struct coral$integer *object);

/**
 * @brief Set this integer instance to its negative of its value.
 * @param [in] object integer instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral$integer$negate(struct coral$integer *object);

/**
 * @brief Compare this integer instance with another integer instance.
 * @param [in] object integer instance.
 * @param [in] other integer instance.
 * @param [out] out receive an integer less than, equal to, or greater
 * than zero if <u>object</u> is considered to be respectively less than,
 * equal to, or greater than <u>other</u>.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other or out is <i>NULL</i>.
 */
bool coral$integer$compare(struct coral$integer *object,
                           struct coral$integer *other,
                           int *out);

/**
 * @brief Perform bitwise AND.
 * @param [in] object integer instance.
 * @param [in] other integer instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other is <i>NULL</i>.
 */
bool coral$integer$and(struct coral$integer *object,
                       struct coral$integer *other);

/**
 * @brief Perform bitwise OR.
 * @param [in] object integer instance.
 * @param [in] other integer instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other is <i>NULL</i>.
 */
bool coral$integer$or(struct coral$integer *object,
                      struct coral$integer *other);

/**
 * @brief Perform bitwise NOT.
 * @param [in] object integer instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral$integer$not(struct coral$integer *object);

/**
 * @brief Perform bitwise XOR.
 * @param [in] object integer instance.
 * @param [in] other integer instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other is <i>NULL</i>.
 */
bool coral$integer$xor(struct coral$integer *object,
                       struct coral$integer *other);

/**
 * @brief Bitwise shift left the integer by given number of bits.
 * @param [in] object integer instance.
 * @param [in] nbits number of bits to left shift integer by.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral$integer$shift_left(struct coral$integer *object,
                              size_t nbits);

/**
 * @brief Bitwise shift right the integer by given number of bits.
 * @param [in] object integer instance.
 * @param [in] nbits number of bits to right shift integer by.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral$integer$shift_right(struct coral$integer *object,
                               size_t nbits);

/**
 * @brief Return the number of bits used for the value of integer (ignoring
 * the sign bit).
 * @param [in] object integer instance.
 * @param [out] out number of bits used to represent the absolute value of
 * the integer.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral$integer$length(struct coral$integer *object,
                          size_t *out);

/**
 * @brief Return the number of 1 bits for the value of integer (ignoring
 * the sign bit).
 * @param [in] object integer instance.
 * @param [out] out number of 1 bits in the absolute value of integer.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral$integer$count(struct coral$integer *object,
                         size_t *out);

/**
 * @brief Return the bit at the given index for the two's complement
 * representation of integer.
 * @param [in] object integer instance.
 * @param [in] at index of bit to retrieve where the least significant bit is 0.
 * @param [out] out receive bit at the given index.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @see <a href=https://en.wikipedia.org/wiki/Two%27s_complement> two's
 * complement
 */
bool coral$integer$get(struct coral$integer *object,
                       size_t at,
                       bool *out);

/**
 * @brief Set the bit at the given index for the two's complement
 * representation of integer.
 * @param [in] object integer instance.
 * @param [in] at index of bit to set where the least significant bit is 0.
 * @param [in] value set the bit to value at the given index.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @see <a href=https://en.wikipedia.org/wiki/Two%27s_complement> two's
 * complement
 */
bool coral$integer$set(struct coral$integer *object,
                       size_t at,
                       bool value);

struct coral_range_values;
/**
 * @brief Find the bit in the integer.
 * @param [in] object integer instance.
 * @param [in] values if provided, will limit the search to only between the
 * given indexes inclusively.
 * @param [in] needle bit that we are looking for.
 * @param [out] out receive the index of the bit found in the integer where
 * the least significant bit is 0.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if needle was not found in the integer.
 */
bool coral$integer$find(struct coral$integer *object,
                        struct coral_range_values *values,
                        bool needle,
                        size_t *out);

#endif /* _CORAL_PRIVATE_INTEGER_H_ */
