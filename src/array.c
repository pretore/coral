#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <coral.h>

#include "private/array.h"
#include "test/cmocka.h"

#pragma mark private

static struct coral_ref *$capacity_range_ref;

__attribute__((constructor))
static void coral$array_load() {
    struct coral_range *range;
    struct coral_range_values values = {
            .first = 16,
            .last = SIZE_MAX
    };
    coral_required_true(coral_range_of_rate(&range, &values, 1.5));
    coral_required_true(coral_set_ref(&$capacity_range_ref, range));
    coral_autorelease_pool_drain();
}

__attribute__((destructor()))
static void coral$array_unload() {
    coral_required_true(coral_clear_ref(&$capacity_range_ref));
    coral_autorelease_pool_drain();
}

bool coral$array_adjust_size_if_needed(struct coral_array *object,
                                       const size_t size) {
    coral_required(object);
    size_t size_ = 0;
    coral_required_true(coral_maximum_size_t(
            object->size, size, &size_));
    if (object->size == size) {
        return true;
    }
    if (!coral_multiply_size_t(object->capacity, size, &size_)) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    unsigned char *data = realloc(object->data, size_);
    if (!data) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    object->data = data;
    for (size_t i = 0, limit = object->count - 1; i < limit; i++) {
        const size_t index = object->count - 1 - i;
        const unsigned char *src = data + (index * object->size);
        unsigned char *dst = data + (index * size);
        memmove(dst, src, object->size);
    }
    object->size = size;
    return true;
}

bool coral$array_adjust_capacity_if_needed(struct coral_array *object,
                                           const size_t capacity) {
    coral_required(object);
    size_t capacity_;
    coral_required_true(coral_maximum_size_t(
            object->capacity, capacity, &capacity_));
    if (object->capacity == capacity_) {
        return true;
    }
    size_t size_;
    if (!coral_multiply_size_t(object->size, capacity_, &size_)) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    unsigned char *data = realloc(object->data, size_);
    if (!data) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    object->data = data;
    object->capacity = capacity_;
    return true;
}

bool coral$array_set_item_at(struct coral_array *object,
                             const size_t at,
                             const struct coral_array_item *item) {
    coral_required(object);
    coral_required_true(object->capacity > at);
    if (item) {
        size_t size_ = 0;
        coral_required_true(coral_maximum_size_t(
                object->size, item->size, &size_));
        if (!coral$array_adjust_size_if_needed(object, size_)) {
            return false;
        }
    }
    unsigned char *dst = object->data + (object->size * at);
    if (item) {
        memcpy(dst, item->data, item->size);
    } else {
        memset(dst, 0, object->size);
    }
    return true;
}

bool coral$array_init(struct coral_array *object,
                      size_t count,
                      size_t size,
                      struct coral_range *capacity_range) {
    coral_required(object);
    if (!capacity_range) {
        coral_required_true(coral_ref_get($capacity_range_ref,
                                          (void **) &capacity_range));
    }
    struct coral_range_values values;
    if (!coral_range_get(capacity_range, &values)
        || values.last < count) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    }
    size_t current_;
    size_t capacity_;
    if (!coral_maximum_size_t(values.first, count, &current_)
        || !coral_range_get_next(capacity_range,
                                 current_,
                                 &capacity_)) {
        coral_error = CORAL_ERROR_CAPACITY_LIMIT_REACHED;
        return false;
    }
    size_t size_;
    if (!coral_multiply_size_t(size, capacity_, &size_)) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    object->data = calloc(1, size_);
    if (size_ && !object->data) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    object->size = size;
    object->count = count;
    object->capacity = capacity_;
    return coral_object_init(object, (void (*)(void *)) coral$array_destroy)
           && coral_set_ref(&object->capacity_range_ref, capacity_range);
}

void coral$array_destroy(struct coral_array *object) {
    coral_required(object);
    free(object->data);
    object->data = NULL;
    coral_required_true(coral_clear_ref(&object->capacity_range_ref));
    coral_required_true(coral_object_destroy(object));
}

bool coral$array_get_capacity(struct coral_array *object,
                              struct coral$array_get_capacity_args *args) {
    coral_required(object);
    coral_required(args);
    coral_required(args->out);
    struct coral_range *capacity;
    return coral_ref_get(object->capacity_range_ref, (void **) &capacity)
           && coral_range_get(capacity, args->out);
}

bool coral$array_get_count(struct coral_array *object,
                           struct coral$array_get_count_args *args) {
    coral_required(object);
    coral_required(args);
    coral_required(args->out);
    *args->out = object->count;
    return true;
}

bool coral$array_set_count(struct coral_array *object,
                           struct coral$array_set_count_args *args) {
    coral_required(object);
    coral_required(args);
    if (object->count == args->count) {
        return true;
    }
    // TODO: on container decrease ...
    if (object->capacity < args->count) {
        size_t capacity_;
        struct coral_range *capacity_range;
        if (!coral_ref_get(object->capacity_range_ref,
                           (void **) &capacity_range)
            || coral_range_get_next(capacity_range,
                                    args->count,
                                    &capacity_)) {
            coral_error = CORAL_ERROR_CAPACITY_LIMIT_REACHED;
            return false;
        }
        if (!coral$array_adjust_capacity_if_needed(object, capacity_)) {
            return false;
        }
    }
    object->count = args->count;
    return true;
}

