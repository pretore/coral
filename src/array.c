#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <coral.h>

#include "private/array.h"
#include "private/coral.h"
#include "private/class.h"
#include "test/cmocka.h"

#pragma mark private -

static struct coral_range *$limit;
static struct coral_class *$class;
static struct coral_class *$class$search_pattern;

#pragma mark + invokables

static bool $array_destroy(struct coral_array *this,
                           void *data,
                           void *args);

static bool $array_is_equal(void *this,
                            struct coral_array *data,
                            struct is_equal_args *args);

static bool $array_copy(void *this,
                        struct coral_array *data,
                        struct copy_args *args);

static bool $array$search_pattern$destroy(
        struct coral_array_search_pattern *this,
        void *data,
        void *args);

__attribute__((constructor(CORAL_CLASS_LOAD_PRIORITY_ARRAY)))
static void $on_load() {
    struct coral_range_values value = {0, SIZE_MAX};
    coral_required_true(coral_range_of_rate(&$limit, value, 1.5));
    coral_required_true(coral_range_retain($limit));

    struct coral_class_method_name $method_names[] = {
            {destroy,  strlen(destroy)},
            {is_equal, strlen(is_equal)},
            {copy,     strlen(copy)},
    };
    /* class for array */
    coral_required_true(coral_class_alloc(&$class));
    coral_required_true(coral_class_init($class));
    coral_required_true(coral_class_retain($class));
    /* destroy */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[0],
            (coral_invokable_t) $array_destroy));
    /* is_equal */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[1],
            (coral_invokable_t) $array_is_equal));
    /* copy */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[2],
            (coral_invokable_t) $array_copy));

    /* class for search_pattern */
    coral_required_true(coral_class_alloc(&$class$search_pattern));
    coral_required_true(coral_class_init($class$search_pattern));
    coral_required_true(coral_class_retain($class$search_pattern));
    /* destroy */
    coral_required_true(coral_class_method_add(
            $class$search_pattern, &$method_names[0],
            (coral_invokable_t) $array$search_pattern$destroy));

    coral_autorelease_pool_drain();
}

__attribute__((destructor(CORAL_CLASS_LOAD_PRIORITY_ARRAY)))
static void $on_unload() {
    coral_required_true(coral_range_release($limit));
    coral_required_true(coral_class_destroy($class));
    coral_required_true(coral_class_destroy($class$search_pattern));

    coral_autorelease_pool_drain();
}

bool coral$array$init(struct coral$array *object,
                      struct coral_range *limit,
                      const size_t count,
                      const size_t size) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!limit) {
        limit = $limit;
    }
    if (!coral_set_reference(&object->limit_ref, limit)) {
        coral_required_true(CORAL_ERROR_ARGUMENT_PTR_IS_NULL != coral_error);
        return false;
    }
    coral$atomic_store(&object->id, 0);
    object->size = size;
    object->data = NULL;
    size_t minimum;
    coral_required_true(coral_range_get_minimum(limit, &minimum));
    return coral$array$set_capacity(object, minimum)
           && coral$array$set_count(object, count);
}

bool coral$array$invalidate(struct coral$array *object,
                            void (*on_destroy)(void *, const size_t)) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (on_destroy) {
        for (size_t i = 0; i < object->count; i++) {
            on_destroy(object->data + (object->size * i), object->size);
        }
    }
    coral_required_true(coral_clear_reference(&object->limit_ref));
    object->size = 0;
    object->count = 0;
    object->capacity = 0;
    free(object->data);
    object->data = NULL;
    atomic_fetch_add(&object->id, 1);
    return true;
}

bool coral$array$get_capacity(struct coral$array *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    *out = object->capacity;
    return true;
}

bool coral$array$set_capacity(struct coral$array *object, size_t capacity) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    /* case 0: no change in capacity */
    if (object->capacity == capacity) {
        return true;
    }
    bool result;
    struct coral_range *range;
    coral_required_true(coral_reference_get(object->limit_ref,
                                            (void **) &range));
    coral_required_true(coral_range_is_inclusive(range, capacity, &result));
    if (!result) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    }
    /* case 1: we decrease capacity below count */
    if (object->count > capacity) {
        object->capacity = capacity;
        object->count = capacity;
        atomic_fetch_add(&object->id, 1);
        return true;
    }
    /* case 2: we increase capacity and need to perform a re-alloc */
    size_t size;
    void *data = NULL;
    if (!coral_multiply_size_t(capacity, object->size, &size)
        || (size && !(data = realloc(object->data, size)))) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    object->capacity = capacity;
    object->data = data;
    atomic_fetch_add(&object->id, 1);
    return true;
}

