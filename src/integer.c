#include <coral.h>
#include <string.h>

#include "private/integer.h"
#include "private/class.h"
#include "test/cmocka.h"

#pragma mark private -

static mpz_t $zero;
static struct coral_class *$class;

#pragma mark + invokables

static bool $integer_destroy(struct coral_integer *this,
                             void *data,
                             void *args);

static bool $integer_hash_code(void *this,
                               struct coral_integer *data,
                               struct hash_code_args *args);

static bool $integer_is_equal(void *this,
                              struct coral_integer *data,
                              struct is_equal_args *args);

__attribute__((constructor(CORAL_CLASS_LOAD_PRIORITY_INTEGER)))
static void $on_load() {
    mpz_init($zero);
    struct coral_class_method_name $method_names[] = {
            {destroy,   strlen(destroy)},
            {hash_code, strlen(hash_code)},
            {is_equal,  strlen(is_equal)},
    };
    coral_required_true(coral_class_alloc(&$class));
    coral_required_true(coral_class_init($class));
    coral_required_true(coral_class_retain($class));
    /* destroy */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[0],
            (coral_invokable_t) $integer_destroy));
    /* hash_code */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[1],
            (coral_invokable_t) $integer_hash_code));
    /* is_equal */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[2],
            (coral_invokable_t) $integer_is_equal));
    /* copy */


    coral_autorelease_pool_drain();
}

__attribute__((destructor(CORAL_CLASS_LOAD_PRIORITY_INTEGER)))
static void $on_unload() {
    coral_required_true(coral_class_destroy($class));
    mpz_clear($zero);

    coral_autorelease_pool_drain();
}

bool coral$integer$of_char_ptr(struct coral$integer *out, const char *value) {
    if (!out || !value) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (coral$integer$init(out)) {
        if (!mpz_set_str(out->mpz, value, 0)) {
            return true;
        }
        coral_required_true(coral$integer$invalidate(out));
        coral_error = CORAL_ERROR_INVALID_VALUE;
    }
    return false;
}

bool coral$integer$of_size_t(struct coral$integer *out, const size_t value) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (coral$integer$init(out)) {
        mpz_set_ui(out->mpz, value);
        return true;
    }
    return false;
}

bool coral$integer$of_ssize_t(struct coral$integer *out, const ssize_t value) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (coral$integer$init(out)) {
        mpz_set_si(out->mpz, value);
        return true;
    }
    return false;
}

bool coral$integer$init(struct coral$integer *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    mpz_init(object->mpz);
    return true;
}

bool coral$integer$invalidate(struct coral$integer *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    mpz_clear(object->mpz);
    return true;
}

bool coral$integer$add(struct coral$integer *object,
                       struct coral$integer *other) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!other) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    mpz_add(object->mpz, object->mpz, other->mpz);
    return true;
}

bool coral$integer$subtract(struct coral$integer *object,
                            struct coral$integer *other) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!other) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    mpz_sub(object->mpz, object->mpz, other->mpz);
    return true;
}

bool coral$integer$multiply(struct coral$integer *object,
                            struct coral$integer *other) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!other) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    mpz_mul(object->mpz, object->mpz, other->mpz);
    return true;
}

bool coral$integer$divide(struct coral$integer *object,
                          struct coral$integer *divisor,
                          struct coral$integer *remainder) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!divisor) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (!mpz_cmp(divisor->mpz, $zero)) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    }
    if (remainder) {
        mpz_tdiv_qr(object->mpz, remainder->mpz, object->mpz, divisor->mpz);
    } else {
        mpz_tdiv_q(object->mpz, object->mpz, divisor->mpz);
    }
    return true;
}

bool coral$integer$absolute(struct coral$integer *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    mpz_abs(object->mpz, object->mpz);
    return true;
}

bool coral$integer$negate(struct coral$integer *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    mpz_neg(object->mpz, object->mpz);
    return true;
}

bool coral$integer$compare(struct coral$integer *object,
                           struct coral$integer *other,
                           int *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!other || !out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    *out = mpz_cmp(object->mpz, other->mpz);
    return true;
}

