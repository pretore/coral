#include <stdlib.h>
#include <coral.h>

#include "private/red_black_tree.h"
#include "test/cmocka.h"

static bool coral$red_black_tree$node_is_on_left(void *node, void *left);

static bool coral$red_black_tree$rotate_N(void *Y, bool is_N_left);

static bool coral$red_black_tree$rotate_N_O(void *Y, bool is_N_left);

static void coral$red_black_tree$get_minimum(void *root, void **out);

static void coral$red_black_tree$get_maximum(void *root, void **out);

struct coral$red_black_tree$node *coral$red_black_tree$node_from(void *node) {
    coral_required(node);
    const char *node_ = node;
    return (struct coral$red_black_tree$node *)
            (node_ - sizeof(struct coral$red_black_tree$node));
}

void *coral$red_black_tree$node_to(struct coral$red_black_tree$node *node) {
    coral_required(node);
    const char *node_ = (const char *) node;
    return (void *) (node_ + sizeof(struct coral$red_black_tree$node));
}

bool coral$red_black_tree$node_of(size_t size, void **out) {
    if (coral$red_black_tree$node_alloc(size, out)) {
        if (coral$red_black_tree$node_init(*out)) {
            return true;
        }
        free(*out);
        *out = NULL;
    }
    return false;
}

bool coral$red_black_tree$node_alloc(const size_t size, void **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    size_t size_;
    struct coral$red_black_tree$node *node_;
    if (!coral_add_size_t(size, sizeof(*node_), &size_)) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    node_ = calloc(1, size_);
    if (!node_) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    *out = coral$red_black_tree$node_to(node_);
    return true;
}

bool coral$red_black_tree$node_init(void *node) {
    if (!node) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    struct coral$red_black_tree$node *node_;
    node_ = coral$red_black_tree$node_from(node);
    node_->parent = (void *) CORAL_RED_BLACK_TREE_COLOR_RED;
    node_->left = NULL;
    node_->right = NULL;
    return true;
}

bool coral$red_black_tree$node_destroy(void *node) {
    if (node) {
        struct coral$red_black_tree$node *node_;
        node_ = coral$red_black_tree$node_from(node);
        free(node_);
    }
    return true;
}

bool coral$red_black_tree$node_get_parent(void *node, void **out) {
    if (!node) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    struct coral$red_black_tree$node *node_;
    node_ = coral$red_black_tree$node_from(node);
    *out = (void *) ((size_t) node_->parent & ~1);
    return true;
}

bool coral$red_black_tree$node_set_parent(void *node, void *parent) {
    if (!node) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    struct coral$red_black_tree$node *node_;
    node_ = coral$red_black_tree$node_from(node);
    bool color;
    coral_required_true(coral$red_black_tree$node_get_color(
            node, &color));
    const size_t value = (size_t) parent | color;
    node_->parent = (struct coral$red_black_tree$node *) value;
    return true;
}

bool coral$red_black_tree$node_get_color(void *node, bool *out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (!node) { /* NIL nodes are black */
        *out = CORAL_RED_BLACK_TREE_COLOR_BLACK;
        return true;
    }
    struct coral$red_black_tree$node *node_;
    node_ = coral$red_black_tree$node_from(node);
    *out = (size_t) node_->parent & 1;
    return true;
}

bool coral$red_black_tree$node_set_color(void *node, const bool color) {
    if (!node) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    struct coral$red_black_tree$node *node_;
    node_ = coral$red_black_tree$node_from(node);
    size_t value = (~1 & (size_t) node_->parent) | (size_t) color;
    node_->parent = (struct coral$red_black_tree$node *) value;
    return true;
}

bool coral$red_black_tree$node_get_left(void *node, void **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (!node) { /* NIL nodes have no children */
        coral_error = CORAL_ERROR_END_OF_SEQUENCE;
        return false;
    }
    struct coral$red_black_tree$node *node_;
    node_ = coral$red_black_tree$node_from(node);
    *out = node_->left;
    return true;
}

bool coral$red_black_tree$node_set_left(void *node, void *left) {
    if (!node) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    struct coral$red_black_tree$node *node_;
    node_ = coral$red_black_tree$node_from(node);
    node_->left = left;
    return true;
}

bool coral$red_black_tree$node_get_right(void *node, void **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (!node) { /* NIL nodes have no children */
        coral_error = CORAL_ERROR_END_OF_SEQUENCE;
        return false;
    }
    struct coral$red_black_tree$node *node_;
    node_ = coral$red_black_tree$node_from(node);
    *out = node_->right;
    return true;
}

