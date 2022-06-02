#ifndef _CORAL_INTEGER_H_
#define _CORAL_INTEGER_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

struct coral_integer;

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
bool coral_integer_of_char_ptr(struct coral_integer **out, const char *value);

/**
 * @brief Create integer from size_t.
 * @param [out] out receive created integer instance.
 * @param [in] value number that the integer should contain initially.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out or value is <i>NULL</i>.
 */
bool coral_integer_of_size_t(struct coral_integer **out, size_t value);

/**
 * @brief Create integer from ssize_t.
 * @param [out] out receive created integer instance.
 * @param [in] value number that the integer should contain initially.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out or value is <i>NULL</i>.
 */
bool coral_integer_of_ssize_t(struct coral_integer **out, ssize_t value);

#pragma mark coral_object

struct coral_class;
/**
 * @brief Retrieve the class for integer.
 * @param [out] out receive class.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral_integer_class(struct coral_class **out);

/**
 * @brief Allocate memory for integer instance.
 * @param [out] out receive the allocated integer instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate space for the integer instance.
 */
bool coral_integer_alloc(struct coral_integer **out);

/**
 * @brief Initialize the integer.
 * @param [in] object integer instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_integer_init(struct coral_integer *object);

/**
 * @brief Destroy the integer instance.
 * @param [in] object instance to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral_integer_destroy(struct coral_integer *object);

extern const char *destroy;

/**
 * @brief Retrieve the hash code of the integer instance.
 * @param [in] object instance whose hash code we would like to retrieve.
 * @param [out] out receive the hash code.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object is not integer an instance.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_integer_hash_code(struct coral_integer *object, size_t *out);

extern const char *hash_code;

/**
 * @brief Check if these two integers are equal.
 * @param [in] object first integer instance.
 * @param [in] other second integer instance.
 * @param [out] out true if <b>object</b> and <b>other</b> are equal,
 * otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other or out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_integer_is_equal(struct coral_integer *object,
                            struct coral_integer *other,
                            bool *out);

extern const char *is_equal;

/**
 * @brief Retain the referenced counted object.
 * @param [in] object whose reference count is to be increase to prevent early
 * collection.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_integer_retain(struct coral_integer *object);

/**
 * @brief Release and possibly free object resources.
 * @param [in] object whose reference count we are to decrease and upon zero
 * where we will free the memory used by <b>object</b> instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_integer_release(struct coral_integer *object);

/**
 * @brief Allow object to return from current function.
 * @param [in] object whose reference count will be retained and released in
 * such a manner so that the instance may return from the current function.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_integer_autorelease(struct coral_integer *object);

#pragma mark coral_integer

/**
 * @brief Add other integer to this integer instance.
 * @param [in] object integer instance.
 * @param [in] other integer instance whose value will be added.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object or other are not integer
 * instances.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_integer_add(struct coral_integer *object,
                       struct coral_integer *other);

/**
 * @brief Subtract other integer from this integer instance.
 * @param [in] object integer instance.
 * @param [in] other integer instance whose value we will subtract from.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object or other are not integer
 * instances.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_integer_subtract(struct coral_integer *object,
                            struct coral_integer *other);

/**
 * @brief Multiply this integer by other integer instance.
 * @param [in] object integer instance.
 * @param [in] other integer instance whose value we will use to multiply.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object or other are not integer
 * instances.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_integer_multiply(struct coral_integer *object,
                            struct coral_integer *other);

/**
 * @brief Divide this integer by divisor integer instance.
 * @param [in] object integer instance.
 * @param [in] divisor integer instance whose value we will use to divide.
 * @param [out] remainder receive remainder if provided.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if divisor is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object or divisor are not integer
 * instances or divisor is <i>ZERO</i>.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_integer_divide(struct coral_integer *object,
                          struct coral_integer *divisor,
                          struct coral_integer **remainder);

/**
 * @brief Set this integer instance to its negative of its value.
 * @param [in] object integer instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object is not an integer instance.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_integer_absolute(struct coral_integer *object);

/**
 * @brief Set this integer instance to its negative of its value.
 * @param [in] object integer instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object is not an integer instance.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_integer_negate(struct coral_integer *object);

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
 * @throws CORAL_ERROR_INVALID_VALUE if object or other are not integer
 * instances.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_integer_compare(struct coral_integer *object,
                           struct coral_integer *other,
                           int *out);

/**
 * @brief Perform bitwise AND.
 * @param [in] object integer instance.
 * @param [in] other integer instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object or other are not integer
 * instances.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_integer_and(struct coral_integer *object,
                       struct coral_integer *other);

/**
 * @brief Perform bitwise OR.
 * @param [in] object integer instance.
 * @param [in] other integer instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object or other are not integer
 * instances.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_integer_or(struct coral_integer *object,
                      struct coral_integer *other);

/**
 * @brief Perform bitwise NOT.
 * @param [in] object integer instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object is not an integer instance.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_integer_not(struct coral_integer *object);

/**
 * @brief Perform bitwise XOR.
 * @param [in] object integer instance.
 * @param [in] other integer instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if other is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object or other are not integer
 * instances.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_integer_xor(struct coral_integer *object,
                       struct coral_integer *other);

/**
 * @brief Bitwise shift left the integer by given number of bits.
 * @param [in] object integer instance.
 * @param [in] nbits number of bits to left shift integer by.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object is not an integer instance.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_integer_shift_left(struct coral_integer *object,
                              size_t nbits);

/**
 * @brief Bitwise shift right the integer by given number of bits.
 * @param [in] object integer instance.
 * @param [in] nbits number of bits to right shift integer by.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object is not an integer instance.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_integer_shift_right(struct coral_integer *object,
                               size_t nbits);

/**
 * @brief Return the number of bits used for the value of integer (ignoring
 * the sign bit).
 * @param [in] object integer instance.
 * @param [out] out number of bits used to represent the value of the integer
 * (ignoring the sign bit).
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object is not an integer instance.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_integer_length(struct coral_integer *object,
                          size_t *out);

/**
 * @brief Return the number of 1 bits for the value of integer (ignoring
 * the sign bit).
 * @param [in] object integer instance.
 * @param [out] out number of 1 bits in the value of integer (ignoring the
 * sign bit).
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_INVALID_VALUE if object is not an integer instance.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_integer_count(struct coral_integer *object,
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
 * @throws CORAL_ERROR_INVALID_VALUE if object is not an integer instance.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 * @see <a href=https://en.wikipedia.org/wiki/Two%27s_complement> two's
 * complement
 */
bool coral_integer_get(struct coral_integer *object,
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
 * @throws CORAL_ERROR_INVALID_VALUE if object is not an integer instance.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 * @see <a href=https://en.wikipedia.org/wiki/Two%27s_complement> two's
 * complement
 */
bool coral_integer_set(struct coral_integer *object,
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
 * @throws CORAL_ERROR_INVALID_VALUE if object is not an integer instance.
 * @throws CORAL_ERROR_OBJECT_IS_UNINITIALIZED if object is uninitialized or
 * (being) destroyed.
 */
bool coral_integer_find(struct coral_integer *object,
                        struct coral_range_values *values,
                        bool needle,
                        size_t *out);

#endif /* _CORAL_INTEGER_H_ */
