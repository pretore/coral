#ifndef _CORAL_PRIVATE_ARRAY_H_
#define _CORAL_PRIVATE_ARRAY_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

struct coral_range;
struct coral_array_item;
struct coral_array {
    size_t capacity;
    size_t count;
    size_t size;
    unsigned char *data;
    struct coral_ref *capacity_range_ref;
};

bool coral$array_init(struct coral_array *object,
                      size_t count,
                      size_t size,
                      struct coral_range *capacity_range);

void coral$array_destroy(struct coral_array *object);

struct coral$array_get_capacity_args {
    struct coral_range_values *out;
};

bool coral$array_get_capacity(struct coral_array *object,
                              struct coral$array_get_capacity_args *args);

struct coral$array_get_count_args {
    size_t *out;
};

bool coral$array_get_count(struct coral_array *object,
                           struct coral$array_get_count_args *args);

struct coral$array_set_count_args {
    size_t count;
};

bool coral$array_set_count(struct coral_array *object,
                           struct coral$array_set_count_args *args);

struct coral$array_get_size_args {
    size_t *out;
};

bool coral$array_get_size(struct coral_array *object,
                          struct coral$array_get_size_args *args);

struct coral$array_set_args {
    size_t at;
    const struct coral_array_item *item;
};

bool coral$array_set(struct coral_array *object,
                     struct coral$array_set_args *args);

struct coral$array_get_args {
    size_t at;
    struct coral_array_item *item;
};

bool coral$array_get(struct coral_array *object,
                     struct coral$array_get_args *args);

struct coral$array_sort_args {
    int (*compare)(const struct coral_array_item *,
                   const struct coral_array_item *);
};

bool coral$array_sort(struct coral_array *object,
                      struct coral$array_sort_args *args);

struct coral$array_find_args {
    const struct coral_range *range;
    const struct coral_array_item *needle;
    bool (*is_equals)(const struct coral_array_item *,
                      const struct coral_array_item *);
    size_t *out;
};

bool coral$array_find(struct coral_array *object,
                      struct coral$array_find_args *args);

struct coral$array_insert_args {
    size_t at;
    const struct coral_array_item *item;
};

bool coral$array_insert(struct coral_array *object,
                        struct coral$array_insert_args *args);

struct coral$array_add_args {
    const struct coral_array_item *item;
};

bool coral$array_add(struct coral_array *object,
                     struct coral$array_add_args *args);

struct coral$array_erase_args {
    size_t at;
};

bool coral$array_erase(struct coral_array *object,
                       struct coral$array_erase_args *args);

bool coral$array_remove(struct coral_array *object, void *args);

#endif /* _CORAL_PRIVATE_ARRAY_H_ */