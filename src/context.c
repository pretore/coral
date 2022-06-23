#include <string.h>
#include <coral.h>

#include "private/context.h"
#include "private/class.h"

#pragma mark private -

static struct coral_class *$class;

#pragma mark + invokables

static bool $context_destroy(struct coral_context *this,
                             void *data,
                             void *args);

static bool $context_hash_code(void *this,
                               struct coral_context *data,
                               struct hash_code_args *args);

static bool $context_is_equal(void *this,
                              struct coral_context *data,
                              struct is_equal_args *args);

__attribute__((constructor(CORAL_CLASS_LOAD_PRIORITY_CONTEXT)))
static void $on_load() {
    struct coral_class_method_name $method_names[] = {
            {destroy,   strlen(destroy)},
            {hash_code, strlen(hash_code)},
            {is_equal,  strlen(is_equal)}
    };
    coral_required_true(coral_class_alloc(&$class));
    coral_required_true(coral_class_init($class));
    coral_required_true(coral_class_retain($class));
    /* destroy */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[0],
            (coral_invokable_t) $context_destroy));
    /* hash_code */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[1],
            (coral_invokable_t) $context_hash_code));
    /* is_equal */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[2],
            (coral_invokable_t) $context_is_equal));

    coral_autorelease_pool_drain();
}

__attribute__((destructor(CORAL_CLASS_LOAD_PRIORITY_CONTEXT)))
static void $on_unload() {
    coral_required_true(coral_class_destroy($class));

    coral_autorelease_pool_drain();
}

struct coral_context {
    void *data;
    void (*on_destroy)(void *);
};

struct $context_get_args {
    void **out;
};

static bool $context_get(void *this,
                         struct coral_context *data,
                         struct $context_get_args *args) {
    coral_required(data);
    coral_required(args);
    *args->out = &(*data).data;
    return true;
}

static bool $context_destroy(struct coral_context *this,
                             void *data,
                             void *args) {
    coral_required(this);
    coral_required(data);
    if (this->on_destroy) {
        this->on_destroy(this->data);
    }
    return true;
}

static bool $context_hash_code(void *this,
                               struct coral_context *data,
                               struct hash_code_args *args) {
    coral_required(this);
    coral_required(args);
    coral_required(args->out);
    return coral_object_hash_code(this, args->out);
}

static bool $context_is_equal(void *this,
                              struct coral_context *data,
                              struct is_equal_args *args) {
    coral_required(this);
    coral_required(args);
    coral_required(args->other);
    coral_required(args->out);
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
    return coral_object_is_equal(this, args->other, args->out);
}

#pragma mark public

bool coral_context_of(struct coral_context **out, void (*on_destroy)(void *)) {
    if (coral_context_alloc(out)) {
        if (coral_context_init(*out, on_destroy)) {
            return true;
        }
        coral_context_destroy(*out);
    }
    return false;
}

bool coral_context_class(struct coral_class **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    return coral_object_copy($class, (void **) out);
}

bool coral_context_alloc(struct coral_context **out) {
    return coral_object_alloc(sizeof(**out), (void **) out);
}

bool
coral_context_init(struct coral_context *object, void (*on_destroy)(void *)) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    object->on_destroy = on_destroy;
    return coral_object_init(object, $class);
}

bool coral_context_destroy(struct coral_context *object) {
    return coral_object_destroy(object);
}

bool coral_context_hash_code(struct coral_context *object, size_t *out) {
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
            true,
            (coral_invokable_t) $context_hash_code,
            &args);
}

bool coral_context_is_equal(void *object, void *other, bool *out) {
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
            true,
            (coral_invokable_t) $context_is_equal,
            &args);
}

bool coral_context_copy(void *object, void **out) {
    return coral_object_copy(object, out);
}

bool coral_context_retain(struct coral_context *object) {
    return coral_object_retain(object);
}

bool coral_context_release(struct coral_context *object) {
    return coral_object_release(object);
}

bool coral_context_autorelease(struct coral_context *object) {
    return coral_object_autorelease(object);
}

bool coral_context_get(struct coral_context *object, void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct $context_get_args args = {
            .out = out
    };
    return coral_object_invoke(
            object,
            true,
            (coral_invokable_t) $context_get,
            &args);
}