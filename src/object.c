#include <stddef.h>
#include <stdlib.h>
#include <coral.h>

#include "private/coral.h"
#include "private/object.h"
#include "private/autorelease_pool.h"
#include "test/cmocka.h"

#pragma mark private

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

bool coral$object_alloc(const size_t size, void **out) {
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
    *out = coral$object_to(object);
    return true;
}

bool coral$object_init(void *object, void (*on_destroy)(void *)) {
    coral_required(object);
    struct coral_object *object_ = coral$object_from(object);
    if (coral$atomic_compare_exchange(&object_->ref_count, 0, 1)) {
        object_->on_destroy = on_destroy;
        coral$autorelease_pool_add_object_init(object);
        return true;
    }
    coral_error = CORAL_ERROR_INITIALIZATION_FAILED;
    return false;
}

void coral$object_destroy(void *object) {
    coral_required(object);
    struct coral_object *object_ = coral$object_from(object);
    coral$atomic_store(&object_->ref_count, 0);
    coral$object_post_notification(object, CORAL_NOTIFICATION_OBJECT_DESTROYED);
    free(object_);
}

bool coral$object_retain(void *object, void *args) {
    coral_required(object);
    struct coral_object *object_ = coral$object_from(object);
    coral$retain(&object_->ref_count);
    return true;
}

static void coral$object_on_destroy_callback(void *object) {
    coral_required_true(coral_object_destroy(object));
}

bool coral$object_release(void *object, void *args) {
    coral_required(object);
    struct coral_object *object_ = coral$object_from(object);
    coral$release(object, &object_->ref_count,
                  coral$object_on_destroy_callback);
    return true;
}

bool coral$object_autorelease(void *object, void *args) {
    coral_required(object);
    coral$object_retain(object, args);
    coral$autorelease_pool_add(object);
    return true;
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

#pragma mark public

bool coral_object_invoke(void *object,
                         coral_invokable_t function,
                         void *args) {
    coral_required(object);
    coral_required(function);
    struct coral_object *object_ = coral$object_from(object);
    const size_t ref_count = coral$atomic_load(&object_->ref_count);
    if (!ref_count /* object is uninitialized */
        || SIZE_MAX == ref_count /* object is (begin) destroyed */) {
        coral_error = CORAL_ERROR_OBJECT_IS_UNINITIALIZED;
        return false;
    }
    coral$autorelease_pool_start();
    const bool result = function(object, args);
    coral$autorelease_pool_end();
    return result;
}

bool coral_object_alloc(const size_t size, void **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral$object_alloc(size, out);
}

bool coral_object_init(void *object, void(*on_destroy)(void *)) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral$object_init(object, on_destroy);
}

bool coral_object_destroy(void *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    struct coral_object *object_ = coral$object_from(object);
    coral$atomic_store(&object_->ref_count, SIZE_MAX);
    void (*on_destroy)(void *) = object_->on_destroy;
    if (on_destroy) {
        object_->on_destroy = NULL;
    } else {
        on_destroy = coral$object_destroy;
    }
    on_destroy(object);
    return true;
}

bool coral_object_retain(void *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$object_retain,
            NULL);
}

bool coral_object_release(void *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$object_release,
            NULL);
}

bool coral_object_autorelease(void *object) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    return coral_object_invoke(
            object,
            (coral_invokable_t) coral$object_autorelease,
            NULL);
}
