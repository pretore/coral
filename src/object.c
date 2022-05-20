#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <coral.h>

#include "private/autorelease_pool.h"
#include "private/coral.h"
#include "private/object.h"
#include "private/class.h"
#include "private/lock.h"
#include "test/cmocka.h"

#pragma mark private -
#pragma mark + dispatch methods

const char *destroy = "destroy";
const char *hash_code = "hash_code";
const char *is_equal = "is_equal";
const char *copy = "copy";

#pragma mark + invokables

static bool $object_hash_code(void *this,
                              void *data,
                              struct hash_code_args *args);

static bool $object_is_equal(void *this,
                             void *data,
                             struct is_equal_args *args);

static struct coral_class *$class;

__attribute__((constructor(CORAL_CLASS_LOAD_PRIORITY_RUNTIME)))
static void $on_load() {
    struct coral_class_method_name $method_names[] = {
            {hash_code, strlen(hash_code)},
            {is_equal,  strlen(is_equal)}
    };
    coral_required_true(coral_class_alloc(&$class));
    coral_required_true(coral_class_init($class));
    coral_required_true(coral_class_retain($class));
    /* hash_code */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[0],
            (coral_invokable_t) $object_hash_code));
    /* is_equal */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[1],
            (coral_invokable_t) $object_is_equal));

    coral_autorelease_pool_drain();
}

__attribute__((destructor(CORAL_CLASS_LOAD_PRIORITY_RUNTIME)))
static void $on_unload() {
    coral_required_true(coral_object_destroy($class));
    coral_autorelease_pool_drain();
}

struct coral_object *coral$object_from(void *object) {
    coral_required(object);
    const char *object_ = object;
    return (struct coral_object *) (object_ - sizeof(struct coral_object));
}

void *coral$object_to(struct coral_object *object) {
    coral_required(object);
    const char *object_ = (const char *) object;
    return (void *) (object_ + sizeof(struct coral_object));
}

static struct coral_object *coral$object_resolve(struct coral_object *object) {
    coral_required(object);
    while (object->copy_of) {
        object = coral$object_from(object->copy_of);
    }
    return object;
}

static bool $object_alloc(const size_t size, void **out) {
    coral_required(out);
    size_t size_;
    struct coral_object *object;
    if (!coral_add_size_t(sizeof(*object), size, &size_)) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    object = calloc(1, size_);
    if (!object) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    object->size = size;
    *out = coral$object_to(object);
    return true;
}

static bool $object_init(void *object, struct coral_class *class) {
    coral_required(object);
    struct coral_object *object_ = coral$object_from(object);
    if (!object_->class
        && !object_->checksum
        && coral$atomic_compare_exchange(&object_->ref_count, 0, 1)) {
        if (!class) {
            class = $class;
        }
        object_->class = coral$object_to(coral$object_resolve(
                coral$object_from(class)));
        object_->checksum = (size_t) object_
                            ^ (size_t) object_->class
                            ^ (size_t) object_->size;
        coral$autorelease_pool_add_object_init(object);
        return true;
    }
    coral_error = CORAL_ERROR_INITIALIZATION_FAILED;
    return false;
}

static bool $is_object(struct coral_object *object) {
    coral_required(object);
    return ((size_t) object
            ^ (size_t) object->class
            ^ (size_t) object->size) == object->checksum;
}

static bool $object_retain(void *this, void *data, void *args) {
    coral_required(this);
    struct coral_object *object_ = coral$object_from(this);
    coral$retain(&object_->ref_count);
    return true;
}

static void $object_on_destroy(void *object) {
    coral_required_true(coral_object_destroy(object));
}

static bool $object_release(void *this, void *data, void *args) {
    coral_required(this);
    struct coral_object *object_ = coral$object_from(this);
    coral$release(this, &object_->ref_count, $object_on_destroy);
    return true;
}

static bool $object_autorelease(void *this, void *data, void *args) {
    coral_required(this);
    const bool result = $object_retain(this, NULL, NULL);
    if (result) {
        coral$autorelease_pool_add(this);
    }
    return result;
}

static bool
$object_hash_code(void *this, void *data, struct hash_code_args *args) {
    coral_required(this);
    coral_required(args);
    *args->out = (size_t) this;
    return true;
}

static bool
$object_is_equal(void *this, void *data, struct is_equal_args *args) {
    coral_required(this);
    coral_required(args);
    coral_required(args->other);
    coral_required(args->out);
    *args->out = (this == args->other);
    return true;
}

