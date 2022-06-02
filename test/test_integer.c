#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <coral.h>

#include "private/integer.h"
#include "test/cmocka.h"

static void check_invalidate_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$invalidate(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$init(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer object = {};
    assert_true(coral$integer$init(&object));
    assert_true(coral$integer$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_of_char_ptr_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$of_char_ptr(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$of_char_ptr((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_of_char_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer object = {};
    assert_true(coral$integer$of_char_ptr(
            &object,
            "826221238914710294871294812617285631892198412643162192184281736249"
            "634961294639146710643197329057023197493217781236481246031640871753"
            "298171906431023762174693164013471937498357293653296589726387162386"
            "128736781263012746398610461309483691386490123649817259873297592837"
            "932657821568732123423425427617843198352371433532824595485248132"));
    assert_true(coral$integer$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_of_char_ptr_error_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer object = {};
    assert_false(coral$integer$of_char_ptr(&object, "abcdef"));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    assert_true(coral$integer$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_of_size_t_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$of_size_t(NULL, 0));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_of_size_t(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer object = {};
    assert_true(coral$integer$of_size_t(&object, SIZE_MAX));
    assert_true(coral$integer$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_of_ssize_t_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$of_ssize_t(NULL, 0));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_of_ssize_t(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer object = {};
    assert_true(coral$integer$of_ssize_t(&object, SSIZE_MAX));
    assert_true(coral$integer$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_add_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$add(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_add_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$add((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_add(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer object = {};
    assert_true(coral$integer$init(&object));
    assert_int_equal(0, mpz_get_si(object.mpz));
    struct coral$integer value = {};
    assert_true(coral$integer$of_size_t(&value, 1));
    assert_int_equal(1, mpz_get_si(value.mpz));
    assert_true(coral$integer$add(&object, &value));
    assert_int_equal(1, mpz_get_si(object.mpz));
    assert_true(coral$integer$invalidate(&value));
    assert_true(coral$integer$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_subtract_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$subtract(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_subtract_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$subtract((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_subtract(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer object = {};
    assert_true(coral$integer$init(&object));
    assert_int_equal(0, mpz_get_si(object.mpz));
    struct coral$integer value = {};
    assert_true(coral$integer$of_size_t(&value, 1));
    assert_int_equal(1, mpz_get_si(value.mpz));
    assert_true(coral$integer$subtract(&object, &value));
    assert_int_equal(-1, mpz_get_si(object.mpz));
    assert_true(coral$integer$invalidate(&value));
    assert_true(coral$integer$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_multiply_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$multiply(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_multiply_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$multiply((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_multiply(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer object = {};
    assert_true(coral$integer$of_size_t(&object, 1));
    assert_int_equal(1, mpz_get_si(object.mpz));
    struct coral$integer other = {};
    assert_true(coral$integer$of_size_t(&other, 2));
    assert_int_equal(2, mpz_get_si(other.mpz));
    assert_true(coral$integer$multiply(&object, &other));
    assert_int_equal(2, mpz_get_si(object.mpz));
    struct coral$integer negative = {};
    assert_true(coral$integer$of_ssize_t(&negative, -5));
    assert_true(coral$integer$multiply(&object, &negative));
    assert_int_equal(-10, mpz_get_si(object.mpz));
    assert_true(coral$integer$invalidate(&negative));
    assert_true(coral$integer$invalidate(&other));
    assert_true(coral$integer$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_divide_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$divide(NULL, (void *)1, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_divide_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$divide((void *)1, NULL, (void*)1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_divide_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer zero = {};
    assert_true(coral$integer$init(&zero));
    assert_int_equal(0, mpz_get_si(zero.mpz));
    assert_false(coral$integer$divide((void *)1, &zero, NULL));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    assert_true(coral$integer$invalidate(&zero));
    coral_error = CORAL_ERROR_NONE;
}

static void check_divide(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer object = {};
    assert_true(coral$integer$of_char_ptr(&object, "5"));
    assert_int_equal(5, mpz_get_si(object.mpz));
    struct coral$integer divisor = {};
    assert_true(coral$integer$of_char_ptr(&divisor, "3"));
    assert_int_equal(3, mpz_get_si(divisor.mpz));
    struct coral$integer remainder = {};
    assert_true(coral$integer$divide(&object, &divisor, &remainder));
    assert_int_equal(1, mpz_get_si(object.mpz));
    assert_int_equal(2, mpz_get_si(remainder.mpz));
    assert_true(coral$integer$invalidate(&object));
    assert_true(coral$integer$of_char_ptr(&object, "16"));
    assert_int_equal(16, mpz_get_si(object.mpz));
    assert_true(coral$integer$divide(&object, &divisor, NULL));
    assert_int_equal(5, mpz_get_si(object.mpz));
    assert_true(coral$integer$invalidate(&remainder));
    assert_true(coral$integer$invalidate(&divisor));
    assert_true(coral$integer$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_absolute_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$absolute(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_absolute(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer object = {};
    assert_true(coral$integer$of_char_ptr(&object, "-5"));
    assert_int_equal(-5, mpz_get_si(object.mpz));
    assert_true(coral$integer$absolute(&object));
    assert_int_equal(5, mpz_get_si(object.mpz));
    assert_true(coral$integer$absolute(&object));
    assert_int_equal(5, mpz_get_si(object.mpz));
    assert_true(coral$integer$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_negate_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$negate(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_negate(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer object = {};
    assert_true(coral$integer$of_char_ptr(&object, "-15"));
    assert_int_equal(-15, mpz_get_si(object.mpz));
    assert_true(coral$integer$negate(&object));
    assert_int_equal(15, mpz_get_si(object.mpz));
    assert_true(coral$integer$negate(&object));
    assert_int_equal(-15, mpz_get_si(object.mpz));
    assert_true(coral$integer$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_compare_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$compare(NULL, (void *)1, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_compare_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$compare((void *)1, NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$compare((void *)1, (void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_compare(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer object = {};
    assert_true(coral$integer$init(&object));
    assert_int_equal(0, mpz_get_si(object.mpz));
    struct coral$integer other = {};
    assert_true(coral$integer$of_char_ptr(&other, "1"));
    assert_int_equal(1, mpz_get_si(other.mpz));
    int result;
    assert_true(coral$integer$compare(&object, &other, &result));
    assert_true(0 > result);
    assert_true(coral$integer$compare(&object, &object, &result));
    assert_true(0 == result);
    assert_true(coral$integer$compare(&other, &other, &result));
    assert_true(0 == result);
    assert_true(coral$integer$compare(&other, &object, &result));
    assert_true(0 < result);
    assert_true(coral$integer$invalidate(&other));
    assert_true(coral$integer$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_and_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$and(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_and_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$and((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_and(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer zero = {};
    assert_true(coral$integer$init(&zero));
    assert_int_equal(0, mpz_get_si(zero.mpz));
    struct coral$integer one = {};
    assert_true(coral$integer$of_char_ptr(&one, "1"));
    assert_int_equal(1, mpz_get_si(one.mpz));
    assert_true(coral$integer$and(&one, &one));
    assert_int_equal(1, mpz_get_si(one.mpz));
    assert_true(coral$integer$and(&zero, &zero));
    assert_int_equal(0, mpz_get_si(zero.mpz));
    assert_true(coral$integer$and(&zero, &one));
    assert_int_equal(0, mpz_get_si(zero.mpz));
    assert_true(coral$integer$and(&one, &zero));
    assert_int_equal(0, mpz_get_si(one.mpz));
    assert_true(coral$integer$invalidate(&zero));
    assert_true(coral$integer$invalidate(&one));
    coral_error = CORAL_ERROR_NONE;
}

static void check_or_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$or(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_or_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$or((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_or(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer zero = {};
    assert_true(coral$integer$init(&zero));
    assert_int_equal(0, mpz_get_si(zero.mpz));
    struct coral$integer one = {};
    assert_true(coral$integer$of_char_ptr(&one, "1"));
    assert_int_equal(1, mpz_get_si(one.mpz));
    assert_true(coral$integer$or(&zero, &zero));
    assert_int_equal(0, mpz_get_si(zero.mpz));
    assert_true(coral$integer$or(&one, &zero));
    assert_int_equal(1, mpz_get_si(one.mpz));
    assert_true(coral$integer$or(&zero, &one));
    assert_int_equal(1, mpz_get_si(one.mpz));
    assert_true(coral$integer$or(&one, &one));
    assert_int_equal(1, mpz_get_si(one.mpz));
    assert_true(coral$integer$invalidate(&one));
    assert_true(coral$integer$invalidate(&zero));
    coral_error = CORAL_ERROR_NONE;
}

static void check_not_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$not(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_not(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer object = {};
    assert_true(coral$integer$init(&object));
    assert_int_equal(0, mpz_get_si(object.mpz));
    assert_true(coral$integer$not(&object));
    assert_int_equal(-1, mpz_get_si(object.mpz));
    assert_true(coral$integer$not(&object));
    assert_int_equal(0, mpz_get_si(object.mpz));
    assert_true(coral$integer$invalidate(&object));
    assert_true(coral$integer$of_size_t(&object, 90));
    assert_true(coral$integer$not(&object));
    assert_int_equal(-91, mpz_get_si(object.mpz));
    assert_true(coral$integer$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_xor_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$xor(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_xor_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$xor((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_xor(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer zero = {};
    assert_true(coral$integer$init(&zero));
    assert_int_equal(0, mpz_get_si(zero.mpz));
    struct coral$integer one = {};
    assert_true(coral$integer$of_size_t(&one, 1));
    assert_int_equal(1, mpz_get_si(one.mpz));
    struct coral$integer object = {};
    assert_true(coral$integer$of_size_t(&object, 1));
    assert_int_equal(1, mpz_get_si(object.mpz));
    assert_true(coral$integer$xor(&object, &zero));
    assert_int_equal(1, mpz_get_si(object.mpz));
    assert_true(coral$integer$xor(&object, &one));
    assert_int_equal(0, mpz_get_si(object.mpz));
    assert_true(coral$integer$invalidate(&object));
    assert_true(coral$integer$invalidate(&one));
    assert_true(coral$integer$invalidate(&zero));
    coral_error = CORAL_ERROR_NONE;
}

static void check_shift_left_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$shift_left(NULL, 0));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_shift_left(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer object = {};
    assert_true(coral$integer$of_size_t(&object, 1));
    assert_int_equal(1, mpz_get_si(object.mpz));
    assert_true(coral$integer$shift_left(&object, 10));
    assert_int_equal(1024, mpz_get_si(object.mpz));
    assert_true(coral$integer$invalidate(&object));
    assert_true(coral$integer$of_ssize_t(&object, -1));
    assert_int_equal(-1, mpz_get_si(object.mpz));
    assert_true(coral$integer$shift_left(&object, 10));
    assert_int_equal(-1024, mpz_get_si(object.mpz));
    assert_true(coral$integer$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_shift_right_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$shift_right(NULL, 0));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_shift_right(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer object = {};
    assert_true(coral$integer$of_size_t(&object, 1024));
    assert_int_equal(1024, mpz_get_si(object.mpz));
    assert_true(coral$integer$shift_right(&object, 10));
    assert_int_equal(1, mpz_get_si(object.mpz));
    assert_true(coral$integer$invalidate(&object));
    assert_true(coral$integer$of_ssize_t(&object, -1024));
    assert_int_equal(-1024, mpz_get_si(object.mpz));
    assert_true(coral$integer$shift_right(&object, 10));
    assert_int_equal(-1, mpz_get_si(object.mpz));
    assert_true(coral$integer$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_length_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$length(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_length_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$length((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_length(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer object = {};
    assert_true(coral$integer$of_size_t(&object, 1024));
    assert_int_equal(1024, mpz_get_si(object.mpz));
    size_t length;
    assert_true(coral$integer$length(&object, &length));
    assert_int_equal(11, length);
    assert_true(coral$integer$invalidate(&object));
    assert_true(coral$integer$of_ssize_t(&object, -2048));
    assert_int_equal(-2048, mpz_get_si(object.mpz));
    assert_true(coral$integer$length(&object, &length));
    assert_int_equal(12, length);
    assert_true(coral$integer$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_count_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$count(NULL, (void*)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_count_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$count((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_count(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer object = {};
    assert_true(coral$integer$of_size_t(&object, 1024));
    assert_int_equal(1024, mpz_get_si(object.mpz));
    size_t count;
    assert_true(coral$integer$count(&object, &count));
    assert_int_equal(1, count);
    assert_true(coral$integer$invalidate(&object));
    assert_true(coral$integer$of_size_t(&object, 255));
    assert_int_equal(255, mpz_get_si(object.mpz));
    assert_true(coral$integer$count(&object, &count));
    assert_int_equal(8, count);
    assert_true(coral$integer$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$get(NULL, 0, (void*)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$get((void *)1, 0, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer object = {};
    assert_true(coral$integer$of_size_t(&object, 1025));
    bool value;
    assert_true(coral$integer$get(&object, 0, &value));
    assert_true(value);
    assert_true(coral$integer$get(&object, 1024, &value));
    assert_false(value);
    assert_true(coral$integer$get(&object, 10, &value));
    assert_true(value);
    assert_true(coral$integer$invalidate(&object));
    assert_true(coral$integer$of_size_t(&object, -1025));
    assert_true(coral$integer$get(&object, 1024, &value));
    assert_false(value);
    /* twos compliment representation for a negative number */
    for (size_t i = 0; i < 10; i++) {
        assert_true(coral$integer$get(&object, i, &value));
        assert_true(value);
    }
    assert_true(coral$integer$get(&object, 10, &value));
    assert_false(value);
    assert_true(coral$integer$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_set_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$set(NULL, 0, false));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_set(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer object = {};
    assert_true(coral$integer$init(&object));
    assert_int_equal(0, mpz_get_si(object.mpz));
    assert_true(coral$integer$set(&object, 10, true));
    bool value;
    assert_true(coral$integer$get(&object, 10, &value));
    assert_true(value);
    assert_true(coral$integer$set(&object, 0, true));
    assert_true(coral$integer$negate(&object));
    /* twos compliment representation for a negative number */
    assert_true(coral$integer$get(&object, 10, &value));
    assert_false(value);
    assert_true(coral$integer$negate(&object));
    assert_true(coral$integer$set(&object, 1024, false));
    assert_true(coral$integer$get(&object, 1024, &value));
    assert_false(value);
    assert_true(coral$integer$set(&object, 10, false));
    assert_true(coral$integer$get(&object, 10, &value));
    assert_false(value);
    assert_int_equal(1, mpz_get_si(object.mpz));
    assert_true(coral$integer$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_find_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$find(NULL, (void *)1, false, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_find_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$integer$find((void *)1, (void*)1, true, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_find_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer object = {};
    assert_true(coral$integer$init(&object));
    size_t at;
    assert_false(coral$integer$find(&object, NULL, true, &at));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    assert_true(coral$integer$set(&object, 200, true));
    struct coral_range_values values = {.first = 19, .last = 100};
    assert_false(coral$integer$find(&object, &values, true, &at));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    assert_true(coral$integer$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_find(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$integer object = {};
    assert_true(coral$integer$init(&object));
    assert_true(coral$integer$set(&object, 1023, true));
    size_t at;
    assert_true(coral$integer$find(&object, NULL, false, &at));
    assert_int_equal(0, at);
    assert_true(coral$integer$find(&object, NULL, true, &at));
    assert_int_equal(1023, at);
    assert_true(coral$integer$invalidate(&object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_class_error_on_null_argument(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_class(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_class(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_integer_class(&class));
    assert_non_null(class);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_destroy_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_destroy(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_alloc_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_alloc(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_alloc(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *object;
    assert_true(coral_integer_alloc(&object));
    assert_true(coral_integer_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_init(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *object;
    assert_true(coral_integer_alloc(&object));
    assert_true(coral_integer_init(object));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_of_char_ptr_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_of_char_ptr(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_of_char_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *object;
    assert_true(coral_integer_of_char_ptr(&object, "-1"));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_of_size_t_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_of_size_t(NULL, 0));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_of_size_t(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *object;
    assert_true(coral_integer_of_size_t(&object, 0));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_of_ssize_t_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_of_ssize_t(NULL, -10));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_of_ssize_t(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *object;
    assert_true(coral_integer_of_ssize_t(&object, -293));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_hash_code(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_hash_code_error_on_object_is_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *object;
    assert_true(coral_integer_alloc(&object));
    size_t code;
    assert_false(coral_integer_hash_code(object, &code));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_integer_class(&class));
    size_t code;
    assert_false(coral_integer_hash_code((struct coral_integer *)class, &code));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_hash_code(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *object;
    assert_true(coral_integer_of_size_t(&object, 10));
    size_t code;
    assert_true(coral_integer_hash_code(object, &code));
    assert_int_equal(10, code);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_is_equal(NULL, (void *)1, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_is_equal((void *)1, NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_is_equal((void *)1, (void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal_on_object_is_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *object;
    assert_true(coral_integer_alloc(&object));
    bool result;
    assert_false(coral_integer_is_equal(object, (void *)1, &result));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_is_equal(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i, *o;
    assert_true(coral_integer_of_size_t(&i, 821612));
    assert_true(coral_integer_of_size_t(&o, 821612));
    bool result;
    assert_true(coral_integer_is_equal(i, o, &result));
    assert_true(result);
    struct coral_integer *p;
    assert_true(coral_integer_alloc(&p));
    assert_true(coral_integer_init(p));
    assert_true(coral_integer_is_equal(i, p, &result));
    assert_false(result);
    struct coral_class *class = NULL;
    assert_true(coral_integer_class(&class));
    assert_true(coral_integer_is_equal(i, (struct coral_integer *)class, &result));
    assert_false(result);
    assert_true(coral_integer_is_equal((struct coral_integer *)class, i,
            &result));
    assert_false(result);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_retain_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_retain(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_retain_error_on_object_is_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *object;
    assert_true(coral_integer_alloc(&object));
    assert_false(coral_integer_retain(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_release_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_release(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_release_error_on_object_is_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *object;
    assert_true(coral_integer_alloc(&object));
    assert_false(coral_integer_release(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_autorelease_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_autorelease(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_autorelease_error_on_object_is_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *object;
    assert_true(coral_integer_alloc(&object));
    assert_false(coral_integer_autorelease(object));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_add_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_add(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_add_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_add((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_add_error_on_object_is_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *object;
    assert_true(coral_integer_alloc(&object));
    assert_false(coral_integer_add(object, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_add_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 1));
    struct coral_class *class = NULL;
    assert_true(coral_integer_class(&class));
    assert_false(coral_integer_add(i, (struct coral_integer *)class));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_add((struct coral_integer *)class, i));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_add(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i, *o;
    assert_true(coral_integer_of_size_t(&i, 1));
    assert_true(coral_integer_of_size_t(&o, 1));
    assert_true(coral_integer_add(i, o));
    size_t code;
    assert_true(coral_integer_hash_code(i, &code));
    assert_int_equal(2, code);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_subtract_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_subtract(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_subtract_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_subtract((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_subtract_error_on_object_is_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *object;
    assert_true(coral_integer_alloc(&object));
    assert_false(coral_integer_subtract(object, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_subtract_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 1));
    struct coral_class *class = NULL;
    assert_true(coral_integer_class(&class));
    assert_false(coral_integer_subtract(i, (struct coral_integer *)class));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_subtract((struct coral_integer *)class, i));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_subtract(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i, *o;
    assert_true(coral_integer_of_size_t(&i, 1));
    assert_true(coral_integer_of_size_t(&o, 1));
    assert_true(coral_integer_subtract(i, o));
    size_t code;
    assert_true(coral_integer_hash_code(i, &code));
    assert_int_equal(0, code);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_multiply_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_multiply(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_multiply_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_multiply((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_multiply_error_on_object_is_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *object;
    assert_true(coral_integer_alloc(&object));
    assert_false(coral_integer_multiply(object, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_multiply_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 1));
    struct coral_class *class = NULL;
    assert_true(coral_integer_class(&class));
    assert_false(coral_integer_multiply(i, (struct coral_integer *)class));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_multiply((struct coral_integer *)class, i));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_multiply(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i, *o;
    assert_true(coral_integer_of_size_t(&i, 2));
    assert_true(coral_integer_of_size_t(&o, 2));
    assert_true(coral_integer_multiply(i, o));
    size_t code;
    assert_true(coral_integer_hash_code(i, &code));
    assert_int_equal(4, code);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_divide_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_divide(NULL, (void *)1, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_divide_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_divide((void *)1, NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_divide_error_on_object_is_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *object;
    assert_true(coral_integer_alloc(&object));
    assert_false(coral_integer_divide(object, (void *)1, NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(object));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_divide_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 1));
    struct coral_class *class = NULL;
    assert_true(coral_integer_class(&class));
    assert_false(coral_integer_divide(i, (struct coral_integer *)class, NULL));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_divide((struct coral_integer *)class, i, NULL));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_divide(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i, *o, *r;
    assert_true(coral_integer_of_size_t(&i, 11));
    assert_true(coral_integer_of_size_t(&o, 2));
    assert_true(coral_integer_divide(i, o, &r));
    size_t code;
    assert_true(coral_integer_hash_code(i, &code));
    assert_int_equal(5, code);
    assert_true(coral_integer_hash_code(r, &code));
    assert_int_equal(1, code);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_absolute_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_absolute(NULL));
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_absolute_error_on_object_is_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_alloc(&i));
    assert_false(coral_integer_absolute(i));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_absolute_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_integer_class(&class));
    assert_false(coral_integer_absolute((struct coral_integer *)class));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_absolute(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_of_ssize_t(&i, -1));
    assert_true(coral_integer_absolute(i));
    size_t code;
    assert_true(coral_integer_hash_code(i, &code));
    assert_int_equal(1, code);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_negate_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_negate(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_negate_error_on_object_is_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_alloc(&i));
    assert_false(coral_integer_negate(i));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_negate_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_integer_class(&class));
    assert_false(coral_integer_negate((struct coral_integer *)class));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_negate(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 1));
    size_t code;
    assert_true(coral_integer_hash_code(i, &code));
    assert_int_equal(1, code);
    assert_true(coral_integer_negate(i));
    assert_true(coral_integer_hash_code(i, &code));
    assert_int_equal(SIZE_MAX, code); // (-1) as unsigned value
    assert_true(coral_integer_negate(i));
    assert_true(coral_integer_hash_code(i, &code));
    assert_int_equal(1, code);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_compare_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_compare(NULL, (void *)1, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_compare_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_compare((void *)1, NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_compare((void *)1, (void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_compare_error_on_object_is_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i, *o;
    assert_true(coral_integer_alloc(&i));
    assert_true(coral_integer_of_size_t(&o, 193));
    int result;
    assert_false(coral_integer_compare(i, o, &result));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(i));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_compare_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_integer_class(&class));
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 10));
    int result;
    assert_false(coral_integer_compare(i, (struct coral_integer *)class,
            &result));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_compare((struct coral_integer *)class, i,
            &result));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_compare(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i, *o, *p;
    assert_true(coral_integer_of_size_t(&i, 10));
    assert_true(coral_integer_of_size_t(&o, 10));
    assert_true(coral_integer_of_ssize_t(&p, -10));
    int result;
    assert_true(coral_integer_compare(i, o, &result));
    assert_int_equal(0, result);
    assert_true(coral_integer_compare(o, i, &result));
    assert_int_equal(0, result);
    assert_true(coral_integer_compare(i, p, &result));
    assert_int_not_equal(0, result);
    assert_true(coral_integer_compare(p, i, &result));
    assert_int_not_equal(0, result);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_and_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_and(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_and_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_and((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_and_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_integer_class(&class));
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 10));
    assert_false(coral_integer_and((struct coral_integer *)class, i));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_and(i, (struct coral_integer *)class));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_and_error_on_object_is_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i, *o;
    assert_true(coral_integer_alloc(&i));
    assert_true(coral_integer_of_size_t(&o, 1));
    assert_false(coral_integer_and(i, o));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(i));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_and(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i, *o, *p;
    assert_true(coral_integer_of_size_t(&i, 1));
    assert_true(coral_integer_of_size_t(&o, 1));
    assert_true(coral_integer_of_size_t(&p, 0));
    assert_true(coral_integer_and(i, o));
    bool result;
    assert_true(coral_integer_is_equal(i, o, &result));
    assert_true(result);
    assert_true(coral_integer_and(i, p));
    assert_true(coral_integer_is_equal(i, p, &result));
    assert_true(result);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_or_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_or(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_or_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_or((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_or_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_integer_class(&class));
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 192));
    assert_false(coral_integer_or((struct coral_integer *)class, i));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_or(i, (struct coral_integer *)class));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_or_error_on_object_is_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i, *o;
    assert_true(coral_integer_alloc(&i));
    assert_true(coral_integer_of_size_t(&o, 193));
    assert_false(coral_integer_or(i, o));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(i));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_or(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i, *o;
    assert_true(coral_integer_of_size_t(&i, 1));
    assert_true(coral_integer_of_size_t(&o, 0));
    assert_true(coral_integer_or(i, o));
    size_t code;
    assert_true(coral_integer_hash_code(i, &code));
    assert_int_equal(1, code);
    assert_true(coral_integer_or(o, i));
    assert_true(coral_integer_hash_code(o, &code));
    assert_int_equal(1, code);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_not_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_not(NULL));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_not_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_integer_class(&class));
    assert_false(coral_integer_not((struct coral_integer *)class));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_not_error_on_object_is_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_alloc(&i));
    assert_false(coral_integer_not(i));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_not(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 1));
    assert_true(coral_integer_not(i));
    size_t code;
    assert_true(coral_integer_hash_code(i, &code));
    assert_int_equal(SIZE_MAX - 1, code);
    assert_true(coral_integer_not(i));
    assert_true(coral_integer_hash_code(i, &code));
    assert_int_equal(1, code);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_xor_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_xor(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_xor_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_xor((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_xor_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_integer_class(&class));
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 128));
    assert_false(coral_integer_xor(i, (struct coral_integer *)class));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_xor((struct coral_integer *)class, i));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_xor_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i, *o;
    assert_true(coral_integer_alloc(&i));
    assert_true(coral_integer_of_size_t(&o, 271));
    assert_false(coral_integer_xor(i, o));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(i));
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_xor(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i, *o;
    assert_true(coral_integer_of_size_t(&i, 73));
    assert_true(coral_integer_of_size_t(&o, 27));
    assert_true(coral_integer_xor(i, o));
    size_t code;
    assert_true(coral_integer_hash_code(i, &code));
    assert_int_equal(82, code); /* 82 = 73 ^ 27 */
    assert_true(coral_integer_negate(i));
    assert_true(coral_integer_xor(i, o));
    assert_true(coral_integer_hash_code(i, &code));
    assert_int_equal(-82 ^ 27, code);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_shift_left_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_shift_left(NULL, 72));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_shift_left_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_integer_class(&class));
    assert_false(coral_integer_shift_left((struct coral_integer *)class, 261));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_shift_left_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_alloc(&i));
    assert_false(coral_integer_shift_left(i, 273));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_shift_left(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 1));
    assert_true(coral_integer_shift_left(i, 0));
    size_t code;
    assert_true(coral_integer_hash_code(i, &code));
    assert_int_equal(1, code);
    assert_true(coral_integer_shift_left(i, 7));
    assert_true(coral_integer_hash_code(i, &code));
    assert_int_equal(1 << 7, code);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_shift_right_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_shift_right(NULL, 231));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_shift_right_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_integer_class(&class));
    assert_false(coral_integer_shift_right((struct coral_integer *)class, 273));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void
check_object_shift_right_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_alloc(&i));
    assert_false(coral_integer_shift_right(i, 273));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_shift_right(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 128));
    assert_true(coral_integer_shift_right(i, 0));
    size_t code;
    assert_true(coral_integer_hash_code(i, &code));
    assert_int_equal(128, code);
    assert_true(coral_integer_shift_right(i, 7));
    assert_true(coral_integer_hash_code(i, &code));
    assert_int_equal(128 >> 7, code);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_length_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_length(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_length_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_length((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_length_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_integer_class(&class));
    size_t length;
    assert_false(coral_integer_length((struct coral_integer *)class, &length));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_length_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_alloc(&i));
    size_t length;
    assert_false(coral_integer_length(i, &length));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_length(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 123));
    size_t length;
    assert_true(coral_integer_length(i, &length));
    assert_int_equal(7, length);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_count_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_count(NULL, (void*)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_count_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_count((void *)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_count_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_integer_class(&class));
    size_t count;
    assert_false(coral_integer_count((struct coral_integer *)class, &count));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_count_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_alloc(&i));
    size_t count;
    assert_false(coral_integer_count(i, &count));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_count(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 123));
    size_t count;
    assert_true(coral_integer_count(i, &count));
    assert_int_equal(6, count);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_get(NULL, 0, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_get((void *)1, 0, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_integer_class(&class));
    bool value;
    assert_false(coral_integer_get((struct coral_integer *)class, 0, &value));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_alloc(&i));
    bool value;
    assert_false(coral_integer_get(i, 0, &value));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_get(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 1));
    bool value;
    assert_true(coral_integer_get(i, 0, &value));
    assert_true(value);
    assert_true(coral_integer_get(i, SIZE_MAX - 1, &value));
    assert_false(false);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_set_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_set(NULL, 0, false));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_set_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_integer_class(&class));
    assert_false(coral_integer_set((struct coral_integer *)class, 0, false));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_set_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_alloc(&i));
    assert_false(coral_integer_set(i, 0, false));
    assert_true(coral_integer_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_set(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 0));
    size_t code;
    assert_true(coral_integer_hash_code(i, &code));
    assert_int_equal(0, code);
    assert_true(coral_integer_set(i, 7, true));
    assert_true(coral_integer_hash_code(i, &code));
    assert_int_equal(128, code);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_find_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_find(NULL, (void *)1, false, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_find_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral_integer_find((void *)1, (void *)1, true, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_find_error_on_invalid_value(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_class *class = NULL;
    assert_true(coral_integer_class(&class));
    size_t at;
    assert_false(coral_integer_find((struct coral_integer *)class, NULL, true, &at));
    assert_int_equal(CORAL_ERROR_INVALID_VALUE, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_find_error_on_object_uninitialized(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_alloc(&i));
    size_t at;
    assert_false(coral_integer_find(i, NULL, true, &at));
    assert_int_equal(CORAL_ERROR_OBJECT_IS_UNINITIALIZED, coral_error);
    assert_true(coral_integer_destroy(i));
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_find_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 0));
    size_t at;
    assert_false(coral_integer_find(i, NULL, true, &at));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_find(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 0));
    size_t at;
    assert_true(coral_integer_find(i, NULL, false, &at));
    assert_int_equal(0, at);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

static void check_object_find_with_values(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral_integer *i;
    assert_true(coral_integer_of_size_t(&i, 131));
    struct coral_range_values values = {
            .first = 5,
            .last = 10
    };
    size_t at;
    assert_true(coral_integer_find(i, &values, true, &at));
    assert_int_equal(7, at);
    coral_autorelease_pool_drain();
    coral_error = CORAL_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_invalidate_error_on_null_object_ptr),
            cmocka_unit_test(check_init_error_on_null_object_ptr),
            cmocka_unit_test(check_init),
            cmocka_unit_test(check_of_char_ptr_error_on_null_argument_ptr),
            cmocka_unit_test(check_of_char_ptr),
            cmocka_unit_test(check_of_char_ptr_error_invalid_value),
            cmocka_unit_test(check_of_size_t_error_on_null_argument_ptr),
            cmocka_unit_test(check_of_size_t),
            cmocka_unit_test(check_of_ssize_t_error_on_null_argument_ptr),
            cmocka_unit_test(check_of_ssize_t),
            cmocka_unit_test(check_add_error_on_null_object_ptr),
            cmocka_unit_test(check_add_error_on_null_argument_ptr),
            cmocka_unit_test(check_add),
            cmocka_unit_test(check_subtract_error_on_null_object_ptr),
            cmocka_unit_test(check_subtract_error_on_null_argument_ptr),
            cmocka_unit_test(check_subtract),
            cmocka_unit_test(check_multiply_error_on_null_object_ptr),
            cmocka_unit_test(check_multiply_error_on_null_argument_ptr),
            cmocka_unit_test(check_multiply),
            cmocka_unit_test(check_divide_error_on_null_object_ptr),
            cmocka_unit_test(check_divide_error_on_null_argument_ptr),
            cmocka_unit_test(check_divide_error_on_invalid_value),
            cmocka_unit_test(check_divide),
            cmocka_unit_test(check_absolute_error_on_null_object_ptr),
            cmocka_unit_test(check_absolute),
            cmocka_unit_test(check_negate_error_on_null_object_ptr),
            cmocka_unit_test(check_negate),
            cmocka_unit_test(check_compare_error_on_null_object_ptr),
            cmocka_unit_test(check_compare_error_on_null_argument_ptr),
            cmocka_unit_test(check_compare),
            cmocka_unit_test(check_and_error_on_null_object_ptr),
            cmocka_unit_test(check_and_error_on_null_argument_ptr),
            cmocka_unit_test(check_and),
            cmocka_unit_test(check_or_error_on_null_object_ptr),
            cmocka_unit_test(check_or_error_on_null_argument_ptr),
            cmocka_unit_test(check_or),
            cmocka_unit_test(check_not_error_on_null_object_ptr),
            cmocka_unit_test(check_not),
            cmocka_unit_test(check_xor_error_on_null_object_ptr),
            cmocka_unit_test(check_xor_error_on_null_argument_ptr),
            cmocka_unit_test(check_xor),
            cmocka_unit_test(check_shift_left_error_on_null_object_ptr),
            cmocka_unit_test(check_shift_left),
            cmocka_unit_test(check_shift_right_error_on_null_object_ptr),
            cmocka_unit_test(check_shift_right),
            cmocka_unit_test(check_length_error_on_null_object_ptr),
            cmocka_unit_test(check_length_error_on_null_argument_ptr),
            cmocka_unit_test(check_length),
            cmocka_unit_test(check_count_error_on_null_object_ptr),
            cmocka_unit_test(check_count_error_on_null_argument_ptr),
            cmocka_unit_test(check_count),
            cmocka_unit_test(check_get_error_on_null_object_ptr),
            cmocka_unit_test(check_get_error_on_null_argument_ptr),
            cmocka_unit_test(check_get),
            cmocka_unit_test(check_set_error_on_null_object_ptr),
            cmocka_unit_test(check_set),
            cmocka_unit_test(check_find_error_on_null_object_ptr),
            cmocka_unit_test(check_find_error_on_null_argument_ptr),
            cmocka_unit_test(check_find_error_on_object_not_found),
            cmocka_unit_test(check_find),
            cmocka_unit_test(check_object_class_error_on_null_argument),
            cmocka_unit_test(check_object_class),
            cmocka_unit_test(check_object_destroy_error_on_null_object_ptr),
            cmocka_unit_test(check_object_alloc_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_alloc),
            cmocka_unit_test(check_object_init_error_on_null_object_ptr),
            cmocka_unit_test(check_object_init),
            cmocka_unit_test(check_object_of_char_ptr_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_of_char_ptr),
            cmocka_unit_test(check_object_of_size_t_error_on_null_object_ptr),
            cmocka_unit_test(check_object_of_size_t),
            cmocka_unit_test(check_object_of_ssize_t_error_on_null_object_ptr),
            cmocka_unit_test(check_object_of_ssize_t),
            cmocka_unit_test(check_object_hash_code_error_on_null_object_ptr),
            cmocka_unit_test(check_object_hash_code_error_on_object_is_uninitialized),
            cmocka_unit_test(check_object_hash_code_error_on_invalid_value),
            cmocka_unit_test(check_object_hash_code),
            cmocka_unit_test(check_object_is_equal_error_on_null_object_ptr),
            cmocka_unit_test(check_object_is_equal_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_is_equal_on_object_is_uninitialized),
            cmocka_unit_test(check_object_is_equal),
            cmocka_unit_test(check_object_retain_error_on_null_object_ptr),
            cmocka_unit_test(check_object_retain_error_on_object_is_uninitialized),
            cmocka_unit_test(check_object_release_error_on_null_object_ptr),
            cmocka_unit_test(check_object_release_error_on_object_is_uninitialized),
            cmocka_unit_test(check_object_autorelease_error_on_null_object_ptr),
            cmocka_unit_test(check_object_autorelease_error_on_object_is_uninitialized),
            cmocka_unit_test(check_object_add_error_on_null_object_ptr),
            cmocka_unit_test(check_object_add_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_add_error_on_object_is_uninitialized),
            cmocka_unit_test(check_object_add_error_on_invalid_value),
            cmocka_unit_test(check_object_add),
            cmocka_unit_test(check_object_subtract_error_on_null_object_ptr),
            cmocka_unit_test(check_object_subtract_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_subtract_error_on_object_is_uninitialized),
            cmocka_unit_test(check_object_subtract_error_on_invalid_value),
            cmocka_unit_test(check_object_subtract),
            cmocka_unit_test(check_object_multiply_error_on_null_object_ptr),
            cmocka_unit_test(check_object_multiply_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_multiply_error_on_object_is_uninitialized),
            cmocka_unit_test(check_object_multiply_error_on_invalid_value),
            cmocka_unit_test(check_object_multiply),
            cmocka_unit_test(check_object_divide_error_on_null_object_ptr),
            cmocka_unit_test(check_object_divide_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_divide_error_on_object_is_uninitialized),
            cmocka_unit_test(check_object_divide_error_on_invalid_value),
            cmocka_unit_test(check_object_divide),
            cmocka_unit_test(check_object_absolute_error_on_null_object_ptr),
            cmocka_unit_test(check_object_absolute_error_on_object_is_uninitialized),
            cmocka_unit_test(check_object_absolute_error_on_invalid_value),
            cmocka_unit_test(check_object_absolute),
            cmocka_unit_test(check_object_negate_error_on_null_object_ptr),
            cmocka_unit_test(check_object_negate_error_on_object_is_uninitialized),
            cmocka_unit_test(check_object_negate_error_on_invalid_value),
            cmocka_unit_test(check_object_negate),
            cmocka_unit_test(check_object_compare_error_on_null_object_ptr),
            cmocka_unit_test(check_object_compare_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_compare_error_on_object_is_uninitialized),
            cmocka_unit_test(check_object_compare_error_on_invalid_value),
            cmocka_unit_test(check_object_compare),
            cmocka_unit_test(check_object_and_error_on_null_object_ptr),
            cmocka_unit_test(check_object_and_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_and_error_on_invalid_value),
            cmocka_unit_test(check_object_and_error_on_object_is_uninitialized),
            cmocka_unit_test(check_object_and),
            cmocka_unit_test(check_object_or_error_on_null_object_ptr),
            cmocka_unit_test(check_object_or_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_or_error_on_invalid_value),
            cmocka_unit_test(check_object_or_error_on_object_is_uninitialized),
            cmocka_unit_test(check_object_or),
            cmocka_unit_test(check_object_not_error_on_null_object_ptr),
            cmocka_unit_test(check_object_not_invalid_value),
            cmocka_unit_test(check_object_not_error_on_object_is_uninitialized),
            cmocka_unit_test(check_object_not),
            cmocka_unit_test(check_object_xor_error_on_null_object_ptr),
            cmocka_unit_test(check_object_xor_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_xor_error_on_invalid_value),
            cmocka_unit_test(check_object_xor_error_on_object_uninitialized),
            cmocka_unit_test(check_object_xor),
            cmocka_unit_test(check_object_shift_left_error_on_null_object_ptr),
            cmocka_unit_test(check_object_shift_left_error_on_invalid_value),
            cmocka_unit_test(check_object_shift_left_error_on_object_uninitialized),
            cmocka_unit_test(check_object_shift_left),
            cmocka_unit_test(check_object_shift_right_error_on_null_object_ptr),
            cmocka_unit_test(check_object_shift_right_error_on_invalid_value),
            cmocka_unit_test(check_object_shift_right_error_on_object_uninitialized),
            cmocka_unit_test(check_object_shift_right),
            cmocka_unit_test(check_object_length_error_on_null_object_ptr),
            cmocka_unit_test(check_object_length_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_length_error_on_invalid_value),
            cmocka_unit_test(check_object_length_error_on_object_uninitialized),
            cmocka_unit_test(check_object_length),
            cmocka_unit_test(check_object_count_error_on_null_object_ptr),
            cmocka_unit_test(check_object_count_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_count_error_on_invalid_value),
            cmocka_unit_test(check_object_count_error_on_object_uninitialized),
            cmocka_unit_test(check_object_count),
            cmocka_unit_test(check_object_get_error_on_null_object_ptr),
            cmocka_unit_test(check_object_get_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_get_error_on_invalid_value),
            cmocka_unit_test(check_object_get_error_on_object_uninitialized),
            cmocka_unit_test(check_object_get),
            cmocka_unit_test(check_object_set_error_on_null_object_ptr),
            cmocka_unit_test(check_object_set_error_on_invalid_value),
            cmocka_unit_test(check_object_set_error_on_object_uninitialized),
            cmocka_unit_test(check_object_set),
            cmocka_unit_test(check_object_find_error_on_null_object_ptr),
            cmocka_unit_test(check_object_find_error_on_null_argument_ptr),
            cmocka_unit_test(check_object_find_error_on_invalid_value),
            cmocka_unit_test(check_object_find_error_on_object_uninitialized),
            cmocka_unit_test(check_object_find_error_on_object_not_found),
            cmocka_unit_test(check_object_find),
            cmocka_unit_test(check_object_find_with_values),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}