bool coral$red_black_tree$node_set_right(void *node, void *right) {
    if (!node) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    struct coral$red_black_tree$node *node_;
    node_ = coral$red_black_tree$node_from(node);
    node_->right = right;
    return true;
}

static bool coral$red_black_tree$node_is_on_left(void *node, void *left) {
    struct coral$red_black_tree$node *node_;
    node_ = coral$red_black_tree$node_from(node);
    const bool is_left = node_->left == left;
    if (!is_left) {
        coral_required_true(node_->right == left);
    }
    return is_left;
}

bool coral$red_black_tree$get_next(void *node, void **out) {
    if (!node) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    void *right;
    coral_required_true(coral$red_black_tree$node_get_right(
            node, &right));
    if (right) {
        coral$red_black_tree$get_minimum(right, out);
        return true;
    }
    void *parent;
    while (true) {
        coral_required_true(coral$red_black_tree$node_get_parent(
                node, &parent));
        if (!parent) {
            break;
        }
        coral_required_true(coral$red_black_tree$node_get_right(
                parent, &right));
        if (node != right) {
            *out = parent;
            return true;
        }
        node = parent;
    }
    coral_error = CORAL_ERROR_END_OF_SEQUENCE;
    return false;
}

bool coral$red_black_tree$get_prev(void *node, void **out) {
    if (!node) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    void *left;
    coral_required_true(coral$red_black_tree$node_get_left(
            node, &left));
    if (left) {
        coral$red_black_tree$get_maximum(left, out);
        return true;
    }
    void *parent;
    while (true) {
        coral_required_true(coral$red_black_tree$node_get_parent(
                node, &parent));
        if (!parent) {
            break;
        }
        coral_required_true(coral$red_black_tree$node_get_left(
                parent, &left));
        if (node != left) {
            *out = parent;
            return true;
        }
        node = parent;
    }
    coral_error = CORAL_ERROR_END_OF_SEQUENCE;
    return false;
}

static bool coral$red_black_tree$rotate_N(void *Y, const bool is_N_left) {
    coral_required(Y);
    bool (*const get_P)(void *, void **) = coral$red_black_tree$node_get_parent;
    bool (*const set_P)(void *, void *) = coral$red_black_tree$node_set_parent;
    void *X;
    coral_required_true(get_P(Y, &X));
    if (!X) {
        coral_error = CORAL_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT;
        return false;
    }
    bool (*const get_N)(void *, void **) = is_N_left
            ? coral$red_black_tree$node_get_left
            : coral$red_black_tree$node_get_right;
    bool (*const set_N)(void *, void *) = is_N_left
            ? coral$red_black_tree$node_set_left
            : coral$red_black_tree$node_set_right;
    bool (*const get_O)(void *, void **) = is_N_left
            ? coral$red_black_tree$node_get_right
            : coral$red_black_tree$node_get_left;
    bool (*const set_O)(void *, void *) = is_N_left
            ? coral$red_black_tree$node_set_right
            : coral$red_black_tree$node_set_left;
    void *y;
    coral_required_true(get_O(X, &y));
    if (Y != y) {
        coral_error = is_N_left
                      ? CORAL_RED_BLACK_TREE_ERROR_Y_IS_NOT_RIGHT_CHILD_OF_X
                      : CORAL_RED_BLACK_TREE_ERROR_Y_IS_NOT_LEFT_CHILD_OF_X;
        return false;
    }
    void *b;
    coral_required_true(get_N(Y, &b));
    if (b) {
        coral_required_true(set_P(b, X));
    }
    coral_required_true(set_O(X, b));
    void *p;
    coral_required_true(get_P(X, &p));
    if (!p) {
        coral_required_true(set_P(Y, NULL));
    } else {
        void *x;
        coral_required_true(get_N(p, &x));
        if (X == x) {
            coral_required_true(set_N(p, Y));
        } else {
            coral_required_true(set_O(p, Y));
        }
        coral_required_true(set_P(Y, p));
    }
    coral_required_true(set_N(Y, X) && set_P(X, Y));
    return true;
}

bool coral$red_black_tree$rotate_left(void *Y) {
    if (!Y) {
        coral_error = CORAL_RED_BLACK_TREE_ERROR_Y_IS_NULL;
        return false;
    }
    return coral$red_black_tree$rotate_N(Y, true);
}