bool coral$integer$and(struct coral$integer *object,
                       struct coral$integer *other) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!other) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    mpz_and(object->mpz, object->mpz, other->mpz);
    return true;
}

bool coral$integer$or(struct coral$integer *object,
                      struct coral$integer *other) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!other) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    mpz_ior(object->mpz, object->mpz, other->mpz);
    return true;
}

bool coral$integer$not(struct coral$integer *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    mpz_com(object->mpz, object->mpz);
    return true;
}

bool coral$integer$xor(struct coral$integer *object,
                       struct coral$integer *other) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!other) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    mpz_xor(object->mpz, object->mpz, other->mpz);
    return true;
}

bool coral$integer$shift_left(struct coral$integer *object, size_t nbits) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    mpz_mul_2exp(object->mpz, object->mpz, nbits);
    return true;
}

bool coral$integer$shift_right(struct coral$integer *object, size_t nbits) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    mpz_tdiv_q_2exp(object->mpz, object->mpz, nbits);
    return true;
}

bool coral$integer$length(struct coral$integer *object,
                          size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    *out = mpz_sizeinbase(object->mpz, 2);
    return true;
}

bool coral$integer$count(struct coral$integer *object,
                         size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    mpz_t result;
    mpz_init(result);
    mpz_abs(result, object->mpz);
    *out = mpz_popcount(result);
    mpz_clear(result);
    return true;
}

bool coral$integer$get(struct coral$integer *object,
                       size_t at,
                       bool *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    *out = mpz_tstbit(object->mpz, at);
    return true;
}

bool coral$integer$set(struct coral$integer *object,
                       size_t at,
                       bool value) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (value) {
        mpz_setbit(object->mpz, at);
    } else {
        mpz_clrbit(object->mpz, at);
    }
    return true;
}

bool coral$integer$find(struct coral$integer *object,
                        struct coral_range_values *values,
                        bool needle,
                        size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    size_t length;
    coral_required_true(coral$integer$length(object, &length));
    size_t min = 0, max = length;
    if (values) {
        coral_minimum_size_t(values->first, values->last, &min);
        coral_maximum_size_t(values->first, values->last, &max);
    }
    if (needle) {
        *out = mpz_scan1(object->mpz, min);
    } else {
        *out = mpz_scan0(object->mpz, min);
    }
    if (max < *out || SIZE_MAX == *out) {
        coral_error = CORAL_ERROR_OBJECT_NOT_FOUND;
        return false;
    }
    return true;
}

struct coral_integer {
    struct coral$integer integer;
};

static bool $integer_instance_of(void *instance) {
    coral_required(instance);
    bool result;
    coral_required_true(coral_object_instance_of(instance,
                                                 $class,
                                                 &result));
    if (!result) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
    }
    return result;
}

static bool $integer_destroy(struct coral_integer *this,
                             void *data,
                             void *args) {
    coral_required(this);
    return coral$integer$invalidate(&this->integer);
}

static bool $integer_hash_code(void *this,
                               struct coral_integer *data,
                               struct hash_code_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    if (!$integer_instance_of(this)) {
        return false;
    }
    *args->out = mpz_get_si(data->integer.mpz);
    return true;
}

static bool $integer_is_equal(void *this,
                              struct coral_integer *data,
                              struct is_equal_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    coral_required(args->other);
    void *objects[2] = {
            this, args->other
    };
    for (size_t i = 0; i < 2; i++) {
        bool result;
        coral_required_true(coral_object_instance_of(objects[i],
                                                     $class,
                                                     &result));
        if (!result) {
            *args->out = false;
            return true;
        }
    }
    int result;
    struct coral_integer *other = args->other;
    if (!coral$integer$compare(&data->integer,
                               &other->integer,
                               &result)) {
        return false;
    }
    *args->out = 0 == result;
    return true;
}

struct $integer_add_args {
    struct coral_integer *other;
};

static bool $integer_add(void *this,
                         struct coral_integer *data,
                         struct $integer_add_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->other);
    void *objects[2] = {
            this, args->other
    };
    for (size_t i = 0; i < 2; i++) {
        if (!$integer_instance_of(objects[i])) {
            return false;
        }
    }
    return coral$integer$add(&data->integer, &args->other->integer);
}

