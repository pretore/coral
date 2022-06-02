#ifndef _CORAL_PRIVATE_AUTORELEASE_POOL_H_
#define _CORAL_PRIVATE_AUTORELEASE_POOL_H_

#include <stddef.h>

struct coral$autorelease_pool$entry {
    struct coral$autorelease_pool$entry *next;
    void *object;
};

/**
 * @brief Return the stack of autorelease pools.
 * @param [out] out stack containing entries.
 * @note If out is <i>NULL</i> we will call abort(3).
 */
void
coral$autorelease_pool$get_stack(struct coral$autorelease_pool$entry **out);

/**
 * @brief Return items being tracked for autorelease.
 * @param [out] out items being tracked for autorelease.
 * @note If out is <i>NULL</i> we will call abort(3).
 */
void
coral$autorelease_pool$get_items(struct coral$autorelease_pool$entry **out);

/**
 * @brief Create new autorelease pool.
 */
void coral$autorelease_pool$start();

/**
 * @brief Destroy autorelease pool and collect any unowned items.
 */
void coral$autorelease_pool$end();

/**
 * @brief Add object to the current autorelease pool.
 * @param [in] object instance to be added to current autorelease pool.
 * @note If object is <i>NULL</i> we will call abort(3).
 */
void coral$autorelease_pool$add(void *object);

/**
 * @brief Add object to the previous autorelease pool.
 * <p>Adding to a previous autorelease pool allows the collection of this
 * object to be deferred.</p>
 * @param [in] object instance to be added to previous autorelease pool.
 * @note If object is <i>NULL</i> or there is no previous autorelease pool we
 * will call abort(3).
 */
void coral$autorelease_pool$add_previous(void *object);

#endif /* _CORAL_PRIVATE_AUTORELEASE_POOL_H_ */
