#include <stdlib.h>
#include <string.h>
#include <coral.h>

#include "private/tree_set.h"
#include "private/coral.h"
#include "private/class.h"
#include "test/cmocka.h"

#pragma mark private -

static const struct coral_range_values $limit = {.first = 0, .last = SIZE_MAX};
static struct coral_class *$class;

#pragma mark + invokables

static bool $tree_set_destroy(struct coral_tree_set *this,
                              void *data,
                              void *args);

static bool $tree_set_is_equal(void *this,
                               struct coral_tree_set *data,
                               struct is_equal_args *args);

static bool $tree_set_copy(void *this,
                           struct coral_tree_set *data,
                           struct copy_args *args);

__attribute__((constructor(CORAL_CLASS_LOAD_PRIORITY_TREE_SET)))
static void $on_load() {
    struct coral_class_method_name $method_names[] = {
            {destroy,  strlen(destroy)},
            {is_equal, strlen(is_equal)},
            {copy,     strlen(copy)},
    };
    coral_required_true(coral_class_alloc(&$class));
    coral_required_true(coral_class_init($class));
    coral_required_true(coral_class_retain($class));
    /* destroy */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[0],
            (coral_invokable_t) $tree_set_destroy));
    /* is_equal */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[1],
            (coral_invokable_t) $tree_set_is_equal));
    /* copy */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[2],
            (coral_invokable_t) $tree_set_copy));

    coral_autorelease_pool_drain();
}

__attribute__((destructor(CORAL_CLASS_LOAD_PRIORITY_TREE_SET)))
static void $on_unload() {
    coral_required_true(coral_class_destroy($class));

    coral_autorelease_pool_drain();
}

bool coral$tree_set$init(struct coral$tree_set *object,
                         struct coral_range_values *limit,
                         const size_t size,
                         int (*compare)(const void *first,
                                        const void *second)) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!size || !compare) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    object->limit = limit ? *limit : $limit;
    object->size = size;
    return coral$red_black_tree$init(&object->tree, compare);
}

bool coral$tree_set$invalidate(struct coral$tree_set *object,
                               void (*on_destroy)(void *)) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    coral_required_true(coral$red_black_tree$invalidate(
            &object->tree, on_destroy));
    object->count = 0;
    return true;
}

bool coral$tree_set$get_count(struct coral$tree_set *object, size_t *out) {
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

bool coral$tree_set$get_size(struct coral$tree_set *object, size_t *out) {
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

bool coral$tree_set$insert(struct coral$tree_set *object, const void *item) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!item) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    size_t maximum;
    coral_required_true(coral_maximum_size_t(
            object->limit.first,
            object->limit.last,
            &maximum));
    if (object->count >= maximum) {
        coral_error = CORAL_ERROR_OBJECT_UNAVAILABLE;
        return false;
    }
    void *insertion_point;
    if (coral$red_black_tree$search(&object->tree, NULL, item,
                                    &insertion_point)) {
        coral_error = CORAL_ERROR_OBJECT_ALREADY_EXISTS;
        return false;
    }
    void *node;
    if (!coral$red_black_tree$node_of(object->size, &node)) {
        return false;
    }
    memcpy(node, item, object->size);
    const bool result = coral$red_black_tree$insert(
            &object->tree, insertion_point, node);
    if (!result) {
        coral_required_true(coral$red_black_tree$node_destroy(node));
    } else {
        atomic_fetch_add(&object->id, 1);
        object->count += 1;
    }
    return result;
}

bool coral$tree_set$delete(struct coral$tree_set *object, const void *item) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!item) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (!object->count) {
        coral_error = CORAL_ERROR_OBJECT_NOT_FOUND;
        return false;
    }
    size_t minimum;
    coral_required_true(coral_minimum_size_t(
            object->limit.first,
            object->limit.last,
            &minimum));
    if (object->count <= minimum) {
        coral_error = CORAL_ERROR_OBJECT_UNAVAILABLE;
        return false;
    }
    void *node;
    if (!coral$red_black_tree$search(&object->tree, NULL, item,
                                     &node)) {
        return false;
    }
    coral_required_true(coral$red_black_tree$delete(
            &object->tree, node));
    atomic_fetch_add(&object->id, 1);
    object->count -= 1;
    return true;
}

bool coral$tree_set$contains(struct coral$tree_set *object, const void *item,
                             bool *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!item || !out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    void *node;
    *out = coral$red_black_tree$search(&object->tree, NULL, item,
                                       &node);
    return true;
}

bool coral$tree_set$get_first(struct coral$tree_set *object, void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    void *node;
    const bool result = coral$red_black_tree$get_first(
            &object->tree, &node);
    if (result) {
        memcpy(out, node, object->size);
    }
    return result;
}

bool coral$tree_set$get_last(struct coral$tree_set *object, void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    void *node;
    const bool result = coral$red_black_tree$get_last(
            &object->tree, &node);
    if (result) {
        memcpy(out, node, object->size);
    }
    return result;
}

