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
    assert_false(coral$tree_set$init(NULL, 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_init_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$init((void *) 1, 0, (void *) 1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$init((void *) 1, 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object = NULL;
    assert_true(coral$tree_set$alloc(&object));
    assert_non_null(object);
    assert_true(coral$tree_set$init(object, sizeof(size_t), (void *) 1));
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

static void check_insert(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(
            object, sizeof(size_t), coral_compare_size_t));
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
    assert_true(coral$tree_set$init(object, size, coral_compare_size_t));
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
    assert_true(coral$tree_set$init(
            object, sizeof(size_t), coral_compare_size_t));
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
    assert_true(coral$tree_set$init(object, size, coral_compare_size_t));
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
    assert_true(coral$tree_set$init(
            object, sizeof(size_t), coral_compare_size_t));
    const size_t i = 98;
    assert_false(coral$tree_set$delete(object, &i));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    assert_true(coral$tree_set$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(
            object, sizeof(size_t), coral_compare_size_t));
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
    assert_true(coral$tree_set$init(
            object, sizeof(size_t), coral_compare_size_t));
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

static void check_first_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$first(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_first_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$first((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_first_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(
            object, sizeof(size_t), coral_compare_size_t));
    size_t i = 0;
    assert_false(coral$tree_set$first(object, (void **) &i));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_first(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(
            object, sizeof(size_t), coral_compare_size_t));
    size_t i = 98;
    assert_true(coral$tree_set$insert(object, &i));
    i = 12;
    assert_true(coral$tree_set$first(object, (void **) &i));
    assert_int_equal(98, i);
    assert_true(coral$tree_set$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_last_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$last(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_last_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$last((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_last_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(
            object, sizeof(size_t), coral_compare_size_t));
    size_t i = 0;
    assert_false(coral$tree_set$last(object, (void **) &i));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_last(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(
            object, sizeof(size_t), coral_compare_size_t));
    size_t i = 98;
    assert_true(coral$tree_set$insert(object, &i));
    i = 12;
    assert_true(coral$tree_set$last(object, (void **) &i));
    assert_int_equal(98, i);
    assert_true(coral$tree_set$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_next_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$next(NULL, (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_next_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$next((void *) 1, NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$next((void *) 1, (void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_next_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(
            object, sizeof(size_t), coral_compare_size_t));
    size_t i = 9;
    assert_false(coral$tree_set$next(object, (const void *) i, (void **) &i));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    assert_true(coral$tree_set$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_next_error_on_end_of_sequence(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(
            object, sizeof(size_t), coral_compare_size_t));
    size_t i = 9;
    assert_true(coral$tree_set$insert(object, &i));
    i = 9;
    assert_false(coral$tree_set$next(object, &i, (void **) &i));
    assert_int_equal(CORAL_ERROR_END_OF_SEQUENCE, coral_error);
    assert_true(coral$tree_set$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_next(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(
            object, sizeof(size_t), coral_compare_size_t));
    size_t i = 0;
    assert_true(coral$tree_set$insert(object, &i));
    i = 9;
    assert_true(coral$tree_set$insert(object, &i));
    i = 0;
    assert_true(coral$tree_set$next(object, &i, (void **) &i));
    assert_int_equal(i, 9);
    assert_true(coral$tree_set$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_prev_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$prev(NULL, (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_prev_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$prev((void *) 1, NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$tree_set$prev((void *) 1, (void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_prev_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(
            object, sizeof(size_t), coral_compare_size_t));
    size_t i = 9;
    assert_false(coral$tree_set$prev(object, (const void *) i, (void **) &i));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    assert_true(coral$tree_set$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_prev_error_on_end_of_sequence(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(
            object, sizeof(size_t), coral_compare_size_t));
    size_t i = 9;
    assert_true(coral$tree_set$insert(object, &i));
    i = 9;
    assert_false(coral$tree_set$prev(object, &i, (void **) &i));
    assert_int_equal(CORAL_ERROR_END_OF_SEQUENCE, coral_error);
    assert_true(coral$tree_set$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_prev(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$tree_set *object;
    assert_true(coral$tree_set$alloc(&object));
    assert_true(coral$tree_set$init(
            object, sizeof(size_t), coral_compare_size_t));
    size_t i = 0;
    assert_true(coral$tree_set$insert(object, &i));
    i = 9;
    assert_true(coral$tree_set$insert(object, &i));
    i = 9;
    assert_true(coral$tree_set$prev(object, &i, (void **) &i));
    assert_int_equal(i, 0);
    assert_true(coral$tree_set$invalidate(object, NULL));
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
            cmocka_unit_test(check_prev_error_on_end_of_sequence),
            cmocka_unit_test(check_prev),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
