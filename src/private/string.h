#ifndef _CORAL_PRIVATE_STRING_H_
#define _CORAL_PRIVATE_STRING_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct coral$string {
    char *data;
    size_t size;
};

#endif /* _CORAL_PRIVATE_INTERFACE_H_ */