bool coral$red_black_tree$rotate_right(void *Y) {
    if (!Y) {
        coral_error = CORAL_RED_BLACK_TREE_ERROR_Y_IS_NULL;
        return false;
    }
    return coral$red_black_tree$rotate_N(Y, false);
}

static bool coral$red_black_tree$rotate_N_O(void *Y, const bool is_N_left) {
    coral_required(Y);
    bool (*const get_P)(void *, void **) = coral$red_black_tree$node_get_parent;
    void *X;
    coral_required_true(get_P(Y, &X));
    if (!X) {
        coral_error = CORAL_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT;
        return false;
    }
    void *Z;
    coral_required_true(get_P(X, &Z));
    if (!Z) {
        coral_error = CORAL_RED_BLACK_TREE_ERROR_X_HAS_NO_PARENT;
        return false;
    }
    bool (*const rotate_N)(void *) = is_N_left
            ? coral$red_black_tree$rotate_left
            : coral$red_black_tree$rotate_right;
    bool (*const rotate_O)(void *) = is_N_left
            ? coral$red_black_tree$rotate_right
            : coral$red_black_tree$rotate_left;
    if (!rotate_N(Y)) {
        return false;
    }
    if (!rotate_O(Y)) {
        switch (coral_error) {
            /* disambiguate the error to be returned */
            case CORAL_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT: {
                coral_error = CORAL_RED_BLACK_TREE_ERROR_X_HAS_NO_PARENT;
                break;
            }
            case CORAL_RED_BLACK_TREE_ERROR_Y_IS_NOT_RIGHT_CHILD_OF_X: {
                coral_error =
                        CORAL_RED_BLACK_TREE_ERROR_X_IS_NOT_RIGHT_CHILD_OF_Z;
                break;
            }
            case CORAL_RED_BLACK_TREE_ERROR_Y_IS_NOT_LEFT_CHILD_OF_X: {
                coral_error =
                        CORAL_RED_BLACK_TREE_ERROR_X_IS_NOT_LEFT_CHILD_OF_Z;
                break;
            }
            default: {
                /* no op */
            }
        }
        return false;
    }
    return true;
}

bool coral$red_black_tree$rotate_left_right(void *Y) {
    if (!Y) {
        coral_error = CORAL_RED_BLACK_TREE_ERROR_Y_IS_NULL;
        return false;
    }
    return coral$red_black_tree$rotate_N_O(Y, true);
}

bool coral$red_black_tree$rotate_right_left(void *Y) {
    if (!Y) {
        coral_error = CORAL_RED_BLACK_TREE_ERROR_Y_IS_NULL;
        return false;
    }
    return coral$red_black_tree$rotate_N_O(Y, false);
}

bool coral$red_black_tree$alloc(struct coral$red_black_tree **out) {
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    *out = calloc(1, sizeof(**out));
    if (!*out) {
        coral_error = CORAL_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    return true;
}

bool coral$red_black_tree$init(struct coral$red_black_tree *object,
                               int (*compare)(const void *first,
                                              const void *second)) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!compare) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    object->compare = compare;
    return true;
}

bool coral$red_black_tree$invalidate(struct coral$red_black_tree *object,
                                     void (*on_node_destroy)(void *)) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    for (void *node = object->root; node;) {
        void *next;
        coral_required_true(coral$red_black_tree$node_get_left(
                node, &next));
        if (next) {
            coral_required_true(coral$red_black_tree$node_set_left(
                    node, NULL));
            node = next;
            continue;
        }
        coral_required_true(coral$red_black_tree$node_get_right(
                node, &next));
        if (next) {
            coral_required_true(coral$red_black_tree$node_set_right(
                    node, NULL));
            node = next;
            continue;
        }
        coral_required_true(coral$red_black_tree$node_get_parent(
                node, &next));
        if (on_node_destroy) {
            on_node_destroy(node);
        }
        coral_required_true(coral$red_black_tree$node_destroy(node));
        node = next;
    }
    object->root = NULL;
    object->compare = NULL;
    return true;
}

bool coral$red_black_tree$get_first(struct coral$red_black_tree *object,
                                    void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (!object->root) {
        coral_error = CORAL_ERROR_OBJECT_NOT_FOUND;
        return false;
    }
    coral$red_black_tree$get_minimum(object->root, out);
    return true;
}