bool coral$array$get_count(struct coral$array *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    *out = object->count;
    return true;
}

bool coral$array$set_count(struct coral$array *object, size_t count) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (count >= object->capacity) {
        size_t capacity;
        struct coral_range *range = NULL;
        coral_required_true(coral_reference_get(
                object->limit_ref, (void **) &range));
        coral_required(range);
        if (!coral_range_get_next(range, count, &capacity)) {
            if (CORAL_ERROR_END_OF_SEQUENCE == coral_error) {
                capacity = count;
            } else {
                if (CORAL_ERROR_INVALID_VALUE != coral_error) {
                    coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
                }
                return false;
            }
        }
        if (!coral$array$set_capacity(object, capacity)) {
            return false;
        }
    }
    if (count > object->count) {
        memset(object->data + (object->count * object->size), 0,
               (count - object->count) * object->size);
    }
    object->count = count;
    atomic_fetch_add(&object->id, 1);
    return true;
}

static bool coral$array$set_size(struct coral$array *object,
                                 const size_t size) {
    coral_required(object);
    if (size <= object->size) {
        return true;
    }
    size_t size_;
    void *data;
    if (!coral_multiply_size_t(object->capacity, size, &size_)
        || !(data = realloc(object->data, size_))) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    object->data = data;
    for (size_t i = object->count; i; i--) {
        unsigned char *src = object->data + (object->size * (i - 1));
        unsigned char *dst = object->data + (size * (i - 1));
        memmove(dst, src, object->size);
    }
    object->size = size;
    return true;
}

bool coral$array$get_size(struct coral$array *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    *out = object->size;
    return true;
}

bool coral$array$get(struct coral$array *object,
                     const size_t at,
                     struct coral$array$item *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (object->count <= at) {
        coral_error = CORAL_ERROR_INDEX_OUT_OF_BOUNDS;
        return false;
    }
    out->size = object->size;
    out->data = object->data + (at * object->size);
    return true;
}

bool coral$array$set(struct coral$array *object,
                     const size_t at,
                     const struct coral$array$item *item) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (item && (!item->size || !item->data)) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    }
    if (object->count <= at) {
        coral_error = CORAL_ERROR_INDEX_OUT_OF_BOUNDS;
        return false;
    }
    if (item && !coral$array$set_size(object, item->size)) {
        return false;
    }
    unsigned char *dst = object->data + (at * object->size);
    if (!item) {
        memset(dst, 0, object->size);
    } else {
        memcpy(dst, item->data, item->size);
    }
    atomic_fetch_add(&object->id, 1);
    return true;
}

bool coral$array$add(struct coral$array *object,
                     const struct coral$array$item *item) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (item && (!item->size || !item->data)) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    }
    size_t count;
    if (!coral_add_size_t(1, object->count, &count)
        || !coral$array$set_count(object, count)
        || (item && !coral$array$set_size(object, item->size))) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    unsigned char *dst = object->data + (object->count - 1) * object->size;
    if (!item) {
        memset(dst, 0, object->size);
    } else {
        memcpy(dst, item->data, item->size);
    }
    atomic_fetch_add(&object->id, 1);
    return true;
}

bool coral$array$remove(struct coral$array *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!object->count) {
        coral_error = CORAL_ERROR_OBJECT_NOT_FOUND;
        return false;
    }
    object->count -= 1;
    atomic_fetch_add(&object->id, 1);
    return true;
}

bool coral$array$insert(struct coral$array *object,
                        const size_t at,
                        const struct coral$array$item *item) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (item && (!item->size || !item->data)) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    }
    if (at >= object->count) {
        coral_error = CORAL_ERROR_INDEX_OUT_OF_BOUNDS;
        return false;
    }
    size_t count;
    if (!coral_add_size_t(1, object->count, &count)
        || !coral$array$set_count(object, count)
        || (item && !coral$array$set_size(object, item->size))) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    count = (object->count - 1) - at;
    if (count) {
        memmove(object->data + ((1 + at) * object->size),
                object->data + (at * object->size),
                count * object->size);
    }
    unsigned char *dst = object->data + (at * object->size);
    if (!item) {
        memset(dst, 0, object->size);
    } else {
        memcpy(dst, item->data, item->size);
    }
    atomic_fetch_add(&object->id, 1);
    return true;
}