bool coral$tree_set$get_next(struct coral$tree_set *object, const void *value,
                             void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!value || !out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    void *node;
    bool result = coral$red_black_tree$search(&object->tree, NULL, value, &node)
                  && coral$red_black_tree$get_next(node, &node);
    if (result) {
        memcpy(out, node, object->size);
    }
    return result;
}

bool coral$tree_set$get_prev(struct coral$tree_set *object, const void *value,
                             void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!value || !out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    void *node;
    bool result = coral$red_black_tree$search(&object->tree, NULL, value, &node)
                  && coral$red_black_tree$get_prev(node, &node);
    if (result) {
        memcpy(out, node, object->size);
    }
    return result;
}

static thread_local int (*$object_compare)(const void *, const void *);

static int $tree_set_object_compare(const void *a, const void *b) {
    coral_required(a);
    coral_required(b);
    return $object_compare(*(void **) a, *(void **) b);
}

struct coral_tree_set {
    struct coral$tree_set tree_set;
    int (*compare)(const void *, const void *);
};

static void $tree_set_destroy$object_destroy(void **object) {
    void *instance = *object;
    if (!instance) {
        return;
    }
    coral_required_true(coral_object_release(instance));
}

static bool
$tree_set_destroy(struct coral_tree_set *this, void *data, void *args) {
    coral_required(this);
    void (*const on_destroy)(void *) =
        (void (*)(void *)) $tree_set_destroy$object_destroy;
    return coral$tree_set$invalidate(&this->tree_set, on_destroy);
}

static bool $tree_set_is_equal(void *this,
                               struct coral_tree_set *data,
                               struct is_equal_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->other);
    coral_required(args->out);
    struct coral_tree_set *object = data;
    struct coral_tree_set *other = args->other;
    struct {
        size_t object;
        size_t other;
    } id = {
            .object = coral$atomic_load(&object->tree_set.id),
            .other = coral$atomic_load(&other->tree_set.id)
    };
    void *q;
    if ((!coral_tree_set_get_first(object, &q)
         && CORAL_ERROR_OBJECT_NOT_FOUND != coral_error)
        || !coral_tree_set_contains(other, q, args->out)) {
        return false;
    }
    *args->out = object->tree_set.count == other->tree_set.count;
    for (size_t i = 0; *args->out
                       && coral$atomic_load(&object->tree_set.id) == id.object
                       && coral$atomic_load(&other->tree_set.id) == id.other
                       && object->tree_set.count == other->tree_set.count
                       && i < object->tree_set.count; i++) {
        if ((!coral_tree_set_get_next(object, q, &q)
             && CORAL_ERROR_END_OF_SEQUENCE != coral_error)
            || !coral_tree_set_contains(other, q, args->out)) {
            return false;
        }
    }
    if (coral$atomic_load(&object->tree_set.id) != id.object) {
        coral_error = CORAL_ERROR_OBJECT_UNAVAILABLE;
        return false;
    }
    if (coral$atomic_load(&other->tree_set.id) != id.other) {
        coral_error = CORAL_ERROR_ARGUMENT_UNAVAILABLE;
        return false;
    }
    return true;
}

static bool $tree_set_copy(void *this,
                           struct coral_tree_set *data,
                           struct copy_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->src);
    struct coral$tree_set *object = &data->tree_set;
    struct coral$tree_set *src = &((struct coral_tree_set *) args->src)->tree_set;

    if (!coral$tree_set$init(object, &src->limit, src->size,
                             src->tree.compare)) {
        return false;
    }
    data->compare = ((struct coral_tree_set *) args->src)->compare;
    if (!src->count) {
        return true;
    }
    void *i;
    if (!coral$tree_set$get_first(src, &i)
        && CORAL_ERROR_OBJECT_NOT_FOUND != coral_error) {
        return false;
    }
    do {
        void *o;
        if (!coral_object_copy(i, &o)
            || !coral_tree_set_insert(data, o)) {
            return false;
        }
    } while (coral$tree_set$get_next(src, &i, &i));
    return true;
}

struct $tree_set_get_count_args {
    size_t *out;
};

static bool $tree_set_get_count(void *this,
                                struct coral_tree_set *data,
                                struct $tree_set_get_count_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    return coral$tree_set$get_count(&data->tree_set, args->out);
}

struct $tree_set_insert_args {
    void *instance;
};

static bool $tree_set_insert(void *this,
                             struct coral_tree_set *data,
                             struct $tree_set_insert_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->instance);
    bool result = false;
    if (coral_object_retain(args->instance)) {
        $object_compare = data->compare;
        if (coral$tree_set$insert(&data->tree_set, &args->instance)) {
            result = true;
        } else {
            coral_required_true(coral_object_release(args->instance));
        }
    }
    return result;
}

struct $tree_set_delete_args {
    void *instance;
};

static bool $tree_set_delete(void *this,
                             struct coral_tree_set *data,
                             struct $tree_set_delete_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->instance);
    $object_compare = data->compare;
    const bool result = coral$tree_set$delete(&data->tree_set, &args->instance);
    if (result) {
        coral_required_true(coral_object_release(args->instance));
    }
    return result;
}

