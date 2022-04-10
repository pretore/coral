#include <stdlib.h>
#include <stdatomic.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <coral.h>

#include "private/coral.h"
#include "private/autorelease_pool.h"
#include "test/cmocka.h"

#pragma mark private

int coral_compare_void_ptr(const void *a, const void *b) {
    if (a == b) {
        return 0;
    } else if (a > b) {
        return 1;
    }
    return -1;
}

int coral_compare_size_t(const void *a, const void *b) {
    const size_t A = *(size_t *) a;
    const size_t B = *(size_t *) b;
    if (A == B) {
        return 0;
    } else if (A > B) {
        return 1;
    }
    return -1;
}

void coral$swap_void_ptr(void **a, void **b) {
    void *tmp = *a;
    *a = *b;
    *b = tmp;
}

void coral$swap_size_t(size_t *a, size_t *b) {
    coral$swap_void_ptr((void **) a, (void **) b);
}

size_t coral$atomic_load(const atomic_size_t *ptr) {
    coral_required(ptr);
    const atomic_size_t value = atomic_load(ptr);
    const size_t result = value;
    return result;
}

void coral$atomic_store(atomic_size_t *ptr, const size_t value) {
    coral_required(ptr);
    atomic_store(ptr, value);
}

bool coral$atomic_compare_exchange(atomic_size_t *ptr, const size_t compare,
                                   const size_t value) {
    coral_required(ptr);
    const atomic_size_t cmp = compare;
    const atomic_size_t val = value;
    return atomic_compare_exchange_strong(ptr, &cmp, val);
}

uintptr_t coral$atomic_load_ptr(const atomic_uintptr_t *ptr) {
    coral_required(ptr);
    const atomic_uintptr_t value = atomic_load(ptr);
    const uintptr_t result = value;
    return result;
}

void coral$atomic_store_ptr(atomic_uintptr_t *ptr, const uintptr_t value) {
    coral_required(ptr);
    atomic_store(ptr, value);
}

bool coral$atomic_compare_exchange_ptr(atomic_uintptr_t *ptr,
                                       const uintptr_t compare,
                                       const uintptr_t value) {
    coral_required(ptr);
    const atomic_uintptr_t cmp = compare;
    const atomic_uintptr_t val = value;
    return atomic_compare_exchange_strong(ptr, &cmp, val);
}

void coral$retain(atomic_size_t *ref_counter) {
    coral_required(ref_counter);
    const atomic_size_t value = atomic_fetch_add(ref_counter, 1);
    const size_t ref_count = value;
    /* check if we retained a destroyed or an uninitialized instance */
    coral_required_true(0 != ref_count && SIZE_MAX != ref_count);
}

void coral$release(void *object, atomic_size_t *ref_counter,
                   void(*on_destroy)(void *object)) {
    coral_required(object);
    coral_required(ref_counter);
    if (coral$atomic_compare_exchange(ref_counter, 1, 0)) {
        if (on_destroy) {
            on_destroy(object);
        }
        return;
    }
    if (!coral$atomic_load(ref_counter)) {
        return;
    }
    const atomic_size_t value = atomic_fetch_sub(ref_counter, 1);
    const size_t ref_count = value;
    /* if we just released a destroyed or an uninitialized instance */
    coral_required_true(0 != ref_count && SIZE_MAX != ref_count);
}

#pragma mark public

void coral_required(const void *obj) {
    coral_required_true(obj != NULL);
}

void coral_required_true(const int check) {
    if (!check) {
        abort();
    }
}

bool coral_add_size_t(const size_t a, const size_t b, size_t *out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    const size_t result = a + b;
    if (result < a) {
        coral_error = CORAL_ERROR_OVERFLOW_OCCURRED;
        return false;
    }
    *out = result;
    return true;
}

bool coral_multiply_size_t(const size_t a, const size_t b, size_t *out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (!a || !b) {
        *out = 0;
        return true;
    }
    const size_t result = a * b;
    if (result / a != b) {
        coral_error = CORAL_ERROR_OVERFLOW_OCCURRED;
        return false;
    }
    *out = result;
    return true;
}

bool coral_minimum_size_t(const size_t a, const size_t b, size_t *out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    *out = a < b ? a : b;
    return true;
}

bool coral_maximum_size_t(const size_t a, const size_t b, size_t *out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    *out = a > b ? a : b;
    return true;
}

bool coral_set_ref(struct coral_ref **out, void *object) {
    return coral_ref_of(out, object) && coral_ref_retain(*out);
}

bool coral_clear_ref(struct coral_ref **ref) {
    if (ref && *ref) {
        if (!coral_ref_release(*ref)) {
            return false;
        }
        *ref = NULL;
    }
    return true;
}

bool coral_set_weak_ref(struct coral_weak_ref **out, void *object) {
    return coral_weak_ref_of(out, object) && coral_weak_ref_retain(*out);
}

bool coral_clear_weak_ref(struct coral_weak_ref **ref) {
    if (ref && *ref) {
        if (!coral_weak_ref_release(*ref)) {
            return false;
        }
        *ref = NULL;
    }
    return true;
}

bool coral_exponential_usleep(uint8_t *state, uint32_t maximum) {
    if (!state) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (!maximum) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    } else if (maximum > 999999) {
        maximum = 999999;
    }
    if (*state >= CHAR_BIT * sizeof(uint32_t) - 1) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        *state = 0;
        return false;
    }
    const unsigned int first = 1 << *state;
    const unsigned int second = (first << 1);
    const unsigned int random_ = random() % (second - first);
    unsigned int limit = first + random_;
    if (limit < first || maximum < limit) {
        limit = maximum;
    }
    struct timespec req = {
            .tv_sec = 0,
            .tv_nsec = limit * 1000
    };
    if (nanosleep(&req, NULL)) {
        coral_error = CORAL_ERROR_SYSCALL_FAILED;
        return false;
    }
    if (limit != maximum) {
        *state = 1 + *state;
    }
    return true;
}