bool coral$array$delete(struct coral$array *object,
                        const size_t at) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (at >= object->count) {
        coral_error = CORAL_ERROR_INDEX_OUT_OF_BOUNDS;
        return false;
    }
    const size_t count = (object->count - 1) - at;
    if (count) {
        memmove(object->data + (at * object->size),
                object->data + ((at + 1) * object->size),
                count * object->size);
    }
    object->count -= 1;
    atomic_fetch_add(&object->id, 1);
    return true;
}

static thread_local struct coral$array *$this;

static thread_local int (*$compare)(const void *,
                                    const void *,
                                    const size_t);

static int $compare_func(const void *a, const void *b) {
    return $compare(a, b, $this->size);
}

bool coral$array$sort(struct coral$array *object,
                      struct coral_range_values *values,
                      int (*compare)(const void *,
                                     const void *,
                                     const size_t)) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!compare) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (!object->count) {
        return true;
    }
    size_t maximum = SIZE_MAX;
    size_t minimum = 0;
    if (values) {
        coral_required_true(coral_minimum_size_t(
                values->first, values->last, &minimum));
        coral_required_true(coral_maximum_size_t(
                values->first, values->last, &maximum));
    }
    coral_required_true(coral_minimum_size_t(
            maximum, object->count - 1, &maximum));
    coral_required_true(coral_maximum_size_t(
            minimum, 0, &minimum));
    size_t count;
    coral_required_true(coral_add_size_t(1, maximum - minimum, &count));
    unsigned char *base = object->data + (object->size * minimum);
    $this = object;
    $compare = compare;
    qsort(base, count, object->size, $compare_func);
    atomic_fetch_add(&object->id, 1);
    return true;
}

static void $context_on_destroy(void *ptr) {
    free(ptr);
}

struct coral$array$search_pattern$context {
    int result;
    bool is_initialized;
};

static bool
$linear_search_pattern$step_function(const size_t current,
                                     const bool previous,
                                     const struct coral_range_values values,
                                     void *context,
                                     size_t *out) {
    coral_required(context);
    struct coral$array$search_pattern$context *c =
            *(struct coral$array$search_pattern$context **) context;
    coral_required(c);
    if (previous) {
        coral_error = CORAL_ERROR_END_OF_SEQUENCE;
        return false;
    }
    if (!c->is_initialized) {
        *out = 0;
        return (c->is_initialized = true);
    }
    *out = 1 + current;
    return 0 != c->result;
}

struct coral$array$search_pattern$context_binary {
    struct coral$array$search_pattern$context context;
    size_t left;
    size_t mid;
    size_t right;
};

static bool
$binary_search_pattern$step_function(const size_t current,
                                     const bool previous,
                                     const struct coral_range_values values,
                                     void *context,
                                     size_t *out) {
    coral_required(context);
    struct coral$array$search_pattern$context_binary *c =
            *(struct coral$array$search_pattern$context_binary **) context;
    coral_required(c);
    if (previous) {
        coral_error = CORAL_ERROR_END_OF_SEQUENCE;
        return false;
    }
    if (!c->context.is_initialized) {
        coral_required_true(coral_minimum_size_t(
                values.first, values.last, &c->left));
        coral_required_true(coral_maximum_size_t(
                values.first, values.last, &c->right));
        coral_required_true(coral_minimum_size_t(
                c->right, $this->count - 1, &c->right));
        c->mid = c->left + ((c->right - c->left) >> 1);
        *out = c->mid;
        return (c->context.is_initialized = true);
    }
    coral_required_true(current == c->mid);
    size_t mid = c->mid;
    if (c->context.result > 0) {
        c->left = 1 + c->mid;
    } else if (c->context.result < 0) {
        c->right = c->mid - 1;
    }
    if (c->context.result) {
        c->mid = c->left + ((c->right - c->left) >> 1);
        *out = c->mid;
    }
    return 0 != c->context.result
           && *out != mid;
}

bool coral$array$search_pattern$init_linear(
        struct coral$array$search_pattern *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    struct coral_range_values values = {0, SIZE_MAX};
    struct coral_context *context;
    struct coral$array$search_pattern$context **address = NULL;
    bool result = coral_context_of(&context, $context_on_destroy)
                  && coral_context_get(context, (void **) &address)
                  && (*address = calloc(1, sizeof(**address)))
                  && coral$range$init(&object->range,
                                      values,
                                      $linear_search_pattern$step_function,
                                      context);
    if (address && !(*address)) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
    }
    return result;
}