struct $integer_subtract_args {
    struct coral_integer *other;
};

static bool $integer_subtract(void *this,
                              struct coral_integer *data,
                              struct $integer_subtract_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->other);
    void *objects[2] = {
            this, args->other
    };
    for (size_t i = 0; i < 2; i++) {
        if (!$integer_instance_of(objects[i])) {
            return false;
        }
    }
    return coral$integer$subtract(&data->integer, &args->other->integer);
}

struct $integer_multiply_args {
    struct coral_integer *other;
};

static bool $integer_multiply(void *this,
                              struct coral_integer *data,
                              struct $integer_multiply_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->other);
    void *objects[2] = {
            this, args->other
    };
    for (size_t i = 0; i < 2; i++) {
        if (!$integer_instance_of(objects[i])) {
            return false;
        }
    }
    return coral$integer$multiply(&data->integer, &args->other->integer);
}

struct $integer_divide_args {
    struct coral_integer *divisor;
    struct coral_integer **remainder;
};

static bool $integer_divide(void *this,
                            struct coral_integer *data,
                            struct $integer_divide_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->divisor);
    void *objects[2] = {
            this, args->divisor
    };
    for (size_t i = 0; i < 2; i++) {
        if (!$integer_instance_of(objects[i])) {
            return false;
        }
    }
    struct coral_integer *object;
    struct coral$integer *remainder = NULL;
    if (args->remainder) {
        if (!coral_integer_of_size_t(&object, 0)) {
            return false;
        }
        coral_required_true(coral_object_autorelease(object));
        remainder = &object->integer;
        *args->remainder = object;
    }
    return coral$integer$divide(&data->integer, &args->divisor->integer,
                                remainder);
}

static bool $integer_absolute(void *this,
                              struct coral_integer *data,
                              void *args) {
    coral_required(data);
    if (!$integer_instance_of(this)) {
        return false;
    }
    return coral$integer$absolute(&data->integer);
}

static bool $integer_negate(void *this,
                            struct coral_integer *data,
                            void *args) {
    coral_required(data);
    if (!$integer_instance_of(this)) {
        return false;
    }
    return coral$integer$negate(&data->integer);
}

struct $integer_compare_args {
    struct coral_integer *other;
    int *out;
};

static bool $integer_compare(void *this,
                             struct coral_integer *data,
                             struct $integer_compare_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->other);
    coral_required(args->out);
    void *objects[2] = {
            this, args->other
    };
    for (size_t i = 0; i < 2; i++) {
        if (!$integer_instance_of(objects[i])) {
            return false;
        }
    }
    return coral$integer$compare(&data->integer, &args->other->integer,
                                 args->out);
}

struct $integer_and_args {
    struct coral_integer *other;
};

static bool $integer_and(void *this,
                         struct coral_integer *data,
                         struct $integer_and_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->other);
    void *objects[2] = {
            this, args->other
    };
    for (size_t i = 0; i < 2; i++) {
        if (!$integer_instance_of(objects[i])) {
            return false;
        }
    }
    return coral$integer$and(&data->integer, &args->other->integer);
}

struct $integer_or_args {
    struct coral_integer *other;
};

static bool $integer_or(void *this,
                        struct coral_integer *data,
                        struct $integer_or_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->other);
    void *objects[2] = {
            this, args->other
    };
    for (size_t i = 0; i < 2; i++) {
        if (!$integer_instance_of(objects[i])) {
            return false;
        }
    }
    return coral$integer$or(&data->integer, &args->other->integer);
}

static bool $integer_not(void *this,
                         struct coral_integer *data,
                         void *args) {
    coral_required(data);
    if (!$integer_instance_of(this)) {
        return false;
    }
    return coral$integer$not(&data->integer);
}

struct $integer_xor_args {
    struct coral_integer *other;
};

static bool $integer_xor(void *this,
                         struct coral_integer *data,
                         struct $integer_xor_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->other);
    void *objects[2] = {
            this, args->other
    };
    for (size_t i = 0; i < 2; i++) {
        if (!$integer_instance_of(objects[i])) {
            return false;
        }
    }
    return coral$integer$xor(&data->integer, &args->other->integer);
}