bool coral$red_black_tree$get_last(struct coral$red_black_tree *object,
                                   void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    if (!object->root) {
        coral_error = CORAL_ERROR_OBJECT_NOT_FOUND;
        return false;
    }
    coral$red_black_tree$get_maximum(object->root, out);
    return true;
}

static void coral$red_black_tree$get_minimum(void *root, void **out) {
    coral_required(root);
    coral_required(out);
    void *left;
    while (true) {
        coral_required_true(coral$red_black_tree$node_get_left(
                root, &left));
        if (!left) {
            *out = root;
            return;
        }
        root = left;
    }
}

static void coral$red_black_tree$get_maximum(void *root, void **out) {
    coral_required(root);
    coral_required(out);
    void *right;
    while (true) {
        coral_required_true(coral$red_black_tree$node_get_right(
                root, &right));
        if (!right) {
            *out = root;
            return;
        }
        root = right;
    }
}

bool coral$red_black_tree$search(struct coral$red_black_tree *object,
                                 const void *root, const void *value,
                                 void **out) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!value || !out) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    *out = NULL;
    for (void *node = (void *) (root ? root : object->root); node;) {
        *out = node;
        const int result = object->compare(value, node);
        if (!result) {
            return true;
        } else if (result < 0) {
            coral_required_true(coral$red_black_tree$node_get_left(
                    node, &node));
        } else {
            coral_required_true(coral$red_black_tree$node_get_right(
                    node, &node));
        }
    }
    coral_error = CORAL_ERROR_OBJECT_NOT_FOUND;
    return false;
}

