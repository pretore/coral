#include <stdlib.h>
#include <coral.h>
#include <string.h>

#include "private/tree_map.h"
#include "private/coral.h"
#include "private/class.h"
#include "test/cmocka.h"

#pragma mark private -

static struct coral_class *$class;

#pragma mark + invokables

static bool $tree_map_destroy(struct coral_tree_map *this,
                              void *data,
                              void *args);

static bool $tree_map_is_equal(void *this,
                               struct coral_tree_map *data,
                               struct is_equal_args *args);

static bool $tree_map_copy(void *this,
                           struct coral_tree_map *data,
                           struct copy_args *args);

__attribute__((constructor(CORAL_CLASS_LOAD_PRIORITY_TREE_MAP)))
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
            (coral_invokable_t) $tree_map_destroy));
    /* is_equal */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[1],
            (coral_invokable_t) $tree_map_is_equal));
//    /* copy */
//    coral_required_true(coral_class_method_add(
//            $class, &$method_names[2],
//            (coral_invokable_t) $tree_set_copy));

    coral_autorelease_pool_drain();
}

__attribute__((destructor(CORAL_CLASS_LOAD_PRIORITY_TREE_MAP)))
static void $on_unload() {
    coral_required_true(coral_class_destroy($class));

    coral_autorelease_pool_drain();
}

bool coral$tree_map$init(struct coral$tree_map *object,
                         struct coral_range_values *limit,
                         const size_t key_size,
                         const size_t value_size,
                         int (*compare)(const void *first,
                                        const void *second)) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!key_size || !value_size || !compare) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    size_t size;
    if (!coral_add_size_t(key_size, value_size, &size)) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    object->key_size = key_size;
    object->value_size = value_size;
    return coral$tree_set$init(&object->tree_set, limit, size, compare);
}

bool coral$tree_map$invalidate(struct coral$tree_map *object,
                               void (*on_destroy)(void *)) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    coral_required_true(coral$tree_set$invalidate(
            &object->tree_set, on_destroy));
    object->key_size = 0;
    object->value_size = 0;
    return true;
}

bool coral$tree_map$get_count(struct coral$tree_map *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral$tree_set$get_count(&object->tree_set, out);
}

bool coral$tree_map$get_size(struct coral$tree_map *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral$tree_set$get_size(&object->tree_set, out);
}

bool coral$tree_map$get_key_size(struct coral$tree_map *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    *out = object->key_size;
    return true;
}

bool coral$tree_map$get_value_size(struct coral$tree_map *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    *out = object->value_size;
    return true;
}

bool coral$tree_map$insert(struct coral$tree_map *object, const void *entry) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral$tree_set$insert(&object->tree_set, entry);
}

bool coral$tree_map$delete(struct coral$tree_map *object, const void *key) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral$tree_set$delete(&object->tree_set, key);
}

bool coral$tree_map$contains(struct coral$tree_map *object, const void *key,
                             bool *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral$tree_set$contains(&object->tree_set, key, out);
}

bool coral$tree_map$get(struct coral$tree_map *object, const void *key,
                        void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!key || !out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    void *node;
    const bool result = coral$red_black_tree$search(
            &object->tree_set.tree, NULL, key, &node);
    if (result) {
        memcpy(out, node, object->tree_set.size);
    }
    return result;
}

bool coral$tree_map$set(struct coral$tree_map *object, const void *entry) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!entry) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    void *node;
    const bool result = coral$red_black_tree$search(
            &object->tree_set.tree, NULL, entry, &node);
    if (result) {
        memcpy(node, entry, object->tree_set.size);
    }
    return result;
}

bool coral$tree_map$get_first(struct coral$tree_map *object, void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral$tree_set$get_first(&object->tree_set, out);
}

bool coral$tree_map$get_last(struct coral$tree_map *object, void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral$tree_set$get_last(&object->tree_set, out);
}

bool coral$tree_map$get_next(struct coral$tree_map *object, void *entry,
                             void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral$tree_set$get_next(&object->tree_set, entry, out);
}

bool coral$tree_map$get_prev(struct coral$tree_map *object, void *entry,
                             void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral$tree_set$get_prev(&object->tree_set, entry, out);
}

static thread_local int (*$object_compare)(const void *, const void *);

static int $tree_map_object_compare(const void *a, const void *b) {
    coral_required(a);
    coral_required(b);
    const struct coral_tree_map_entry *A = a;
    const struct coral_tree_map_entry *B = b;
    return $object_compare(A->key, B->key);
}

struct coral_tree_map {
    struct coral$tree_map tree_map;

    int (*compare)(const void *, const void *);
};

static void
$tree_map_destroy$object_destroy(struct coral_tree_map_entry *entry) {
    if (!entry) {
        return;
    }
    coral_required_true(coral_object_release(entry->key));
    if (!entry->value) {
        return;
    }
    coral_required_true(coral_object_release(entry->value));
}

static bool
$tree_map_destroy(struct coral_tree_map *this, void *data, void *args) {
    coral_required(this);
    void (*const on_destroy)(void *) =
    (void (*)(void *)) $tree_map_destroy$object_destroy;
    return coral$tree_map$invalidate(&this->tree_map, on_destroy);
}

static bool $tree_map_is_equal(void *this,
                               struct coral_tree_map *data,
                               struct is_equal_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->other);
    coral_required(args->out);
    struct coral_tree_map *object = data;
    struct coral_tree_map *other = args->other;

    return false;
}

struct $tree_map_get_count_args {
    size_t *out;
};

static bool $tree_map_get_count(void *this,
                                struct coral_tree_map *data,
                                struct $tree_map_get_count_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    return coral$tree_map$get_count(&data->tree_map, args->out);
}

