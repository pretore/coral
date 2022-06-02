#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <coral.h>

#include "private/tree_set.h"
#include "private/coral.h"
#include "test/cmocka.h"

static void check_alloc_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$alloc(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_alloc(void **state) {
    struct coral$tree_set *object = NULL;
    assert_true(coral$tree_set$alloc(&object));
    assert_non_null(object);
    free(object);
}

static void check_invalidate_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$invalidate(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$init(NULL, NULL, 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_init_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$init((void *) 1, NULL, 0, (void *) 1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$init((void *) 1, NULL, 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object = NULL;
    assert_true(coral$tree_set$alloc(&object));
    assert_non_null(object);
    assert_true(coral$tree_set$init(object, NULL, sizeof(size_t), (void *) 1));
    assert_int_equal(0, coral$atomic_load(&object->id));
    assert_int_equal(sizeof(size_t), object->size);
    assert_int_equal(0, object->count);
    assert_ptr_equal(1, object->tree.compare);
    assert_null(object->tree.root);
    assert_true(coral$tree_set$invalidate(object, NULL));
    assert_null(object->tree.compare);
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_count_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$get_count(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_count_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$get_count((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_count(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set object = {};
    size_t count = ~0;
    assert_true(coral$tree_set$get_count(&object, &count));
    assert_int_equal(0, count);
    object.count = 10;
    assert_true(coral$tree_set$get_count(&object, &count));
    assert_int_equal(10, count);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_size_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$get_size(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_size_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$get_size((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_size(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set object = {};
    size_t size = ~0;
    assert_true(coral$tree_set$get_size(&object, &size));
    assert_int_equal(0, size);
    object.size = 827;
    assert_true(coral$tree_set$get_size(&object, &size));
    assert_int_equal(827, size);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$insert(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$insert((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_error_on_object_unavailable(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    struct coral_range_values limit = {0, 0};
    assert_true(coral$tree_set$init(object, &limit,
                                    sizeof(size_t), coral_compare_size_t));
    size_t i = 876;
    assert_false(coral$tree_set$insert(object, &i));
    assert_int_equal(CORAL_ERROR_OBJECT_UNAVAILABLE, coral_error);
    assert_true(coral$tree_set$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(object, NULL, sizeof(size_t), coral_compare_size_t));
    size_t i = 876;
    assert_int_equal(0, coral$atomic_load(&object->id));
    assert_int_equal(0, object->count);
    assert_true(coral$tree_set$insert(object, &i));
    assert_int_equal(1, coral$atomic_load(&object->id));
    assert_int_equal(1, object->count);
    assert_int_equal(i, *(size_t *) object->tree.root);
    assert_true(coral$tree_set$invalidate(object, NULL));
    assert_int_equal(0, object->count);
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_large_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    const size_t size = 4 * sizeof(void *);
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(object, NULL, size, coral_compare_size_t));
    assert_int_equal(0, coral$atomic_load(&object->id));
    assert_int_equal(0, object->count);
    assert_true(coral$tree_set$insert(object, check_insert_large_value));
    assert_int_equal(1, coral$atomic_load(&object->id));
    assert_int_equal(1, object->count);
    assert_memory_equal(object->tree.root, check_insert_large_value, size);
    assert_true(coral$tree_set$invalidate(object, NULL));
    assert_int_equal(0, object->count);
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_error_on_object_already_exists(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(object, NULL, sizeof(size_t),
                                    coral_compare_size_t));
    const size_t i = 20;
    assert_int_equal(0, coral$atomic_load(&object->id));
    assert_int_equal(0, object->count);
    assert_true(coral$tree_set$insert(object, &i));
    assert_int_equal(1, coral$atomic_load(&object->id));
    assert_int_equal(1, object->count);
    const size_t o = i;
    assert_ptr_not_equal(&i, &o);
    assert_int_equal(i, o);
    assert_false(coral$tree_set$insert(object, &o));
    assert_int_equal(CORAL_ERROR_OBJECT_ALREADY_EXISTS, coral_error);
    assert_int_equal(1, coral$atomic_load(&object->id));
    assert_int_equal(1, object->count);
    assert_true(coral$tree_set$invalidate(object, NULL));
    assert_int_equal(0, object->count);
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_large_value_on_object_already_exists(void **state) {
    coral_error = CORAL_ERROR_NONE;
    const size_t size = 4 * sizeof(void *);
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(object, NULL, size, coral_compare_size_t));
    assert_int_equal(0, coral$atomic_load(&object->id));
    assert_int_equal(0, object->count);
    assert_true(coral$tree_set$insert(
            object, check_insert_large_value_on_object_already_exists));
    assert_int_equal(1, coral$atomic_load(&object->id));
    assert_int_equal(1, object->count);
    assert_memory_equal(object->tree.root,
                        check_insert_large_value_on_object_already_exists,
                        size);
    assert_false(coral$tree_set$insert(
            object, check_insert_large_value_on_object_already_exists));
    assert_int_equal(CORAL_ERROR_OBJECT_ALREADY_EXISTS, coral_error);
    assert_int_equal(1, coral$atomic_load(&object->id));
    assert_int_equal(1, object->count);
    assert_true(coral$tree_set$invalidate(object, NULL));
    assert_int_equal(0, object->count);
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$delete(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$delete((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete_error_on_object_does_not_exist(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(object, NULL, sizeof(size_t), coral_compare_size_t));
    const size_t i = 98;
    assert_false(coral$tree_set$delete(object, &i));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    assert_true(coral$tree_set$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete_error_on_object_unavailable(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    struct coral_range_values limit = {1, 1};
    assert_true(coral$tree_set$init(object, &limit,
                                    sizeof(size_t), coral_compare_size_t));
    size_t i = 876;
    assert_true(coral$tree_set$insert(object, &i));
    assert_false(coral$tree_set$delete(object, &i));
    assert_int_equal(CORAL_ERROR_OBJECT_UNAVAILABLE, coral_error);
    assert_true(coral$tree_set$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(object, NULL, sizeof(size_t), coral_compare_size_t));
    const size_t i = 87;
    assert_true(coral$tree_set$insert(object, &i));
    assert_int_equal(1, coral$atomic_load(&object->id));
    assert_int_equal(1, object->count);
    assert_true(coral$tree_set$delete(object, &i));
    assert_int_equal(2, coral$atomic_load(&object->id));
    assert_int_equal(0, object->count);
    assert_true(coral$tree_set$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_contains_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$contains(NULL, (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_contains_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$contains((void *) 1, NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    assert_false(coral$tree_set$contains((void *) 1, (void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_contains(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(object, NULL, sizeof(size_t), coral_compare_size_t));
    bool is_member;
    const size_t i = 87;
    assert_true(coral$tree_set$contains(object, &i, &is_member));
    assert_false(is_member);
    assert_true(coral$tree_set$insert(object, &i));
    assert_true(coral$tree_set$contains(object, &i, &is_member));
    assert_true(is_member);
    assert_true(coral$tree_set$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_first_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$get_first(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_first_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$get_first((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_first_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(object, NULL, sizeof(size_t), coral_compare_size_t));
    size_t i = 0;
    assert_false(coral$tree_set$get_first(object, (void **) &i));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_first(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(object, NULL, sizeof(size_t), coral_compare_size_t));
    size_t i = 98;
    assert_true(coral$tree_set$insert(object, &i));
    i = 12;
    assert_true(coral$tree_set$get_first(object, (void **) &i));
    assert_int_equal(98, i);
    assert_true(coral$tree_set$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_last_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$get_last(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_last_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$get_last((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_last_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(object, NULL, sizeof(size_t), coral_compare_size_t));
    size_t i = 0;
    assert_false(coral$tree_set$get_last(object, (void **) &i));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_last(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(object, NULL, sizeof(size_t), coral_compare_size_t));
    size_t i = 98;
    assert_true(coral$tree_set$insert(object, &i));
    i = 12;
    assert_true(coral$tree_set$get_last(object, (void **) &i));
    assert_int_equal(98, i);
    assert_true(coral$tree_set$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_next_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$get_next(NULL, (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_next_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$get_next((void *) 1, NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$get_next((void *) 1, (void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_next_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(object, NULL, sizeof(size_t),
                                    coral_compare_size_t));
    size_t i = 9;
    assert_false(coral$tree_set$get_next(object, (const void *) i,
                                         (void **) &i));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    assert_true(coral$tree_set$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_next_error_on_end_of_sequence(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(object, NULL, sizeof(size_t), coral_compare_size_t));
    size_t i = 9;
    assert_true(coral$tree_set$insert(object, &i));
    i = 9;
    assert_false(coral$tree_set$get_next(object, &i, (void **) &i));
    assert_int_equal(CORAL_ERROR_END_OF_SEQUENCE, coral_error);
    assert_true(coral$tree_set$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_next(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(object, NULL, sizeof(size_t), coral_compare_size_t));
    size_t i = 0;
    assert_true(coral$tree_set$insert(object, &i));
    i = 9;
    assert_true(coral$tree_set$insert(object, &i));
    i = 0;
    assert_true(coral$tree_set$get_next(object, &i, (void **) &i));
    assert_int_equal(i, 9);
    assert_true(coral$tree_set$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_prev_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$get_prev(NULL, (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_prev_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$get_prev((void *) 1, NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$get_prev((void *) 1, (void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_prev_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(object, NULL, sizeof(size_t), coral_compare_size_t));
    size_t i = 9;
    assert_false(coral$tree_set$get_prev(object, (const void *) i, (void **)
    &i));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    assert_true(coral$tree_set$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_prev_error_on_end_of_sequence(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(object, NULL, sizeof(size_t), coral_compare_size_t));
    size_t i = 9;
    assert_true(coral$tree_set$insert(object, &i));
    i = 9;
    assert_false(coral$tree_set$get_prev(object, &i, (void **) &i));
    assert_int_equal(CORAL_ERROR_END_OF_SEQUENCE, coral_error);
    assert_true(coral$tree_set$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_prev(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(object, NULL, sizeof(size_t), coral_compare_size_t));
    size_t i = 0;
    assert_true(coral$tree_set$insert(object, &i));
    i = 9;
    assert_true(coral$tree_set$insert(object, &i));
    i = 9;
    assert_true(coral$tree_set$get_prev(object, &i, (void **) &i));
    assert_int_equal(i, 0);
    assert_true(coral$tree_set$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_class_error_on_null_argument(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_class(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_class(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_tree_set_class(&class));
    assert_non_null(class);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_destroy_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_destroy(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_alloc_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_alloc(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_alloc(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_true(coral_tree_set_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_init(NULL, (void *)1, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_init((void *)1, (void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_true(coral_tree_set_init(object, NULL, coral_compare_void_ptr));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_hash_code(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_hash_code((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    size_t code;
    assert_false(coral_tree_set_hash_code(object, &code));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_tree_set_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_true(coral_tree_set_init(object, NULL, coral_compare_void_ptr));
    size_t code = 0;
    assert_true(coral_tree_set_hash_code(object, &code));
    assert_int_not_equal(code, 0);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_is_equal(NULL, (void *)1, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_is_equal((void *)1, NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_is_equal((void *)1, (void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_true(coral_tree_set_init(object, NULL, coral_compare_void_ptr));
    struct coral_tree_set *other;
    assert_true(coral_tree_set_alloc(&other));
    assert_true(coral_tree_set_init(other, NULL, coral_compare_void_ptr));
    bool result;
    assert_true(coral_tree_set_is_equal(object, other, &result));
    assert_true(result);
    assert_true(coral_tree_set_is_equal(other, object, &result));
    assert_true(result);
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *q, *w;
    assert_true(coral_object_alloc(0, &q));
    assert_true(coral_object_init(q, class));
    assert_true(coral_object_alloc(0, &w));
    assert_true(coral_object_init(w, class));
    assert_true(coral_tree_set_insert(object, q));
    assert_true(coral_tree_set_insert(other, q));
    assert_true(coral_tree_set_is_equal(object, other, &result));
    assert_true(result);
    assert_true(coral_tree_set_is_equal(other, object,&result));
    assert_true(result);
    assert_true(coral_tree_set_insert(object, w));
    assert_true(coral_tree_set_is_equal(object, other, &result));
    assert_false(result);
    assert_true(coral_tree_set_is_equal(other, object, &result));
    assert_false(result);
    assert_true(coral_tree_set_insert(other, w));
    assert_true(coral_tree_set_is_equal(object, other, &result));
    assert_true(result);
    assert_true(coral_tree_set_is_equal(other, object,&result));
    assert_true(result);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_copy_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_copy(NULL, (void*)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_copy_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_copy((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_copy_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object, *other = NULL;
    assert_true(coral_tree_set_alloc(&object));
    assert_false(coral_tree_set_copy(object, &other));
    assert_true(coral_tree_set_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_copy(void **state) {
    // TODO: implement (once get and set is done) ...
    assert_true(false);
}

static void check_object_retain_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_retain(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_retain_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_false(coral_tree_set_retain(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_tree_set_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_release_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_release(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_release_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_false(coral_tree_set_release(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_tree_set_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_autorelease_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_autorelease(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_autorelease_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_false(coral_tree_set_autorelease(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_tree_set_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_count_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_get_count(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_count_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_get_count((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_count_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_false(coral_tree_set_get_count(object, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_tree_set_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_count(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_true(coral_tree_set_init(object, NULL, coral_compare_void_ptr));
    size_t count;
    assert_true(coral_tree_set_get_count(object, &count));
    assert_int_equal(0, count);
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *q;
    assert_true(coral_object_alloc(0, &q));
    assert_true(coral_object_init(q, class));
    assert_true(coral_tree_set_insert(object, q));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_insert_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_insert(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_insert_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_insert((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_insert_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_false(coral_tree_set_insert(object, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_tree_set_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_insert_error_on_object_already_exists(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class;
    assert_true(coral_object_class(&class));
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_true(coral_tree_set_init(object, NULL, coral_compare_void_ptr));
    void *i;
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    assert_true(coral_tree_set_insert(object, i));
    assert_false(coral_tree_set_insert(object, i));
    assert_int_equal(CORAL_ERROR_OBJECT_ALREADY_EXISTS, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_insert_error_on_object_unavailable(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    struct coral_range_values limit = {
            0, 0
    };
    assert_true(coral_tree_set_init(object, &limit, coral_compare_void_ptr));
    struct coral_class *class;
    assert_true(coral_object_class(&class));
    void *i;
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    assert_false(coral_tree_set_insert(object, i));
    assert_int_equal(CORAL_ERROR_OBJECT_UNAVAILABLE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_insert(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_true(coral_tree_set_init(object, NULL, coral_compare_void_ptr));
    struct coral_class *class;
    assert_true(coral_object_class(&class));
    for (size_t i = 0; i < 5; i++) {
        void *o;
        assert_true(coral_object_alloc(0, &o));
        assert_true(coral_object_init(o, class));
        assert_true(coral_tree_set_insert(object, o));
    }
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_delete_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_delete(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_delete_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_delete((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_delete_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *i;
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_false(coral_tree_set_delete(object, i));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_tree_set_destroy(object));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_delete_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_true(coral_tree_set_init(object, NULL, coral_compare_void_ptr));
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *i;
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    assert_false(coral_tree_set_delete(object, i));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_delete_error_on_object_unavailable(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    struct coral_range_values limit = {1, SIZE_MAX};
    assert_true(coral_tree_set_init(object, &limit, coral_compare_void_ptr));
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *i;
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    assert_true(coral_tree_set_insert(object, i));
    assert_false(coral_tree_set_delete(object, i));
    assert_int_equal(CORAL_ERROR_OBJECT_UNAVAILABLE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_delete(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_true(coral_tree_set_init(object, NULL, coral_compare_void_ptr));
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *i;
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    assert_true(coral_tree_set_insert(object, i));
    assert_true(coral_tree_set_delete(object, i));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_contains_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_contains(NULL, (void *)1, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_contains_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_contains((void *)1, NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_contains((void *)1, (void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_contains_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    void *i = (void *)1;
    bool result;
    assert_false(coral_tree_set_contains(object, i, &result));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_tree_set_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_contains(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_true(coral_tree_set_init(object, NULL, coral_compare_void_ptr));
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *i;
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    bool result;
    assert_true(coral_tree_set_contains(object, i, &result));
    assert_false(result);
    assert_true(coral_tree_set_insert(object, i));
    assert_true(coral_tree_set_contains(object, i, &result));
    assert_true(result);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_first_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_get_first(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_first_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_get_first((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_first_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_true(coral_tree_set_init(object, NULL, coral_compare_void_ptr));
    void *i;
    assert_false(coral_tree_set_get_first(object, &i));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_first_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    void *i;
    assert_false(coral_tree_set_get_first(object, &i));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_tree_set_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_first(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_true(coral_tree_set_init(object, NULL, coral_compare_void_ptr));
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *i;
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    assert_true(coral_tree_set_insert(object, i));
    void *o;
    assert_true(coral_tree_set_get_first(object, &o));
    assert_ptr_equal(i, o);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_last_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_get_last(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_last_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_get_last((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_last_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_true(coral_tree_set_init(object, NULL, coral_compare_void_ptr));
    void *i;
    assert_false(coral_tree_set_get_last(object, &i));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_last_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    void *i;
    assert_false(coral_tree_set_get_last(object, &i));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_tree_set_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_last(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_true(coral_tree_set_init(object, NULL, coral_compare_void_ptr));
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *i;
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    assert_true(coral_tree_set_insert(object, i));
    void *o;
    assert_true(coral_tree_set_get_last(object, &o));
    assert_ptr_equal(i, o);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_next_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_get_next(NULL, (void *)1, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_next_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_get_next((void *)1, NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_get_next((void *)1, (void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_next_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_true(coral_tree_set_init(object, NULL, coral_compare_void_ptr));
    void *i;
    assert_false(coral_tree_set_get_next(object, i, &i));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_next_error_on_end_of_sequence(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_true(coral_tree_set_init(object, NULL, coral_compare_void_ptr));
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *i;
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    assert_true(coral_tree_set_insert(object, i));
    assert_false(coral_tree_set_get_next(object, i, &i));
    assert_int_equal(CORAL_ERROR_END_OF_SEQUENCE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_next_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    void *i = (void *)1;
    assert_false(coral_tree_set_get_next(object, i, &i));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_tree_set_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_next(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_true(coral_tree_set_init(object, NULL, coral_compare_void_ptr));
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *i;
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    assert_true(coral_tree_set_insert(object, i));
    void *o;
    assert_true(coral_object_alloc(0, &o));
    assert_true(coral_object_init(o, class));
    assert_true(coral_tree_set_insert(object, o));
    assert_true(coral_tree_set_get_first(object, &i));
    assert_true(coral_tree_set_get_last(object, &o));
    assert_true(coral_tree_set_get_next(object, i, &i));
    assert_ptr_equal(i, o);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_prev_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_get_prev(NULL, (void *)1, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_prev_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_get_prev((void *)1, NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_tree_set_get_prev((void *)1, (void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_prev_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_true(coral_tree_set_init(object, NULL, coral_compare_void_ptr));
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *i;
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    assert_false(coral_tree_set_get_prev(object, i, &i));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_prev_error_on_end_of_sequence(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_true(coral_tree_set_init(object, NULL, coral_compare_void_ptr));
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *i;
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    assert_true(coral_tree_set_insert(object, i));
    assert_false(coral_tree_set_get_prev(object, i, &i));
    assert_int_equal(CORAL_ERROR_END_OF_SEQUENCE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_prev_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    void *i = (void *)1;
    assert_false(coral_tree_set_get_prev(object, &i, &i));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_tree_set_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_prev(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_tree_set *object;
    assert_true(coral_tree_set_alloc(&object));
    assert_true(coral_tree_set_init(object, NULL, coral_compare_void_ptr));
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *i;
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    assert_true(coral_tree_set_insert(object, i));
    void *o;
    assert_true(coral_object_alloc(0, &o));
    assert_true(coral_object_init(o, class));
    assert_true(coral_tree_set_insert(object, o));
    assert_true(coral_tree_set_get_first(object, &o));
    assert_true(coral_tree_set_get_last(object, &i));
    assert_true(coral_tree_set_get_prev(object, i, &i));
    assert_ptr_equal(i, o);
    coral_autorelease_pool_drain();
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
            cmocka_unit_test(check_insert_error_on_null_object_ptr),
            cmocka_unit_test(check_insert_error_on_null_argument_ptr),
            cmocka_unit_test(check_insert_error_on_object_unavailable),
            cmocka_unit_test(check_insert),
            cmocka_unit_test(check_insert_large_value),
            cmocka_unit_test(check_insert_error_on_object_already_exists),
            cmocka_unit_test(check_insert_large_value_on_object_already_exists),
            cmocka_unit_test(check_delete_error_on_null_object_ptr),
            cmocka_unit_test(check_delete_error_on_null_argument_ptr),
            cmocka_unit_test(check_delete_error_on_object_does_not_exist),
            cmocka_unit_test(check_delete_error_on_object_unavailable),
            cmocka_unit_test(check_delete),
            cmocka_unit_test(check_contains_error_on_null_object_ptr),
            cmocka_unit_test(check_contains_error_on_null_argument_ptr),
            cmocka_unit_test(check_contains),
            cmocka_unit_test(check_get_first_error_on_null_object_ptr),
            cmocka_unit_test(check_get_first_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_first_error_on_object_not_found),
            cmocka_unit_test(check_get_first),
            cmocka_unit_test(check_get_last_error_on_null_object_ptr),
            cmocka_unit_test(check_get_last_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_last_error_on_object_not_found),
            cmocka_unit_test(check_get_last),
            cmocka_unit_test(check_get_next_error_on_null_object_ptr),
            cmocka_unit_test(check_get_next_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_next_error_on_object_not_found),
            cmocka_unit_test(check_get_next_error_on_end_of_sequence),
            cmocka_unit_test(check_get_next),
            cmocka_unit_test(check_get_prev_error_on_null_object_ptr),
            cmocka_unit_test(check_get_prev_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_prev_error_on_object_not_found),
            cmocka_unit_test(check_get_prev_error_on_end_of_sequence),
            cmocka_unit_test(check_get_prev),
            cmocka_unit_test(check_object_class_error_on_null_argument),
            cmocka_unit_test(check_object_class),
            cmocka_unit_test(check_object_destroy_error_on_null_object_ptr),
            cmocka_unit_test(check_object_alloc_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_alloc),
            cmocka_unit_test(check_object_init_error_on_null_object_ptr),
            cmocka_unit_test(check_object_init_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_init),
            cmocka_unit_test(check_object_hash_code_error_on_null_object_ptr),
            cmocka_unit_test(check_object_hash_code_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_hash_code_error_on_object_uninitialized),
            cmocka_unit_test(check_object_hash_code),
            cmocka_unit_test(check_object_is_equal_error_on_null_object_ptr),
            cmocka_unit_test(check_object_is_equal_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_is_equal),
            cmocka_unit_test(check_object_copy_error_on_null_object_ptr),
            cmocka_unit_test(check_object_copy_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_copy_error_on_object_uninitialized),
            //cmocka_unit_test(check_object_copy),
            cmocka_unit_test(check_object_retain_error_on_null_object_ptr),
            cmocka_unit_test(check_object_retain_error_on_object_uninitialized),
            cmocka_unit_test(check_object_release_error_on_null_object_ptr),
            cmocka_unit_test(check_object_release_error_on_object_uninitialized),
            cmocka_unit_test(check_object_autorelease_error_on_null_object_ptr),
            cmocka_unit_test(check_object_autorelease_error_on_object_uninitialized),
            cmocka_unit_test(check_object_get_count_error_on_null_object_ptr),
            cmocka_unit_test(check_object_get_count_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_get_count_error_on_object_uninitialized),
            cmocka_unit_test(check_object_get_count),
            cmocka_unit_test(check_object_insert_error_on_null_object_ptr),
            cmocka_unit_test(check_object_insert_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_insert_error_on_object_uninitialized),
            cmocka_unit_test(check_object_insert_error_on_object_already_exists),
            cmocka_unit_test(check_object_insert_error_on_object_unavailable),
            cmocka_unit_test(check_object_insert),
            cmocka_unit_test(check_object_delete_error_on_null_object_ptr),
            cmocka_unit_test(check_object_delete_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_delete_error_on_object_uninitialized),
            cmocka_unit_test(check_object_delete_error_on_object_not_found),
            cmocka_unit_test(check_object_delete_error_on_object_unavailable),
            cmocka_unit_test(check_object_delete),
            cmocka_unit_test(check_object_contains_error_on_null_object_ptr),
            cmocka_unit_test(check_object_contains_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_contains_error_on_object_uninitialized),
            cmocka_unit_test(check_object_contains),
            cmocka_unit_test(check_object_get_first_error_on_null_object_ptr),
            cmocka_unit_test(check_object_get_first_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_get_first_error_on_object_not_found),
            cmocka_unit_test(check_object_get_first_error_on_object_uninitialized),
            cmocka_unit_test(check_object_get_first),
            cmocka_unit_test(check_object_get_last_error_on_null_object_ptr),
            cmocka_unit_test(check_object_get_last_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_get_last_error_on_object_not_found),
            cmocka_unit_test(check_object_get_last_error_on_object_uninitialized),
            cmocka_unit_test(check_object_get_last),
            cmocka_unit_test(check_object_get_next_error_on_null_object_ptr),
            cmocka_unit_test(check_object_get_next_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_get_next_error_on_object_not_found),
            cmocka_unit_test(check_object_get_next_error_on_end_of_sequence),
            cmocka_unit_test(check_object_get_next_error_on_object_uninitialized),
            cmocka_unit_test(check_object_get_next),
            cmocka_unit_test(check_object_get_prev_error_on_null_object_ptr),
            cmocka_unit_test(check_object_get_prev_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_get_prev_error_on_object_not_found),
            cmocka_unit_test(check_object_get_prev_error_on_end_of_sequence),
            cmocka_unit_test(check_object_get_prev_error_on_object_uninitialized),
            cmocka_unit_test(check_object_get_prev),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
