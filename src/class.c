#include <stddef.h>
#include <string.h>
#include <coral.h>

#include "private/class.h"
#include "private/tree_map.h"
#include "test/cmocka.h"

#pragma mark private -

static struct coral_class *$class;

#pragma mark +invokables

static bool $class_destroy(struct coral_class *this,
                           void *data,
                           void *args);

static bool $class_hash_code(void *this,
                             struct coral_class *data,
                             struct hash_code_args *args);

static bool $class_is_equal(void *this,
                            struct coral_class *data,
                            struct is_equal_args *args);

__attribute__((constructor(CORAL_CLASS_LOAD_PRIORITY_BOOTSTRAP)))
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
            (coral_invokable_t) $class_destroy));
    /* hash_code */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[1],
            (coral_invokable_t) $class_hash_code));
    /* is_equal */
    coral_required_true(coral_class_method_add(
            $class, &$method_names[2],
            (coral_invokable_t) $class_is_equal));

    coral_autorelease_pool_drain();
}

__attribute__((destructor(CORAL_CLASS_LOAD_PRIORITY_BOOTSTRAP)))
static void $on_unload() {
    coral_required_true(coral_object_destroy($class));

    coral_autorelease_pool_drain();
}

struct coral_class {
    struct coral$tree_map methods;
};

struct $method_entry {
    struct {
        struct coral_class_method_name name;
    } key;
    struct {
        coral_invokable_t invokable;
    } value;
};

static int
$class_method_name_compare(const struct coral_class_method_name *a,
                           const struct coral_class_method_name *b) {
    if (a->size < b->size) {
        return (-1);
    } else if (a->size > b->size) {
        return 1;
    }
    return memcmp(a->data, b->data, a->size);
}

static bool $class_destroy(struct coral_class *this,
                           void *data,
                           void *args) {
    coral_required(this);
    return coral$tree_map$invalidate(&this->methods, NULL);
}

static bool $class_hash_code(void *this,
                             struct coral_class *data,
                             struct hash_code_args *args) {
    coral_required(this);
    coral_required(args);
    coral_required(args->out);
    return coral_object_hash_code(this, args->out);
}

static bool $class_is_equal(void *this,
                            struct coral_class *data,
                            struct is_equal_args *args) {
    coral_required(this);
    coral_required(args);
    coral_required(args->out);
    return coral_object_is_equal(this, args->other, args->out);
}

struct $class_method_add_args {
    struct coral_class_method_name *name;
    coral_invokable_t invokable;
};

static bool $class_method_add(void *this,
                              struct coral_class *data,
                              struct $class_method_add_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->name);
    coral_required(args->name->data);
    coral_required_true(args->name->size > 0);
    coral_required(args->invokable);
    struct $method_entry entry;
    bool result = coral$tree_map$get(&data->methods, args->name,
                                     (void **) &entry);
    if (result) {
        coral_error = CORAL_ERROR_METHOD_ALREADY_EXISTS;
        return false;
    }
    entry.key.name = *args->name;
    entry.value.invokable = args->invokable;
    return coral$tree_map$insert(&data->methods, &entry);
}

struct $class_method_remove_args {
    struct coral_class_method_name *name;
};

static bool $class_method_remove(void *this,
                                 struct coral_class *data,
                                 struct $class_method_remove_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->name);
    coral_required(args->name->data);
    coral_required_true(args->name->size > 0);
    struct $method_entry entry;
    bool result = coral$tree_map$get(&data->methods, args->name,
                                     (void **) &entry);
    if (!result) {
        coral_required_true(CORAL_ERROR_OBJECT_NOT_FOUND == coral_error);
        coral_error = CORAL_ERROR_METHOD_NOT_FOUND;
        return false;
    }
    return coral$tree_map$delete(&data->methods, args->name);
}

struct $class_method_get_args {
    struct coral_class_method_name *name;
    coral_invokable_t *out;
};

static bool $class_method_get(void *this,
                              struct coral_class *data,
                              struct $class_method_get_args *args) {
    coral_required(data);
    coral_required(args);
    coral_required(args->name);
    coral_required(args->name->data);
    coral_required_true(args->name->size > 0);
    coral_required(args->out);
    struct $method_entry entry;
    bool result = coral$tree_map$get(&data->methods, args->name,
                                     (void **) &entry);
    if (!result) {
        coral_required_true(CORAL_ERROR_OBJECT_NOT_FOUND == coral_error);
        coral_error = CORAL_ERROR_METHOD_NOT_FOUND;
    } else {
        *args->out = entry.value.invokable;
    }
    return result;
}

#pragma mark public -

bool coral_class_alloc(struct coral_class **out) {
    return coral_object_alloc(sizeof(**out), (void **) out);
}

bool coral_class_init(struct coral_class *object) {
    return coral_object_init(object, $class)
           && coral$tree_map$init(
            &object->methods,
            sizeof((*(struct $method_entry *) 0).key),
            sizeof((*(struct $method_entry *) 0).value),
            (int (*)(const void *, const void *)) $class_method_name_compare);
}

bool coral_class_destroy(struct coral_class *object) {
    return coral_object_destroy(object);
}

bool coral_class_hash_code(struct coral_class *object, size_t *out) {
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
            (coral_invokable_t) $class_hash_code,
            &args);
}

bool coral_class_is_equal(struct coral_class *object, void *other, bool *out) {
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
    return coral_object_instance_of(object, $class, out)
           && *out
           && coral_object_instance_of(other, $class, out)
           && *out
           && coral_object_invoke(
            object,
            (coral_invokable_t) $class_is_equal,
            &args);
}

bool coral_class_copy(struct coral_class *object, struct coral_class *out) {
    return coral_object_copy(object, (void **) out);
}

bool coral_class_retain(struct coral_class *object) {
    return coral_object_retain(object);
}

bool coral_class_release(struct coral_class *object) {
    return coral_object_release(object);
}

bool coral_class_autorelease(struct coral_class *object) {
    return coral_object_autorelease(object);
}

bool coral_class_method_add(struct coral_class *object,
                            struct coral_class_method_name *name,
                            coral_invokable_t invokable) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!name || !invokable) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (!name->size || !name->data) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
        return false;
    }
    struct $class_method_add_args args = {
            .name = name,
            .invokable = invokable
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $class_method_add,
            &args);
}

bool coral_class_method_remove(struct coral_class *object,
                               struct coral_class_method_name *name) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!name) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (!name->size || !name->data) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
    }
    struct $class_method_remove_args args = {
            .name = name
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $class_method_remove,
            &args);
}

bool coral_class_method_get(struct coral_class *object,
                            struct coral_class_method_name *name,
                            coral_invokable_t *out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!name || !out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (!name->size || !name->data) {
        coral_error = CORAL_ERROR_INVALID_VALUE;
    }
    struct $class_method_get_args args = {
            .name = name,
            .out = out
    };
    return coral_object_invoke(
            object,
            (coral_invokable_t) $class_method_get,
            &args);
}