bool coral$array$search_pattern$init_binary(
        struct coral$array$search_pattern *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    struct coral_range_values values = {0, SIZE_MAX};
    struct coral_context *context;
    struct coral$array$search_pattern$context_binary **address = NULL;
    bool result = coral_context_of(&context, $context_on_destroy)
                  && coral_context_get(context, (void **) &address)
                  && (*address = calloc(1, sizeof(**address)))
                  && coral$range$init(&object->range,
                                      values,
                                      $binary_search_pattern$step_function,
                                      context);
    if (address && !(*address)) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
    }
    return result;
}

bool coral$array$search_pattern$invalidate(
        struct coral$array$search_pattern *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral$range$invalidate(&object->range);
}

bool coral$array$find(struct coral$array *object,
                      const struct coral_range_values *values,
                      struct coral$array$search_pattern *search_pattern,
                      const struct coral$array$item *needle,
                      int (*compare)(const void *,
                                     const void *,
                                     const size_t),
                      size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!search_pattern || !compare || !out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (needle && (!needle->size || !needle->data)) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    }
    if (values) {
        search_pattern->range.values = *values;
    }
    const size_t id = coral$atomic_load(&object->id);
    $this = object; /* pass object into the search pattern's step function */
    while (coral$range$get_next(&search_pattern->range, *out, out)) {
        if (id != coral$atomic_load(&object->id)) {
            coral_error = CORAL_ERROR_OBJECT_UNAVAILABLE;
            return false;
        }
        struct coral$array$item item;
        if (!coral$array$get(object, *out, &item)) {
            break;
        }
        size_t size = item.size;
        void *data = NULL;
        if (needle) {
            coral_required_true(coral_minimum_size_t(
                    item.size, needle->size, &size));
            data = needle->data;
        }
        const int result = compare(data, item.data, size);
        if (!result) {
            return true;
        }
        struct coral$array$search_pattern$context **context;
        coral_required_true(coral_context_get(
                search_pattern->range.context, (void **) &context));
        (*context)->result = result;
    }
    coral_error = CORAL_ERROR_OBJECT_NOT_FOUND;
    return false;
}

struct coral_array {
    struct coral$array array;
};

static void $array_destroy$object_destroy(void **object, const size_t size) {
    void *instance = *object;
    if (!instance) {
        return;
    }
    coral_required_true(coral_object_release(instance));
}

static bool $array_destroy(struct coral_array *this,
                           void *data,
                           void *args) {
    coral_required(this);
    void (*const on_destroy)(void *, const size_t) =
    (void (*)(void *, const size_t)) $array_destroy$object_destroy;
    return coral$array$invalidate(&this->array, on_destroy);
}

static bool $array_is_equal(void *this,
                            struct coral_array *data,
                            struct is_equal_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->other);
    coral_required(args->out);
    struct coral_array *object = data;
    struct coral_array *other = args->other;
    struct {
        size_t object;
        size_t other;
    } id = {
            .object = coral$atomic_load(&object->array.id),
            .other = coral$atomic_load(&other->array.id)
    };
    *args->out = object->array.count == other->array.count;
    for (size_t i = 0; *args->out
                       && coral$atomic_load(&object->array.id) == id.object
                       && coral$atomic_load(&other->array.id) == id.other
                       && object->array.count == other->array.count
                       && i < object->array.count; i++) {
        struct coral$array$item o, p;
        if (!coral$array$get((struct coral$array *) object, i,
                             (struct coral$array$item *) &o)) {
            coral_error = CORAL_ERROR_OBJECT_UNAVAILABLE;
            return false;
        }
        if (!coral$array$get((struct coral$array *) other, i,
                             (struct coral$array$item *) &p)) {
            coral_error = CORAL_ERROR_ARGUMENT_UNAVAILABLE;
            return false;
        }
        void *q = *(void **) o.data, *w = *(void **) p.data;
        struct is_equal_args args_ = {
                .out = args->out,
                .other = w
        };
        if (q != w &&
            (NULL == q
             || NULL == w
             || !coral_object_dispatch(q, true, is_equal, &args_)
             || (CORAL_ERROR_METHOD_NOT_FOUND == coral_error
                 && !coral_object_is_equal(q, w, args->out)))) {
            return false;
        }
    }
    if (coral$atomic_load(&object->array.id) != id.object) {
        coral_error = CORAL_ERROR_OBJECT_UNAVAILABLE;
        return false;
    }
    if (coral$atomic_load(&other->array.id) != id.other) {
        coral_error = CORAL_ERROR_ARGUMENT_UNAVAILABLE;
        return false;
    }
    return true;
}