struct $integer_shift_left_args {
    size_t nbits;
};

static bool $integer_shift_left(void *this,
                                struct coral_integer *data,
                                struct $integer_shift_left_args *args) {
    coral_required(data);
    coral_required(args);
    if (!$integer_instance_of(this)) {
        return false;
    }
    return coral$integer$shift_left(&data->integer, args->nbits);
}

struct $integer_shift_right_args {
    size_t nbits;
};

static bool $integer_shift_right(void *this,
                                 struct coral_integer *data,
                                 struct $integer_shift_right_args *args) {
    coral_required(data);
    coral_required(args);
    if (!$integer_instance_of(this)) {
        return false;
    }
    return coral$integer$shift_right(&data->integer, args->nbits);
}

struct $integer_length_args {
    size_t *out;
};

static bool $integer_length(void *this,
                            struct coral_integer *data,
                            struct $integer_length_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    if (!$integer_instance_of(this)) {
        return false;
    }
    return coral$integer$length(&data->integer, args->out);
}

struct $integer_count_args {
    size_t *out;
};

static bool $integer_count(void *this,
                           struct coral_integer *data,
                           struct $integer_count_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    if (!$integer_instance_of(this)) {
        return false;
    }
    return coral$integer$count(&data->integer, args->out);
}

struct $integer_get_args {
    size_t at;
    bool *out;
};

static bool $integer_get(void *this,
                         struct coral_integer *data,
                         struct $integer_get_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    if (!$integer_instance_of(this)) {
        return false;
    }
    return coral$integer$get(&data->integer, args->at, args->out);
}

struct $integer_set_args {
    size_t at;
    bool value;
};

static bool $integer_set(void *this,
                         struct coral_integer *data,
                         struct $integer_set_args *args) {
    coral_required(data);
    coral_required(args);
    if (!$integer_instance_of(this)) {
        return false;
    }
    return coral$integer$set(&data->integer, args->at, args->value);
}

struct $integer_find_args {
    struct coral_range_values *values;
    bool needle;
    size_t *out;
};

static bool $integer_find(void *this,
                          struct coral_integer *data,
                          struct $integer_find_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->out);
    if (!$integer_instance_of(this)) {
        return false;
    }
    return coral$integer$find(&data->integer, args->values, args->needle,
                              args->out);
}

#pragma mark public -

bool coral_integer_of_char_ptr(struct coral_integer **out,
                               const char *value) {
    if (coral_integer_alloc(out)) {
        if (coral_object_init(*out, $class)
            && coral$integer$of_char_ptr(&(*out)->integer, value)) {
            return true;
        }
        const size_t error = coral_error;
        coral_required_true(coral_integer_destroy(*out));
        coral_error = error;
    }
    return false;
}

bool coral_integer_of_size_t(struct coral_integer **out,
                             const size_t value) {
    if (coral_integer_alloc(out)) {
        if (coral_object_init(*out, $class)
            && coral$integer$of_size_t(&(*out)->integer, value)) {
            return true;
        }
        const size_t error = coral_error;
        coral_required_true(coral_integer_destroy(*out));
        coral_error = error;
    }
    return false;
}

bool coral_integer_of_ssize_t(struct coral_integer **out,
                              const ssize_t value) {
    if (coral_integer_alloc(out)) {
        if (coral_object_init(*out, $class)
            && coral$integer$of_ssize_t(&(*out)->integer, value)) {
            return true;
        }
        const size_t error = coral_error;
        coral_required_true(coral_integer_destroy(*out));
        coral_error = error;
    }
    return false;
}

bool coral_integer_class(struct coral_class **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral_object_copy($class, (void **) out);
}

bool coral_integer_alloc(struct coral_integer **out) {
    return coral_object_alloc(sizeof(**out), (void **) out);
}

bool coral_integer_init(struct coral_integer *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    bool result = false;
    if (coral_object_init(object, $class)) {
        if (coral$integer$init(&object->integer)) {
            result = true;
        }
    }
    return result;
}