bool coral$array_get_size(struct coral_array *object,
                          struct coral$array_get_size_args *args) {
    coral_required(object);
    coral_required(args);
    coral_required(args->out);
    *args->out = object->size;
    return true;
}

bool coral$array_set(struct coral_array *object,
                     struct coral$array_set_args *args) {
    coral_required(object);
    coral_required(args);
    if (object->count <= args->at) {
        coral_error = CORAL_ERROR_INDEX_OUT_OF_BOUNDS;
        return false;
    }
    return coral$array_set_item_at(object, args->at, args->item);
}

bool coral$array_get(struct coral_array *object,
                     struct coral$array_get_args *args) {
    coral_required(object);
    coral_required(args);
    coral_required(args->item);
    if (object->count <= args->at) {
        coral_error = CORAL_ERROR_INDEX_OUT_OF_BOUNDS;
        return false;
    }
    struct coral_array_item *item = args->item;
    item->size = object->size;
    item->data = object->data + (args->at * object->size);
    return true;
}

static thread_local struct coral_array *$this;

static thread_local int (*$array_compare)(
        const struct coral_array_item *,
        const struct coral_array_item *);

static int coral$array_compare(const void *a, const void *b) {
    const size_t size = $this->size;
    struct coral_array_item A = {
            .size = size,
            .data = (void *) a
    };
    struct coral_array_item B = {
            .size = size,
            .data = (void *) b
    };
    return $array_compare(&A, &B);
}

bool coral$array_sort(struct coral_array *object,
                      struct coral$array_sort_args *args) {
    coral_required(object);
    coral_required(args);
    coral_required(args->compare);
    $this = object;
    $array_compare = args->compare;
    qsort(object->data,
          object->count,
          object->size,
          coral$array_compare);
    return true;
}

bool coral$array_find(struct coral_array *object,
                      struct coral$array_find_args *args) {
    coral_required(object);
    coral_required(args);
    coral_required(args->needle);
    coral_required(args->is_equals);
    coral_required(args->out);
    if (!object->count) {
        coral_error = CORAL_ERROR_OBJECT_NOT_FOUND;
        return false;
    }
    struct coral_range *range = (struct coral_range *) args->range;
    if (!range) {
        struct coral_range_values values = {
                .first = 0,
                .last = object->count - 1
        };
        if (!coral_range_of_delta(&range, &values, 1)) {
            return false;
        }
    }
    size_t at;
    struct coral_array_item item;
    if (!coral_range_get_min(range, &at)) {
        return false;
    }
    do {
        if (!coral_array_get(object, at, &item)) {
            coral_error = CORAL_ERROR_OBJECT_NOT_FOUND;
            return false;
        }
        if (args->is_equals(args->needle, &item)) {
            *args->out = at;
            return true;
        }
    } while (coral_range_get_next(range, at, &at));
    coral_error = CORAL_ERROR_OBJECT_NOT_FOUND;
    return false;
}

bool coral$array_insert(struct coral_array *object,
                        struct coral$array_insert_args *args) {
    coral_required(object);
    coral_required(args);
    if (object->count <= args->at) {
        coral_error = CORAL_ERROR_INDEX_OUT_OF_BOUNDS;
        return false;
    }
    if (object->count == object->capacity) {
        size_t capacity_;
        struct coral_range *capacity_range;
        if (!coral_ref_get(object->capacity_range_ref,
                           (void **) &capacity_range)
            || !coral_range_get_next(capacity_range,
                                     object->capacity,
                                     &capacity_)) {
            coral_error = CORAL_ERROR_CAPACITY_LIMIT_REACHED;
            return false;
        }
        if (!coral$array_adjust_capacity_if_needed(object, capacity_)) {
            return false;
        }
    }
    unsigned char *src = object->data + (args->at * object->size);
    unsigned char *dst = (unsigned char *) (src + object->size);
    const size_t n = object->data + (object->count * object->size) - src;
    memmove(dst, src, n);
    object->count += 1;
    const bool result = coral$array_set_item_at(object, args->at, args->item);
    if (!result) {
        object->count -= 1;
        memmove(src, dst, n);
    }
    // TODO: on container increase
    return result;
}

bool coral$array_add(struct coral_array *object,
                     struct coral$array_add_args *args) {
    coral_required(object);
    coral_required(args);
    if (object->count == object->capacity) {
        size_t capacity_;
        struct coral_range *capacity_range;
        if (!coral_ref_get(object->capacity_range_ref,
                           (void **) &capacity_range)
            || !coral_range_get_next(capacity_range,
                                     object->capacity,
                                     &capacity_)) {
            coral_error = CORAL_ERROR_CAPACITY_LIMIT_REACHED;
            return false;
        }
        if (!coral$array_adjust_capacity_if_needed(object, capacity_)) {
            return false;
        }
    }
    if (!coral$array_set_item_at(object, object->count, args->item)) {
        return false;
    }
    object->count += 1;
    return true;
}