static bool $array_copy(void *this,
                        struct coral_array *data,
                        struct copy_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->src);
    struct coral$array *object = &data->array;
    struct coral$array *src = &((struct coral_array *) args->src)->array;

    struct coral_range *limit;
    coral_required_true(coral_reference_get(src->limit_ref, (void **) &limit));
    if (!coral$array$init(object, limit, src->count, src->size)) {
        return false;
    }
    for (size_t i = 0; i < src->count; i++) {
        struct coral$array$item o, p;
        coral_required_true(coral$array$get(
                object, i, (struct coral$array$item *) &o));
        coral_required_true(coral$array$get(
                src, i, (struct coral$array$item *) &p));
        void *q = *(void **) o.data;
        if (q && !coral_object_copy(q, p.data)
            || !coral_object_retain(*(void **) p.data)) {
            return false;
        }
    }
    return true;
}

struct $array_get_capacity_args {
    size_t *out;
};

static bool $array_get_capacity(void *this,
                                struct coral_array *data,
                                struct $array_get_capacity_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    struct coral$array *object = &data->array;
    return coral$array$get_capacity(object, args->out);
}

struct $array_set_capacity_args {
    size_t capacity;
};

static bool $array_set_capacity(void *this,
                                struct coral_array *data,
                                struct $array_set_capacity_args *args) {
    coral_required(data);
    coral_required(args);
    struct coral$array *object = &data->array;
    return coral$array$set_capacity(object, args->capacity);
}

struct $array_get_count_args {
    size_t *out;
};

static bool $array_get_count(void *this,
                             struct coral_array *data,
                             struct $array_get_count_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    struct coral$array *object = &data->array;
    return coral$array$get_count(object, args->out);
}

struct $array_set_count_args {
    size_t count;
};

static bool $array_set_count(void *this,
                             struct coral_array *data,
                             struct $array_set_count_args *args) {
    coral_required(data);
    coral_required(args);
    struct coral$array *object = &data->array;
    return coral$array$set_count(object, args->count);
}

struct $array_get_args {
    size_t at;
    void **out;
};

static bool $array_get(void *this,
                       struct coral_array *data,
                       struct $array_get_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    struct coral$array *object = &data->array;
    struct coral$array$item item;
    bool result = coral$array$get(object, args->at, &item);
    if (result) {
        void *instance = *(void **) item.data;
        *args->out = instance;
        if (instance) {
            result = coral_object_autorelease(instance);
        }
    }
    return result;
}

struct $array_set_args {
    size_t at;
    void *instance;
};

static bool $array_set(void *this,
                       struct coral_array *data,
                       struct $array_set_args *args) {
    coral_required(data);
    coral_required(args);
    struct coral$array *object = &data->array;

    struct coral$array$item item;
    if (!coral$array$get(object, args->at, &item)) {
        return false;
    }
    void *i = *(void **) item.data;
    if (i) {
        coral_required_true(coral_object_release(i));
    }
    bool result;
    if (!args->instance) {
        result = coral$array$set(object, args->at, NULL);
    } else {
        item.size = sizeof(&args->instance);
        item.data = &args->instance;
        result = coral_object_retain(args->instance);
        if (result) {
            result = coral$array$set(object, args->at, &item);
            if (!result) {
                coral_required_true(coral_object_release(args->instance));
            }
        }
    }
    return result;
}

struct $array_add_args {
    void *instance;
};

static bool $array_add(void *this,
                       struct coral_array *data,
                       struct $array_add_args *args) {
    coral_required(data);
    coral_required(args);
    struct coral$array *object = &data->array;

    if (!args->instance) {
        return coral$array$add(object, NULL);
    }
    struct coral$array$item item = {
            .data = &args->instance,
            .size = sizeof(&args->instance)
    };
    if (!coral_object_retain(args->instance)) {
        return false;
    }
    bool result = coral$array$add(object, &item);
    if (!result) {
        coral_required_true(coral_object_release(args->instance));
    }
    return result;
}