struct $tree_map_insert_args {
    const struct coral_tree_map_entry *entry;
};

static bool $tree_map_insert(void *this,
                             struct coral_tree_map *data,
                             struct $tree_map_insert_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->entry);
    coral_required(args->entry->key);
    const struct coral_tree_map_entry *e = args->entry;
    if (coral_object_retain(e->key)) { // FIXME: must copy key ...
        if (!e->value || (e->value && coral_object_retain(e->value))) {
            $object_compare = data->compare;
            if (coral$tree_map$insert(&data->tree_map, e)) {
                return true;
            } else if (e->value) {
                coral_required_true(coral_object_release(e->value));
            }
        }
        coral_required_true(coral_object_release(e->key));
    }
    return false;
}

struct $tree_map_delete_args {
    const void *key;
};

static bool $tree_map_delete(void *this,
                             struct coral_tree_map *data,
                             struct $tree_map_delete_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->key);
    struct coral_tree_map_entry e;
    $object_compare = data->compare;
    if (coral$tree_map$get(&data->tree_map, &args->key, (void **) &e)
        && coral$tree_map$delete(&data->tree_map, &e.key)) {
        coral_required_true(coral_object_release(e.key));
        if (e.value) {
            coral_required_true(coral_object_release(e.value));
        }
        return true;
    }
    return false;
}

struct $tree_map_contains_args {
    const void *key;
    bool *out;
};

static bool $tree_map_contains(void *this,
                               struct coral_tree_map *data,
                               struct $tree_map_contains_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->key);
    coral_required(args->out);
    $object_compare = data->compare;
    return coral$tree_map$contains(&data->tree_map, &args->key, args->out);
}

struct $tree_map_get_args {
    const void *key;
    void **out;
};

static bool $tree_map_get(void *this,
                          struct coral_tree_map *data,
                          struct $tree_map_get_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->key);
    coral_required(args->out);
    $object_compare = data->compare;
    struct coral_tree_map_entry e;
    bool result = coral$tree_map$get(&data->tree_map, &args->key, (void **) &e);
    if (result) {
        if (e.value) {
            coral_required_true(coral_object_autorelease(e.value));
        }
        *args->out = e.value;
    }
    return result;
}

struct $tree_map_set_args {
    struct coral_tree_map_entry *entry;
};

static bool $tree_map_set(void *this,
                          struct coral_tree_map *data,
                          struct $tree_map_set_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->entry);
    coral_required(args->entry->key);
    $object_compare = data->compare;
    struct coral_tree_map_entry *e;
    if (!coral$tree_map$get(&data->tree_map, &args->entry->key, (void **) &e)) {
        return false;
    }
    if (args->entry->value) {
        coral_required_true(coral_object_retain(args->entry->value));
    }
    if (e->value) {
        coral_required_true(coral_object_release(e->value));
    }
    e->value = args->entry->value;
    bool result = coral$tree_map$set(&data->tree_map, e);
    if (!result && e->value) {
        coral_required_true(coral_object_release(e->value));
    }
    return result;
}

#pragma mark public -

bool coral_tree_map_class(struct coral_class **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral_object_copy($class, (void **) out);
}

bool coral_tree_map_alloc(struct coral_tree_map **out) {
    return coral_object_alloc(sizeof(**out), (void **) out);
}

bool coral_tree_map_init(struct coral_tree_map *object,
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
        if (coral$tree_map$init(
                &object->tree_map,
                limit,
                sizeof(((struct coral_tree_map_entry *) 0)->key),
                sizeof(((struct coral_tree_map_entry *) 0)->value),
                $tree_map_object_compare)) {
            object->compare = compare;
            result = true;
        }
    }
    return result;
}

bool coral_tree_map_destroy(struct coral_tree_map *object) {
    return coral_object_destroy(object);
}

bool coral_tree_map_hash_code(struct coral_tree_map *object, size_t *out) {
    return coral_object_hash_code(object, out);
}

bool coral_tree_map_is_equal(struct coral_tree_map *object,
                             struct coral_tree_map *other,
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

bool coral_tree_map_copy(struct coral_tree_map *object,
                         struct coral_tree_map **out) {
    return coral_object_copy(object, (void **) out);
}

bool coral_tree_map_retain(struct coral_tree_map *object) {
    return coral_object_retain(object);
}

bool coral_tree_map_release(struct coral_tree_map *object) {
    return coral_object_release(object);
}

bool coral_tree_map_autorelease(struct coral_tree_map *object) {
    return coral_object_autorelease(object);
}

bool coral_tree_map_get_count(struct coral_tree_map *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $tree_map_get_count_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $tree_map_get_count,
            &args);
}

bool coral_tree_map_insert(struct coral_tree_map *object,
                           struct coral_tree_map_entry *entry) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!entry) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (!entry->key) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    }
    struct $tree_map_insert_args args = {
            .entry = entry
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $tree_map_insert,
            &args);
}

bool coral_tree_map_delete(struct coral_tree_map *object, const void *key) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!key) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $tree_map_delete_args args = {
            .key = key
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $tree_map_delete,
            &args);
}

bool coral_tree_map_contains(struct coral_tree_map *object, const void *key,
                             bool *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!key || !out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $tree_map_contains_args args = {
            .key = key,
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $tree_map_contains,
            &args);
}

bool coral_tree_map_get(struct coral_tree_map *object, const void *key,
        void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!key || !out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $tree_map_get_args args = {
            .key = key,
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $tree_map_get,
            &args);
}

bool coral_tree_map_set(struct coral_tree_map *object,
        struct coral_tree_map_entry *entry) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!entry) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (!entry->key) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    }
    struct $tree_map_set_args args = {
        .entry = entry
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $tree_map_set,
            &args);
}