bool coral_integer_destroy(struct coral_integer *object) {
    return coral_object_destroy(object);
}

bool coral_integer_hash_code(struct coral_integer *object, size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct hash_code_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $integer_hash_code,
            &args);
}

bool coral_integer_is_equal(struct coral_integer *object,
                            struct coral_integer *other,
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
    return coral_object_invoke(
            object,
            (coral_invokable_t) $integer_is_equal,
            &args);
}

bool coral_integer_retain(struct coral_integer *object) {
    return coral_object_retain(object);
}

bool coral_integer_release(struct coral_integer *object) {
    return coral_object_release(object);
}

bool coral_integer_autorelease(struct coral_integer *object) {
    return coral_object_autorelease(object);
}

bool coral_integer_add(struct coral_integer *object,
                       struct coral_integer *other) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!other) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $integer_add_args args = {
            .other = other
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $integer_add,
            &args);
}

bool coral_integer_subtract(struct coral_integer *object,
                            struct coral_integer *other) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!other) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $integer_subtract_args args = {
            .other = other
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $integer_subtract,
            &args);
}

bool coral_integer_multiply(struct coral_integer *object,
                            struct coral_integer *other) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!other) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $integer_multiply_args args = {
            .other = other
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $integer_multiply,
            &args);
}

bool coral_integer_divide(struct coral_integer *object,
                          struct coral_integer *divisor,
                          struct coral_integer **remainder) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!divisor) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $integer_divide_args args = {
            .divisor = divisor,
            .remainder = remainder
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $integer_divide,
            &args);
}

bool coral_integer_absolute(struct coral_integer *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral_object_invoke(
            object,
            (coral_invokable_t) $integer_absolute,
            NULL);
}

bool coral_integer_negate(struct coral_integer *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral_object_invoke(
            object,
            (coral_invokable_t) $integer_negate,
            NULL);
}

bool coral_integer_compare(struct coral_integer *object,
                           struct coral_integer *other,
                           int *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!other || !out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $integer_compare_args args = {
            .other = other,
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $integer_compare,
            &args);
}

bool coral_integer_and(struct coral_integer *object,
                       struct coral_integer *other) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!other) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $integer_and_args args = {
            .other = other
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $integer_and,
            &args);
}

bool coral_integer_or(struct coral_integer *object,
                      struct coral_integer *other) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!other) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $integer_or_args args = {
            .other = other
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $integer_or,
            &args);
}

bool coral_integer_not(struct coral_integer *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral_object_invoke(
            object,
            (coral_invokable_t) $integer_not,
            NULL);
}

bool coral_integer_xor(struct coral_integer *object,
                       struct coral_integer *other) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!other) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $integer_xor_args args = {
        .other = other
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $integer_xor,
            &args);
}

bool coral_integer_shift_left(struct coral_integer *object,
                              size_t nbits) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!nbits) {
        return true;
    }
    struct $integer_shift_left_args args = {
            .nbits = nbits
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $integer_shift_left,
            &args);
}

bool coral_integer_shift_right(struct coral_integer *object,
                               size_t nbits) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!nbits) {
        return true;
    }
    struct $integer_shift_right_args args = {
        .nbits = nbits
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $integer_shift_right,
            &args);
}

bool coral_integer_length(struct coral_integer *object,
                          size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $integer_length_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $integer_length,
            &args);
}

bool coral_integer_count(struct coral_integer *object,
                         size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $integer_count_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $integer_count,
            &args);
}

bool coral_integer_get(struct coral_integer *object,
                       size_t at,
                       bool *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $integer_get_args args = {
            .at = at,
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t)$integer_get,
            &args);
}

bool coral_integer_set(struct coral_integer *object,
                       size_t at,
                       bool value) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    struct $integer_set_args args = {
            .at = at,
            .value = value
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $integer_set,
            &args);
}

bool coral_integer_find(struct coral_integer *object,
                        struct coral_range_values *values,
                        bool needle,
                        size_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $integer_find_args args = {
            .values = values,
            .needle = needle,
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $integer_find,
            &args);
}