struct $tree_set_contains_args {
    void *instance;
    bool *out;
};

static bool $tree_set_contains(void *this,
                               struct coral_tree_set *data,
                               struct $tree_set_contains_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    coral_required(args->instance);
    $object_compare = data->compare;
    return coral$tree_set$contains(&data->tree_set, &args->instance,
                                   args->out);
}

struct $tree_set_get_first_args {
    void **out;
};

static bool $tree_set_get_first(void *this,
                                struct coral_tree_set *data,
                                struct $tree_set_get_first_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    const bool result = coral$tree_set$get_first(&data->tree_set, args->out);
    if (result) {
        coral_required_true(coral_object_autorelease(*args->out));
    }
    return result;
}

struct $tree_set_get_last_args {
    void **out;
};

static bool $tree_set_get_last(void *this,
                               struct coral_tree_set *data,
                               struct $tree_set_get_last_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    const bool result = coral$tree_set$get_last(&data->tree_set, args->out);
    if (result) {
        coral_required_true(coral_object_autorelease(*args->out));
    }
    return result;
}

struct $tree_set_get_next_args {
    void *instance;
    void **out;
};

static bool $tree_set_get_next(void *this,
                               struct coral_tree_set *data,
                               struct $tree_set_get_next_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    coral_required(args->instance);
    $object_compare = data->compare;
    const bool result = coral$tree_set$get_next(&data->tree_set,
                                                &args->instance,
                                                args->out);
    if (result) {
        coral_required_true(coral_object_autorelease(*args->out));
    }
    return result;
}

struct $tree_set_get_prev_args {
    void *instance;
    void **out;
};

static bool $tree_set_get_prev(void *this,
                               struct coral_tree_set *data,
                               struct $tree_set_get_prev_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    coral_required(args->instance);
    $object_compare = data->compare;
    const bool result = coral$tree_set$get_prev(&data->tree_set,
                                                &args->instance,
                                                args->out);
    if (result) {
        coral_required_true(coral_object_autorelease(*args->out));
    }
    return result;
}

#pragma mark public -

bool coral_tree_set_class(struct coral_class **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral_object_copy($class, (void **) out);
}

bool coral_tree_set_alloc(struct coral_tree_set **out) {
    return coral_object_alloc(sizeof(**out), (void **) out);
}

bool coral_tree_set_init(struct coral_tree_set *object,
                         struct coral_range_values *limit,
                         int (*compare)(const void *first,
                                        const void *second)) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!compare) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    bool result = false;
    if (coral_object_init(object, $class)) {
        if (coral$tree_set$init(&object->tree_set, limit, sizeof(void *),
                                $tree_set_object_compare)) {
            object->compare = compare;
            result = true;
        }
    }
    return result;
}

bool coral_tree_set_destroy(struct coral_tree_set *object) {
    return coral_object_destroy(object);
}

bool coral_tree_set_hash_code(struct coral_tree_set *object, size_t *out) {
    return coral_object_hash_code(object, out);
}

bool coral_tree_set_is_equal(struct coral_tree_set *object,
                             struct coral_tree_set *other,
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
           && coral_object_dispatch(object, is_equal, &args);
}

bool coral_tree_set_copy(struct coral_tree_set *object,
                         struct coral_tree_set **out) {
    return coral_object_copy(object, (void **) out);
}

bool coral_tree_set_retain(struct coral_tree_set *object) {
    return coral_object_retain(object);
}

bool coral_tree_set_release(struct coral_tree_set *object) {
    return coral_object_release(object);
}

bool coral_tree_set_autorelease(struct coral_tree_set *object) {
    return coral_object_autorelease(object);
}

bool coral_tree_set_get_count(struct coral_tree_set *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $tree_set_get_count_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $tree_set_get_count,
            &args);
}

bool coral_tree_set_insert(struct coral_tree_set *object, void *instance) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!instance) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $tree_set_insert_args args = {
            .instance = instance
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $tree_set_insert,
            &args);
}

bool coral_tree_set_delete(struct coral_tree_set *object, void *instance) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!instance) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $tree_set_delete_args args = {
            .instance = instance
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $tree_set_delete,
            &args);
}

bool coral_tree_set_contains(struct coral_tree_set *object, void *instance,
                             bool *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out || !instance) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $tree_set_contains_args args = {
            .out = out,
            .instance = instance
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $tree_set_contains,
            &args);
}

bool coral_tree_set_get_first(struct coral_tree_set *object, void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $tree_set_get_first_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $tree_set_get_first,
            &args);
}

bool coral_tree_set_get_last(struct coral_tree_set *object, void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $tree_set_get_last_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $tree_set_get_last,
            &args);
}

bool coral_tree_set_get_next(struct coral_tree_set *object, void *instance,
                             void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out || !instance) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $tree_set_get_next_args args = {
            .instance = instance,
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $tree_set_get_next,
            &args);
}

bool coral_tree_set_get_prev(struct coral_tree_set *object, void *instance,
                             void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out || !instance) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $tree_set_get_prev_args args = {
            .instance = instance,
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $tree_set_get_prev,
            &args);
}
