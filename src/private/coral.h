#ifndef _CORAL_PRIVATE_CORAL_H_
#define _CORAL_PRIVATE_CORAL_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdatomic.h>

/* Maximum microseconds that we will sleep for during an exponential backoff */
#define MAXIMUM_USLEEP 250

/**
 * @brief Swap the contents of two void* variables.
 * @param [in,out] a min void* to be swapped with.
 * @param [in,out] b second void* to be swapped with.
 */
void coral$swap_void_ptr(void **a, void **b);

/**
 * @brief Swap the contents of two size_t variables.
 * @param [in,out] a min size_t to be swapped with.
 * @param [in,out] b second size_t to be swapped with.
 */
void coral$swap_size_t(size_t *a, size_t *b);

/**
 * @brief Retrieve the value stored atomically.
 * @param [in] ptr where we will load the value atomically.
 * @return atomically stored value.
 * @note If ptr is <i>NULL</i> we will call abort(3).
 */
size_t coral$atomic_load(const atomic_size_t *ptr);

/**
 * @brief Store the value atomically.
 * @param [in] ptr where we will store the value atomically.
 * @param [in] value which we are to store.
 * @note If ptr is <i>NULL</i> we will call abort(3).
 */
void coral$atomic_store(atomic_size_t *ptr, size_t value);

/**
 * @brief Exchange values atomically.
 * @param [in] ptr whose contents we are to compare and exchange with
 * <b>value</b> if matching <b>compare</b>.
 * @param [in] compare used to check the value of <b>ptr</b>.
 * @param [in] value to be stored in <b>ptr</b> if <b>compare</b> matched the
 * contents of <b>ptr</b>.
 * @return <i>true</i> if exchange did occur; <i>false</i> otherwise.
 * @note If ptr is <i>NULL</i> we will call abort(3).
 */
bool coral$atomic_compare_exchange(atomic_size_t *ptr, size_t compare,
                                   size_t value);

/**
 * @brief Retrieve the pointer atomically.
 * @param [in] ptr from which we will load the pointer atomically.
 * @return atomically stored pointer.
 * @note If ptr is <i>NULL</i> we will call abort(3).
 */
uintptr_t coral$atomic_load_ptr(const atomic_uintptr_t *ptr);

/**
 * @brief Store the pointer atomically.
 * @param [in] ptr where we will store the pointer atomically.
 * @param [in] value which we are to store.
 * @note If ptr is <i>NULL</i> we will call abort(3).
 */
void coral$atomic_store_ptr(atomic_uintptr_t *ptr, uintptr_t value);

/**
 * @brief Exchange pointers atomically.
 * @param [in] ptr whose contents we are to compare and exchange with
 * <b>value</b> if matching <b>compare</b>.
 * @param [in] compare used to check the value of <b>ptr</b>.
 * @param [in] value to be stored in <b>ptr</b> if <b>compare</b> matched the
 * contents of <b>ptr</b>.
 * @return <i>true</i> if exchange did occur; <i>false</i> otherwise.
 * @note If ptr is <i>NULL</i> we will call abort(3).
 */
bool coral$atomic_compare_exchange_ptr(atomic_uintptr_t *ptr, uintptr_t compare,
                                       uintptr_t value);

/**
 * @brief Increase the reference counter.
 * @param [in] ref_counter counter to increase.
 * @note If ref_counter is <i>0</i> or <i>SIZE_MAX</i> we will call abort(3).
 */
void coral$retain(atomic_size_t *ref_counter);

/**
 * @brief Decrease the reference counter and optionally destroy and free object.
 * @param [in] object instance to deallocate after reference counter
 * indicated that object is not in use anymore.
 * @param [in] ref_counter counter to decrease.
 * @param [in] destroy function to call to release resources used by object.
 * @note If object, ref_counter<i>NULL</i> or ref_counter is <i>0</i> we
 * will call abort(3).
 */
void coral$release(void *object, atomic_size_t *ref_counter,
                   void(*on_destroy)(void *object));

#endif /* _CORAL_PRIVATE_CORAL_H_ */
