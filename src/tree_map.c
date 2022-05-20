#include <stdlib.h>
#include <coral.h>
#include <string.h>

#include "private/tree_map.h"
#include "private/coral.h"
#include "test/cmocka.h"

#pragma mark private

bool coral$tree_map$alloc(struct coral$tree_map **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    *out = calloc(1, sizeof(**out));
    if (!*out) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    return true;
}

bool coral$tree_map$init(struct coral$tree_map *object,
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
    return coral$tree_set$init(&object->tree_set, size, compare);
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

bool coral$tree_map$first(struct coral$tree_map *object, void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral$tree_set$first(&object->tree_set, out);
}

bool coral$tree_map$last(struct coral$tree_map *object, void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral$tree_set$last(&object->tree_set, out);
}

bool coral$tree_map$next(struct coral$tree_map *object, void *entry,
                         void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral$tree_set$next(&object->tree_set, entry, out);
}

bool coral$tree_map$prev(struct coral$tree_map *object, void *entry,
                         void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral$tree_set$prev(&object->tree_set, entry, out);
}