static bool $array_remove(void *this, struct coral_array *data, void *args) {
    coral_required(data);
    struct coral$array *object = &data->array;

    struct coral$array$item item;
    void *instance;
    if (object->count
        && coral$array$get(object, object->count - 1, &item)
        && (instance = *(void **) item.data)) {
        coral_required_true(coral_object_release(instance));
    }
    return coral$array$remove(object);
}

struct $array_insert_args {
    size_t at;
    void *instance;
};

static bool $array_insert(void *this,
                          struct coral_array *data,
                          struct $array_insert_args *args) {
    coral_required(data);
    coral_required(args);
    struct coral$array *object = &data->array;

    if (!args->instance) {
        return coral$array$insert(object, args->at, NULL);
    }
    if (!coral_object_retain(args->instance)) {
        return false;
    }
    struct coral$array$item item = {
            .data = &args->instance,
            .size = sizeof(&args->instance)
    };
    bool result = coral$array$insert(object, args->at, &item);
    if (!result) {
        coral_required_true(coral_object_release(args->instance));
    }
    return result;
}

struct $array_delete_args {
    size_t at;
};

static bool $array_delete(void *this,
                          struct coral_array *data,
                          struct $array_delete_args *args) {
    coral_required(data);
    coral_required(args);
    struct coral$array *object = &data->array;

    struct coral$array$item item;
    void *instance;
    if (coral$array$get(object, args->at, &item)
        && (instance = *(void **) item.data)) {
        coral_required_true(coral_object_release(instance));
    }
    return coral$array$delete(object, args->at);
}

static thread_local int (*$object_compare)(const void *, const void *);

static int
$array_object_compare(const void *a, const void *b, const size_t size) {
    coral_required(a);
    coral_required(b);
    return $object_compare(*(void **) a, *(void **) b);
}

struct $array_sort_args {
    struct coral_range_values *values;
    int (*compare)(const void *, const void *);
};

static bool $array_sort(void *this,
                        struct coral_array *data,
                        struct $array_sort_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->compare);
    $object_compare = args->compare;
    struct coral$array *object = &data->array;
    return coral$array$sort(object, args->values, $array_object_compare);
}

struct coral_array_search_pattern {
    struct coral$array$search_pattern search_pattern;
};

static bool $array$search_pattern$destroy(
        struct coral_array_search_pattern *this,
        void *data,
        void *args) {
    coral_required(this);
    return coral$array$search_pattern$invalidate(&this->search_pattern);
}

struct $array_find_args {
    const struct coral_range_values *values;
    struct coral_array_search_pattern *search_pattern;
    const void *needle;
    size_t *out;
    int (*compare)(const void *,
                   const void *);
};

static bool $coral_array_find(void *this,
                              struct coral_array *data,
                              struct $array_find_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->search_pattern);
    coral_required(args->compare);
    coral_required(args->out);
    bool result;
    coral_required_true(coral_object_instance_of(
            args->search_pattern, $class$search_pattern, &result));
    if (!result) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    }
    $object_compare = args->compare;
    struct coral$array$item item = {
            .data = &args->needle,
            .size = sizeof(&args->needle)
    };
    return coral$array$find(&data->array,
                            args->values,
                            &args->search_pattern->search_pattern,
                            &item,
                            $array_object_compare,
                            args->out);
}

#pragma mark public -

bool coral_array_class(struct coral_class **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral_object_copy($class, (void **) out);
}

bool coral_array_alloc(struct coral_array **out) {
    return coral_object_alloc(sizeof(**out), (void **) out);
}

bool coral_array_init(struct coral_array *object,
                      struct coral_range *limit,
                      size_t count) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    bool result = false;
    if (coral_object_init(object, $class)) {
        if (coral$array$init(&object->array, limit, count, sizeof(void *))) {
            result = true;
        }
    }
    return result;
}

bool coral_array_destroy(struct coral_array *object) {
    return coral_object_destroy(object);
}

bool coral_array_hash_code(struct coral_array *object, size_t *out) {
    return coral_object_hash_code(object, out);
}

bool coral_array_is_equal(struct coral_array *object,
                          struct coral_array *other,
                          bool *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!other || !out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct is_equal_args args = {
            .other = other,
            .out = out
    };
    return coral_object_instance_of(object, $class, out)
           && *out
           && coral_object_instance_of(other, $class, out)
           && *out
           && coral_object_invoke(
                   object,
                   true,
                   (coral_invokable_t) $array_is_equal,
                   &args);
}

