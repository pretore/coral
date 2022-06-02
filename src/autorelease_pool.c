#include <stddef.h>
#include <stdlib.h>
#include <threads.h>
#include <coral.h>

#include "private/autorelease_pool.h"
#include "test/cmocka.h"

#pragma mark private

static thread_local struct coral$autorelease_pool$entry *$items = NULL;
static thread_local struct coral$autorelease_pool$entry *$stack = NULL;

static void
coral$autorelease_pool$drain(struct coral$autorelease_pool$entry *end) {
    while ($items && $items != end) {
        struct coral$autorelease_pool$entry *entry = $items;
        if (entry->object && !coral_object_release(entry->object)) {
            coral_required_true(
                    CORAL_ERROR_OBJECT_IS_UNINITIALIZED == coral_error);
            coral_error = CORAL_ERROR_NONE;
        }
        $items = entry->next;
        free(entry);
    }
}

static void
coral$autorelease_pool$push(struct coral$autorelease_pool$entry **head,
                            void *object) {
    coral_required(head);
    struct coral$autorelease_pool$entry *entry = calloc(1, sizeof(*entry));
    // TODO: try and handle low memory conditions
    coral_required(entry);
    entry->object = object;
    entry->next = *head;
    *head = entry;
}

void
coral$autorelease_pool$get_stack(struct coral$autorelease_pool$entry **out) {
    coral_required(out);
    *out = $stack;
}

void
coral$autorelease_pool$get_items(struct coral$autorelease_pool$entry **out) {
    coral_required(out);
    *out = $items;
}

void coral$autorelease_pool$add(void *object) {
    coral_required(object);
    coral$autorelease_pool$push(&$items, object);
}

void coral$autorelease_pool$add_previous(void *object) {
    coral_required(object);
    coral_required($stack);
    coral_required($stack->next);
    struct coral$autorelease_pool$entry *entry = $stack->next->object;
    coral$autorelease_pool$push(&entry->next, object);
}

void coral$autorelease_pool$start() {
    coral$autorelease_pool$push(&$items, NULL);
    coral$autorelease_pool$push(&$stack, $items);
}

void coral$autorelease_pool$end() {
    struct coral$autorelease_pool$entry *stack = $stack;
    if (!stack) {
        return;
    }
    $stack = stack->next;
    struct coral$autorelease_pool$entry *item = stack->object;
    coral$autorelease_pool$drain(item->next);
    free(stack);
}

#pragma mark public

void coral_autorelease_pool_drain() {
    while ($stack) {
        struct coral$autorelease_pool$entry *entry = $stack;
        $stack = entry->next;
        free(entry);
    }
    coral$autorelease_pool$drain(NULL);
}
