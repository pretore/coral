#ifndef _CORAL_AUTORELEASE_POOL_H_
#define _CORAL_AUTORELEASE_POOL_H_

#include <stddef.h>

/**
 * @brief Release all objects in the autorelease pool.
 */
void coral_autorelease_pool_drain();

#endif /* _CORAL_AUTORELEASE_POOL_H_ */