bool coral$array_erase(struct coral_array *object,
                       struct coral$array_erase_args *args) {
    coral_required(object);
    coral_required(args);
    if (object->count <= args->at) {
        coral_error = CORAL_ERROR_INDEX_OUT_OF_BOUNDS;
        return false;
    }
    unsigned char *dst = object->data + (args->at * object->size);
    unsigned char *src = (unsigned char *) (dst + object->size);
    const size_t n = object->data + (object->count * object->size) - src;
    memmove(dst, src, n);
    object->count -= 1;
    return true;
}

bool coral$array_remove(struct coral_array *object, void *args) {
    coral_required(object);
    if (!object->count) {
        coral_error = CORAL_ERROR_OBJECT_NOT_FOUND;
        return false;
    }
    object->count -= 1;
    return true;
}

#pragma mark public

bool coral_array_of_objects(struct coral_array **out) {
    if (coral_array_alloc(out)) {
        if (coral_array_init(*out,
                             0,
                             sizeof(struct coral_object *),
                             NULL)) {
            // TODO: add listening for CORAL_CONTAINER_* notifications
            return true;
        }
        coral_array_destroy(*out);
    }
    return false;
}

bool coral_array_alloc(struct coral_array **out) {
    return coral_object_alloc(sizeof(struct coral_array), (void **) out);
}

bool coral_array_init(struct coral_array *object,
                      const size_t count,
                      const size_t size,
                      struct coral_range *capacity_range) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral$array_init(object,
                            count,
                            size,
                            capacity_range);
}

bool coral_array_destroy(struct coral_array *object) {
    return coral_object_destroy(object);
}

bool coral_array_retain(struct coral_array *object) {
    return coral_object_retain(object);
}

bool coral_array_release(struct coral_array *object) {
    return coral_object_release(object);
}

bool coral_array_autorelease(struct coral_array *object) {
    return coral_object_autorelease(object);
}

bool coral_array_get_capacity(struct coral_array *object,
                              struct coral_range_values *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct coral$array_get_capacity_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$array_get_capacity,
            &args);
}

bool coral_array_get_count(struct coral_array *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct coral$array_get_count_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$array_get_count,
            &args);
}

bool coral_array_set_count(struct coral_array *object, const size_t count) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    struct coral$array_set_count_args args = {
            .count = count
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$array_set_count,
            &args);
}

bool coral_array_get_size(struct coral_array *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct coral$array_get_count_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$array_get_size,
            &args);
}

bool coral_array_set(struct coral_array *object, const size_t at,
                     const struct coral_array_item *item) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    /* NULL item means clear contents at index */
    struct coral$array_set_args args = {
            .at = at
    };
    if (item) {
        if (!item->size || !item->data) {
            coral_error = CORAL_ERROR_INVALID_VALUE;
            return false;
        }
        args.item = item;
    }
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$array_set,
            &args);
}

bool coral_array_get(struct coral_array *object, size_t at,
                     struct coral_array_item *item) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!item) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct coral$array_get_args args = {
            .at = at,
            .item = item
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$array_get,
            &args);
}

bool coral_array_sort(struct coral_array *object,
                      int (*compare)(const struct coral_array_item *,
                                     const struct coral_array_item *)) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!compare) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct coral$array_sort_args args = {
            .compare = compare
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$array_sort,
            &args);
}

bool coral_array_find(struct coral_array *object,
                      const struct coral_range *range,
                      const struct coral_array_item *needle,
                      bool (*is_equals)(const struct coral_array_item *,
                                        const struct coral_array_item *),
                      size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    /* NULL range means we will search the whole array */
    if (!needle || !is_equals || !out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct coral$array_find_args args = {
            .range = range,
            .needle = needle,
            .is_equals = is_equals,
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$array_find,
            &args);
}

bool coral_array_insert(struct coral_array *object, const size_t at,
                        const struct coral_array_item *item) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    /* NULL item means we insert a zeroed out item */
    struct coral$array_insert_args args = {
            .at = at
    };
    if (item) {
        if (!item->size || !item->data) {
            coral_error = CORAL_ERROR_INVALID_VALUE;
            return false;
        }
        args.item = item;
    }
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$array_insert,
            &args);
}

bool coral_array_add(struct coral_array *object,
                     const struct coral_array_item *item) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    /* NULL item means we add a zeroed out item */
    struct coral$array_add_args args = {};
    if (item) {
        if (!item->size || !item->data) {
            coral_error = CORAL_ERROR_INVALID_VALUE;
            return false;
        }
        args.item = item;
    }
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$array_add,
            &args);
}

bool coral_array_erase(struct coral_array *object, const size_t at) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    struct coral$array_erase_args args = {
            .at = at
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$array_erase,
            &args);
}

bool coral_array_remove(struct coral_array *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$array_remove,
            NULL);
}
