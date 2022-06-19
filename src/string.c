#include <stdlib.h>
#include <coral.h>
#include <string.h>

#include "private/string.h"
#include "private/tree_map.h"
#include "private/lock.h"
#include "private/class.h"
#include "test/cmocka.h"

#pragma mark private

static struct coral_class *$class;
static struct coral$lock $lock;
static struct coral$tree_map $pool;

struct coral_string {
    struct coral$string string;
};

struct $pool$entry {
    struct {
        struct coral_string *object;
    } key;
    struct {
        size_t count;
    } value;
};

static int $string$compare(const struct coral$string *a,
                           const struct coral$string *b) {
    if (a->size < b->size) {
        return (-1);
    } else if (a->size > b->size) {
        return 1;
    }
    return strncmp(a->data, b->data, a->size);
}

static void $string$on_destroy(struct coral$string *object) {
    free(object->data);
}

static void $pool$on_destroy(struct $pool$entry *entry) {
    coral_required_true(coral_string_destroy(entry->key.object));
}

__attribute__((constructor))
static void $on_load() {
    coral_required_true(coral$lock$init(&$lock));
    coral_required_true(coral$tree_map$init(
            &$pool,
            NULL,
            sizeof((*(struct $pool$entry *) 0).key),
            sizeof((*(struct $pool$entry *) 0).value),
            (int (*)(const void *, const void *)) $string$compare));

    coral_required_true(coral_class_alloc(&$class));
    coral_required_true(coral_class_init($class));
    // TODO: add interfaces ...
    // TODO: add attributes ...
    // TODO: add method signatures ...
    // TODO: set invokables ...
    coral_required_true(coral_class_retain($class));
    coral_autorelease_pool_drain();
}

__attribute__((destructor))
static void $on_unload() {
    coral_required_true(coral_class_release($class));
    coral_required_true(coral$tree_map$invalidate(
            &$pool, (void (*)(void *)) $pool$on_destroy));
    coral_required_true(coral$lock$invalidate(&$lock));
    coral_autorelease_pool_drain();
}

#pragma mark public

bool coral_string_alloc(struct coral_string **out) {
    return coral_object_alloc(sizeof(**out), (void **) out);
}

bool coral_string_init(struct coral_string *object,
                       unsigned char *data,
                       size_t size) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_NOT_FOUND;
        return false;
    }
    if (!data) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (!size) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    }
    void (*on_destroy)(void *) = (void (*)(void *)) $string$on_destroy;
    if (!coral_object_init(object, on_destroy)) {
        return false;
    }
    // TODO: check if there is an existing string ...
    //  (provide "copy-on-write" result)
    struct coral$string *string = &object->string;
    string->data = malloc(size);
    if (!string->data) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    memcpy(string->data, data, size);
    string->size = size;
    return true;
}

bool coral_string_destroy(struct coral_string *object) {
    return coral_object_destroy(object);
}

bool coral_string_class(struct coral_class const **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    *out = $class; // FIXME: return an autorelease copy
    return true;
}
