#ifndef _CORAL_ERROR_H_
#define _CORAL_ERROR_H_

#include <stddef.h>
#include <threads.h>

#define CORAL_ERROR_NONE                            0
#define CORAL_ERROR_MEMORY_ALLOCATION_FAILED        1
#define CORAL_ERROR_OVERFLOW_OCCURRED               2
#define CORAL_ERROR_LOSS_OF_PRECISION               3
#define CORAL_ERROR_INVALID_VALUE                   4
#define CORAL_ERROR_SYSCALL_FAILED                  5
#define CORAL_ERROR_OBJECT_UNAVAILABLE              6
#define CORAL_ERROR_ARGUMENT_UNAVAILABLE            7
#define CORAL_ERROR_OBJECT_PTR_IS_NULL              8
#define CORAL_ERROR_ARGUMENT_PTR_IS_NULL            9
#define CORAL_ERROR_INITIALIZATION_FAILED           10
#define CORAL_ERROR_OBJECT_ALREADY_EXISTS           11
#define CORAL_ERROR_OBJECT_NOT_FOUND                12
#define CORAL_ERROR_OBJECT_IS_UNINITIALIZED         13
#define CORAL_ERROR_INDEX_OUT_OF_BOUNDS             14
#define CORAL_ERROR_END_OF_SEQUENCE                 15
#define CORAL_ERROR_METHOD_NOT_FOUND                16
#define CORAL_ERROR_METHOD_ALREADY_EXISTS           17

extern thread_local size_t coral_error;

#endif /* _CORAL_ERROR_H_ */