static bool
$object_get_dispatch(void *object, const char *method, coral_invokable_t *out) {
    coral_required(object);
    coral_required(method);
    coral_required(out);
    struct coral_object *object_ = coral$object_from(object);
    if (!$is_object(object_)) { /* checksum failed */
        coral_error = CORAL_ERROR_OBJECT_IS_UNINITIALIZED;
        return false;
    }
    struct coral_class *class = object_->class;
    struct coral_class_method_name name = {
            .data = method,
            .size = strlen(method)
    };
    return coral_class_method_get(class, &name, out);
}

void
coral$object_add_observer(struct coral_object *object,
                          void *observer, const char *event,
                          void(*on_event)(void *object, const char *event)) {
    coral_required(object);
    coral_required(observer);
    // when event = NULL that means all events
}

void
coral$object_remove_observer(struct coral_object *object,
                             void *observer, const char *event) {
    coral_required(object);
    coral_required(observer);
    // when event = NULL that means all events
}

void coral$object_post_notification(void *object, const char *notification) {
    coral_required(object);
    coral_required(notification);
    // lock on the dictionary for events ...
    // for every item in the event ... post to
}

#pragma mark public -

bool coral_object_invoke(void *object, coral_invokable_t function, void *args) {
    coral_required(object);
    coral_required(function);
    struct coral_object *object_ = coral$object_from(object);
    const size_t ref_count = coral$atomic_load(&object_->ref_count);
    if (!ref_count /* object is uninitialized */
        || !$is_object(object_) /* checksum failed */) {
        coral_error = CORAL_ERROR_OBJECT_IS_UNINITIALIZED;
        return false;
    }
    void *data = coral$object_to(coral$object_resolve(object_));
    coral$autorelease_pool_start();
    const bool result = function(object, data, args);
    coral$autorelease_pool_end();
    return result;
}

bool coral_object_dispatch(void *object, const char *method, void *args) {
    coral_invokable_t function;
    return $object_get_dispatch(object, method, &function)
           && coral_object_invoke(object, function, args);
}

bool coral_object_class(struct coral_class **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral_object_copy($class, (void **) out);
}

bool coral_object_alloc(const size_t size, void **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return $object_alloc(size, out);
}

bool coral_object_init(void *object, struct coral_class *class) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!class) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return $object_init(object, class);
}

bool coral_object_destroy(void *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    coral_invokable_t destroy_func = NULL;
    if (!$object_get_dispatch(object, destroy, &destroy_func)
        && CORAL_ERROR_METHOD_NOT_FOUND != coral_error
        && CORAL_ERROR_OBJECT_IS_UNINITIALIZED != coral_error) {
        return false;
    }
    struct coral_object *object_ = coral$object_from(object);
    coral$atomic_store(&object_->ref_count, 0);
    if (object_->copy_of) {
        coral_required_true($object_release(object_->copy_of, NULL, NULL));
    }
    if (destroy_func) {
        coral_required_true(destroy_func(object, object, NULL));
    }
    coral$object_post_notification(object, CORAL_NOTIFICATION_OBJECT_DESTROYED);
    free(object_);
    return true;
}

bool coral_object_instance_of(void *object, struct coral_class *class,
                              bool *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!class || !out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct coral_object *object_ = coral$object_from(object);
    *out = object_->class == class;
    return true;
}

bool coral_object_hash_code(void *object, size_t *out) {
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
            (coral_invokable_t) $object_hash_code,
            &args);
}

bool coral_object_is_equal(void *object, void *other, bool *out) {
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
            (coral_invokable_t) $object_is_equal,
            &args);
}

bool coral_object_copy(void *object, void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (object == *out) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    }
    coral_invokable_t copy_func = NULL;
    if (!$object_get_dispatch(object, copy, &copy_func)
        && CORAL_ERROR_METHOD_NOT_FOUND != coral_error) {
        return false;
    }
    bool result = false, did_init = false;
    struct coral_object *object_ = coral$object_from(object);
    if ($object_alloc(object_->size, out)
        && $object_init(*out, object_->class)
        && (did_init = true)
        && $object_retain(object, NULL, NULL)
        && (coral$object_from(*out)->copy_of = object)) {
        if (copy_func) { // TODO: change to a copy-on-write implementation ...
            struct copy_args args = {
                    .src = object
            };
            result = copy_func(*out, *out, &args);
        } else {
            result = true;
        }
    }
    if (!result) {
        if (!did_init) {
            const size_t error = coral_error;
            coral_required_true(coral_object_destroy(*out));
            coral_error = error;
        }
        *out = NULL;
    }
    return result;
}

bool coral_object_retain(void *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral_object_invoke(object, $object_retain, NULL);
}

bool coral_object_release(void *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral_object_invoke(object, $object_release, NULL);
}

bool coral_object_autorelease(void *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral_object_invoke(object, $object_autorelease, NULL);
}