bool coral_array_copy(struct coral_array *object, struct coral_array **out) {
    return coral_object_copy(object, (void **) out);
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

bool coral_array_get_capacity(struct coral_array *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $array_get_capacity_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            true,
            (coral_invokable_t) $array_get_capacity,
            &args);
}

bool coral_array_set_capacity(struct coral_array *object, size_t capacity) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    struct $array_set_capacity_args args = {
            .capacity = capacity
    };
    return coral_object_invoke(
            object,
            false,
            (coral_invokable_t) $array_set_capacity,
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
    struct $array_get_count_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            true,
            (coral_invokable_t) $array_get_count,
            &args);
}

bool coral_array_set_count(struct coral_array *object, size_t count) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    struct $array_set_count_args args = {
            .count = count
    };
    return coral_object_invoke(
            object,
            false,
            (coral_invokable_t) $array_set_count,
            &args);
}

bool coral_array_get(struct coral_array *object, size_t at, void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $array_get_args args = {
            .at = at,
            .out = out
    };
    return coral_object_invoke(
            object,
            true,
            (coral_invokable_t) $array_get,
            &args);
}

bool coral_array_set(struct coral_array *object, size_t at, void *instance) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    struct $array_set_args args = {
            .at = at,
            .instance = instance
    };
    return coral_object_invoke(
            object,
            false,
            (coral_invokable_t) $array_set,
            &args);
}

bool coral_array_add(struct coral_array *object, void *instance) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    struct $array_add_args args = {
            .instance = instance
    };
    return coral_object_invoke(
            object,
            false,
            (coral_invokable_t) $array_add,
            &args);
}

bool coral_array_remove(struct coral_array *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral_object_invoke(
            object,
            false,
            (coral_invokable_t) $array_remove,
            NULL);
}

bool coral_array_insert(struct coral_array *object,
                        size_t at,
                        void *instance) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    struct $array_insert_args args = {
            .at = at,
            .instance = instance
    };
    return coral_object_invoke(
            object,
            false,
            (coral_invokable_t) $array_insert,
            &args);
}

bool coral_array_delete(struct coral_array *object, size_t at) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    struct $array_delete_args args = {
            .at = at
    };
    return coral_object_invoke(
            object,
            false,
            (coral_invokable_t) $array_delete,
            &args);
}

bool coral_array_sort(struct coral_array *object,
                      struct coral_range_values *values,
                      int (*compare)(const void *,
                                     const void *)) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!compare) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $array_sort_args args = {
            .values = values,
            .compare = compare
    };
    return coral_object_invoke(
            object,
            false,
            (coral_invokable_t) $array_sort,
            &args);
}

bool coral_array_search_pattern_class(struct coral_class **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral_object_copy($class, (void **) out);
}

bool coral_array_search_pattern_of_linear(
        struct coral_array_search_pattern **out) {
    if (coral_object_alloc(sizeof(**out), (void **) out)) {
        struct coral$array$search_pattern *object = &(*out)->search_pattern;
        if (coral_object_init(*out, $class$search_pattern)
            && coral$array$search_pattern$init_linear(object)) {
            return true;
        }
        coral_required_true(coral_array_search_pattern_destroy(*out));
    }
    return false;
}

bool coral_array_search_pattern_of_binary(
        struct coral_array_search_pattern **out) {
    if (coral_object_alloc(sizeof(**out), (void **) out)) {
        struct coral$array$search_pattern *object = &(*out)->search_pattern;
        if (coral_object_init(*out, $class$search_pattern)
            && coral$array$search_pattern$init_binary(object)) {
            return true;
        }
        coral_required_true(coral_array_search_pattern_destroy(*out));
    }
    return false;
}

bool coral_array_search_pattern_destroy(
        struct coral_array_search_pattern *object) {
    return coral_object_destroy(object);
}

bool coral_array_find(struct coral_array *object,
                      const struct coral_range_values *values,
                      struct coral_array_search_pattern *search_pattern,
                      const void *needle,
                      int (*compare)(const void *,
                                     const void *),
                      size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!search_pattern || !compare || !out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $array_find_args args = {
            .values = values,
            .search_pattern = search_pattern,
            .needle = needle,
            .compare = compare,
            .out = out
    };
    return coral_object_invoke(
            object,
            false,
            (coral_invokable_t) $coral_array_find,
            &args);
}
