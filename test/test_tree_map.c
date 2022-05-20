#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <coral.h>
#include <string.h>

#include "private/tree_map.h"
#include "private/coral.h"
#include "test/cmocka.h"

static void check_alloc_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$alloc(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_alloc(void **state) {
    struct coral$tree_map *object = NULL;
    assert_true(coral$tree_map$alloc(&object));
    assert_non_null(object);
    free(object);
}

static void check_invalidate_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$invalidate(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$init(
            NULL, 1, 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_init_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$init(
            (void *) 1, 0, 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    assert_false(coral$tree_map$init(
            (void *) 1, 1, 0, (void *) 1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    assert_false(coral$tree_map$init(
            (void *) 1, 1, 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    const size_t size = sizeof(size_t);
    struct coral$tree_map *object = NULL;
    assert_true(coral$tree_map$alloc(&object));
    assert_non_null(object);
    assert_true(coral$tree_map$init(
            object, size, size, (void *) 1));
    assert_int_equal(size, object->key_size);
    assert_int_equal(size, object->value_size);
    assert_true(coral$tree_map$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_count_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$get_count(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_count_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$get_count((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_count(void **state) {
    struct coral$tree_map object = {};
    size_t count = ~0;
    assert_true(coral$tree_map$get_count(&object, &count));
    assert_int_equal(0, count);
    object.tree_set.count = 165;
    assert_true(coral$tree_map$get_count(&object, &count));
    assert_int_equal(165, count);
}

static void check_get_size_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$get_size(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_size_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$get_size((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_size(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_map object = {};
    size_t size = ~0;
    assert_true(coral$tree_map$get_size(&object, &size));
    assert_int_equal(0, size);
    object.tree_set.size = 252;
    assert_true(coral$tree_map$get_size(&object, &size));
    assert_int_equal(252, size);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_key_size_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$get_key_size(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_key_size_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$get_key_size((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_key_size(void **state) {
    struct coral$tree_map object = {};
    size_t size = ~0;
    assert_true(coral$tree_map$get_key_size(&object, &size));
    assert_int_equal(0, size);
    object.key_size = 125;
    assert_true(coral$tree_map$get_key_size(&object, &size));
    assert_int_equal(125, size);
}

static void check_get_value_size_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$get_value_size(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_value_size_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$get_value_size((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_value_size(void **state) {
    struct coral$tree_map object = {};
    size_t size = ~0;
    assert_true(coral$tree_map$get_value_size(&object, &size));
    assert_int_equal(0, size);
    object.value_size = 19825;
    assert_true(coral$tree_map$get_value_size(&object, &size));
    assert_int_equal(19825, size);
}

static void check_insert_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$insert(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$insert((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_map *object = NULL;
    assert_true(coral$tree_map$alloc(&object));
    assert_true(coral$tree_map$init(
            object, sizeof(size_t), sizeof(size_t), coral_compare_size_t));
    void *d = object->tree_set.tree.root;
    assert_null(d);
    struct entry {
        size_t key; /* key MUST always be the first value */
        size_t value;
    } e = {10, 726};
    assert_true(coral$tree_map$insert(object, &e));
    d = object->tree_set.tree.root;
    assert_non_null(d);
    assert_ptr_not_equal(d, &e);
    assert_memory_equal(d, &e, sizeof(struct entry));
    assert_true(coral$tree_map$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_large_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_map *object = NULL;
    assert_true(coral$tree_map$alloc(&object));
    assert_true(coral$tree_map$init(
            object, sizeof(size_t), 32, coral_compare_size_t));
    void *d = object->tree_set.tree.root;
    assert_null(d);
    struct entry {
        size_t key; /* key MUST always be the first value */
        char value[32];
    } e;
    e.key = 10;
    memcpy(e.value, check_insert_large_value, 32);
    assert_true(coral$tree_map$insert(object, &e));
    d = object->tree_set.tree.root;
    assert_non_null(d);
    assert_ptr_not_equal(d, &e);
    assert_memory_equal(d, &e, sizeof(struct entry));
    assert_true(coral$tree_map$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_error_on_object_already_exists(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_map *object = NULL;
    assert_true(coral$tree_map$alloc(&object));
    assert_true(coral$tree_map$init(
            object, sizeof(size_t), sizeof(size_t), coral_compare_size_t));
    struct entry {
        size_t key; /* key MUST always be the first value */
        size_t value;
    } e = {10, 726};
    assert_true(coral$tree_map$insert(object, &e));
    struct entry d = {e.key, 736728};
    assert_false(coral$tree_map$insert(object, &d));
    assert_int_equal(CORAL_ERROR_OBJECT_ALREADY_EXISTS, coral_error);
    assert_true(coral$tree_map$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_large_value_on_object_already_exists(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_map *object = NULL;
    assert_true(coral$tree_map$alloc(&object));
    assert_true(coral$tree_map$init(
            object, sizeof(size_t), 32, coral_compare_size_t));
    struct entry {
        size_t key; /* key MUST always be the first value */
        char value[32];
    } e;
    e.key = 10;
    memcpy(e.value, check_insert_large_value_on_object_already_exists, 32);
    assert_true(coral$tree_map$insert(object, &e));
    struct entry d = {e.key, 0};
    assert_false(coral$tree_map$insert(object, &d));
    assert_int_equal(CORAL_ERROR_OBJECT_ALREADY_EXISTS, coral_error);
    assert_true(coral$tree_map$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$delete(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$delete((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete_error_on_object_does_not_exist(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_map *object = NULL;
    assert_true(coral$tree_map$alloc(&object));
    assert_true(coral$tree_map$init(
            object, sizeof(size_t), sizeof(size_t), coral_compare_size_t));
    const size_t key = 10;
    assert_false(coral$tree_map$delete(object, &key));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_map *object;
    assert_true(coral$tree_map$alloc(&object));
    assert_true(coral$tree_map$init(
            object, sizeof(size_t), sizeof(void *), coral_compare_size_t));
    struct entry {
        size_t key; /* key MUST always be the first value */
        void *value;
    } e = {30, NULL};
    assert_true(coral$tree_map$insert(object, &e));
    assert_int_equal(1, coral$atomic_load(&object->tree_set.id));
    assert_int_equal(1, object->tree_set.count);
    assert_true(coral$tree_map$delete(object, &e.key));
    assert_int_equal(2, coral$atomic_load(&object->tree_set.id));
    assert_int_equal(0, object->tree_set.count);
    assert_true(coral$tree_map$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_contains_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$contains(NULL, (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_contains_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$contains((void *) 1, NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    assert_false(coral$tree_map$contains((void *) 1, (void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_contains(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_map *object;
    assert_true(coral$tree_map$alloc(&object));
    assert_true(coral$tree_map$init(
            object, sizeof(size_t), sizeof(size_t), coral_compare_size_t));
    bool contains;
    struct entry {
        size_t key; /* key MUST always be the first value */
        size_t value;
    } e = {625, 726};
    assert_true(coral$tree_map$contains(object, &e, &contains));
    assert_false(contains);
    assert_true(coral$tree_map$insert(object, &e));
    assert_true(coral$tree_map$contains(object, &e, &contains));
    assert_true(contains);
    assert_true(coral$tree_map$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$get(NULL, (void *)1, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$get((void *)1, NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$get((void *)1, (void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_map *object;
    assert_true(coral$tree_map$alloc(&object));
    assert_true(coral$tree_map$init(
            object, sizeof(size_t), sizeof(size_t), coral_compare_size_t));
    size_t key = 29;
    void *out;
    assert_false(coral$tree_map$get(object, &key, &out));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_map *object;
    assert_true(coral$tree_map$alloc(&object));
    assert_true(coral$tree_map$init(
            object, sizeof(size_t), sizeof(size_t), coral_compare_size_t));
    struct entry {
        size_t key;
        size_t value;
    } e = {87, 83621};
    assert_true(coral$tree_map$insert(object, &e));
    struct entry d;
    assert_true(coral$tree_map$get(object, &e.key, (void **) &d));
    assert_memory_equal(&e, &d, sizeof(struct entry));
    assert_true(coral$tree_map$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$set(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$set((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_map *object;
    assert_true(coral$tree_map$alloc(&object));
    assert_true(coral$tree_map$init(
            object, sizeof(size_t), sizeof(size_t), coral_compare_size_t));
    struct entry {
        size_t key;
        size_t value;
    } e = {2123, 0};
    assert_false(coral$tree_map$set(object, &e));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_set(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_map *object;
    assert_true(coral$tree_map$alloc(&object));
    assert_true(coral$tree_map$init(
            object, sizeof(size_t), sizeof(size_t), coral_compare_size_t));
    struct entry {
        size_t key;
        size_t value;
    } e = {23, 26271};
    assert_true(coral$tree_map$insert(object, &e));
    e.value = 0;
    assert_true(coral$tree_map$set(object, &e));
    struct entry d;
    assert_true(coral$tree_map$get(object, &e.key, (void **) &d));
    assert_memory_equal(&e, &d, sizeof(struct entry));
    assert_true(coral$tree_map$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_first_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$first(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_first_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$first((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_first_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_map *object;
    assert_true(coral$tree_map$alloc(&object));
    assert_true(coral$tree_map$init(
            object, sizeof(size_t), sizeof(size_t), coral_compare_size_t));
    const size_t key = 1285;
    assert_false(coral$tree_map$first(object, (void **) &key));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_first(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_map *object;
    assert_true(coral$tree_map$alloc(&object));
    assert_true(coral$tree_map$init(
            object, sizeof(size_t), sizeof(void *), coral_compare_size_t));
    struct entry {
        size_t key;
        void* value;
    } e = {24, check_first};
    assert_true(coral$tree_map$insert(object, &e));
    struct entry d;
    assert_true(coral$tree_map$first(object, (void **) &d));
    assert_ptr_not_equal(&e, &d);
    assert_memory_equal(&d, &e, sizeof(struct entry));
    assert_true(coral$tree_map$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_last_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$last(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_last_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$last((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_last_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_map *object;
    assert_true(coral$tree_map$alloc(&object));
    assert_true(coral$tree_map$init(
            object, sizeof(size_t), sizeof(size_t), coral_compare_size_t));
    const size_t key = 1285;
    assert_false(coral$tree_map$last(object, (void **) &key));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_last(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_map *object;
    assert_true(coral$tree_map$alloc(&object));
    assert_true(coral$tree_map$init(
            object, sizeof(size_t), sizeof(void *), coral_compare_size_t));
    struct entry {
        size_t key;
        void* value;
    } e = {24, check_last};
    assert_true(coral$tree_map$insert(object, &e));
    struct entry d;
    assert_true(coral$tree_map$last(object, (void **) &d));
    assert_ptr_not_equal(&e, &d);
    assert_memory_equal(&d, &e, sizeof(struct entry));
    assert_true(coral$tree_map$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_next_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$next(NULL, (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_next_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$next((void *) 1, NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$next((void *) 1, (void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_next_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_map *object;
    assert_true(coral$tree_map$alloc(&object));
    assert_true(coral$tree_map$init(
            object, sizeof(size_t), sizeof(size_t), coral_compare_size_t));
    struct entry {
        size_t key;
        size_t value;
    } e = {19, 7262};
    assert_false(coral$tree_map$next(object, &e, (void **) &e));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    assert_true(coral$tree_map$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_next_error_on_end_of_sequence(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_map *object;
    assert_true(coral$tree_map$alloc(&object));
    assert_true(coral$tree_map$init(
            object, sizeof(size_t), sizeof(size_t), coral_compare_size_t));
    struct entry {
        size_t key;
        size_t value;
    } e = {55, 252121};
    assert_true(coral$tree_map$insert(object, &e));
    assert_false(coral$tree_map$next(object, &e, (void **) &e));
    assert_int_equal(CORAL_ERROR_END_OF_SEQUENCE, coral_error);
    assert_true(coral$tree_map$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_next(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_map *object;
    assert_true(coral$tree_map$alloc(&object));
    assert_true(coral$tree_map$init(
            object, sizeof(size_t), sizeof(size_t), coral_compare_size_t));
    struct entry {
        size_t key;
        size_t value;
    } e = {55, 252121};
    assert_true(coral$tree_map$insert(object, &e));
    struct entry d = {10, 2652};
    assert_true(coral$tree_map$insert(object, &d));
    struct entry c = {};
    assert_memory_not_equal(&c, &e, sizeof(struct entry));
    assert_true(coral$tree_map$next(object, &d, (void **) &c));
    assert_memory_equal(&c, &e, sizeof(struct entry));
    assert_true(coral$tree_map$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_prev_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$prev(NULL, (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_prev_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$prev((void *) 1, NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_map$prev((void *) 1, (void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_prev_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_map *object;
    assert_true(coral$tree_map$alloc(&object));
    assert_true(coral$tree_map$init(
            object, sizeof(size_t), sizeof(size_t), coral_compare_size_t));
    struct entry {
        size_t key;
        size_t value;
    } e = {356, 7262};
    assert_false(coral$tree_map$prev(object, &e, (void **) &e));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    assert_true(coral$tree_map$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_prev(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_map *object;
    assert_true(coral$tree_map$alloc(&object));
    assert_true(coral$tree_map$init(
            object, sizeof(size_t), sizeof(size_t), coral_compare_size_t));
    struct entry {
        size_t key;
        size_t value;
    } e = {55, 252121};
    assert_true(coral$tree_map$insert(object, &e));
    struct entry d = {10, 2652};
    assert_true(coral$tree_map$insert(object, &d));
    struct entry c = {};
    assert_memory_not_equal(&c, &d, sizeof(struct entry));
    assert_true(coral$tree_map$prev(object, &e, (void **) &c));
    assert_memory_equal(&c, &d, sizeof(struct entry));
    assert_true(coral$tree_map$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_alloc_error_on_null_argument_ptr),
            cmocka_unit_test(check_alloc),
            cmocka_unit_test(check_invalidate_error_on_null_object_ptr),
            cmocka_unit_test(check_init_error_on_null_object_ptr),
            cmocka_unit_test(check_init_error_on_null_argument_ptr),
            cmocka_unit_test(check_init),
            cmocka_unit_test(check_get_count_error_on_null_object_ptr),
            cmocka_unit_test(check_get_count_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_count),
            cmocka_unit_test(check_get_size_error_on_null_object_ptr),
            cmocka_unit_test(check_get_size_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_size),
            cmocka_unit_test(check_get_key_size_error_on_null_object_ptr),
            cmocka_unit_test(check_get_key_size_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_key_size),
            cmocka_unit_test(check_get_value_size_error_on_null_object_ptr),
            cmocka_unit_test(check_get_value_size_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_value_size),
            cmocka_unit_test(check_insert_error_on_null_object_ptr),
            cmocka_unit_test(check_insert_error_on_null_argument_ptr),
            cmocka_unit_test(check_insert),
            cmocka_unit_test(check_insert_large_value),
            cmocka_unit_test(check_insert_error_on_object_already_exists),
            cmocka_unit_test(check_insert_large_value_on_object_already_exists),
            cmocka_unit_test(check_delete_error_on_null_object_ptr),
            cmocka_unit_test(check_delete_error_on_null_argument_ptr),
            cmocka_unit_test(check_delete_error_on_object_does_not_exist),
            cmocka_unit_test(check_delete),
            cmocka_unit_test(check_contains_error_on_null_object_ptr),
            cmocka_unit_test(check_contains_error_on_null_argument_ptr),
            cmocka_unit_test(check_contains),
            cmocka_unit_test(check_get_error_on_null_object_ptr),
            cmocka_unit_test(check_get_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_error_on_object_not_found),
            cmocka_unit_test(check_get),
            cmocka_unit_test(check_set_error_on_null_object_ptr),
            cmocka_unit_test(check_set_error_on_null_argument_ptr),
            cmocka_unit_test(check_set_error_on_object_not_found),
            cmocka_unit_test(check_set),
            cmocka_unit_test(check_first_error_on_null_object_ptr),
            cmocka_unit_test(check_first_error_on_null_argument_ptr),
            cmocka_unit_test(check_first_error_on_object_not_found),
            cmocka_unit_test(check_first),
            cmocka_unit_test(check_last_error_on_null_object_ptr),
            cmocka_unit_test(check_last_error_on_null_argument_ptr),
            cmocka_unit_test(check_last_error_on_object_not_found),
            cmocka_unit_test(check_last),
            cmocka_unit_test(check_next_error_on_null_object_ptr),
            cmocka_unit_test(check_next_error_on_null_argument_ptr),
            cmocka_unit_test(check_next_error_on_object_not_found),
            cmocka_unit_test(check_next_error_on_end_of_sequence),
            cmocka_unit_test(check_next),
            cmocka_unit_test(check_prev_error_on_null_object_ptr),
            cmocka_unit_test(check_prev_error_on_null_argument_ptr),
            cmocka_unit_test(check_prev_error_on_object_not_found),
            cmocka_unit_test(check_prev)
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
