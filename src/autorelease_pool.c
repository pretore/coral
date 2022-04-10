#include <stddef.h>
#include <stdlib.h>
#include <threads.h>
#include <coral.h>

#include "private/autorelease_pool.h"
#include "test/cmocka.h"

#pragma mark private

static thread_local struct coral$autorelease_pool_entry *$pool = NULL;
static thread_local struct coral$autorelease_pool_entry *$stack = NULL;

static void
coral$autorelease_pool_drain(struct coral$autorelease_pool_entry *end) {
    while ($pool && $pool != end) {
        struct coral$autorelease_pool_entry *entry = $pool;
        if (entry->object && !coral_object_release(entry->object)) {
            coral_required_true(
                    CORAL_ERROR_OBJECT_IS_UNINITIALIZED == coral_error);
            coral_error = CORAL_ERROR_NONE;
        }
        $pool = entry->next;
        free(entry);
    }
}

static void
coral$autorelease_pool_push(struct coral$autorelease_pool_entry **head,
                            void *object) {
    struct coral$autorelease_pool_entry *entry = calloc(1, sizeof(*entry));
    // TODO: try and handle low memory conditions
    coral_required(entry);
    entry->object = object;
    entry->next = *head;
    *head = entry;
}

void coral$autorelease_pool_add_object_init(void *object) {
    coral$autorelease_pool_push(&$pool, object);
}

void coral$autorelease_pool_add(void *object) {
    coral_required($stack);
    struct coral$autorelease_pool_entry *entry = $stack->object;
    coral$autorelease_pool_push(&entry->next, object);
}

void coral$autorelease_pool_start() {
    if (!$pool) {
        $pool = calloc(1, sizeof(*$pool));
    }
    coral$autorelease_pool_push(&$stack, $pool);
}

void coral$autorelease_pool_end() {
    struct coral$autorelease_pool_entry *end = $stack;
    if (!end) {
        return;
    }
    $stack = end->next;
    coral$autorelease_pool_drain(end->object);
    free(end);
}

#pragma mark public

void coral_autorelease_pool_drain() {
    while ($stack) {
        struct coral$autorelease_pool_entry *entry = $stack;
        $stack = entry->next;
        free(entry);
    }
    coral$autorelease_pool_drain(NULL);
}
