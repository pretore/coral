#ifndef _CORAL_STRING_H_
#define _CORAL_STRING_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct coral_string;

bool coral_string_alloc(struct coral_string **out);

bool coral_string_init(struct coral_string *object,
                       unsigned char *data,
                       size_t size);

bool coral_string_destroy(struct coral_string *object);

bool coral_string_class(struct coral_class const **out);

// TODO: coral_object_dispatch methods ...


#endif /* _CORAL_STRING_H_ */