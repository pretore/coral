#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <coral.h>

#include "private/array.h"
#include "private/coral.h"
#include "test/cmocka.h"

static void check_invalidate_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$invalidate(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_invalidate(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 10, sizeof(size_t)));
    assert_int_equal(15, object.capacity);
    assert_int_equal(10, object.count);
    assert_int_equal(sizeof(size_t), object.size);
    assert_int_equal(2, coral$atomic_load(&object.id));
    assert_non_null(object.limit_ref);
    assert_non_null(object.data);
    assert_true(coral$array$invalidate(&object, NULL));
    assert_int_equal(0, object.capacity);
    assert_int_equal(0, object.count);
    assert_int_equal(0, object.size);
    assert_int_equal(3, coral$atomic_load(&object.id));
    assert_null(object.limit_ref);
    assert_null(object.data);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$init(NULL, (void *) 1, 10, sizeof(size_t)));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_init_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {100, 200};
    struct coral_range *range;
    assert_true(coral_range_of_delta(&range, values, 10));
    struct coral$array object = {};
    assert_true(coral$array$init(&object, range, 10, sizeof(size_t)));
    assert_true(coral$array$invalidate(&object, NULL));
    assert_false(coral$array$init(&object, range, 210, sizeof(size_t)));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_init_error_on_memory_allocation_failed(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_false(coral$array$init(&object, NULL, SIZE_MAX, sizeof(size_t)));
    assert_int_equal(CORAL_ERROR_MEMORY_ALLOCATION_FAILED, coral_error);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 0, 0));
    assert_int_equal(0, object.capacity);
    assert_int_equal(0, object.count);
    assert_int_equal(0, object.size);
    assert_int_equal(1, coral$atomic_load(&object.id));
    assert_non_null(object.limit_ref);
    assert_null(object.data);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_capacity_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$get_capacity(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_capacity_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$get_capacity((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_capacity(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {473, 1122};
    struct coral_range *limit;
    assert_true(coral_range_of_delta(&limit, values, -20));
    struct coral$array object = {};
    assert_true(coral$array$init(&object, limit, 10, sizeof(size_t)));
    size_t capacity;
    assert_true(coral$array$get_capacity(&object, &capacity));
    assert_int_equal(values.first, capacity);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_capacity_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$set_capacity(NULL, 0));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_capacity_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {100, 200};
    struct coral_range *limit;
    assert_true(coral_range_of_delta(&limit, values, 7));
    struct coral$array object = {};
    assert_true(coral$array$init(&object, limit, 0, 0));
    assert_false(coral$array$set_capacity(&object, 99));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_capacity_error_on_memory_allocation_failed(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 0, sizeof(size_t)));
    assert_false(coral$array$set_capacity(&object, SIZE_MAX));
    assert_int_equal(CORAL_ERROR_MEMORY_ALLOCATION_FAILED, coral_error);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_capacity(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 10, sizeof(size_t)));
    assert_int_equal(15, object.capacity);
    assert_int_equal(10, object.count);
    assert_true(coral$array$set_capacity(&object, 7));
    assert_int_equal(7, object.capacity);
    assert_int_equal(7, object.count);
    assert_true(coral$array$set_capacity(&object, 20));
    assert_int_equal(20, object.capacity);
    assert_int_equal(7, object.count);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_count_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$get_count(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_count_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$get_count((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_count(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 10, sizeof(size_t)));
    size_t count;
    assert_true(coral$array$get_count(&object, &count));
    assert_int_equal(10, count);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_count_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$set_count(NULL, 0));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_count_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {100, 200};
    struct coral_range *limit;
    assert_true(coral_range_of_delta(&limit, values, 7));
    struct coral$array object = {};
    assert_true(coral$array$init(&object, limit, 10, sizeof(size_t)));
    assert_false(coral$array$set_count(&object, 1 + values.last));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_count_error_on_memory_allocation_failed(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {0, SIZE_MAX};
    struct coral_range *limit;
    assert_true(coral_range_of_delta(&limit, values, 1));
    struct coral$array object = {};
    assert_true(coral$array$init(&object, limit, 0, sizeof(size_t)));
    assert_false(coral$array$set_count(&object, SIZE_MAX - 1));
    assert_int_equal(CORAL_ERROR_MEMORY_ALLOCATION_FAILED, coral_error);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_count(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 0, sizeof(size_t)));
    assert_int_equal(0, object.count);
    assert_true(coral$array$set_count(&object, 1));
    assert_int_equal(1, object.count);
    size_t value = 0;
    assert_memory_equal(&value, object.data, sizeof(size_t));
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_size_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$get_size(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_size_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$get_size((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_size(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 0, sizeof(size_t)));
    assert_int_equal(sizeof(size_t), object.size);
    assert_true(coral$array$invalidate(&object, NULL));
    assert_true(coral$array$init(&object, NULL, 0, 0));
    assert_int_equal(0, object.size);
    assert_true(coral$array$invalidate(&object, NULL));
    assert_true(coral$array$init(&object, NULL, 0, 13));
    assert_int_equal(13, object.size);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$get(NULL, 0, (void *) 1));
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$get((void *) 1, 0, NULL));
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_error_on_index_out_of_bounds(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 2, sizeof(size_t)));
    struct coral$array$item i = {};
    assert_false(coral$array$get(&object, 10, &i));
    assert_int_equal(CORAL_ERROR_INDEX_OUT_OF_BOUNDS, coral_error);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_get(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 2, sizeof(size_t)));
    struct coral$array$item i = {};
    assert_true(coral$array$get(&object, 1, &i));
    assert_int_equal(i.size, object.size);
    assert_non_null(i.data);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$set(NULL, 0, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array$item i = {};
    assert_false(coral$array$set((void *) 1, 0, &i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_error_on_index_out_of_bounds(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 2, sizeof(size_t)));
    assert_false(coral$array$set(&object, 10, NULL));
    assert_int_equal(CORAL_ERROR_INDEX_OUT_OF_BOUNDS, coral_error);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_error_on_memory_allocation_failed(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 2, sizeof(size_t)));
    struct coral$array$item i = {
            .size = SIZE_MAX,
            .data = check_set_error_on_memory_allocation_failed
    };
    assert_false(coral$array$set(&object, 1, &i));
    assert_int_equal(CORAL_ERROR_MEMORY_ALLOCATION_FAILED, coral_error);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_set(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 2, sizeof(char)));
    assert_int_equal(object.size, sizeof(char));
    char c = 0;
    assert_memory_equal(object.data, &c, sizeof(char));
    size_t s = SIZE_MAX;
    struct coral$array$item i = {
            .size = sizeof(s),
            .data = &s
    };
    assert_true(coral$array$set(&object, 0, &i));
    assert_int_equal(object.size, i.size);
    assert_memory_equal(object.data, &s, object.size);
    assert_true(coral$array$set(&object, 0, NULL));
    s = 0;
    assert_memory_equal(object.data, &s, object.size);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_add_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$add(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_add_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array$item i = {};
    assert_false(coral$array$add((void *) 1, &i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_add_error_on_memory_allocation_failed(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 2, sizeof(size_t)));
    struct coral$array$item i = {
            .size = SIZE_MAX,
            .data = check_add_error_on_memory_allocation_failed
    };
    assert_false(coral$array$add(&object, &i));
    assert_int_equal(CORAL_ERROR_MEMORY_ALLOCATION_FAILED, coral_error);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_add(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 0, sizeof(char)));
    assert_int_equal(1, coral$atomic_load(&object.id));
    assert_int_equal(sizeof(char), object.size);
    char c = 10;
    struct coral$array$item i = {
            .size = sizeof(c),
            .data = &c
    };
    assert_true(coral$array$add(&object, &i));
    assert_int_equal(4, coral$atomic_load(&object.id));
    assert_int_equal(sizeof(char), object.size);
    assert_int_equal(1, object.count);
    assert_memory_equal(object.data, &c, object.size);
    size_t s = SIZE_MAX;
    i.size = sizeof(s);
    i.data = &s;
    assert_true(coral$array$add(&object, &i));
    assert_int_equal(7, coral$atomic_load(&object.id));
    assert_int_equal(sizeof(size_t), object.size);
    assert_int_equal(2, object.count);
    assert_memory_equal(object.data + sizeof(size_t), &s, object.size);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_remove_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$remove(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_remove_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 0, sizeof(char)));
    assert_false(coral$array$remove(&object));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_remove(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 10, sizeof(char)));
    assert_int_equal(2, coral$atomic_load(&object.id));
    assert_int_equal(10, object.count);
    assert_true(coral$array$remove(&object));
    assert_int_equal(3, coral$atomic_load(&object.id));
    assert_int_equal(9, object.count);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$insert(NULL, 0, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array$item i = {};
    assert_false(coral$array$insert((void *) 1, 0, &i));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_error_on_index_out_of_bounds(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 2, sizeof(char)));
    assert_false(coral$array$insert(&object, object.count, NULL));
    assert_int_equal(CORAL_ERROR_INDEX_OUT_OF_BOUNDS, coral_error);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_error_on_memory_allocation_failed(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 2, sizeof(char)));
    struct coral$array$item i = {
            .size = SIZE_MAX,
            .data = check_insert_error_on_memory_allocation_failed
    };
    assert_false(coral$array$insert(&object, 0, &i));
    assert_int_equal(CORAL_ERROR_MEMORY_ALLOCATION_FAILED, coral_error);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 2, sizeof(char)));
    assert_int_equal(2, coral$atomic_load(&object.id));
    char c = 0;
    assert_memory_equal(object.data, &c, sizeof(c));
    c = 10;
    struct coral$array$item i = {
            .size = sizeof(c),
            .data = &c
    };
    assert_true(coral$array$insert(&object, 0, &i));
    assert_int_equal(5, coral$atomic_load(&object.id));
    assert_int_equal(3, object.count);
    assert_memory_equal(object.data, &c, sizeof(c));
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete_error_on_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$delete(NULL, 0));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete_error_on_index_out_of_bounds(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 2, sizeof(char)));
    assert_false(coral$array$delete(&object, object.count));
    assert_int_equal(CORAL_ERROR_INDEX_OUT_OF_BOUNDS, coral_error);
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 2, sizeof(size_t)));
    assert_int_equal(2, object.count);
    assert_int_equal(2, coral$atomic_load(&object.id));
    assert_true(coral$array$delete(&object, 0));
    assert_int_equal(1, object.count);
    assert_int_equal(3, coral$atomic_load(&object.id));
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_sort_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$sort(NULL, (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_sort_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$sort((void *) 1, (void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_sort(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 0, sizeof(size_t)));
    struct coral$array$item item = {
            .size = object.size
    };
    size_t limit = 10;
    for (size_t i = 0; i < limit; i++) {
        size_t o = (limit - 1) - i;
        item.data = &o;
        assert_true(coral$array$add(&object, &item));
    }
    assert_true(coral$array$sort(&object, NULL, memcmp));
    for (size_t i = 0; i < limit; i++) {
        assert_true(coral$array$get(&object, i, &item));
        assert_memory_equal(&i, item.data, object.size);
    }
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_sort_with_range(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 0, sizeof(size_t)));
    struct coral$array$item item = {
            .size = object.size
    };
    size_t limit = 10;
    for (size_t i = 0; i < limit; i++) {
        size_t o = (limit - 1) - i;
        item.data = &o;
        assert_true(coral$array$add(&object, &item));
    }
    struct coral_range_values values = {1, 8};
    assert_true(coral$array$sort(&object, &values, memcmp));
    for (size_t i = 0; i < limit; i++) {
        assert_true(coral$array$get(&object, i, &item));
        if (i >= values.first && i <= values.last) {
            assert_memory_equal(&i, item.data, object.size);
        } else {
            size_t o = (limit - 1) - i;
            assert_memory_equal(&o, item.data, object.size);
        }
    }
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void
check_search_pattern_invalidate_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$search_pattern$invalidate(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_search_pattern_init_linear_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$search_pattern$init_linear(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_search_pattern_init_linear(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array$search_pattern object = {};
    assert_true(coral$array$search_pattern$init_linear(&object));
    assert_true(coral$array$search_pattern$invalidate(&object));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void
check_search_pattern_init_binary_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$search_pattern$init_binary(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_search_pattern_init_binary(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array$search_pattern object = {};
    assert_true(coral$array$search_pattern$init_binary(&object));
    assert_true(coral$array$search_pattern$invalidate(&object));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_find_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$find(NULL, (void *) 1, (void *) 1, (void *) 1,
                                  (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_find_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$find((void *) 1, (void *) 1, NULL,
                                  (void *) 1, (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$find((void *) 1, (void *) 1, (void *) 1,
                                  (void *) 1, NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$array$find((void *) 1, (void *) 1, (void *) 1,
                             (void *) 1, (void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_find_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array$item needle = {
            .data = (void *) 1
    };
    assert_false(coral$array$find((void *) 1, (void *) 1, (void *)1,
                                  &needle, (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    needle.size = sizeof(size_t);
    needle.data = NULL;
    assert_false(coral$array$find((void *) 1, (void *) 1, (void *)1,
                                  &needle, (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_find_with_linear_search_pattern(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 0, sizeof(size_t)));
    struct coral$array$item item = {
            .size = object.size
    };
    size_t limit = 10;
    for (size_t i = 0; i < limit; i++) {
        size_t o = (limit - 1) - i;
        item.data = &o;
        assert_true(coral$array$add(&object, &item));
    }
    struct coral$array$search_pattern search_pattern = {};
    assert_true(coral$array$search_pattern$init_linear(&search_pattern));
    size_t value = 6;
    item.data = &value;
    size_t at = 0;
    assert_true(coral$array$find(&object, NULL, &search_pattern,
                                 &item, memcmp, &at));
    assert_int_equal(3, at);
    assert_int_equal(value, ((size_t *) object.data)[at]);
    assert_true(coral$array$search_pattern$invalidate(&search_pattern));
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static int $cmp_nullable(const void *a, const void *b, size_t size) {
    if (a) {
        return memcmp(a, b, size);
    }
    const unsigned char *data = b;
    for (size_t i = 0; i < size; i++) {
        if (data[i]) {
            return -data[i];
        }
    }
    return 0;
}

static void check_find_null_items_with_linear_search_pattern(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 0, sizeof(size_t)));
    struct coral$array$item item = {
            .size = object.size
    };
    size_t limit = 10;
    for (size_t i = 1; i <= limit; i++) {
        item.data = &i;
        assert_true(coral$array$add(&object, 7 == i ? NULL : &item));
    }
    struct coral$array$search_pattern search_pattern = {};
    assert_true(coral$array$search_pattern$init_linear(&search_pattern));
    size_t at = 0;
    assert_true(coral$array$find(&object, NULL, &search_pattern,
                                 NULL, $cmp_nullable, &at));
    assert_int_equal(6, at);
    assert_int_equal(0, ((size_t *) object.data)[at]);
    assert_true(coral$array$search_pattern$invalidate(&search_pattern));
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_find_with_binary_search_pattern(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 0, sizeof(size_t)));
    struct coral$array$item item = {
            .size = object.size
    };
    size_t limit = 10;
    for (size_t i = 1; i <= limit; i++) {
        item.data = &i;
        assert_true(coral$array$add(&object, &item));
    }
    struct coral$array$search_pattern search_pattern = {};
    assert_true(coral$array$search_pattern$init_binary(&search_pattern));
    size_t value = 6;
    item.data = &value;
    size_t at = 0;
    assert_true(coral$array$find(&object, NULL, &search_pattern,
                                 &item, memcmp, &at));
    assert_int_equal(5, at);
    assert_int_equal(value, ((size_t *) object.data)[at]);
    assert_true(coral$array$search_pattern$invalidate(&search_pattern));
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void
check_find_with_binary_search_patten_with_duplicates(void **state) {
    coral_error = CORAL_ERROR_NONE;
    unsigned char array[] = {
            1, 2, 4, 4, 4, 5, 6, 7
    };
    struct coral$array object = {};
    assert_true(coral$array$init(&object, NULL, 0, sizeof(char)));
    struct coral$array$item item = {
            .size = object.size
    };
    for (size_t i = 0; i < sizeof(array); i++) {
        item.data = &array[i];
        assert_true(coral$array$add(&object, &item));
    }
    struct coral$array$search_pattern search_pattern = {};
    assert_true(coral$array$search_pattern$init_binary(&search_pattern));
    size_t value = 4;
    item.data = &value;
    size_t at = 0;
    assert_true(coral$array$find(&object, NULL, &search_pattern,
                                 &item, memcmp, &at));
    assert_int_equal(value, ((char *) object.data)[at]);
    assert_true(coral$array$search_pattern$invalidate(&search_pattern));
    assert_true(coral$array$invalidate(&object, NULL));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_class_error_on_null_argument(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_class(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_class(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_array_class(&class));
    assert_non_null(class);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_destroy_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_destroy(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_alloc_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_alloc(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_alloc(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_init(NULL, (void *)1, 10));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {100, 200};
    struct coral_range *range;
    assert_true(coral_range_of_delta(&range, values, 10));
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, range, 10));
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_init(object, range, 210));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    assert_true(coral_array_destroy(object));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init_error_on_memory_allocation_failed(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_init(object, NULL, SIZE_MAX));
    assert_int_equal(CORAL_ERROR_MEMORY_ALLOCATION_FAILED, coral_error);
    assert_true(coral_array_destroy(object));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, 10));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_hash_code(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_hash_code((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    size_t code;
    assert_false(coral_array_hash_code(object, &code));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_array_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, 10));
    size_t code = 0;
    assert_true(coral_array_hash_code(object, &code));
    assert_int_not_equal(code, 0);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_is_equal(NULL, (void *)1, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_is_equal((void *)1, NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_is_equal((void *)1, (void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, 10));
    struct coral_array *other;
    bool result;
    assert_true(coral_array_alloc(&other));
    assert_true(coral_array_init(other, NULL, 9));
    assert_true(coral_array_is_equal(object, other, &result));
    assert_false(result);
    assert_true(coral_array_alloc(&other));
    assert_true(coral_array_init(other, NULL, 10));
    assert_true(coral_array_is_equal(object, other, &result));
    assert_true(result);
    void *q;
    assert_true(coral_object_alloc(0, &q));
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    assert_true(coral_object_init(q, class));

    // TODO: add object instance for non-NULL is_equals test
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_copy_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_copy(NULL, (void*)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_copy_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_copy((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_copy_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object, *other = NULL;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_copy(object, &other));
    assert_true(coral_array_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_copy(void **state) {
    // TODO: implement (once get and set is done) ...
    assert_true(false);
}

static void check_object_retain_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_retain(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_retain_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_retain(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_array_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_release_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_release(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_release_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_release(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_array_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_autorelease_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_autorelease(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_autorelease_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_autorelease(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_array_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_capacity_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_get_capacity(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_capacity_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_get_capacity((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_get_capacity_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    size_t capacity = 0;
    assert_false(coral_array_get_capacity(object, &capacity));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_array_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_capacity(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {473, 1122};
    struct coral_range *limit;
    assert_true(coral_range_of_delta(&limit, values, -20));
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, limit, 10));
    size_t capacity;
    assert_true(coral_array_get_capacity(object, &capacity));
    assert_int_equal(values.first, capacity);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_set_capacity_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_set_capacity(NULL, 100));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_set_capacity_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_set_capacity(object, 100));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_array_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_set_capacity_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {100, 200};
    struct coral_range *limit;
    assert_true(coral_range_of_delta(&limit, values, 7));
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, limit, 0));
    assert_false(coral_array_set_capacity(object, 99));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_set_capacity_error_on_memory_allocation_failed(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, 0));
    assert_false(coral_array_set_capacity(object, SIZE_MAX));
    assert_int_equal(CORAL_ERROR_MEMORY_ALLOCATION_FAILED, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_set_capacity(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, 10));
    struct coral$array *array = (struct coral$array*) object;
    assert_int_equal(15, array->capacity);
    assert_int_equal(10, array->count);
    assert_true(coral_array_set_capacity(object, 7));
    assert_int_equal(7, array->capacity);
    assert_int_equal(7, array->count);
    assert_true(coral_array_set_capacity(object, 20));
    assert_int_equal(20, array->capacity);
    assert_int_equal(7, array->count);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_count_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_get_count(NULL, (void*)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_count_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_get_count((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_count_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    size_t count;
    assert_false(coral_array_get_count(object, &count));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_array_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_count(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, 10));
    size_t count;
    assert_true(coral_array_get_count(object, &count));
    assert_int_equal(10, count);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_set_count_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_set_count(NULL, 192));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_set_count_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_set_count(object, 123));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_array_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_set_count_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {100, 200};
    struct coral_range *limit;
    assert_true(coral_range_of_delta(&limit, values, 7));
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, limit, 10));
    assert_false(coral_array_set_count(object, 1 + values.last));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_set_count_error_on_memory_allocation_failure(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_range_values values = {0, SIZE_MAX};
    struct coral_range *limit;
    assert_true(coral_range_of_delta(&limit, values, 1));
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, limit, 0));
    assert_false(coral_array_set_count(object, SIZE_MAX - 1));
    assert_int_equal(CORAL_ERROR_MEMORY_ALLOCATION_FAILED, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_set_count(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, 0));
    struct coral$array *array = (struct coral$array*)object;
    assert_int_equal(0, array->count);
    assert_true(coral_array_set_count(object, 1));
    assert_int_equal(1, array->count);
    size_t value = 0;
    assert_memory_equal(&value, array->data, sizeof(size_t));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_get(NULL, 0, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_get((void *)1, 0, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_error_on_index_out_of_bounds(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, 2));
    void *i;
    assert_false(coral_array_get(object, 10, &i));
    assert_int_equal(CORAL_ERROR_INDEX_OUT_OF_BOUNDS, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    void *i;
    assert_false(coral_array_get(object, 0, &i));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_array_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, 2));
    void *i = (void*) 1;
    assert_true(coral_array_get(object, 1, &i));
    assert_null(i);
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    assert_true(coral_object_retain(i));
    struct coral$array$item item = {
            .data = &i,
            .size = sizeof(void *)
    };
    struct coral$array *array = (struct coral$array*)object;
    assert_true(coral$array$set(array, 1, &item));
    void *o = NULL;
    assert_true(coral_array_get(object, 1, &o));
    assert_non_null(o);
    assert_ptr_equal(i, o);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_set_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_set(NULL, 0, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_set_error_on_index_out_of_bounds(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, 2));
    assert_false(coral_array_set(object, 10, NULL));
    assert_int_equal(CORAL_ERROR_INDEX_OUT_OF_BOUNDS, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_set_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_set(object, 10, NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_array_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_set(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, 10));
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *i;
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    void *o;
    assert_true(coral_array_get(object, 5, &o));
    assert_null(o);
    assert_true(coral_array_set(object, 5, i));
    assert_true(coral_array_get(object, 5, &o));
    assert_ptr_equal(o, i);
    assert_true(coral_array_set(object, 5, NULL));
    assert_true(coral_array_get(object, 5, &o));
    assert_null(o);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_add_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_add(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_add_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_add(object, NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_array_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_add(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, 0));
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *i;
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    assert_true(coral_array_add(object, i));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_remove_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_remove(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_remove_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_remove(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_array_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_remove_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, 0));
    assert_false(coral_array_remove(object));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_remove(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, 10));
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *i;
    assert_true(coral_object_alloc(0, &i));
    assert_true(coral_object_init(i, class));
    assert_true(coral_array_add(object, i));
    struct coral$array *array = (struct coral$array *)object;
    assert_int_equal(11, array->count);
    assert_true(coral_array_remove(object));
    assert_int_equal(10, array->count);
    assert_true(coral_array_remove(object));
    assert_int_equal(9, array->count);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_insert_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_insert(NULL, 0, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_insert_error_on_index_out_of_bounds(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, 2));
    assert_false(coral_array_insert(object, 2, NULL));
    assert_int_equal(CORAL_ERROR_INDEX_OUT_OF_BOUNDS, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_insert_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_insert(object, 0, NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_array_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_insert(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, 1));
    void *i = (void *)1;
    assert_true(coral_array_get(object, 0, &i));
    assert_null(i);
    struct coral_class *class = NULL;
    assert_true(coral_object_class(&class));
    void *o;
    assert_true(coral_object_alloc(0, &o));
    assert_true(coral_object_init(o, class));
    struct coral$array *array = (struct coral$array *)object;
    assert_int_equal(1, array->count);
    assert_true(coral_array_insert(object, 0, o));
    assert_int_equal(2, array->count);
    assert_true(coral_array_get(object, 0, &i));
    assert_ptr_equal(i, o);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_delete_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_delete(NULL, 0));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_delete_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_delete(object, 0));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_array_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_delete_error_on_index_out_of_bounds(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, 1));
    assert_false(coral_array_delete(object, 10));
    assert_int_equal(CORAL_ERROR_INDEX_OUT_OF_BOUNDS, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_delete(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, 10));
    struct coral$array *array = (struct coral$array *)object;
    assert_int_equal(10, array->count);
    assert_true(coral_array_delete(object, 5));
    assert_int_equal(9, array->count);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_sort_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_sort(NULL, (void *)1, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_sort_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_sort((void *)1, (void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_sort(void **state) {
    coral_error = CORAL_ERROR_NONE;
    const size_t count = 100;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, count));
    struct coral_class *class;
    assert_true(coral_object_class(&class));
    void *instance;
    for (size_t i = 0, limit = count - 2; i < limit; i++) {
        assert_true(coral_object_alloc(0, &instance));
        assert_true(coral_object_init(instance, class));
        assert_true(coral_array_set(object, i, instance));
    }
    assert_true(coral_array_sort(object, NULL, coral_compare_void_ptr));
    assert_non_null(instance);
    assert_true(coral_array_get(object, 0, &instance));
    assert_null(instance);
    assert_true(coral_array_get(object, 1, &instance));
    assert_null(instance);
    assert_true(coral_array_get(object, 2, &instance));
    assert_non_null(instance);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_sort_with_range(void **state) {
    coral_error = CORAL_ERROR_NONE;
    const size_t count = 100;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, count));
    struct coral_class *class;
    assert_true(coral_object_class(&class));
    void *instance;
    for (size_t i = 0, limit = count - 10; i < limit; i++) {
        assert_true(coral_object_alloc(0, &instance));
        assert_true(coral_object_init(instance, class));
        assert_true(coral_array_set(object, i, instance));
    }
    struct coral_range_values values = {0, 90};
    assert_true(coral_array_sort(object, &values, coral_compare_void_ptr));
    assert_non_null(instance);
    assert_true(coral_array_get(object, 0, &instance));
    assert_null(instance);
    assert_true(coral_array_get(object, 1, &instance));
    assert_non_null(instance);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_sort_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_sort(object, NULL, (void*)1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_array_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_search_pattern_class_error_on_null_argument(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_search_pattern_class(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_search_pattern_class(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_array_search_pattern_class(&class));
    assert_non_null(class);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_search_pattern_destroy_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_search_pattern_destroy(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_search_pattern_of_linear_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_search_pattern_of_linear(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_search_pattern_of_linear(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array_search_pattern *object;
    assert_true(coral_array_search_pattern_of_linear(&object));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_search_pattern_of_binary_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_search_pattern_of_binary(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_search_pattern_of_binary(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array_search_pattern *object;
    assert_true(coral_array_search_pattern_of_binary(&object));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_find_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_find(NULL, (void *)1, (void *)1, (void *)1,
                                  (void *)1, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_find_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_find((void *)1, (void *)1, NULL, (void *)1,
                                  (void *)1, (void*)1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_find((void *)1, (void *)1, (void*)1, (void *)1,
                                  NULL, (void*)1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_array_find((void *)1, (void *)1, (void *)1, (void *)1,
                                      (void*)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_find_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_false(coral_array_find(object, (void *)1, (void *)1, (void *)1,
                                  (void *)1, (void*)1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_array_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_find(void **state) {
    coral_error = CORAL_ERROR_NONE;
    const size_t count = 100;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, count));
    struct coral_class *class;
    assert_true(coral_object_class(&class));
    void *needle;
    for (size_t i = 0, limit = count; i < limit; i++) {
        assert_true(coral_object_alloc(0, &needle));
        assert_true(coral_object_init(needle, class));
        assert_true(coral_array_set(object, i, needle));
    }
    const size_t index = 76;
    assert_true(coral_array_get(object, index, &needle));
    struct coral_array_search_pattern *search_pattern;
    assert_true(coral_array_search_pattern_of_linear(&search_pattern));
    size_t out;
    assert_true(coral_array_find(object, NULL, search_pattern, needle,
                                 coral_compare_void_ptr, &out));
    assert_int_equal(index, out);
    assert_true(coral_array_sort(object, NULL, coral_compare_void_ptr));
    assert_true(coral_array_get(object, index, &needle));
    assert_true(coral_array_search_pattern_of_binary(&search_pattern));
    out = 0;
    assert_true(coral_array_find(object, NULL, search_pattern, needle,
                                 coral_compare_void_ptr, &out));
    assert_int_equal(index, out);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_find_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    const size_t count = 100;
    struct coral_array *object;
    assert_true(coral_array_alloc(&object));
    assert_true(coral_array_init(object, NULL, count));
    struct coral_class *class;
    assert_true(coral_object_class(&class));
    void *needle;
    for (size_t i = 0, limit = count; i < limit; i++) {
        assert_true(coral_object_alloc(0, &needle));
        assert_true(coral_object_init(needle, class));
        assert_true(coral_array_set(object, i, needle));
    }
    struct coral_array_search_pattern *search_pattern;
    assert_true(coral_array_search_pattern_of_linear(&search_pattern));
    size_t out;
    assert_false(coral_array_find(object, NULL, search_pattern, NULL,
                                  coral_compare_void_ptr, &out));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    assert_true(coral_array_sort(object, NULL, coral_compare_void_ptr));
    assert_true(coral_array_search_pattern_of_binary(&search_pattern));
    assert_false(coral_array_find(object, NULL, search_pattern, NULL,
                                  coral_compare_void_ptr, &out));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_invalidate_error_on_null_object_ptr),
            cmocka_unit_test(check_invalidate),
            cmocka_unit_test(check_init_error_on_null_object_ptr),
            cmocka_unit_test(check_init_error_on_invalid_value),
            cmocka_unit_test(check_init_error_on_memory_allocation_failed),
            cmocka_unit_test(check_init),
            cmocka_unit_test(check_get_capacity_error_on_null_object_ptr),
            cmocka_unit_test(check_get_capacity_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_capacity),
            cmocka_unit_test(check_set_capacity_error_on_null_object_ptr),
            cmocka_unit_test(check_set_capacity_error_on_invalid_value),
            cmocka_unit_test(check_set_capacity_error_on_memory_allocation_failed),
            cmocka_unit_test(check_set_capacity),
            cmocka_unit_test(check_get_count_error_on_null_object_ptr),
            cmocka_unit_test(check_get_count_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_count),
            cmocka_unit_test(check_set_count_error_on_null_object_ptr),
            cmocka_unit_test(check_set_count_error_on_invalid_value),
            cmocka_unit_test(check_set_count_error_on_memory_allocation_failed),
            cmocka_unit_test(check_set_count),
            cmocka_unit_test(check_get_size_error_on_null_object_ptr),
            cmocka_unit_test(check_get_size_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_size),
            cmocka_unit_test(check_get_error_on_null_object_ptr),
            cmocka_unit_test(check_get_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_error_on_index_out_of_bounds),
            cmocka_unit_test(check_get),
            cmocka_unit_test(check_set_error_on_null_object_ptr),
            cmocka_unit_test(check_set_error_on_invalid_value),
            cmocka_unit_test(check_set_error_on_index_out_of_bounds),
            cmocka_unit_test(check_set_error_on_memory_allocation_failed),
            cmocka_unit_test(check_set),
            cmocka_unit_test(check_add_error_on_null_object_ptr),
            cmocka_unit_test(check_add_error_on_invalid_value),
            cmocka_unit_test(check_add_error_on_memory_allocation_failed),
            cmocka_unit_test(check_add),
            cmocka_unit_test(check_remove_error_on_null_object_ptr),
            cmocka_unit_test(check_remove_error_on_object_not_found),
            cmocka_unit_test(check_remove),
            cmocka_unit_test(check_insert_error_on_null_object_ptr),
            cmocka_unit_test(check_insert_error_on_invalid_value),
            cmocka_unit_test(check_insert_error_on_index_out_of_bounds),
            cmocka_unit_test(check_insert_error_on_memory_allocation_failed),
            cmocka_unit_test(check_insert),
            cmocka_unit_test(check_delete_error_on_object_ptr),
            cmocka_unit_test(check_delete_error_on_index_out_of_bounds),
            cmocka_unit_test(check_delete),
            cmocka_unit_test(check_sort_error_on_null_object_ptr),
            cmocka_unit_test(check_sort_error_on_null_argument_ptr),
            cmocka_unit_test(check_sort),
            cmocka_unit_test(check_sort_with_range),
            cmocka_unit_test(check_search_pattern_invalidate_error_on_null_object_ptr),
            cmocka_unit_test(check_search_pattern_init_linear_error_on_null_object_ptr),
            cmocka_unit_test(check_search_pattern_init_linear),
            cmocka_unit_test(check_search_pattern_init_binary_error_on_null_object_ptr),
            cmocka_unit_test(check_search_pattern_init_binary),
            cmocka_unit_test(check_find_error_on_null_object_ptr),
            cmocka_unit_test(check_find_error_on_null_argument_ptr),
            cmocka_unit_test(check_find_error_on_invalid_value),
            cmocka_unit_test(check_find_with_linear_search_pattern),
            cmocka_unit_test(check_find_null_items_with_linear_search_pattern),
            cmocka_unit_test(check_find_with_binary_search_pattern),
            cmocka_unit_test(check_find_with_binary_search_patten_with_duplicates),
            cmocka_unit_test(check_object_class_error_on_null_argument),
            cmocka_unit_test(check_object_class),
            cmocka_unit_test(check_object_destroy_error_on_null_object_ptr),
            cmocka_unit_test(check_object_alloc_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_alloc),
            cmocka_unit_test(check_object_init_error_on_null_object_ptr),
            cmocka_unit_test(check_object_init_error_on_invalid_value),
            cmocka_unit_test(check_object_init_error_on_memory_allocation_failed),
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
            // cmocka_unit_test(check_object_copy),
            cmocka_unit_test(check_object_retain_error_on_null_object_ptr),
            cmocka_unit_test(check_object_retain_error_on_object_uninitialized),
            cmocka_unit_test(check_object_release_error_on_null_object_ptr),
            cmocka_unit_test(check_object_release_error_on_object_uninitialized),
            cmocka_unit_test(check_object_autorelease_error_on_null_object_ptr),
            cmocka_unit_test(check_object_autorelease_error_on_object_uninitialized),
            cmocka_unit_test(check_object_get_capacity_error_on_null_object_ptr),
            cmocka_unit_test(check_object_get_capacity_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_get_capacity_error_on_object_uninitialized),
            cmocka_unit_test(check_object_get_capacity),
            cmocka_unit_test(check_object_set_capacity_error_on_null_object_ptr),
            cmocka_unit_test(check_object_set_capacity_error_on_object_uninitialized),
            cmocka_unit_test(check_object_set_capacity_error_on_invalid_value),
            cmocka_unit_test(check_object_set_capacity_error_on_memory_allocation_failed),
            cmocka_unit_test(check_object_set_capacity),
            cmocka_unit_test(check_object_get_count_error_on_null_object_ptr),
            cmocka_unit_test(check_object_get_count_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_get_count_error_on_object_uninitialized),
            cmocka_unit_test(check_object_get_count),
            cmocka_unit_test(check_object_set_count_error_on_null_object_ptr),
            cmocka_unit_test(check_object_set_count_error_on_object_uninitialized),
            cmocka_unit_test(check_object_set_count_error_on_invalid_value),
            cmocka_unit_test(check_object_set_count_error_on_memory_allocation_failure),
            cmocka_unit_test(check_object_set_count),
            cmocka_unit_test(check_object_get_error_on_null_object_ptr),
            cmocka_unit_test(check_object_get_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_get_error_on_index_out_of_bounds),
            cmocka_unit_test(check_object_get_error_on_object_uninitialized),
            cmocka_unit_test(check_object_get),
            cmocka_unit_test(check_object_set_error_on_null_object_ptr),
            cmocka_unit_test(check_object_set_error_on_index_out_of_bounds),
            cmocka_unit_test(check_object_set_error_on_object_uninitialized),
            cmocka_unit_test(check_object_set),
            cmocka_unit_test(check_object_add_error_on_null_object_ptr),
            cmocka_unit_test(check_object_add_error_on_object_uninitialized),
            cmocka_unit_test(check_object_add),
            cmocka_unit_test(check_object_remove_error_on_null_object_ptr),
            cmocka_unit_test(check_object_remove_error_on_object_uninitialized),
            cmocka_unit_test(check_object_remove_error_on_object_not_found),
            cmocka_unit_test(check_object_remove),
            cmocka_unit_test(check_object_insert_error_on_null_object_ptr),
            cmocka_unit_test(check_object_insert_error_on_index_out_of_bounds),
            cmocka_unit_test(check_object_insert_error_on_object_uninitialized),
            cmocka_unit_test(check_object_insert),
            cmocka_unit_test(check_object_delete_error_on_null_object_ptr),
            cmocka_unit_test(check_object_delete_error_on_object_uninitialized),
            cmocka_unit_test(check_object_delete_error_on_index_out_of_bounds),
            cmocka_unit_test(check_object_delete),
            cmocka_unit_test(check_object_sort_error_on_null_object_ptr),
            cmocka_unit_test(check_object_sort_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_sort),
            cmocka_unit_test(check_object_sort_with_range),
            cmocka_unit_test(check_object_sort_error_on_object_uninitialized),
            cmocka_unit_test(check_object_search_pattern_class_error_on_null_argument),
            cmocka_unit_test(check_object_search_pattern_class),
            cmocka_unit_test(check_object_search_pattern_destroy_error_on_null_object_ptr),
            cmocka_unit_test(check_object_search_pattern_of_linear_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_search_pattern_of_linear),
            cmocka_unit_test(check_object_search_pattern_of_binary_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_search_pattern_of_binary),
            cmocka_unit_test(check_object_find_error_on_null_object_ptr),
            cmocka_unit_test(check_object_find_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_find_error_on_object_uninitialized),
            cmocka_unit_test(check_object_find),
            cmocka_unit_test(check_object_find_error_on_object_not_found),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
