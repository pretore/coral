#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <coral.h>

#include "private/coral.h"
#include "private/array.h"

static void check_alloc_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_alloc(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_alloc(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array = NULL;
    assert_true(coral_array_alloc(&array));
    assert_int_equal(0, array->capacity);
    assert_int_equal(0, array->count);
    assert_int_equal(0, array->size);
    assert_ptr_equal(NULL, array->data);
    assert_true(coral_array_destroy(array));
    coral_error = CORAL_ERROR_NONE;
}

static void check_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_init(NULL,
                                  0,
                                  0,
                                  NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array = NULL;
    assert_true(coral_array_alloc(&array));
    assert_ptr_equal(NULL, array->data);
    assert_true(coral_array_init(array,
                                 10,
                                 sizeof(void *),
                                 NULL));
    assert_ptr_not_equal(NULL, array->data);
    assert_int_equal(16 * 1.5, array->capacity);
    assert_int_equal(10, array->count);
    assert_int_equal(sizeof(void *), array->size);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_init_empty(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array = NULL;
    assert_true(coral_array_alloc(&array));
    assert_ptr_equal(NULL, array->data);
    assert_true(coral_array_init(array,
                                 0,
                                 sizeof(void *),
                                 NULL));
    assert_ptr_not_equal(NULL, array->data);
    assert_int_equal(0, array->count);
    assert_int_equal(sizeof(void *), array->size);;
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_retain_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_retain(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_array_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_release_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_release(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_array_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_autorelease_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_autorelease(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_array_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_get_capacity_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_get_capacity(object, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_array_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_count_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_get_count(object, (size_t *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_array_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_count_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_set_count(object, 10));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_array_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_size_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_get_size(object, (size_t *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_array_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_set(object, 1, NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_array_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_get(object, 1, (struct coral_array_item *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_array_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_sort_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_sort(object,
                                  (int (*)(const struct coral_array_item *,
                                           const struct coral_array_item *)) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_array_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_insert(object, 1, NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_array_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_add_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_add(object, NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_array_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_delete(object, 0));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_array_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_remove_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_remove(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    coral_array_destroy(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_retain_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_retain(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_release_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_release(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_autorelease_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_autorelease(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_capacity_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values;
    assert_false(coral_array_get_capacity(NULL, &values));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_capacity_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_get_capacity((struct coral_array *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_capacity(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_of_objects(&object));
    struct coral_range_values values;
    assert_true(coral_array_get_capacity(object, &values));
    assert_int_equal(16, values.first);
    assert_int_equal(SIZE_MAX, values.last);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_count_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    size_t count = 0;
    assert_false(coral_array_get_count(NULL, &count));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_count_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_get_count((struct coral_array *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_count(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array = NULL;
    assert_true(coral_array_alloc(&array));
    assert_true(coral_array_init(array,
                                 10,
                                 sizeof(void *),
                                 NULL));
    size_t count = 0;
    assert_true(coral_array_get_count(array, &count));
    assert_int_equal(10, count);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_count_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_set_count(NULL, 10));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_count_error_on_memory_allocation(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array = NULL;
    assert_true(coral_array_alloc(&array));
    assert_true(coral_array_init(array,
                                 16,
                                 sizeof(void *),
                                 NULL));
    assert_int_equal(16, array->count);
    assert_false(coral_array_set_count(array, SIZE_MAX));
    assert_int_equal(CORAL_ERROR_MEMORY_ALLOCATION_FAILED, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_count_to_zero(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array = NULL;
    assert_true(coral_array_alloc(&array));
    assert_true(coral_array_init(array,
                                 16,
                                 sizeof(void *),
                                 NULL));
    assert_int_equal(16, array->count);
    assert_true(coral_array_set_count(array, 0));
    assert_int_equal(0, array->count);
    assert_int_equal(24, array->capacity);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_count_to_lower_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array = NULL;
    assert_true(coral_array_alloc(&array));
    assert_true(coral_array_init(array,
                                 16,
                                 sizeof(void *),
                                 NULL));
    assert_int_equal(16, array->count);
    assert_true(coral_array_set_count(array, 8));
    assert_int_equal(8, array->count);
    assert_int_equal(24, array->capacity);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_count_to_higher_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array = NULL;
    assert_true(coral_array_alloc(&array));
    assert_true(coral_array_init(array,
                                 8,
                                 sizeof(void *),
                                 NULL));
    assert_int_equal(8, array->count);
    assert_true(coral_array_set_count(array, 16));
    assert_int_equal(16, array->count);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_count_no_change(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array = NULL;
    assert_true(coral_array_alloc(&array));
    assert_true(coral_array_init(array,
                                 8,
                                 sizeof(void *),
                                 NULL));
    assert_int_equal(8, array->count);
    assert_true(coral_array_set_count(array, 8));
    assert_int_equal(8, array->count);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_size_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    size_t size = 0;
    assert_false(coral_array_get_size(NULL, &size));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_size_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_get_size((struct coral_array *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_size(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array = NULL;
    assert_true(coral_array_alloc(&array));
    assert_true(coral_array_init(array,
                                 10,
                                 sizeof(void *),
                                 NULL));
    size_t size = 0;
    assert_true(coral_array_get_size(array, &size));
    assert_int_equal(sizeof(void *), size);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_set(NULL, 10, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_error_on_item_out_of_bounds(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array = NULL;
    assert_true(coral_array_alloc(&array));
    assert_true(coral_array_init(array,
                                 0,
                                 sizeof(void *),
                                 NULL));
    struct coral_array_item item = {
            .size = array->size,
            .data = (void *) 1
    };
    assert_false(coral_array_set(array, array->count, &item));
    assert_int_equal(CORAL_ERROR_INDEX_OUT_OF_BOUNDS, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_larger_size(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array = NULL;
    assert_true(coral_array_alloc(&array));
    assert_true(coral_array_init(array,
                                 1,
                                 sizeof(void *) / 2,
                                 NULL));
    struct coral_array_item item = {
            .size = sizeof(void *),
            .data = array
    };
    assert_true(coral_array_set(array, 0, &item));
    assert_ptr_not_equal(item.data, array->data);
    assert_memory_equal(item.data, array->data, item.size);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

// FIXME: check_array_set_smaller_size ... should have no affect

static void check_set(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array = NULL;
    assert_true(coral_array_alloc(&array));
    assert_true(coral_array_init(array,
                                 1,
                                 sizeof(size_t),
                                 NULL));
    const size_t value = SIZE_MAX;
    struct coral_array_item item = {
            .size = sizeof(size_t),
            .data = (void *) &value
    };
    assert_true(coral_array_set(array, 0, &item));
    assert_ptr_not_equal(item.data, array->data);
    assert_memory_equal(item.data, array->data, item.size);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_get(NULL, 0, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_get((void *) 1, 0, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_error_on_item_out_of_bounds(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array = NULL;
    assert_true(coral_array_alloc(&array));
    assert_true(coral_array_init(array,
                                 10,
                                 sizeof(void *),
                                 NULL));
    struct coral_array_item item;
    assert_false(coral_array_get(array, array->count, &item));
    assert_int_equal(CORAL_ERROR_INDEX_OUT_OF_BOUNDS, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_get(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array = NULL;
    assert_true(coral_array_alloc(&array));
    assert_true(coral_array_init(array,
                                 10,
                                 sizeof(void *),
                                 NULL));
    struct coral_array_item item;
    assert_true(coral_array_get(array, 4, &item));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    assert_int_equal(0, *(size_t *) item.data);
    assert_int_equal(sizeof(void *), item.size);
    item.size = 4;
    item.data = &check_get;
    assert_true(coral_array_set(array, 4, &item));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    assert_true(coral_array_get(array, 4, &item));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    assert_int_equal(sizeof(void *), item.size);
    assert_ptr_not_equal(&check_get, item.data);
    assert_memory_equal(&check_get, item.data, 4);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_of_objects_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_of_objects(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_of_objects(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array = NULL;
    assert_true(coral_array_of_objects(&array));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static int compare(const struct coral_array_item *a,
                   const struct coral_array_item *b) {
    assert_non_null(a);
    assert_non_null(b);
    assert_int_equal(a->size, b->size);
    return memcmp(a->data, b->data, a->size);
}

static void check_sort_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_sort(NULL, compare));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_sort_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_sort((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_sort(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array = NULL;
    assert_true(coral_array_alloc(&array));
    assert_true(coral_array_init(array,
                                 0,
                                 sizeof(unsigned char),
                                 NULL));
    const unsigned char values[10] = {
            9, 8, 7, 6, 5,
            4, 3, 2, 1, 0
    };
    assert_true(coral_array_set_count(array, sizeof(values)));
    struct coral_array_item item = {
            .size = 1
    };
    for (size_t i = 0, limit = sizeof(values); i < limit; i++) {
        item.data = (void *) &values[i];
        assert_true(coral_array_set(array, i, &item));
    }
    assert_true(coral_array_sort(array, compare));
    for (size_t i = 0, limit = sizeof(values); i < limit; i++) {
        assert_true(coral_array_get(array, i, &item));
        assert_int_equal((unsigned char) i, *(unsigned char *) item.data);
    }
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

// TODO: check_array_find ...

static void check_insert_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_insert(NULL, 1, (struct coral_array_item *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array_item item = {
            .size = 0,
            .data = (void *) 1
    };
    assert_false(coral_array_insert((struct coral_array *) 1, 1, &item));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    item.size = 1;
    item.data = NULL;
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_insert((struct coral_array *) 1, 1, &item));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_error_on_out_of_bounds(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array;
    assert_true(coral_array_alloc(&array));
    assert_true(coral_array_init(array,
                                 10,
                                 sizeof(void *),
                                 NULL));
    assert_false(coral_array_insert(array, array->count, NULL));
    assert_int_equal(CORAL_ERROR_INDEX_OUT_OF_BOUNDS, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_last_item(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array;
    assert_true(coral_array_alloc(&array));
    assert_true(coral_array_init(array,
                                 10,
                                 sizeof(void *),
                                 NULL));
    struct coral_array_item item = {
            .size = array->size,
            .data = check_insert_last_item
    };
    assert_true(coral_array_insert(array, array->count - 1, &item));
    assert_int_equal(11, array->count);
    assert_int_equal(24, array->capacity);
    item.size = 0;
    item.data = NULL;
    assert_true(coral_array_get(array, array->count - 2, &item));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    assert_int_equal(array->size, item.size);
    assert_ptr_not_equal(&check_insert_last_item, item.data);
    assert_memory_equal(&check_insert_last_item, item.data,
                        item.size);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_first_item(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array;
    assert_true(coral_array_alloc(&array));
    assert_true(coral_array_init(array,
                                 10,
                                 sizeof(void *),
                                 NULL));
    struct coral_array_item item = {
            .size = array->size,
            .data = check_insert_first_item
    };
    assert_true(coral_array_insert(array, 0, &item));
    assert_int_equal(11, array->count);
    assert_int_equal(24, array->capacity);
    item.size = 0;
    item.data = NULL;
    assert_true(coral_array_get(array, 0, &item));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    assert_int_equal(array->size, item.size);
    assert_ptr_not_equal(&check_insert_first_item, item.data);
    assert_memory_equal(&check_insert_first_item, item.data, item.size);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_no_capacity_increase(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array;
    assert_true(coral_array_alloc(&array));
    assert_true(coral_array_init(array,
                                 10,
                                 sizeof(void *),
                                 NULL));
    assert_true(coral_array_set_count(array, 6));
    assert_int_equal(24, array->capacity);
    assert_int_equal(6, array->count);
    struct coral_array_item item = {
            .size = array->size,
            .data = check_insert_no_capacity_increase
    };
    assert_true(coral_array_insert(array, 4, &item));
    assert_int_equal(24, array->capacity);
    assert_int_equal(7, array->count);
    item.size = 0;
    item.data = NULL;
    assert_true(coral_array_get(array, 4, &item));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    assert_int_equal(array->size, item.size);
    assert_ptr_not_equal(&check_insert_no_capacity_increase, item.data);
    assert_memory_equal(&check_insert_no_capacity_increase,
                        item.data, item.size);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_on_error_restore_count(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *array;
    assert_true(coral_array_alloc(&array));
    assert_true(coral_array_init(array,
                                 10,
                                 sizeof(void *),
                                 NULL));
    struct coral_array_item item = {
            .size = array->size,
            .data = check_insert_on_error_restore_count
    };
    assert_true(coral_array_set(array, 1, &item));
    item.size = SIZE_MAX;
    assert_false(coral_array_insert(array, 0, &item));
    assert_int_equal(CORAL_ERROR_MEMORY_ALLOCATION_FAILED, coral_error);
    assert_int_equal(10, array->count);
    assert_int_equal(24, array->capacity);
    item.size = 0;
    item.data = NULL;
    coral_error = CORAL_ERROR_NONE;
    assert_true(coral_array_get(array, 1, &item));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    assert_int_equal(array->size, item.size);
    assert_ptr_not_equal(&check_insert_on_error_restore_count, item.data);
    assert_memory_equal(&check_insert_on_error_restore_count, item.data,
                        item.size);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

// TODO: check array_insert with increase in capacity
// TODO: check array_insert with error_on_capacity_limit_reached

static void check_add_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_add(NULL, NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_add_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array_item item = {
            .data = (void *) 1
    };
    assert_false(coral_array_add((struct coral_array *) 1, &item));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    item.data = NULL;
    item.size = 1;
    assert_false(coral_array_add((struct coral_array *) 1, &item));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_add(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *capacity;
    struct coral_range_values value = {0, 1};
    assert_true(coral_range_of_delta(&capacity, &value, 1));
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object,
                                 0,
                                 sizeof(void *),
                                 capacity));
    struct coral_array_item item = {
            .size = 1,
            .data = &check_add
    };
    assert_true(coral_array_add(object, &item));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_add_error_capacity_limit_reached(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *capacity;
    struct coral_range_values value = {0, 1};
    assert_true(coral_range_of_delta(&capacity, &value, 1));
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object,
                                 0,
                                 sizeof(void *),
                                 capacity));
    struct coral_array_item item = {
            .size = 1,
            .data = &check_add_error_capacity_limit_reached
    };
    assert_true(coral_array_add(object, &item));
    assert_false(coral_array_add(object, &item));
    assert_int_equal(CORAL_ERROR_CAPACITY_LIMIT_REACHED, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_delete(NULL, 0));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete_error_on_index_out_of_bounds(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *capacity;
    struct coral_range_values value = {0, 1};
    assert_true(coral_range_of_delta(&capacity, &value, 1));
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object,
                                 0,
                                 sizeof(void *),
                                 capacity));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    assert_false(coral_array_delete(object, 0));
    assert_int_equal(CORAL_ERROR_INDEX_OUT_OF_BOUNDS, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *capacity;
    struct coral_range_values value = {0, 2};
    assert_true(coral_range_of_delta(&capacity, &value, 1));
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object,
                                 1,
                                 sizeof(void *),
                                 capacity));
    assert_int_equal(1, object->count);
    assert_true(coral_array_delete(object, 0));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    assert_int_equal(0, object->count);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_remove_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_remove(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_remove(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range *capacity;
    struct coral_range_values value = {0, 3};
    assert_true(coral_range_of_delta(&capacity, &value, 1));
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object,
                                 2,
                                 sizeof(void *),
                                 capacity));
    struct coral_array_item item = {
            .size = 1,
            .data = &check_remove
    };
    assert_true(coral_array_set(object, object->count - 1, &item));
    assert_int_equal(2, object->count);
    assert_true(coral_array_remove(object));
    assert_int_equal(CORAL_ERROR_NONE, coral_error);
    assert_int_equal(1, object->count);
    assert_true(coral_array_get(object, object->count - 1, &item));
    assert_int_equal(object->size, item.size);
    for (size_t i = 0; i < object->size; i++) {
        unsigned char *data = (unsigned char *)item.data;
        assert_int_equal(0, data[i]);
    }
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_alloc_error_on_null_argument_ptr),
            cmocka_unit_test(check_alloc),
            cmocka_unit_test(check_init_error_on_null_object_ptr),
            cmocka_unit_test(check_init),
            cmocka_unit_test(check_init_empty),
            cmocka_unit_test(check_retain_error_on_object_uninitialized),
            cmocka_unit_test(check_release_error_on_object_uninitialized),
            cmocka_unit_test(check_autorelease_error_on_object_uninitialized),
            cmocka_unit_test(check_get_capacity_error_on_object_uninitialized),
            cmocka_unit_test(check_get_count_error_on_object_uninitialized),
            cmocka_unit_test(check_set_count_error_on_object_uninitialized),
            cmocka_unit_test(check_get_size_error_on_object_uninitialized),
            cmocka_unit_test(check_set_error_on_object_uninitialized),
            cmocka_unit_test(check_get_error_on_object_uninitialized),
            cmocka_unit_test(check_sort_error_on_object_uninitialized),
            cmocka_unit_test(check_insert_error_on_object_uninitialized),
            cmocka_unit_test(check_add_error_on_object_uninitialized),
            cmocka_unit_test(check_delete_error_on_object_uninitialized),
            cmocka_unit_test(check_remove_error_on_object_uninitialized),
            cmocka_unit_test(check_retain_error_on_null_object_ptr),
            cmocka_unit_test(check_release_error_on_null_object_ptr),
            cmocka_unit_test(check_autorelease_error_on_null_object_ptr),
            cmocka_unit_test(check_get_capacity_error_on_null_object_ptr),
            cmocka_unit_test(check_get_capacity_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_capacity),
            cmocka_unit_test(check_get_count_error_on_null_object_ptr),
            cmocka_unit_test(check_get_count_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_count),
            cmocka_unit_test(check_set_count_error_on_null_object_ptr),
            cmocka_unit_test(check_set_count_error_on_memory_allocation),
            cmocka_unit_test(check_set_count_to_zero),
            cmocka_unit_test(check_set_count_to_lower_value),
            cmocka_unit_test(check_set_count_to_higher_value),
            cmocka_unit_test(check_set_count_no_change),
            cmocka_unit_test(check_get_size_error_on_null_object_ptr),
            cmocka_unit_test(check_get_size_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_size),
            cmocka_unit_test(check_set_error_on_null_object_ptr),
            cmocka_unit_test(check_set_error_on_item_out_of_bounds),
            cmocka_unit_test(check_set_larger_size),
            cmocka_unit_test(check_set),
            cmocka_unit_test(check_get_error_on_null_object_ptr),
            cmocka_unit_test(check_get_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_error_on_item_out_of_bounds),
            cmocka_unit_test(check_get),
            cmocka_unit_test(check_of_objects_error_on_null_argument_ptr),
            cmocka_unit_test(check_of_objects),
            cmocka_unit_test(check_sort_error_on_null_object_ptr),
            cmocka_unit_test(check_sort_error_on_null_argument_ptr),
            cmocka_unit_test(check_sort),
            cmocka_unit_test(check_insert_error_on_null_object_ptr),
            cmocka_unit_test(check_insert_error_on_invalid_value),
            cmocka_unit_test(check_insert_error_on_out_of_bounds),
            cmocka_unit_test(check_insert_last_item),
            cmocka_unit_test(check_insert_first_item),
            cmocka_unit_test(check_insert_no_capacity_increase),
            cmocka_unit_test(check_insert_on_error_restore_count),
            cmocka_unit_test(check_delete_error_on_null_object_ptr),
            cmocka_unit_test(check_delete_error_on_index_out_of_bounds),
            cmocka_unit_test(check_delete),
            cmocka_unit_test(check_add_error_on_null_object_ptr),
            cmocka_unit_test(check_add_error_on_invalid_value),
            cmocka_unit_test(check_add),
            cmocka_unit_test(check_add_error_capacity_limit_reached),
            cmocka_unit_test(check_remove_error_on_null_object_ptr),
            cmocka_unit_test(check_remove),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}