bool coral$red_black_tree$insert(struct coral$red_black_tree *object,
                                 void *parent, void *child) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!child || (!parent && object->root)) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    /* required that child be RED */
    bool color;
    coral_required_true(coral$red_black_tree$node_set_color(
            child, CORAL_RED_BLACK_TREE_COLOR_RED));
    /* insert */
    if (!parent) {
        parent = object->root;
    }
    if (!parent) {
        object->root = child;
    } else {
        const int result = object->compare(child, parent);
        if (!result) { /* duplicates are not allowed */
            coral_error = CORAL_ERROR_OBJECT_ALREADY_EXISTS;
            return false;
        }
        const bool is_N_left = result < 0;
        bool (*const get_N)(void *, void **) = is_N_left
                ? coral$red_black_tree$node_get_left
                : coral$red_black_tree$node_get_right;
        bool (*const set_N)(void *, void *) = is_N_left
                ? coral$red_black_tree$node_set_left
                : coral$red_black_tree$node_set_right;
        void *node;
        coral_required_true(get_N(parent, &node));
        if (node) { /* replace existing node in parent */
            coral_required_true(coral$red_black_tree$node_get_color(
                    node, &color));
            coral_required_true(coral$red_black_tree$node_set_color(
                    node, CORAL_RED_BLACK_TREE_COLOR_RED));
            coral_required_true(coral$red_black_tree$node_set_color(
                    child, color));
            coral_required_true(coral$red_black_tree$node_set_parent(
                    child, parent));
            coral_required_true(set_N(parent, child));
            parent = child;
            child = node;
        }
        coral_required_true(coral$red_black_tree$node_set_parent(
                child, parent));
        coral_required_true(set_N(parent, child));
    }
    /* repair */
    while (true) {
        coral_required_true(coral$red_black_tree$node_get_color(
                child, &color));
        coral_required_true(CORAL_RED_BLACK_TREE_COLOR_RED == color);
        if (!parent) {
            coral_required_true(coral$red_black_tree$node_set_color(
                    child, CORAL_RED_BLACK_TREE_COLOR_BLACK));
            return true;
        }
        coral_required_true(coral$red_black_tree$node_get_color(
                parent, &color));
        /* case 1: parent is BLACK */
        if (CORAL_RED_BLACK_TREE_COLOR_BLACK == color) {
            return true;
        }
        void *grandparent, *sibling;
        coral_required_true(coral$red_black_tree$node_get_parent(
                parent, &grandparent));
        coral_required_true(coral$red_black_tree$node_get_left(
                grandparent, &sibling));
        if (sibling == parent) {
            coral_required_true(coral$red_black_tree$node_get_right(
                    grandparent, &sibling));
        }
        coral_required_true(coral$red_black_tree$node_get_color(
                sibling, &color));
        /* Legend: C - child            B - BLACK
         *         P - parent           R - RED
         *         G - grandparent
         *         S - sibling
         */
        /* case 2a: parent's sibling is RED */
        if (CORAL_RED_BLACK_TREE_COLOR_RED == color) {
               /*         G(B)                      G(R)
                *         /  \                      /  \
                *      P(R)  S(R)       =>       P(B)  S(B)
                *      /                         /
                *   C(R)                       C(R)
                * --------------------- or ---------------------
                *         G(B)                      G(R)
                *         /  \                      /  \
                *      S(R)  P(R)       =>       S(B)  P(B)
                *              \                         \
                *              C(R)                      C(R)
                */
            coral_required_true(coral$red_black_tree$node_set_color(
                    parent, CORAL_RED_BLACK_TREE_COLOR_BLACK));
            coral_required_true(coral$red_black_tree$node_set_color(
                    sibling, CORAL_RED_BLACK_TREE_COLOR_BLACK));
            coral_required_true(coral$red_black_tree$node_set_color(
                    grandparent, CORAL_RED_BLACK_TREE_COLOR_RED));
            /* Changing grandparent to RED may have introduced two consecutive
             * REDs - grandparent and possibly great-grandparent. We will
             * iterate inorder to correct this violation.
             */
            child = grandparent;
            coral_required_true(coral$red_black_tree$node_get_parent(
                    child, &parent));
            continue;
        }
        /* case 2b: parent's sibling is BLACK */
        const bool is_parent_left = coral$red_black_tree$node_is_on_left(
                grandparent, parent);
        const bool is_child_left = coral$red_black_tree$node_is_on_left(
                parent, child);
        switch (is_parent_left << 1 | is_child_left) {
               /*         G(B)                      P(B)
                *         /  \                      /  \
                *     S(B)    P(R)      =>       G(R)   C(R)
                *                \                /
                *                C(R)           S(B)
                * --------------------- or ---------------------
                *         G(B)                      P(B)
                *         /  \                      /  \
                *     P(R)    S(B)      =>      C(R)    G(R)
                *     /                                    \
                *  C(R)                                    S(B)
                */
            case 0:
            case 3: {
                coral_required_true(coral$red_black_tree$rotate_N(
                        parent, !is_parent_left));
                coral_required_true(coral$red_black_tree$node_set_color(
                        parent, CORAL_RED_BLACK_TREE_COLOR_BLACK));
                coral_required_true(coral$red_black_tree$node_set_color(
                        grandparent, CORAL_RED_BLACK_TREE_COLOR_RED));
                if (object->root == grandparent) {
                    object->root = parent;
                }
                return true;
            }
               /*         G(B)                      C(B)
                *         /  \                      /  \
                *     S(B)    P(R)      =>       G(R)   P(R)
                *             /                   /
                *           C(R)                S(B)
                * --------------------- or ---------------------
                *         G(B)                      C(B)
                *         /  \                      /  \
                *     P(R)    S(B)      =>      P(R)    G(R)
                *        \                                 \
                *        C(R)                              S(B)
                */
            case 1:
            case 2: {
                coral_required_true(coral$red_black_tree$rotate_N_O(
                        child, !is_child_left));
                coral_required_true(coral$red_black_tree$node_set_color(
                        child, CORAL_RED_BLACK_TREE_COLOR_BLACK));
                coral_required_true(coral$red_black_tree$node_set_color(
                        grandparent, CORAL_RED_BLACK_TREE_COLOR_RED));
                if (object->root == grandparent) {
                    object->root = child;
                }
                return true;
            }
        }
    }
}

