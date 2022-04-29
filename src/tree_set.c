#include <stdlib.h>
#include <string.h>
#include <coral.h>

#include "private/tree_set.h"
#include "private/coral.h"
#include "test/cmocka.h"

#pragma mark private

bool coral$tree_set$alloc(struct coral$tree_set **out) {
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

bool coral$tree_set$init(struct coral$tree_set *object,
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

static void coral$tree_set$delete_item(struct coral$tree_set *object,
                                       void *item) {
    coral_required_true(coral$red_black_tree$delete(
            &object->tree, item));
    atomic_fetch_add(&object->id, 1);
    object->count -= 1;
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
    void *node;
    if (!coral$red_black_tree$search(&object->tree, NULL, item,
                                     &node)) {
        return false;
    }
    coral$tree_set$delete_item(object, node);
    return true;
}

bool coral$tree_set$contains(struct coral$tree_set *object, const void *item,
                             bool *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!item | !out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    void *node;
    *out = coral$red_black_tree$search(&object->tree, NULL, item,
                                       &node);
    return true;
}

bool coral$tree_set$first(struct coral$tree_set *object, void **out) {
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

bool coral$tree_set$last(struct coral$tree_set *object, void **out) {
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

bool coral$tree_set$next(struct coral$tree_set *object, const void *value,
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

bool coral$tree_set$prev(struct coral$tree_set *object, const void *value,
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

bool coral$tree_set$iterator(struct coral$tree_set *object,
                             struct coral$tree_set$iterator **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct coral$tree_set$iterator *it = calloc(1, sizeof(*it));
    if (!it) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    it->id = coral$atomic_load(&object->id);
    it->tree_set = object;
    *out = it;
    return coral$red_black_tree$get_first(&object->tree, &it->data)
           || CORAL_ERROR_OBJECT_NOT_FOUND == coral_error;
}

static bool coral$tree_set$iterator_check_if_container_modified(
        struct coral$tree_set$iterator *object) {
    coral_required(object);
    const size_t id = coral$atomic_load(&object->tree_set->id);
    return id != object->id;
}

bool coral$tree_set$iterator_next(struct coral$tree_set$iterator *object,
                                  void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (!object->data) {
        coral_error = CORAL_ERROR_END_OF_SEQUENCE;
        return false;
    }
    if (coral$tree_set$iterator_check_if_container_modified(object)) {
        coral_error = CORAL_ERROR_OBJECT_UNAVAILABLE;
        return false;
    }
    memcpy(out, object->data, object->tree_set->size);
    bool result = coral$red_black_tree$get_next(object->data, &object->data);
    if (!result) {
        result = NULL != object->data;
        object->data = NULL;
    }
    return result;
}

bool coral$tree_set$iterator_delete(struct coral$tree_set$iterator *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!object->data) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    }
    if (coral$tree_set$iterator_check_if_container_modified(object)) {
        coral_error = CORAL_ERROR_OBJECT_UNAVAILABLE;
        return false;
    }
    void *node = object->data;
    coral$red_black_tree$get_next(object->data, &object->data);
    coral$tree_set$delete_item(object->tree_set, node);
    if (node == object->data) {
        object->data = NULL;
    }
    object->id += 1;
    return true;
}