bool coral$red_black_tree$delete(struct coral$red_black_tree *object,
                                 void *node) {
    if (!object) {
        coral_error = CORAL_ERROR_OBJECT_PTR_IS_NULL;
        return false;
    }
    if (!node) {
        coral_error = CORAL_ERROR_ARGUMENT_PTR_IS_NULL;
        return false;
    }
    /* delete */
    bool color;
    void *left, *right, *parent, *double_black = NULL;
    while (!double_black) {
        coral_required_true(coral$red_black_tree$node_get_color(
                node, &color));
        coral_required_true(coral$red_black_tree$node_get_left(
                node, &left));
        coral_required_true(coral$red_black_tree$node_get_right(
                node, &right));
        coral_required_true(coral$red_black_tree$node_get_parent(
                node, &parent));
        switch ((NULL == left) << 1 | (NULL == right)) {
            case 3: { /* leaf node */
                if (parent) {
                    bool (*const set_N)(void *, void *) =
                    coral$red_black_tree$node_is_on_left(parent, node)
                        ? coral$red_black_tree$node_set_left
                        : coral$red_black_tree$node_set_right;
                    coral_required_true(set_N(parent, NULL));
                }
                coral_required_true(coral$red_black_tree$node_destroy(node));
                if (node == object->root) {
                    object->root = NULL;
                } else if (CORAL_RED_BLACK_TREE_COLOR_BLACK == color) {
                    double_black = node;
                    continue;
                }
                return true;
            }
            case 2:
            case 1: { /* node with single child */
                void *child = left ? left : right;
                /* 'child' to replace 'node' */
                bool color_;
                coral_required_true(coral$red_black_tree$node_get_color(
                        child, &color_));
                if (parent) {
                    bool (*const set_N)(void *, void *) =
                    coral$red_black_tree$node_is_on_left(parent,node)
                        ? coral$red_black_tree$node_set_left
                        : coral$red_black_tree$node_set_right;
                    coral_required_true(set_N(parent, child));
                }
                coral_required_true(coral$red_black_tree$node_set_parent(
                        child, parent));
                coral_required_true(coral$red_black_tree$node_set_color(
                        child, CORAL_RED_BLACK_TREE_COLOR_BLACK));
                if (node == object->root) {
                    object->root = child;
                }
                coral_required_true(coral$red_black_tree$node_destroy(node));
                if (CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                        && CORAL_RED_BLACK_TREE_COLOR_BLACK == color_) {
                    double_black = child;
                    continue;
                }
                return true;
            }
            case 0: { /* node with two children */
                void *next;
                coral_required_true(coral$red_black_tree$get_next(
                        node, &next));
                /* swap 'next' with 'node' */
                bool color_;
                coral_required_true(coral$red_black_tree$node_get_color(
                        next, &color_));
                void *parent_, *left_, *right_;
                coral_required_true(coral$red_black_tree$node_get_parent(
                        next, &parent_));
                coral_required_true(coral$red_black_tree$node_get_left(
                        next, &left_));
                coral_required_true(coral$red_black_tree$node_get_right(
                        next, &right_));

                coral_required_true(coral$red_black_tree$node_set_parent(
                        node, node == parent_ ? next : parent_));
                coral_required_true(coral$red_black_tree$node_set_parent(
                        next, next == parent ? node : parent));
                coral_required_true(coral$red_black_tree$node_set_left(
                        node, node == left_ ? next : left_));
                coral_required_true(coral$red_black_tree$node_set_left(
                        next, next == left ? node : left));
                coral_required_true(coral$red_black_tree$node_set_right(
                        node, node == right_ ? next : right_));
                coral_required_true(coral$red_black_tree$node_set_right(
                        next, next == right ? node : right));
                coral_required_true(coral$red_black_tree$node_set_color(
                        node, color_));
                coral_required_true(coral$red_black_tree$node_set_color(
                        next, color));

                if (parent_ && node != parent_) {
                    bool (*const set_N)(void *, void *) =
                    coral$red_black_tree$node_is_on_left(parent_,next)
                        ? coral$red_black_tree$node_set_left
                        : coral$red_black_tree$node_set_right;
                    coral_required_true(set_N(parent_, node));
                }
                if (parent && next != parent) {
                    bool (*const set_N)(void *, void *) =
                    coral$red_black_tree$node_is_on_left(parent,node)
                        ? coral$red_black_tree$node_set_left
                        : coral$red_black_tree$node_set_right;
                    coral_required_true(set_N(parent, next));
                }
                if (left_ && node != left_) {
                    coral_required_true(coral$red_black_tree$node_set_parent(
                            left_, node));
                }
                if (left && next != left) {
                    coral_required_true(coral$red_black_tree$node_set_parent(
                            left, next));
                }
                if (right_ && node != right_) {
                    coral_required_true(coral$red_black_tree$node_set_parent(
                            right_, node));
                }
                if (right && next != right) {
                    coral_required_true(coral$red_black_tree$node_set_parent(
                            right, next));
                }
                if (node == object->root) {
                    object->root = next;
                }
                continue;
            }
        }
    }
    /* repair */
    /* Legend: C - child            B - BLACK
     *         P - parent           R - RED
     *         N - node             ? - ANY COLOR
     *         S - sibling         (B)- DOUBLE BLACK
     *                             ...- SUBTREE
     */
    while (true) {
        /* case 1: root is double black
         *          N((B))
         *     drop double black
         *           N(B)
         * [done]
         */
        if (double_black == object->root) {
            break;
        }
        struct coral$red_black_tree$node* parent_;
        parent_ = coral$red_black_tree$node_from(parent);
        bool is_left; /* double black on left or right */
        void *sibling;
        if (parent_->left == double_black || !parent_->left) {
            sibling = parent_->right;
            is_left = true;
        } else {
            sibling = parent_->left;
            is_left = false;
        }
        coral_required_true(coral$red_black_tree$node_get_left(
                sibling, &left));
        coral_required_true(coral$red_black_tree$node_get_right(
                sibling, &right));
        /* case 2: if sibling is RED
         *           P(B)              |             P(B)
         *          /    \             |            /    \
         *      N((B))   S(R)          |          S(R)   N((B))
         *        / \     / \          |          / \      / \
         *      ... ... C(B) C(B)      |       C(B) C(B) ... ...
         *              / \  / \       |       / \  / \
         *            ... .... ...     |     ... .... ...
         *                    rotate S through P
         *                  switch P and S colors
         *           S(B)              |            S(B)
         *          /    \             |           /    \
         *        P(R)   C(B)          |         C(B)   P(R)
         *       /   \   /  \          |        /   \   /  \
         *   N((B)) C(B)... ...        |       ... ...C(B)  N((B))
         *   /  \   /  \               |              /  \   /  \
         * ... ......  ...             |            ... ......  ...
         */
        if (coral$red_black_tree$node_get_color(parent, &color)
            && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
            && coral$red_black_tree$node_get_color(sibling, &color)
            && CORAL_RED_BLACK_TREE_COLOR_RED == color
            && coral$red_black_tree$node_get_color(left, &color)
            && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
            && coral$red_black_tree$node_get_color(right, &color)
            && CORAL_RED_BLACK_TREE_COLOR_BLACK == color) {
            coral_required_true(coral$red_black_tree$rotate_N(
                    sibling, is_left));
            coral_required_true(coral$red_black_tree$node_set_color(
                    parent, CORAL_RED_BLACK_TREE_COLOR_RED));
            coral_required_true(coral$red_black_tree$node_set_color(
                    sibling, CORAL_RED_BLACK_TREE_COLOR_BLACK));
            if (parent == object->root) {
                object->root = sibling;
            }
            continue; /* sibling has changed */
        }
        /* case 3: if sibling is BLACK
         *           P(B)              |             P(B)
         *          /    \             |            /    \
         *      N((B))   S(B)          |          S(B)   N((B))
         *        / \     / \          |          / \      / \
         *      ... ... C(B) C(B)      |       C(B) C(B) ... ...
         *              / \  / \       |       / \  / \
         *            ... .... ...     |     ... .... ...
         *                        color S RED
         *                parent is now double black
         *           P((B))            |            P((B))
         *          /    \             |            /    \
         *        N(B)   S(R)          |          S(R)    N(B)
         *        / \     / \          |          / \      / \
         *      ... ... C(B) C(B)      |       C(B) C(B) ... ...
         *              / \  / \       |       / \  / \
         *            ... .... ...     |     ... .... ...
         */
        if (coral$red_black_tree$node_get_color(parent, &color)
            && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
            && coral$red_black_tree$node_get_color(sibling, &color)
            && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
            && coral$red_black_tree$node_get_color(left, &color)
            && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
            && coral$red_black_tree$node_get_color(right, &color)
            && CORAL_RED_BLACK_TREE_COLOR_BLACK == color) {
            coral_required_true(coral$red_black_tree$node_set_color(
                    sibling, CORAL_RED_BLACK_TREE_COLOR_RED));
            double_black = parent;
            coral_required_true(coral$red_black_tree$node_get_parent(
                    parent, &parent));
            continue; /* go to case 1 as parent is now double black */
        }
        /* case 4: If parent is RED sibling and children are BLACK
         *           P(R)              |             P(R)
         *          /    \             |            /    \
         *      N((B))   S(B)          |          S(B)   N((B))
         *        / \     / \          |          / \      / \
         *      ... ... C(B) C(B)      |       C(B) C(B) ... ...
         *              / \  / \       |       / \  / \
         *            ... .... ...     |     ... .... ...
         *                   color P BLACK and S RED
         *           P(B)              |             P(B)
         *          /    \             |            /    \
         *        N(B)   S(R)          |          S(R)    N(B)
         *        / \     / \          |          / \      / \
         *      ... ... C(B) C(B)      |       C(B) C(B) ... ...
         *              / \  / \       |       / \  / \
         *            ... .... ...     |     ... .... ...
         *  [done]
         */
        if (coral$red_black_tree$node_get_color(parent, &color)
            && CORAL_RED_BLACK_TREE_COLOR_RED == color
            && coral$red_black_tree$node_get_color(sibling, &color)
            && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
            && coral$red_black_tree$node_get_color(left, &color)
            && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
            && coral$red_black_tree$node_get_color(right, &color)
            && CORAL_RED_BLACK_TREE_COLOR_BLACK == color) {
            coral_required_true(coral$red_black_tree$node_set_color(
                    parent, CORAL_RED_BLACK_TREE_COLOR_BLACK));
            coral_required_true(coral$red_black_tree$node_set_color(
                    sibling, CORAL_RED_BLACK_TREE_COLOR_RED));
            break;
        }
        /* closest C from N */
        void *closest = is_left ? left : right;
        /* farthest C from N */
        void *farthest = is_left ? right : left;
        /* case 5: Sibling is BLACK and closest child is RED while farthest
         * child is BLACK.
         *           P(B)              |             P(B)
         *          /    \             |            /    \
         *      N((B))   S(B)          |          S(B)   N((B))
         *        / \     / \          |          / \      / \
         *      ... ... C(R) C(B)      |       C(B) C(R) ... ...
         *              / \  / \       |       / \  / \
         *            ... .... ...     |     ... .... ...
         *                   rotate C(R) through S
         *              change S to RED and C(R) to BLACK
         *           P(B)              |             P(B)
         *          /    \             |            /    \
         *      N((B))   C(B)          |          C(B)   N((B))
         *        / \     / \          |          / \      / \
         *      ... ... ...  S(R)      |       S(R) ...  ... ...
         *                   / \       |       / \
         *                 ... C(B)    |    C(B) ...
         *                     / \     |    / \
         *                   ... ...   |  ... ...
         */
        if (coral$red_black_tree$node_get_color(parent, &color)
            && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
            && coral$red_black_tree$node_get_color(sibling, &color)
            && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
            && coral$red_black_tree$node_get_color(closest, &color)
            && CORAL_RED_BLACK_TREE_COLOR_RED == color
            && coral$red_black_tree$node_get_color(farthest, &color)
            && CORAL_RED_BLACK_TREE_COLOR_BLACK == color) {
            coral_required_true(coral$red_black_tree$rotate_N(
                    closest, !is_left));
            coral_required_true(coral$red_black_tree$node_set_color(
                    sibling, CORAL_RED_BLACK_TREE_COLOR_RED));
            coral_required_true(coral$red_black_tree$node_set_color(
                    closest, CORAL_RED_BLACK_TREE_COLOR_BLACK));
            continue; /* sibling has changed */
        }
        /* case 6: Sibling is BLACK and farthest child is RED.
         *           P(?)              |             P(?)
         *          /    \             |            /    \
         *      N((B))   S(B)          |          S(B)   N((B))
         *        / \     / \          |          / \     / \
         *      ... ... ...  C(R)      |       C(R) ... ... ...
         *                  / \        |       / \
         *                ... ...      |     ... ...
         *                     rotate S through P
         *                      set S to P color
         *                    set P and C to BLACK
         *           S(?)              |             S(?)
         *          /    \             |            /    \
         *        P(B)   C(B)          |          P(B)   C(B)
         *        / \     / \          |          / \     / \
         *     N(B) ... ... ...        |        ... ... ... N(B)
         *     / \                     |                     / \
         *   ... ...                   |                   ... ...
         *  [done]
         */
        if (coral$red_black_tree$node_get_color(sibling, &color)
            && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
            && coral$red_black_tree$node_get_color(farthest, &color)
            && CORAL_RED_BLACK_TREE_COLOR_RED == color) {
            coral_required_true(coral$red_black_tree$rotate_N(
                    sibling, is_left));
            coral_required_true(coral$red_black_tree$node_get_color(
                    parent, &color));
            coral_required_true(coral$red_black_tree$node_set_color(
                    sibling, color));
            coral_required_true(coral$red_black_tree$node_set_color(
                    parent, CORAL_RED_BLACK_TREE_COLOR_BLACK));
            coral_required_true(coral$red_black_tree$node_set_color(
                    farthest, CORAL_RED_BLACK_TREE_COLOR_BLACK));
            if (parent == object->root) {
                object->root = sibling;
            }
            break;
        }
    }
    return true;
}
