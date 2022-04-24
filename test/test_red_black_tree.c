#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <coral.h>

#include "private/red_black_tree.h"
#include "test/cmocka.h"

static void check_node_alloc_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$node_alloc(0, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_node_alloc_error_on_memory_allocation_failed(void **s) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$node_alloc(SIZE_MAX, (void **) 1));
    assert_int_equal(CORAL_ERROR_MEMORY_ALLOCATION_FAILED, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_node_alloc(void **state) {
    struct object {
        size_t count;
    } *object;
    assert_true(coral$red_black_tree$node_alloc(
            sizeof(*object), (void **) &object));
    assert_true(coral$red_black_tree$node_destroy(object));
}

static void check_node_destroy_no_error_on_null_object_ptr(void **state) {
    assert_true(coral$red_black_tree$node_destroy(NULL));
}

static void check_node_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$node_init(NULL));
    coral_error = CORAL_ERROR_NONE;
}

static void check_node_init(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *node;
    assert_true(coral$red_black_tree$node_alloc(0, &node));
    struct coral$red_black_tree$node *node_;
    node_ = coral$red_black_tree$node_from(node);
    node_->right = (void *) ~0;
    node_->left = (void *) ~0;
    node_->parent = (void *) ~0;
    assert_true(coral$red_black_tree$node_init(node));
    assert_null(node_->parent);
    assert_null(node_->left);
    assert_null(node_->right);
    assert_true(coral$red_black_tree$node_destroy(node));
    coral_error = CORAL_ERROR_NONE;
}

static void check_node_of_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$node_of(0, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_node_of_error_on_memory_allocation_failed(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$node_of(SIZE_MAX, (void *) 1));
    assert_int_equal(CORAL_ERROR_MEMORY_ALLOCATION_FAILED, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_node_of(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *node;
    assert_true(coral$red_black_tree$node_of(0, &node));
    struct coral$red_black_tree$node *node_;
    node_ = coral$red_black_tree$node_from(node);
    assert_true(coral$red_black_tree$node_init(node));
    assert_null(node_->parent);
    assert_null(node_->left);
    assert_null(node_->right);
    assert_true(coral$red_black_tree$node_destroy(node));
    coral_error = CORAL_ERROR_NONE;
}

static void check_node_set_parent_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$node_set_parent(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_node_set_parent(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *node;
    assert_true(coral$red_black_tree$node_of(0, &node));
    struct coral$red_black_tree$node *node_;
    node_ = coral$red_black_tree$node_from(node);
    node_->parent = (void *) CORAL_RED_BLACK_TREE_COLOR_RED;
    assert_true(coral$red_black_tree$node_set_parent(node, &coral_error));
    assert_ptr_equal((void *) ((size_t) node_->parent & ~1), &coral_error);
    assert_int_equal(((size_t) node_->parent & 1),
                     CORAL_RED_BLACK_TREE_COLOR_RED);
    node_->parent = (void *) CORAL_RED_BLACK_TREE_COLOR_BLACK;
    assert_true(coral$red_black_tree$node_set_parent(node, &coral_error));
    assert_ptr_equal((void *) ((size_t) node_->parent & ~1), &coral_error);
    assert_int_equal(((size_t) node_->parent & 1),
                     CORAL_RED_BLACK_TREE_COLOR_BLACK);
    assert_true(coral$red_black_tree$node_destroy(node));
    coral_error = CORAL_ERROR_NONE;
}

static void check_node_get_parent_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$node_get_parent(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_node_get_parent_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$node_get_parent((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_node_get_parent(void **state) {
    void *node;
    assert_true(coral$red_black_tree$node_of(0, &node));
    void *parent;
    assert_true(coral$red_black_tree$node_get_parent(node, &parent));
    assert_null(parent);
    assert_true(coral$red_black_tree$node_set_parent(node, &coral_error));
    assert_true(coral$red_black_tree$node_get_parent(node, &parent));
    assert_int_equal(&coral_error, parent);
    assert_true(coral$red_black_tree$node_destroy(node));
}

static void check_node_get_color_black_on_null_object_ptr(void **state) {
    bool color = CORAL_RED_BLACK_TREE_COLOR_RED;
    assert_true(coral$red_black_tree$node_get_color(
            NULL, &color));
    assert_int_equal(CORAL_RED_BLACK_TREE_COLOR_BLACK, color);
}

static void check_node_get_color_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$node_get_color((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_node_get_color(void **state) {
    void *node;
    assert_true(coral$red_black_tree$node_of(0, &node));
    bool color;
    assert_true(coral$red_black_tree$node_get_color(node, &color));
    assert_int_equal(CORAL_RED_BLACK_TREE_COLOR_RED, color);
    struct coral$red_black_tree$node *node_;
    node_ = coral$red_black_tree$node_from(node);
    node_->parent = (void *) CORAL_RED_BLACK_TREE_COLOR_BLACK;
    assert_true(coral$red_black_tree$node_get_color(node, &color));
    assert_int_equal(CORAL_RED_BLACK_TREE_COLOR_BLACK, color);
    node_->parent = (void *) CORAL_RED_BLACK_TREE_COLOR_RED;
    assert_true(coral$red_black_tree$node_get_color(node, &color));
    assert_int_equal(CORAL_RED_BLACK_TREE_COLOR_RED, color);
    assert_true(coral$red_black_tree$node_destroy(node));
}

static void check_node_set_color_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$node_set_color(
            NULL, CORAL_RED_BLACK_TREE_COLOR_BLACK));
    coral_error = CORAL_ERROR_NONE;
}

static void check_node_set_color(void **state) {
    void *node;
    assert_true(coral$red_black_tree$node_of(0, &node));
    bool color;
    assert_true(coral$red_black_tree$node_get_color(node, &color));
    assert_int_equal(CORAL_RED_BLACK_TREE_COLOR_RED, color);
    assert_true(coral$red_black_tree$node_set_color(
            node, CORAL_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(coral$red_black_tree$node_get_color(node, &color));
    assert_int_equal(CORAL_RED_BLACK_TREE_COLOR_BLACK, color);
    assert_true(coral$red_black_tree$node_set_color(
            node, CORAL_RED_BLACK_TREE_COLOR_RED));
    assert_true(coral$red_black_tree$node_get_color(node, &color));
    assert_int_equal(CORAL_RED_BLACK_TREE_COLOR_RED, color);
    assert_true(coral$red_black_tree$node_destroy(node));
}

static void check_node_get_left_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$node_get_left(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_END_OF_SEQUENCE, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_node_get_left_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$node_get_left((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_node_get_left(void **state) {
    void *node;
    assert_true(coral$red_black_tree$node_of(0, &node));
    void *left = &check_node_get_left;
    assert_true(coral$red_black_tree$node_get_left(node, &left));
    assert_null(left);
    struct coral$red_black_tree$node *node_;
    node_ = coral$red_black_tree$node_from(node);
    node_->left = &coral_error;
    assert_true(coral$red_black_tree$node_get_left(node, &left));
    assert_ptr_equal(&coral_error, left);
    assert_true(coral$red_black_tree$node_destroy(node));
}

static void check_node_set_left_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$node_set_left(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_node_set_left(void **state) {
    void *node;
    assert_true(coral$red_black_tree$node_of(0, &node));
    void *left;
    assert_true(coral$red_black_tree$node_get_left(node, &left));
    assert_null(left);
    assert_true(coral$red_black_tree$node_set_left(node, &coral_error));
    assert_true(coral$red_black_tree$node_get_left(node, &left));
    assert_ptr_equal(&coral_error, left);
    assert_true(coral$red_black_tree$node_destroy(node));
}

static void check_node_get_right_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$node_get_right(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_END_OF_SEQUENCE, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_node_get_right_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$node_get_right((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_node_get_right(void **state) {
    void *node;
    assert_true(coral$red_black_tree$node_of(0, &node));
    void *right = &check_node_get_right;
    assert_true(coral$red_black_tree$node_get_right(node, &right));
    assert_null(right);
    struct coral$red_black_tree$node *node_;
    node_ = coral$red_black_tree$node_from(node);
    node_->right = &coral_error;
    assert_true(coral$red_black_tree$node_get_right(node, &right));
    assert_ptr_equal(&coral_error, right);
    assert_true(coral$red_black_tree$node_destroy(node));
}

static void check_node_set_right_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$node_set_right(NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_node_set_right(void **state) {
    void *node;
    assert_true(coral$red_black_tree$node_of(0, &node));
    void *right;
    assert_true(coral$red_black_tree$node_get_right(node, &right));
    assert_null(right);
    assert_true(coral$red_black_tree$node_set_right(node, &coral_error));
    assert_true(coral$red_black_tree$node_get_right(node, &right));
    assert_ptr_equal(&coral_error, right);
    assert_true(coral$red_black_tree$node_destroy(node));
}

static void check_rotate_left_error_on_Y_is_null(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$rotate_left(NULL));
    assert_int_equal(CORAL_RED_BLACK_TREE_ERROR_Y_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_left_error_on_Y_has_no_parent(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *Y;
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_false(coral$red_black_tree$rotate_left(Y));
    assert_int_equal(CORAL_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT, coral_error);
    assert_true(coral$red_black_tree$node_destroy(Y));
    coral_error = CORAL_ERROR_NONE;
}

static void
check_rotate_left_error_on_Y_is_not_right_child_of_X(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *X, *Y;
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_set_left(X, Y)
                && coral$red_black_tree$node_set_parent(Y, X));
    assert_false(coral$red_black_tree$rotate_left(Y));
    assert_int_equal(CORAL_RED_BLACK_TREE_ERROR_Y_IS_NOT_RIGHT_CHILD_OF_X,
                     coral_error);
    assert_true(coral$red_black_tree$node_destroy(Y));
    assert_true(coral$red_black_tree$node_destroy(X));
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_left_a_b_c_pL_X_Y(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *a, *b, *c, *p, *X, *Y;
    assert_true(coral$red_black_tree$node_of(0, &a));
    assert_true(coral$red_black_tree$node_of(0, &b));
    assert_true(coral$red_black_tree$node_of(0, &c));
    assert_true(coral$red_black_tree$node_of(0, &p));
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_set_left(p, X)
                && coral$red_black_tree$node_set_parent(X, p));
    assert_true(coral$red_black_tree$node_set_left(X, a)
                && coral$red_black_tree$node_set_parent(a, X));
    assert_true(coral$red_black_tree$node_set_right(X, Y)
                && coral$red_black_tree$node_set_parent(Y, X));
    assert_true(coral$red_black_tree$node_set_left(Y, b)
                && coral$red_black_tree$node_set_parent(b, Y));
    assert_true(coral$red_black_tree$node_set_right(Y, c)
                && coral$red_black_tree$node_set_parent(c, Y));
    assert_true(coral$red_black_tree$rotate_left(Y));
    void *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(coral$red_black_tree$node_get_left(p, &Y_)
                && Y == Y_
                && coral$red_black_tree$node_get_parent(Y, &p_)
                && p == p_);
    assert_true(coral$red_black_tree$node_get_left(Y, &X_)
                && X == X_
                && coral$red_black_tree$node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_get_left(X, &a_)
                && a == a_
                && coral$red_black_tree$node_get_parent(a, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_get_right(X, &b_)
                && b == b_
                && coral$red_black_tree$node_get_parent(b, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_get_right(Y, &c_)
                && c == c_
                && coral$red_black_tree$node_get_parent(c, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_destroy(Y)
                && coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(p)
                && coral$red_black_tree$node_destroy(c)
                && coral$red_black_tree$node_destroy(b)
                && coral$red_black_tree$node_destroy(a));
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_left_a_b_c_pR_X_Y(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *a, *b, *c, *p, *X, *Y;
    assert_true(coral$red_black_tree$node_of(0, &a));
    assert_true(coral$red_black_tree$node_of(0, &b));
    assert_true(coral$red_black_tree$node_of(0, &c));
    assert_true(coral$red_black_tree$node_of(0, &p));
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_set_right(p, X)
                && coral$red_black_tree$node_set_parent(X, p));
    assert_true(coral$red_black_tree$node_set_left(X, a)
                && coral$red_black_tree$node_set_parent(a, X));
    assert_true(coral$red_black_tree$node_set_right(X, Y)
                && coral$red_black_tree$node_set_parent(Y, X));
    assert_true(coral$red_black_tree$node_set_left(Y, b)
                && coral$red_black_tree$node_set_parent(b, Y));
    assert_true(coral$red_black_tree$node_set_right(Y, c)
                && coral$red_black_tree$node_set_parent(c, Y));
    assert_true(coral$red_black_tree$rotate_left(Y));
    void *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(coral$red_black_tree$node_get_right(p, &Y_)
                && Y == Y_
                && coral$red_black_tree$node_get_parent(Y, &p_)
                && p == p_);
    assert_true(coral$red_black_tree$node_get_left(Y, &X_)
                && X == X_
                && coral$red_black_tree$node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_get_left(X, &a_)
                && a == a_
                && coral$red_black_tree$node_get_parent(a, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_get_right(X, &b_)
                && b == b_
                && coral$red_black_tree$node_get_parent(b, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_get_right(Y, &c_)
                && c == c_
                && coral$red_black_tree$node_get_parent(c, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_destroy(Y)
                && coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(p)
                && coral$red_black_tree$node_destroy(c)
                && coral$red_black_tree$node_destroy(b)
                && coral$red_black_tree$node_destroy(a));
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_left_a_b_c_X_Y(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *a, *b, *c, *X, *Y;
    assert_true(coral$red_black_tree$node_of(0, &a));
    assert_true(coral$red_black_tree$node_of(0, &b));
    assert_true(coral$red_black_tree$node_of(0, &c));
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_set_left(X, a)
                && coral$red_black_tree$node_set_parent(a, X));
    assert_true(coral$red_black_tree$node_set_right(X, Y)
                && coral$red_black_tree$node_set_parent(Y, X));
    assert_true(coral$red_black_tree$node_set_left(Y, b)
                && coral$red_black_tree$node_set_parent(b, Y));
    assert_true(coral$red_black_tree$node_set_right(Y, c)
                && coral$red_black_tree$node_set_parent(c, Y));
    assert_true(coral$red_black_tree$rotate_left(Y));
    void *a_, *b_, *c_, *X_, *Y_, *root_;
    assert_true(coral$red_black_tree$node_get_parent(Y, &root_)
                && NULL == root_);
    assert_true(coral$red_black_tree$node_get_left(Y, &X_)
                && X == X_
                && coral$red_black_tree$node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_get_left(X, &a_)
                && a == a_
                && coral$red_black_tree$node_get_parent(a, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_get_right(X, &b_)
                && b == b_
                && coral$red_black_tree$node_get_parent(b, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_get_right(Y, &c_)
                && c == c_
                && coral$red_black_tree$node_get_parent(c, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_destroy(Y)
                && coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(c)
                && coral$red_black_tree$node_destroy(b)
                && coral$red_black_tree$node_destroy(a));
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_left_a_c_pL_X_Y(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *a, *c, *p, *X, *Y;
    assert_true(coral$red_black_tree$node_of(0, &a));
    assert_true(coral$red_black_tree$node_of(0, &c));
    assert_true(coral$red_black_tree$node_of(0, &p));
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_set_left(p, X)
                && coral$red_black_tree$node_set_parent(X, p));
    assert_true(coral$red_black_tree$node_set_left(X, a)
                && coral$red_black_tree$node_set_parent(a, X));
    assert_true(coral$red_black_tree$node_set_right(X, Y)
                && coral$red_black_tree$node_set_parent(Y, X));
    assert_true(coral$red_black_tree$node_set_right(Y, c)
                && coral$red_black_tree$node_set_parent(c, Y));
    assert_true(coral$red_black_tree$rotate_left(Y));
    void *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(coral$red_black_tree$node_get_left(p, &Y_)
                && Y == Y_
                && coral$red_black_tree$node_get_parent(Y, &p_)
                && p == p_);
    assert_true(coral$red_black_tree$node_get_left(Y, &X_)
                && X == X_
                && coral$red_black_tree$node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_get_left(X, &a_)
                && a == a_
                && coral$red_black_tree$node_get_parent(a, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_get_right(X, &b_)
                && NULL == b_);
    assert_true(coral$red_black_tree$node_get_right(Y, &c_)
                && c == c_
                && coral$red_black_tree$node_get_parent(c, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_destroy(Y)
                && coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(p)
                && coral$red_black_tree$node_destroy(c)
                && coral$red_black_tree$node_destroy(a));
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_left_a_c_pR_X_Y(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *a, *c, *p, *X, *Y;
    assert_true(coral$red_black_tree$node_of(0, &a));
    assert_true(coral$red_black_tree$node_of(0, &c));
    assert_true(coral$red_black_tree$node_of(0, &p));
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_set_right(p, X)
                && coral$red_black_tree$node_set_parent(X, p));
    assert_true(coral$red_black_tree$node_set_left(X, a)
                && coral$red_black_tree$node_set_parent(a, X));
    assert_true(coral$red_black_tree$node_set_right(X, Y)
                && coral$red_black_tree$node_set_parent(Y, X));
    assert_true(coral$red_black_tree$node_set_right(Y, c)
                && coral$red_black_tree$node_set_parent(c, Y));
    assert_true(coral$red_black_tree$rotate_left(Y));
    void *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(coral$red_black_tree$node_get_right(p, &Y_)
                && Y == Y_
                && coral$red_black_tree$node_get_parent(Y, &p_)
                && p == p_);
    assert_true(coral$red_black_tree$node_get_left(Y, &X_)
                && X == X_
                && coral$red_black_tree$node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_get_left(X, &a_)
                && a == a_
                && coral$red_black_tree$node_get_parent(a, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_get_right(X, &b_)
                && NULL == b_);
    assert_true(coral$red_black_tree$node_get_right(Y, &c_)
                && c == c_
                && coral$red_black_tree$node_get_parent(c, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_destroy(Y)
                && coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(p)
                && coral$red_black_tree$node_destroy(c)
                && coral$red_black_tree$node_destroy(a));
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_left_a_c_X_Y(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *a, *c, *X, *Y;
    assert_true(coral$red_black_tree$node_of(0, &a));
    assert_true(coral$red_black_tree$node_of(0, &c));
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_set_left(X, a)
                && coral$red_black_tree$node_set_parent(a, X));
    assert_true(coral$red_black_tree$node_set_right(X, Y)
                && coral$red_black_tree$node_set_parent(Y, X));
    assert_true(coral$red_black_tree$node_set_right(Y, c)
                && coral$red_black_tree$node_set_parent(c, Y));
    assert_true(coral$red_black_tree$rotate_left(Y));
    void *a_, *b_, *c_, *X_, *Y_, *root_;
    assert_true(coral$red_black_tree$node_get_parent(Y, &root_)
                && NULL == root_);
    assert_true(coral$red_black_tree$node_get_left(Y, &X_)
                && X == X_
                && coral$red_black_tree$node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_get_left(X, &a_)
                && a == a_
                && coral$red_black_tree$node_get_parent(a, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_get_right(X, &b_)
                && NULL == b_);
    assert_true(coral$red_black_tree$node_get_right(Y, &c_)
                && c == c_
                && coral$red_black_tree$node_get_parent(c, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_destroy(Y)
                && coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(c)
                && coral$red_black_tree$node_destroy(a));
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_right_error_on_Y_is_null(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$rotate_right(NULL));
    assert_int_equal(CORAL_RED_BLACK_TREE_ERROR_Y_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_right_error_on_Y_has_no_parent(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *Y;
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_false(coral$red_black_tree$rotate_right(Y));
    assert_int_equal(CORAL_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT, coral_error);
    assert_true(coral$red_black_tree$node_destroy(Y));
    coral_error = CORAL_ERROR_NONE;
}

static void
check_rotate_right_error_on_Y_is_not_left_child_of_X(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *X, *Y;
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_set_right(X, Y)
                && coral$red_black_tree$node_set_parent(Y, X));
    assert_false(coral$red_black_tree$rotate_right(Y));
    assert_int_equal(CORAL_RED_BLACK_TREE_ERROR_Y_IS_NOT_LEFT_CHILD_OF_X,
                     coral_error);
    assert_true(coral$red_black_tree$node_destroy(Y));
    assert_true(coral$red_black_tree$node_destroy(X));
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_right_a_b_c_pL_X_Y(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *a, *b, *c, *p, *X, *Y;
    assert_true(coral$red_black_tree$node_of(0, &a));
    assert_true(coral$red_black_tree$node_of(0, &b));
    assert_true(coral$red_black_tree$node_of(0, &c));
    assert_true(coral$red_black_tree$node_of(0, &p));
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_set_left(p, X)
                && coral$red_black_tree$node_set_parent(X, p));
    assert_true(coral$red_black_tree$node_set_right(X, c)
                && coral$red_black_tree$node_set_parent(c, X));
    assert_true(coral$red_black_tree$node_set_left(X, Y)
                && coral$red_black_tree$node_set_parent(Y, X));
    assert_true(coral$red_black_tree$node_set_right(Y, b)
                && coral$red_black_tree$node_set_parent(b, Y));
    assert_true(coral$red_black_tree$node_set_left(Y, a)
                && coral$red_black_tree$node_set_parent(a, Y));
    assert_true(coral$red_black_tree$rotate_right(Y));
    void *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(coral$red_black_tree$node_get_left(p, &Y_)
                && Y == Y_
                && coral$red_black_tree$node_get_parent(Y, &p_)
                && p == p_);
    assert_true(coral$red_black_tree$node_get_right(Y, &X_)
                && X == X_
                && coral$red_black_tree$node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_get_left(Y, &a_)
                && a == a_
                && coral$red_black_tree$node_get_parent(a, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_get_left(X, &b_)
                && b == b_
                && coral$red_black_tree$node_get_parent(b, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_get_right(X, &c_)
                && c == c_
                && coral$red_black_tree$node_get_parent(c, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_destroy(Y)
                && coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(p)
                && coral$red_black_tree$node_destroy(c)
                && coral$red_black_tree$node_destroy(b)
                && coral$red_black_tree$node_destroy(a));
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_right_a_b_c_pR_X_Y(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *a, *b, *c, *p, *X, *Y;
    assert_true(coral$red_black_tree$node_of(0, &a));
    assert_true(coral$red_black_tree$node_of(0, &b));
    assert_true(coral$red_black_tree$node_of(0, &c));
    assert_true(coral$red_black_tree$node_of(0, &p));
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_set_right(p, X)
                && coral$red_black_tree$node_set_parent(X, p));
    assert_true(coral$red_black_tree$node_set_right(X, c)
                && coral$red_black_tree$node_set_parent(c, X));
    assert_true(coral$red_black_tree$node_set_left(X, Y)
                && coral$red_black_tree$node_set_parent(Y, X));
    assert_true(coral$red_black_tree$node_set_right(Y, b)
                && coral$red_black_tree$node_set_parent(b, Y));
    assert_true(coral$red_black_tree$node_set_left(Y, a)
                && coral$red_black_tree$node_set_parent(a, Y));
    assert_true(coral$red_black_tree$rotate_right(Y));
    void *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(coral$red_black_tree$node_get_right(p, &Y_)
                && Y == Y_
                && coral$red_black_tree$node_get_parent(Y, &p_)
                && p == p_);
    assert_true(coral$red_black_tree$node_get_right(Y, &X_)
                && X == X_
                && coral$red_black_tree$node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_get_left(Y, &a_)
                && a == a_
                && coral$red_black_tree$node_get_parent(a, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_get_left(X, &b_)
                && b == b_
                && coral$red_black_tree$node_get_parent(b, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_get_right(X, &c_)
                && c == c_
                && coral$red_black_tree$node_get_parent(c, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_destroy(Y)
                && coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(p)
                && coral$red_black_tree$node_destroy(c)
                && coral$red_black_tree$node_destroy(b)
                && coral$red_black_tree$node_destroy(a));
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_right_a_b_c_X_Y(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *a, *b, *c, *X, *Y;
    assert_true(coral$red_black_tree$node_of(0, &a));
    assert_true(coral$red_black_tree$node_of(0, &b));
    assert_true(coral$red_black_tree$node_of(0, &c));
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_set_right(X, c)
                && coral$red_black_tree$node_set_parent(c, X));
    assert_true(coral$red_black_tree$node_set_left(X, Y)
                && coral$red_black_tree$node_set_parent(Y, X));
    assert_true(coral$red_black_tree$node_set_right(Y, b)
                && coral$red_black_tree$node_set_parent(b, Y));
    assert_true(coral$red_black_tree$node_set_left(Y, a)
                && coral$red_black_tree$node_set_parent(a, Y));
    assert_true(coral$red_black_tree$rotate_right(Y));
    void *a_, *b_, *c_, *X_, *Y_, *root_;
    assert_true(coral$red_black_tree$node_get_parent(Y, &root_)
                && NULL == root_);
    assert_true(coral$red_black_tree$node_get_right(Y, &X_)
                && X == X_
                && coral$red_black_tree$node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_get_left(Y, &a_)
                && a == a_
                && coral$red_black_tree$node_get_parent(a, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_get_left(X, &b_)
                && b == b_
                && coral$red_black_tree$node_get_parent(b, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_get_right(X, &c_)
                && c == c_
                && coral$red_black_tree$node_get_parent(c, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_destroy(Y)
                && coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(c)
                && coral$red_black_tree$node_destroy(b)
                && coral$red_black_tree$node_destroy(a));
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_right_a_c_pL_X_Y(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *a, *c, *p, *X, *Y;
    assert_true(coral$red_black_tree$node_of(0, &a));
    assert_true(coral$red_black_tree$node_of(0, &c));
    assert_true(coral$red_black_tree$node_of(0, &p));
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_set_left(p, X)
                && coral$red_black_tree$node_set_parent(X, p));
    assert_true(coral$red_black_tree$node_set_right(X, c)
                && coral$red_black_tree$node_set_parent(c, X));
    assert_true(coral$red_black_tree$node_set_left(X, Y)
                && coral$red_black_tree$node_set_parent(Y, X));
    assert_true(coral$red_black_tree$node_set_left(Y, a)
                && coral$red_black_tree$node_set_parent(a, Y));
    assert_true(coral$red_black_tree$rotate_right(Y));
    void *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(coral$red_black_tree$node_get_left(p, &Y_)
                && Y == Y_
                && coral$red_black_tree$node_get_parent(Y, &p_)
                && p == p_);
    assert_true(coral$red_black_tree$node_get_right(Y, &X_)
                && X == X_
                && coral$red_black_tree$node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_get_left(Y, &a_)
                && a == a_
                && coral$red_black_tree$node_get_parent(a, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_get_left(X, &b_)
                && NULL == b_);
    assert_true(coral$red_black_tree$node_get_right(X, &c_)
                && c == c_
                && coral$red_black_tree$node_get_parent(c, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_destroy(Y)
                && coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(p)
                && coral$red_black_tree$node_destroy(c)
                && coral$red_black_tree$node_destroy(a));
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_right_a_c_pR_X_Y(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *a, *c, *p, *X, *Y;
    assert_true(coral$red_black_tree$node_of(0, &a));
    assert_true(coral$red_black_tree$node_of(0, &c));
    assert_true(coral$red_black_tree$node_of(0, &p));
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_set_right(p, X)
                && coral$red_black_tree$node_set_parent(X, p));
    assert_true(coral$red_black_tree$node_set_right(X, c)
                && coral$red_black_tree$node_set_parent(c, X));
    assert_true(coral$red_black_tree$node_set_left(X, Y)
                && coral$red_black_tree$node_set_parent(Y, X));
    assert_true(coral$red_black_tree$node_set_left(Y, a)
                && coral$red_black_tree$node_set_parent(a, Y));
    assert_true(coral$red_black_tree$rotate_right(Y));
    void *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(coral$red_black_tree$node_get_right(p, &Y_)
                && Y == Y_
                && coral$red_black_tree$node_get_parent(Y, &p_)
                && p == p_);
    assert_true(coral$red_black_tree$node_get_right(Y, &X_)
                && X == X_
                && coral$red_black_tree$node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_get_left(Y, &a_)
                && a == a_
                && coral$red_black_tree$node_get_parent(a, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_get_left(X, &b_)
                && NULL == b_);
    assert_true(coral$red_black_tree$node_get_right(X, &c_)
                && c == c_
                && coral$red_black_tree$node_get_parent(c, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_destroy(Y)
                && coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(p)
                && coral$red_black_tree$node_destroy(c)
                && coral$red_black_tree$node_destroy(a));
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_right_a_c_X_Y(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *a, *c, *X, *Y;
    assert_true(coral$red_black_tree$node_of(0, &a));;
    assert_true(coral$red_black_tree$node_of(0, &c));
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_set_right(X, c)
                && coral$red_black_tree$node_set_parent(c, X));
    assert_true(coral$red_black_tree$node_set_left(X, Y)
                && coral$red_black_tree$node_set_parent(Y, X));
    assert_true(coral$red_black_tree$node_set_left(Y, a)
                && coral$red_black_tree$node_set_parent(a, Y));
    assert_true(coral$red_black_tree$rotate_right(Y));
    void *a_, *b_, *c_, *X_, *Y_, *root_;
    assert_true(coral$red_black_tree$node_get_parent(Y, &root_)
                && NULL == root_);
    assert_true(coral$red_black_tree$node_get_right(Y, &X_)
                && X == X_
                && coral$red_black_tree$node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_get_left(Y, &a_)
                && a == a_
                && coral$red_black_tree$node_get_parent(a, &Y_)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_get_left(X, &b_)
                && NULL == b_);
    assert_true(coral$red_black_tree$node_get_right(X, &c_)
                && c == c_
                && coral$red_black_tree$node_get_parent(c, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_destroy(Y)
                && coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(c)
                && coral$red_black_tree$node_destroy(a));
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_left_right_error_on_Y_is_null(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$rotate_left_right(NULL));
    assert_int_equal(CORAL_RED_BLACK_TREE_ERROR_Y_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_left_right_error_on_Y_has_no_parent(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *Y;
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_false(coral$red_black_tree$rotate_left_right(Y));
    assert_int_equal(CORAL_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT, coral_error);
    assert_true(coral$red_black_tree$node_destroy(Y));
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_left_right_error_on_X_has_no_parent(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *X, *Y;
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_set_parent(Y, X)
                && coral$red_black_tree$node_set_right(X, Y));
    assert_false(coral$red_black_tree$rotate_left_right(Y));
    assert_int_equal(CORAL_RED_BLACK_TREE_ERROR_X_HAS_NO_PARENT, coral_error);
    assert_true(coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(Y));
    coral_error = CORAL_ERROR_NONE;
}

static void
check_rotate_left_right_error_on_Y_is_not_right_child_of_X(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *X, *Y, *Z;
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_of(0, &Z));
    assert_true(coral$red_black_tree$node_set_parent(Y, X)
                && coral$red_black_tree$node_set_left(X, Y));
    assert_true(coral$red_black_tree$node_set_parent(X, Z)
                && coral$red_black_tree$node_set_left(Z, X));
    assert_false(coral$red_black_tree$rotate_left_right(Y));
    assert_int_equal(CORAL_RED_BLACK_TREE_ERROR_Y_IS_NOT_RIGHT_CHILD_OF_X,
                     coral_error);
    assert_true(coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(Y)
                && coral$red_black_tree$node_destroy(Z));
    coral_error = CORAL_ERROR_NONE;
}

static void
check_rotate_left_right_error_on_X_is_not_left_child_of_Z(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *X, *Y, *Z;
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_of(0, &Z));
    assert_true(coral$red_black_tree$node_set_parent(Y, X)
                && coral$red_black_tree$node_set_right(X, Y));
    assert_true(coral$red_black_tree$node_set_parent(X, Z)
                && coral$red_black_tree$node_set_right(Z, X));
    assert_false(coral$red_black_tree$rotate_left_right(Y));
    assert_int_equal(CORAL_RED_BLACK_TREE_ERROR_X_IS_NOT_LEFT_CHILD_OF_Z,
                     coral_error);
    assert_true(coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(Y)
                && coral$red_black_tree$node_destroy(Z));
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_left_right_a_b_c_d_pL_X_Y_Z(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *a, *b, *c, *d, *p, *X, *Y, *Z;
    assert_true(coral$red_black_tree$node_of(0, &a));
    assert_true(coral$red_black_tree$node_of(0, &b));
    assert_true(coral$red_black_tree$node_of(0, &c));
    assert_true(coral$red_black_tree$node_of(0, &d));
    assert_true(coral$red_black_tree$node_of(0, &p));
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_of(0, &Z));
    assert_true(coral$red_black_tree$node_set_parent(Y, X)
                && coral$red_black_tree$node_set_right(X, Y));
    assert_true(coral$red_black_tree$node_set_parent(X, Z)
                && coral$red_black_tree$node_set_left(Z, X));
    assert_true(coral$red_black_tree$node_set_parent(Z, p)
                && coral$red_black_tree$node_set_left(p, Z));
    assert_true(coral$red_black_tree$node_set_parent(a, X)
                && coral$red_black_tree$node_set_left(X, a));
    assert_true(coral$red_black_tree$node_set_parent(b, Y)
                && coral$red_black_tree$node_set_left(Y, b));
    assert_true(coral$red_black_tree$node_set_parent(c, Y)
                && coral$red_black_tree$node_set_right(Y, c));
    assert_true(coral$red_black_tree$node_set_parent(d, Z)
                && coral$red_black_tree$node_set_right(Z, d));
    assert_true(coral$red_black_tree$rotate_left_right(Y));
    void *a_, *b_, *c_, *d_, *p_, *X_, *Y_, *Z_;
    assert_true(coral$red_black_tree$node_get_parent(a, &X_)
                && X == X_
                && coral$red_black_tree$node_get_left(X, &a_)
                && a == a_);
    assert_true(coral$red_black_tree$node_get_parent(b, &X_)
                && X == X_
                && coral$red_black_tree$node_get_right(X, &b_)
                && b == b_);
    assert_true(coral$red_black_tree$node_get_parent(c, &Z_)
                && Z == Z_
                && coral$red_black_tree$node_get_left(Z, &c_)
                && c == c_);
    assert_true(coral$red_black_tree$node_get_parent(d, &Z_)
                && Z == Z_
                && coral$red_black_tree$node_get_right(Z, &d_)
                && d == d_);
    assert_true(coral$red_black_tree$node_get_parent(X, &Y_)
                && Y == Y_
                && coral$red_black_tree$node_get_left(Y, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_get_parent(Z, &Y_)
                && Y == Y_
                && coral$red_black_tree$node_get_right(Y, &Z_)
                && Z == Z_);
    assert_true(coral$red_black_tree$node_get_parent(Y, &p_)
                && p == p_
                && coral$red_black_tree$node_get_left(p, &Y)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_destroy(a)
                && coral$red_black_tree$node_destroy(b)
                && coral$red_black_tree$node_destroy(c)
                && coral$red_black_tree$node_destroy(d)
                && coral$red_black_tree$node_destroy(p)
                && coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(Y)
                && coral$red_black_tree$node_destroy(Z));
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_left_right_a_b_c_d_pR_X_Y_Z(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *a, *b, *c, *d, *p, *X, *Y, *Z;
    assert_true(coral$red_black_tree$node_of(0, &a));
    assert_true(coral$red_black_tree$node_of(0, &b));
    assert_true(coral$red_black_tree$node_of(0, &c));
    assert_true(coral$red_black_tree$node_of(0, &d));
    assert_true(coral$red_black_tree$node_of(0, &p));
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_of(0, &Z));
    assert_true(coral$red_black_tree$node_set_parent(Y, X)
                && coral$red_black_tree$node_set_right(X, Y));
    assert_true(coral$red_black_tree$node_set_parent(X, Z)
                && coral$red_black_tree$node_set_left(Z, X));
    assert_true(coral$red_black_tree$node_set_parent(Z, p)
                && coral$red_black_tree$node_set_right(p, Z));
    assert_true(coral$red_black_tree$node_set_parent(a, X)
                && coral$red_black_tree$node_set_left(X, a));
    assert_true(coral$red_black_tree$node_set_parent(b, Y)
                && coral$red_black_tree$node_set_left(Y, b));
    assert_true(coral$red_black_tree$node_set_parent(c, Y)
                && coral$red_black_tree$node_set_right(Y, c));
    assert_true(coral$red_black_tree$node_set_parent(d, Z)
                && coral$red_black_tree$node_set_right(Z, d));
    assert_true(coral$red_black_tree$rotate_left_right(Y));
    void *a_, *b_, *c_, *d_, *p_, *X_, *Y_, *Z_;
    assert_true(coral$red_black_tree$node_get_parent(a, &X_)
                && X == X_
                && coral$red_black_tree$node_get_left(X, &a_)
                && a == a_);
    assert_true(coral$red_black_tree$node_get_parent(b, &X_)
                && X == X_
                && coral$red_black_tree$node_get_right(X, &b_)
                && b == b_);
    assert_true(coral$red_black_tree$node_get_parent(c, &Z_)
                && Z == Z_
                && coral$red_black_tree$node_get_left(Z, &c_)
                && c == c_);
    assert_true(coral$red_black_tree$node_get_parent(d, &Z_)
                && Z == Z_
                && coral$red_black_tree$node_get_right(Z, &d_)
                && d == d_);
    assert_true(coral$red_black_tree$node_get_parent(X, &Y_)
                && Y == Y_
                && coral$red_black_tree$node_get_left(Y, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_get_parent(Z, &Y_)
                && Y == Y_
                && coral$red_black_tree$node_get_right(Y, &Z_)
                && Z == Z_);
    assert_true(coral$red_black_tree$node_get_parent(Y, &p_)
                && p == p_
                && coral$red_black_tree$node_get_right(p, &Y)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_destroy(a)
                && coral$red_black_tree$node_destroy(b)
                && coral$red_black_tree$node_destroy(c)
                && coral$red_black_tree$node_destroy(d)
                && coral$red_black_tree$node_destroy(p)
                && coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(Y)
                && coral$red_black_tree$node_destroy(Z));
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_right_left_error_on_Y_is_null(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$rotate_right_left(NULL));
    assert_int_equal(CORAL_RED_BLACK_TREE_ERROR_Y_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_right_left_error_on_Y_has_no_parent(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *Y;
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_false(coral$red_black_tree$rotate_right_left(Y));
    assert_int_equal(CORAL_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT, coral_error);
    assert_true(coral$red_black_tree$node_destroy(Y));
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_right_left_error_on_X_has_no_parent(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *X, *Y;
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_set_parent(Y, X)
                && coral$red_black_tree$node_set_left(X, Y));
    assert_false(coral$red_black_tree$rotate_right_left(Y));
    assert_int_equal(CORAL_RED_BLACK_TREE_ERROR_X_HAS_NO_PARENT, coral_error);
    assert_true(coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(Y));
    coral_error = CORAL_ERROR_NONE;
}

static void
check_rotate_right_left_error_on_Y_is_not_left_child_of_X(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *X, *Y, *Z;
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_of(0, &Z));
    assert_true(coral$red_black_tree$node_set_parent(Y, X)
                && coral$red_black_tree$node_set_right(X, Y));
    assert_true(coral$red_black_tree$node_set_parent(X, Z)
                && coral$red_black_tree$node_set_left(Z, X));
    assert_false(coral$red_black_tree$rotate_right_left(Y));
    assert_int_equal(CORAL_RED_BLACK_TREE_ERROR_Y_IS_NOT_LEFT_CHILD_OF_X,
                     coral_error);
    assert_true(coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(Y)
                && coral$red_black_tree$node_destroy(Z));
    coral_error = CORAL_ERROR_NONE;
}

static void
check_rotate_right_left_error_on_X_is_not_right_child_of_Z(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *X, *Y, *Z;
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_of(0, &Z));
    assert_true(coral$red_black_tree$node_set_parent(Y, X)
                && coral$red_black_tree$node_set_left(X, Y));
    assert_true(coral$red_black_tree$node_set_parent(X, Z)
                && coral$red_black_tree$node_set_left(Z, X));
    assert_false(coral$red_black_tree$rotate_right_left(Y));
    assert_int_equal(CORAL_RED_BLACK_TREE_ERROR_X_IS_NOT_RIGHT_CHILD_OF_Z,
                     coral_error);
    assert_true(coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(Y)
                && coral$red_black_tree$node_destroy(Z));
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_right_left_a_b_c_d_pL_X_Y_Z(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *a, *b, *c, *d, *p, *X, *Y, *Z;
    assert_true(coral$red_black_tree$node_of(0, &a));
    assert_true(coral$red_black_tree$node_of(0, &b));
    assert_true(coral$red_black_tree$node_of(0, &c));
    assert_true(coral$red_black_tree$node_of(0, &d));
    assert_true(coral$red_black_tree$node_of(0, &p));
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_of(0, &Z));
    assert_true(coral$red_black_tree$node_set_parent(Y, X)
                && coral$red_black_tree$node_set_left(X, Y));
    assert_true(coral$red_black_tree$node_set_parent(X, Z)
                && coral$red_black_tree$node_set_right(Z, X));
    assert_true(coral$red_black_tree$node_set_parent(Z, p)
                && coral$red_black_tree$node_set_left(p, Z));
    assert_true(coral$red_black_tree$node_set_parent(a, Z)
                && coral$red_black_tree$node_set_left(Z, a));
    assert_true(coral$red_black_tree$node_set_parent(b, Y)
                && coral$red_black_tree$node_set_left(Y, b));
    assert_true(coral$red_black_tree$node_set_parent(c, Y)
                && coral$red_black_tree$node_set_right(Y, c));
    assert_true(coral$red_black_tree$node_set_parent(d, X)
                && coral$red_black_tree$node_set_right(X, d));
    assert_true(coral$red_black_tree$rotate_right_left(Y));
    void *a_, *b_, *c_, *d_, *p_, *X_, *Y_, *Z_;
    assert_true(coral$red_black_tree$node_get_parent(a, &Z_)
                && Z == Z_
                && coral$red_black_tree$node_get_left(Z, &a_)
                && a == a_);
    assert_true(coral$red_black_tree$node_get_parent(b, &Z_)
                && Z == Z_
                && coral$red_black_tree$node_get_right(Z, &b_)
                && b == b_);
    assert_true(coral$red_black_tree$node_get_parent(c, &X_)
                && X == X_
                && coral$red_black_tree$node_get_left(X, &c_)
                && c == c_);
    assert_true(coral$red_black_tree$node_get_parent(d, &X_)
                && X == X_
                && coral$red_black_tree$node_get_right(X, &d_)
                && d == d_);
    assert_true(coral$red_black_tree$node_get_parent(X, &Y_)
                && Y == Y_
                && coral$red_black_tree$node_get_right(Y, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_get_parent(Z, &Y_)
                && Y == Y_
                && coral$red_black_tree$node_get_left(Y, &Z_)
                && Z == Z_);
    assert_true(coral$red_black_tree$node_get_parent(Y, &p_)
                && p == p_
                && coral$red_black_tree$node_get_left(p, &Y)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_destroy(a)
                && coral$red_black_tree$node_destroy(b)
                && coral$red_black_tree$node_destroy(c)
                && coral$red_black_tree$node_destroy(d)
                && coral$red_black_tree$node_destroy(p)
                && coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(Y)
                && coral$red_black_tree$node_destroy(Z));
    coral_error = CORAL_ERROR_NONE;
}

static void check_rotate_right_left_a_b_c_d_pR_X_Y_Z(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *a, *b, *c, *d, *p, *X, *Y, *Z;
    assert_true(coral$red_black_tree$node_of(0, &a));
    assert_true(coral$red_black_tree$node_of(0, &b));
    assert_true(coral$red_black_tree$node_of(0, &c));
    assert_true(coral$red_black_tree$node_of(0, &d));
    assert_true(coral$red_black_tree$node_of(0, &p));
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_of(0, &Z));
    assert_true(coral$red_black_tree$node_set_parent(Y, X)
                && coral$red_black_tree$node_set_left(X, Y));
    assert_true(coral$red_black_tree$node_set_parent(X, Z)
                && coral$red_black_tree$node_set_right(Z, X));
    assert_true(coral$red_black_tree$node_set_parent(Z, p)
                && coral$red_black_tree$node_set_right(p, Z));
    assert_true(coral$red_black_tree$node_set_parent(a, Z)
                && coral$red_black_tree$node_set_left(Z, a));
    assert_true(coral$red_black_tree$node_set_parent(b, Y)
                && coral$red_black_tree$node_set_left(Y, b));
    assert_true(coral$red_black_tree$node_set_parent(c, Y)
                && coral$red_black_tree$node_set_right(Y, c));
    assert_true(coral$red_black_tree$node_set_parent(d, X)
                && coral$red_black_tree$node_set_right(X, d));
    assert_true(coral$red_black_tree$rotate_right_left(Y));
    void *a_, *b_, *c_, *d_, *p_, *X_, *Y_, *Z_;
    assert_true(coral$red_black_tree$node_get_parent(a, &Z_)
                && Z == Z_
                && coral$red_black_tree$node_get_left(Z, &a_)
                && a == a_);
    assert_true(coral$red_black_tree$node_get_parent(b, &Z_)
                && Z == Z_
                && coral$red_black_tree$node_get_right(Z, &b_)
                && b == b_);
    assert_true(coral$red_black_tree$node_get_parent(c, &X_)
                && X == X_
                && coral$red_black_tree$node_get_left(X, &c_)
                && c == c_);
    assert_true(coral$red_black_tree$node_get_parent(d, &X_)
                && X == X_
                && coral$red_black_tree$node_get_right(X, &d_)
                && d == d_);
    assert_true(coral$red_black_tree$node_get_parent(X, &Y_)
                && Y == Y_
                && coral$red_black_tree$node_get_right(Y, &X_)
                && X == X_);
    assert_true(coral$red_black_tree$node_get_parent(Z, &Y_)
                && Y == Y_
                && coral$red_black_tree$node_get_left(Y, &Z_)
                && Z == Z_);
    assert_true(coral$red_black_tree$node_get_parent(Y, &p_)
                && p == p_
                && coral$red_black_tree$node_get_right(p, &Y)
                && Y == Y_);
    assert_true(coral$red_black_tree$node_destroy(a)
                && coral$red_black_tree$node_destroy(b)
                && coral$red_black_tree$node_destroy(c)
                && coral$red_black_tree$node_destroy(d)
                && coral$red_black_tree$node_destroy(p)
                && coral$red_black_tree$node_destroy(X)
                && coral$red_black_tree$node_destroy(Y)
                && coral$red_black_tree$node_destroy(Z));
    coral_error = CORAL_ERROR_NONE;
}

static void check_alloc_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$alloc(NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_alloc(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree *object;
    assert_true(coral$red_black_tree$alloc(&object));
    assert_null(object->root);
    assert_null(object->compare);
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_init_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    int (*compare)(const void *, const void *) = (void *) 1;
    assert_false(coral$red_black_tree$init(NULL,
                                           compare));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_init_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$init((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_invalidate_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void (*on_node_destroy)(void *) = (void *) 1;
    assert_false(coral$red_black_tree$invalidate(NULL, on_node_destroy));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check$on_node_destroy(void *node) {
    function_called();
    assert_non_null(node);
}

static void check_invalidate(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *a, *b, *c, *d, *p, *X, *Y, *Z;
    assert_true(coral$red_black_tree$node_of(0, &a));
    assert_true(coral$red_black_tree$node_of(0, &b));
    assert_true(coral$red_black_tree$node_of(0, &c));
    assert_true(coral$red_black_tree$node_of(0, &d));
    assert_true(coral$red_black_tree$node_of(0, &p));
    assert_true(coral$red_black_tree$node_of(0, &X));
    assert_true(coral$red_black_tree$node_of(0, &Y));
    assert_true(coral$red_black_tree$node_of(0, &Z));
    assert_true(coral$red_black_tree$node_set_parent(Y, X)
                && coral$red_black_tree$node_set_left(X, Y));
    assert_true(coral$red_black_tree$node_set_parent(X, Z)
                && coral$red_black_tree$node_set_right(Z, X));
    assert_true(coral$red_black_tree$node_set_parent(Z, p)
                && coral$red_black_tree$node_set_right(p, Z));
    assert_true(coral$red_black_tree$node_set_parent(a, Z)
                && coral$red_black_tree$node_set_left(Z, a));
    assert_true(coral$red_black_tree$node_set_parent(b, Y)
                && coral$red_black_tree$node_set_left(Y, b));
    assert_true(coral$red_black_tree$node_set_parent(c, Y)
                && coral$red_black_tree$node_set_right(Y, c));
    assert_true(coral$red_black_tree$node_set_parent(d, X)
                && coral$red_black_tree$node_set_right(X, d));
    struct coral$red_black_tree object = {
            .root = p
    };
    expect_function_calls(check$on_node_destroy, 8);
    assert_true(coral$red_black_tree$invalidate(
            &object, check$on_node_destroy));
    coral_error = CORAL_ERROR_NONE;
}

static void check_search_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$search(
            NULL, NULL, (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_search_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$search(
            (void *) 1, NULL, NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$search(
            (void *) 1, NULL, (void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_search_error_on_object_not_found(void **state) {
    coral_error = CORAL_ERROR_NONE;
    const size_t size = sizeof(size_t);
    size_t *a, *b, *c, *d, *e, *f, *g;
    assert_true(coral$red_black_tree$node_of(size, (void **) &a));
    assert_true(coral$red_black_tree$node_of(size, (void **) &b));
    assert_true(coral$red_black_tree$node_of(size, (void **) &c));
    assert_true(coral$red_black_tree$node_of(size, (void **) &d));
    assert_true(coral$red_black_tree$node_of(size, (void **) &e));
    assert_true(coral$red_black_tree$node_of(size, (void **) &f));
    assert_true(coral$red_black_tree$node_of(size, (void **) &g));
    *a = 0;
    *b = 1;
    *c = 2;
    *d = 3;
    *e = 4;
    *f = 5;
    *g = 6;
    coral_required_true(coral$red_black_tree$node_set_parent(b, d)
                        && coral$red_black_tree$node_set_left(d, b));
    coral_required_true(coral$red_black_tree$node_set_parent(f, d)
                        && coral$red_black_tree$node_set_right(d, f));
    coral_required_true(coral$red_black_tree$node_set_parent(a, b)
                        && coral$red_black_tree$node_set_left(b, a));
    coral_required_true(coral$red_black_tree$node_set_parent(c, b)
                        && coral$red_black_tree$node_set_right(b, c));
    coral_required_true(coral$red_black_tree$node_set_parent(e, f)
                        && coral$red_black_tree$node_set_left(f, e));
    coral_required_true(coral$red_black_tree$node_set_parent(g, f)
                        && coral$red_black_tree$node_set_right(f, g));
    struct coral$red_black_tree tree = {
            .root = d,
            .compare = coral_compare_size_t
    };
    size_t *item = NULL;
    size_t key = 10;
    assert_false(coral$red_black_tree$search(
            &tree, NULL, &key, (void **) &item));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    assert_ptr_equal(g, item); /* insertion point found in tree */
    assert_true(coral$red_black_tree$invalidate(&tree, NULL));
    coral_error = CORAL_ERROR_NONE;
}

static void check_search_empty_tree(void **state) {
    coral_error = CORAL_ERROR_NONE;
    size_t value = 12;
    struct coral$red_black_tree tree = {};
    void *at = &value;
    assert_false(coral$red_black_tree$search(&tree, NULL, &value, &at));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    assert_null(at);
    coral_error = CORAL_ERROR_NONE;
}

static void check_search(void **state) {
    coral_error = CORAL_ERROR_NONE;
    const size_t size = sizeof(size_t);
    size_t *a, *b, *c, *d, *e, *f, *g;
    assert_true(coral$red_black_tree$node_of(size, (void **) &a));
    assert_true(coral$red_black_tree$node_of(size, (void **) &b));
    assert_true(coral$red_black_tree$node_of(size, (void **) &c));
    assert_true(coral$red_black_tree$node_of(size, (void **) &d));
    assert_true(coral$red_black_tree$node_of(size, (void **) &e));
    assert_true(coral$red_black_tree$node_of(size, (void **) &f));
    assert_true(coral$red_black_tree$node_of(size, (void **) &g));
    *a = 0;
    *b = 1;
    *c = 2;
    *d = 3;
    *e = 4;
    *f = 5;
    *g = 6;
    coral_required_true(coral$red_black_tree$node_set_parent(b, d)
                        && coral$red_black_tree$node_set_left(d, b));
    coral_required_true(coral$red_black_tree$node_set_parent(f, d)
                        && coral$red_black_tree$node_set_right(d, f));
    coral_required_true(coral$red_black_tree$node_set_parent(a, b)
                        && coral$red_black_tree$node_set_left(b, a));
    coral_required_true(coral$red_black_tree$node_set_parent(c, b)
                        && coral$red_black_tree$node_set_right(b, c));
    coral_required_true(coral$red_black_tree$node_set_parent(e, f)
                        && coral$red_black_tree$node_set_left(f, e));
    coral_required_true(coral$red_black_tree$node_set_parent(g, f)
                        && coral$red_black_tree$node_set_right(f, g));
    struct coral$red_black_tree tree = {
            .root = d,
            .compare = coral_compare_size_t
    };
    size_t *item = NULL;
    size_t key = 2;
    assert_true(coral$red_black_tree$search(
            &tree, NULL, &key, (void **) &item));
    assert_ptr_equal(c, item); /* exact match found in tree */
    assert_true(coral$red_black_tree$invalidate(&tree, NULL));
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$insert(
            NULL, (void *) 1, (void *) 1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree object = {
            .root = (void *) 1
    };
    assert_false(coral$red_black_tree$insert(
            &object, NULL, (void *) 1));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$insert(
            &object, (void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_error_on_object_already_exists(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree *object;
    assert_true(coral$red_black_tree$alloc(&object));
    assert_true(coral$red_black_tree$init(object, coral_compare_size_t));
    size_t *value;
    assert_true(coral$red_black_tree$node_of(
            sizeof(*value), (void **) &value));
    *value = 99;
    object->root = value;
    assert_true(coral$red_black_tree$node_set_color(
            value, CORAL_RED_BLACK_TREE_COLOR_BLACK));
    size_t *node;
    assert_true(coral$red_black_tree$node_of(
            sizeof(*node), (void **) &node));
    *node = 99;
    assert_ptr_not_equal(value, node);
    assert_false(coral$red_black_tree$insert(object, value, node));
    assert_int_equal(CORAL_ERROR_OBJECT_ALREADY_EXISTS, coral_error);
    assert_true(coral$red_black_tree$node_destroy(node));
    assert_true(coral$red_black_tree$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_case_0(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree *object;
    assert_true(coral$red_black_tree$alloc(&object));
    assert_true(coral$red_black_tree$init(object, coral_compare_size_t));
    size_t *a;
    assert_true(coral$red_black_tree$node_of(sizeof(*a), (void **) &a));
    *a = 45;
    bool color = CORAL_RED_BLACK_TREE_COLOR_BLACK;
    assert_true(coral$red_black_tree$node_get_color(a, &color));
    assert_int_equal(CORAL_RED_BLACK_TREE_COLOR_RED, color);
    void *at;
    assert_false(coral$red_black_tree$search(object, NULL, a, &at));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    assert_null(at);
    assert_null(object->root);
    assert_true(coral$red_black_tree$insert(object, at, a));
    assert_true(coral$red_black_tree$node_get_color(a, &color));
    assert_int_equal(CORAL_RED_BLACK_TREE_COLOR_BLACK, color);
    assert_ptr_equal(a, object->root);
    assert_true(coral$red_black_tree$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_case_1(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree *object;
    assert_true(coral$red_black_tree$alloc(&object));
    assert_true(coral$red_black_tree$init(object, coral_compare_size_t));
    size_t *root, *a, *b;
    assert_true(coral$red_black_tree$node_of(sizeof(*root), (void **) &root));
    *root = 199;
    object->root = root;
    assert_true(coral$red_black_tree$node_set_color(
            root, CORAL_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(coral$red_black_tree$node_of(sizeof(*a), (void **) &a));
    *a = 50;
    assert_true(coral$red_black_tree$insert(object, root, a));
    void *root_, *a_, *b_;
    bool color = CORAL_RED_BLACK_TREE_COLOR_RED;
    assert_true(object->root == root
                && coral$red_black_tree$node_get_color(root, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(root, &a_)
                && a == a_
                && coral$red_black_tree$node_get_color(a, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_right(root, &b_)
                && NULL == b_);
    assert_true(coral$red_black_tree$node_of(sizeof(*b), (void **) &b));
    *b = 263;
    assert_true(coral$red_black_tree$insert(object, root, b));
    color = CORAL_RED_BLACK_TREE_COLOR_BLACK;
    assert_true(object->root == root
                && coral$red_black_tree$node_get_left(root, &a_)
                && a == a_
                && coral$red_black_tree$node_get_parent(a, &root_)
                && root == root_
                && coral$red_black_tree$node_get_color(a, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_right(root, &b_)
                && b == b_
                && coral$red_black_tree$node_get_parent(b, &root_)
                && root == root_
                && coral$red_black_tree$node_get_color(b, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color);
    assert_true(coral$red_black_tree$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_case_2b_configuration_3(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree *object;
    assert_true(coral$red_black_tree$alloc(&object));
    assert_true(coral$red_black_tree$init(object, coral_compare_size_t));
    size_t *a, *b, *c;
    assert_true(coral$red_black_tree$node_of(sizeof(*a), (void **) &a));
    assert_true(coral$red_black_tree$node_of(sizeof(*b), (void **) &b));
    assert_true(coral$red_black_tree$node_of(sizeof(*c), (void **) &c));
    *a = 4;
    *b = 2;
    *c = 1;
    assert_true(coral$red_black_tree$insert(object, NULL, a));
    assert_true(coral$red_black_tree$insert(object, a, b));
    void *a_, *b_, *c_;
    bool color = CORAL_RED_BLACK_TREE_COLOR_RED;
    assert_true(object->root == a
                && coral$red_black_tree$node_get_color(a, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(a, &b_)
                && b == b_
                && coral$red_black_tree$node_get_color(b, &color)
                && color == CORAL_RED_BLACK_TREE_COLOR_RED);
    assert_true(coral$red_black_tree$insert(object, b, c));
    assert_true(object->root == b
                && coral$red_black_tree$node_get_color(b, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(b, &c_)
                && c == c_
                && coral$red_black_tree$node_get_color(c, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_parent(c, &b_)
                && b == b_
                && coral$red_black_tree$node_get_right(b, &a_)
                && a == a_
                && coral$red_black_tree$node_get_color(a, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_parent(a, &b_)
                && b == b_);
    assert_true(coral$red_black_tree$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_case_2b_configuration_2(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree *object;
    assert_true(coral$red_black_tree$alloc(&object));
    assert_true(coral$red_black_tree$init(object, coral_compare_size_t));
    size_t *a, *b, *c;
    assert_true(coral$red_black_tree$node_of(sizeof(*a), (void **) &a));
    assert_true(coral$red_black_tree$node_of(sizeof(*b), (void **) &b));
    assert_true(coral$red_black_tree$node_of(sizeof(*c), (void **) &c));
    *a = 4;
    *b = 2;
    *c = 1;
    assert_true(coral$red_black_tree$insert(object, NULL, a));
    assert_true(coral$red_black_tree$insert(object, a, c));
    void *a_, *b_, *c_;
    bool color = CORAL_RED_BLACK_TREE_COLOR_RED;
    assert_true(object->root == a
                && coral$red_black_tree$node_get_color(a, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(a, &c_)
                && c == c_
                && coral$red_black_tree$node_get_color(c, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color);
    assert_true(coral$red_black_tree$insert(object, c, b));
    assert_true(object->root == b
                && coral$red_black_tree$node_get_color(b, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(b, &c_)
                && c == c_
                && coral$red_black_tree$node_get_color(c, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_parent(c, &b_)
                && b == b_
                && coral$red_black_tree$node_get_right(b, &a_)
                && a == a_
                && coral$red_black_tree$node_get_color(a, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_parent(a, &b_)
                && b == b_);
    assert_true(coral$red_black_tree$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_case_2b_configuration_1(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree *object;
    assert_true(coral$red_black_tree$alloc(&object));
    assert_true(coral$red_black_tree$init(object, coral_compare_size_t));
    size_t *a, *b, *c;
    assert_true(coral$red_black_tree$node_of(sizeof(*a), (void **) &a));
    assert_true(coral$red_black_tree$node_of(sizeof(*b), (void **) &b));
    assert_true(coral$red_black_tree$node_of(sizeof(*c), (void **) &c));
    *a = 4;
    *b = 2;
    *c = 1;
    assert_true(coral$red_black_tree$insert(object, NULL, c));
    assert_true(coral$red_black_tree$insert(object, c, a));
    void *a_, *b_, *c_;
    bool color = CORAL_RED_BLACK_TREE_COLOR_RED;
    assert_true(object->root == c
                && coral$red_black_tree$node_get_color(c, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_right(c, &a_)
                && a == a_
                && coral$red_black_tree$node_get_color(a, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color);
    assert_true(coral$red_black_tree$insert(object, a, b));
    assert_true(object->root == b
                && coral$red_black_tree$node_get_color(b, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(b, &c_)
                && c == c_
                && coral$red_black_tree$node_get_color(c, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_parent(c, &b_)
                && b == b_
                && coral$red_black_tree$node_get_right(b, &a_)
                && a == a_
                && coral$red_black_tree$node_get_color(a, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_parent(a, &b_)
                && b == b_);
    assert_true(coral$red_black_tree$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_case_2b_configuration_0(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree *object;
    assert_true(coral$red_black_tree$alloc(&object));
    assert_true(coral$red_black_tree$init(object, coral_compare_size_t));
    size_t *a, *b, *c;
    assert_true(coral$red_black_tree$node_of(sizeof(*a), (void **) &a));
    assert_true(coral$red_black_tree$node_of(sizeof(*b), (void **) &b));
    assert_true(coral$red_black_tree$node_of(sizeof(*c), (void **) &c));
    *a = 4;
    *b = 2;
    *c = 1;
    assert_true(coral$red_black_tree$insert(object, NULL, c));
    assert_true(coral$red_black_tree$insert(object, c, b));
    void *a_, *b_, *c_;
    bool color = CORAL_RED_BLACK_TREE_COLOR_RED;
    assert_true(object->root == c
                && coral$red_black_tree$node_get_color(c, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_right(c, &b_)
                && b == b_
                && coral$red_black_tree$node_get_color(b, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color);
    assert_true(coral$red_black_tree$insert(object, b, a));
    assert_true(object->root == b
                && coral$red_black_tree$node_get_color(b, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(b, &c_)
                && c == c_
                && coral$red_black_tree$node_get_color(c, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_parent(c, &b_)
                && b == b_
                && coral$red_black_tree$node_get_right(b, &a_)
                && a == a_
                && coral$red_black_tree$node_get_color(a, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_parent(a, &b_)
                && b == b_);
    assert_true(coral$red_black_tree$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert_case_2a(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree *object;
    assert_true(coral$red_black_tree$alloc(&object));
    assert_true(coral$red_black_tree$init(object, coral_compare_size_t));
    size_t *a, *b, *c, *d;
    assert_true(coral$red_black_tree$node_of(sizeof(*a), (void **) &a));
    assert_true(coral$red_black_tree$node_of(sizeof(*b), (void **) &b));
    assert_true(coral$red_black_tree$node_of(sizeof(*c), (void **) &c));
    assert_true(coral$red_black_tree$node_of(sizeof(*d), (void **) &d));
    *a = 5;
    *b = 4;
    *c = 2;
    *d = 1;
    assert_true(coral$red_black_tree$insert(object, NULL, b));
    assert_true(coral$red_black_tree$insert(object, b, a));
    assert_true(coral$red_black_tree$insert(object, b, c));
    void *a_, *b_, *c_, *d_;
    bool color = CORAL_RED_BLACK_TREE_COLOR_RED;
    assert_true(object->root == b
                && coral$red_black_tree$node_get_color(b, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_right(b, &a_)
                && a == a_
                && coral$red_black_tree$node_get_color(a, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_left(b, &c_)
                && c == c_
                && coral$red_black_tree$node_get_color(c, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color);
    assert_true(coral$red_black_tree$insert(object, c, d));
    assert_true(object->root == b
                && coral$red_black_tree$node_get_color(b, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_right(b, &a_)
                && a == a_
                && coral$red_black_tree$node_get_color(a, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(b, &c_)
                && c == c_
                && coral$red_black_tree$node_get_color(c, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(c, &d_)
                && d == d_
                && coral$red_black_tree$node_get_color(d, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color);
    assert_true(coral$red_black_tree$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_insert(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree *object;
    assert_true(coral$red_black_tree$alloc(&object));
    assert_true(coral$red_black_tree$init(object, coral_compare_size_t));
    size_t *a, *b, *c, *d, *e;
    assert_true(coral$red_black_tree$node_of(sizeof(*a), (void **) &a));
    assert_true(coral$red_black_tree$node_of(sizeof(*b), (void **) &b));
    assert_true(coral$red_black_tree$node_of(sizeof(*c), (void **) &c));
    assert_true(coral$red_black_tree$node_of(sizeof(*d), (void **) &d));
    assert_true(coral$red_black_tree$node_of(sizeof(*e), (void **) &e));
    *a = 9;
    *b = 5;
    *c = 2;
    *d = 4;
    *e = 1;
    assert_true(coral$red_black_tree$insert(object, NULL, a));
    assert_true(coral$red_black_tree$insert(object, a, b));
    assert_true(coral$red_black_tree$insert(object, b, c));
    void *a_, *b_, *c_, *d_, *e_;
    bool color = CORAL_RED_BLACK_TREE_COLOR_RED;
    assert_true(object->root == b
                && coral$red_black_tree$node_get_color(b, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(b, &c_)
                && c == c_
                && coral$red_black_tree$node_get_color(c, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_parent(c, &b_)
                && b == b_
                && coral$red_black_tree$node_get_right(b, &a_)
                && a == a_
                && coral$red_black_tree$node_get_color(a, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_parent(a, &b_)
                && b == b_);
    assert_true(coral$red_black_tree$insert(object, b, d));
    assert_true(object->root == b
                && coral$red_black_tree$node_get_color(b, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(b, &d_)
                && d == d_
                && coral$red_black_tree$node_get_color(d, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_parent(d, &b_)
                && b == b_
                && coral$red_black_tree$node_get_left(d, &c_)
                && c == c_
                && coral$red_black_tree$node_get_color(c, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_parent(c, &d_)
                && d == d_
                && coral$red_black_tree$node_get_right(b, &a_)
                && a == a_
                && coral$red_black_tree$node_get_color(a, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_parent(a, &b_)
                && b == b_);
    assert_true(coral$red_black_tree$insert(object, c, e));
    assert_true(object->root == b
                && coral$red_black_tree$node_get_color(b, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_right(b, &a_)
                && a == a_
                && coral$red_black_tree$node_get_color(a, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_parent(a, &b_)
                && b == b_
                && coral$red_black_tree$node_get_left(b, &c_)
                && c == c_
                && coral$red_black_tree$node_get_color(c, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_parent(a, &c_)
                && b == b_
                && coral$red_black_tree$node_get_right(c, &d_)
                && d == d_
                && coral$red_black_tree$node_get_color(d, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_parent(d, &c_)
                && c == c_
                && coral$red_black_tree$node_get_left(c, &e_)
                && e == e_
                && coral$red_black_tree$node_get_color(e, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_parent(e, &c_)
                && c == c_);
    assert_true(coral$red_black_tree$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_first_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *node;
    assert_false(coral$red_black_tree$get_first(NULL, &node));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_first_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$get_first((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_first_error_on_empty_tree(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree tree = {};
    void *node;
    assert_false(coral$red_black_tree$get_first(&tree, &node));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_first(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree *object;
    assert_true(coral$red_black_tree$alloc(&object));
    assert_true(coral$red_black_tree$init(object, coral_compare_size_t));
    size_t *a, *b, *c, *d, *e;
    assert_true(coral$red_black_tree$node_of(sizeof(*a), (void **) &a));
    assert_true(coral$red_black_tree$node_of(sizeof(*b), (void **) &b));
    assert_true(coral$red_black_tree$node_of(sizeof(*c), (void **) &c));
    assert_true(coral$red_black_tree$node_of(sizeof(*d), (void **) &d));
    assert_true(coral$red_black_tree$node_of(sizeof(*e), (void **) &e));
    *a = 9;
    *b = 5;
    *c = 4;
    *d = 2;
    *e = 1;
    assert_true(coral$red_black_tree$insert(object, NULL, a));
    void *first;
    assert_true(coral$red_black_tree$get_first(object, &first));
    assert_ptr_equal(a, first);
    assert_int_equal(*a, *(size_t *) first);
    assert_true(coral$red_black_tree$insert(object, a, b));
    assert_true(coral$red_black_tree$get_first(object, &first));
    assert_ptr_equal(b, first);
    assert_int_equal(*b, *(size_t *) first);
    assert_true(coral$red_black_tree$insert(object, b, c));
    assert_true(coral$red_black_tree$get_first(object, &first));
    assert_ptr_equal(c, first);
    assert_int_equal(*c, *(size_t *) first);
    assert_true(coral$red_black_tree$insert(object, c, d));
    assert_true(coral$red_black_tree$get_first(object, &first));
    assert_ptr_equal(d, first);
    assert_int_equal(*d, *(size_t *) first);
    assert_true(coral$red_black_tree$insert(object, d, e));
    assert_true(coral$red_black_tree$get_first(object, &first));
    assert_ptr_equal(e, first);
    assert_int_equal(*e, *(size_t *) first);
    assert_true(coral$red_black_tree$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_last_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *node;
    assert_false(coral$red_black_tree$get_last(NULL, &node));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_last_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree tree = {};
    void *node;
    assert_false(coral$red_black_tree$get_last(&tree, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_last_error_on_empty_tree(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree tree = {};
    void *node;
    assert_false(coral$red_black_tree$get_last(&tree, &node));
    assert_int_equal(CORAL_ERROR_OBJECT_NOT_FOUND, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_last(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree *object;
    assert_true(coral$red_black_tree$alloc(&object));
    assert_true(coral$red_black_tree$init(object, coral_compare_size_t));
    size_t *a, *b, *c, *d, *e;
    assert_true(coral$red_black_tree$node_of(sizeof(*a), (void **) &a));
    assert_true(coral$red_black_tree$node_of(sizeof(*b), (void **) &b));
    assert_true(coral$red_black_tree$node_of(sizeof(*c), (void **) &c));
    assert_true(coral$red_black_tree$node_of(sizeof(*d), (void **) &d));
    assert_true(coral$red_black_tree$node_of(sizeof(*e), (void **) &e));
    *a = 1;
    *b = 2;
    *c = 4;
    *d = 5;
    *e = 9;
    assert_true(coral$red_black_tree$insert(object, NULL, a));
    void *last;
    assert_true(coral$red_black_tree$get_last(object, &last));
    assert_ptr_equal(a, last);
    assert_int_equal(*a, *(size_t *) last);
    assert_true(coral$red_black_tree$insert(object, a, b));
    assert_true(coral$red_black_tree$get_last(object, &last));
    assert_ptr_equal(b, last);
    assert_int_equal(*b, *(size_t *) last);
    assert_true(coral$red_black_tree$insert(object, b, c));
    assert_true(coral$red_black_tree$get_last(object, &last));
    assert_ptr_equal(c, last);
    assert_int_equal(*c, *(size_t *) last);
    assert_true(coral$red_black_tree$insert(object, c, d));
    assert_true(coral$red_black_tree$get_last(object, &last));
    assert_ptr_equal(d, last);
    assert_int_equal(*d, *(size_t *) last);
    assert_true(coral$red_black_tree$insert(object, d, e));
    assert_true(coral$red_black_tree$get_last(object, &last));
    assert_ptr_equal(e, last);
    assert_int_equal(*e, *(size_t *) last);
    assert_true(coral$red_black_tree$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_next_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *node;
    assert_false(coral$red_black_tree$get_next(NULL, &node));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_next_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$get_next((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_next(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree *object;
    assert_true(coral$red_black_tree$alloc(&object));
    assert_true(coral$red_black_tree$init(object, coral_compare_size_t));
    size_t *a, *b, *c, *d, *e;
    assert_true(coral$red_black_tree$node_of(sizeof(*a), (void **) &a));
    assert_true(coral$red_black_tree$node_of(sizeof(*b), (void **) &b));
    assert_true(coral$red_black_tree$node_of(sizeof(*c), (void **) &c));
    assert_true(coral$red_black_tree$node_of(sizeof(*d), (void **) &d));
    assert_true(coral$red_black_tree$node_of(sizeof(*e), (void **) &e));
    *a = 9;
    *b = 5;
    *c = 4;
    *d = 2;
    *e = 1;
    assert_true(coral$red_black_tree$insert(object, NULL, a));
    assert_true(coral$red_black_tree$insert(object, a, b));
    assert_true(coral$red_black_tree$insert(object, b, c));
    assert_true(coral$red_black_tree$insert(object, c, d));
    assert_true(coral$red_black_tree$insert(object, d, e));
    void *node;
    assert_true(coral$red_black_tree$get_first(object, &node));
    assert_ptr_equal(e, node);
    assert_int_equal(1, *(size_t *) node);
    assert_true(coral$red_black_tree$get_next(node, &node));
    assert_ptr_equal(d, node);
    assert_int_equal(2, *(size_t *) node);
    assert_true(coral$red_black_tree$get_next(node, &node));
    assert_ptr_equal(c, node);
    assert_int_equal(4, *(size_t *) node);
    assert_true(coral$red_black_tree$get_next(node, &node));
    assert_ptr_equal(b, node);
    assert_int_equal(5, *(size_t *) node);
    assert_true(coral$red_black_tree$get_next(node, &node));
    assert_ptr_equal(a, node);
    assert_int_equal(9, *(size_t *) node);
    assert_false(coral$red_black_tree$get_next(node, &node));
    assert_int_equal(CORAL_ERROR_END_OF_SEQUENCE, coral_error);
    assert_true(coral$red_black_tree$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_prev_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    void *node;
    assert_false(coral$red_black_tree$get_prev(NULL, &node));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_prev_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$get_prev((void *) 1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_get_prev(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree *object;
    assert_true(coral$red_black_tree$alloc(&object));
    assert_true(coral$red_black_tree$init(object, coral_compare_size_t));
    size_t *a, *b, *c, *d, *e;
    assert_true(coral$red_black_tree$node_of(sizeof(*a), (void **) &a));
    assert_true(coral$red_black_tree$node_of(sizeof(*b), (void **) &b));
    assert_true(coral$red_black_tree$node_of(sizeof(*c), (void **) &c));
    assert_true(coral$red_black_tree$node_of(sizeof(*d), (void **) &d));
    assert_true(coral$red_black_tree$node_of(sizeof(*e), (void **) &e));
    *a = 1;
    *b = 2;
    *c = 4;
    *d = 5;
    *e = 9;
    assert_true(coral$red_black_tree$insert(object, NULL, a));
    assert_true(coral$red_black_tree$insert(object, a, b));
    assert_true(coral$red_black_tree$insert(object, b, c));
    assert_true(coral$red_black_tree$insert(object, c, d));
    assert_true(coral$red_black_tree$insert(object, d, e));
    void *node;
    assert_true(coral$red_black_tree$get_last(object, &node));
    assert_ptr_equal(e, node);
    assert_int_equal(9, *(size_t *) node);
    assert_true(coral$red_black_tree$get_prev(node, &node));
    assert_ptr_equal(d, node);
    assert_int_equal(5, *(size_t *) node);
    assert_true(coral$red_black_tree$get_prev(node, &node));
    assert_ptr_equal(c, node);
    assert_int_equal(4, *(size_t *) node);
    assert_true(coral$red_black_tree$get_prev(node, &node));
    assert_ptr_equal(b, node);
    assert_int_equal(2, *(size_t *) node);
    assert_true(coral$red_black_tree$get_prev(node, &node));
    assert_ptr_equal(a, node);
    assert_int_equal(1, *(size_t *) node);
    assert_false(coral$red_black_tree$get_prev(node, &node));
    assert_int_equal(CORAL_ERROR_END_OF_SEQUENCE, coral_error);
    assert_true(coral$red_black_tree$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete_error_on_null_object_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$delete(NULL, (void *)1));
    assert_int_equal(CORAL_ERROR_OBJECT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete_error_on_null_argument_ptr(void **state) {
    coral_error = CORAL_ERROR_NONE;
    assert_false(coral$red_black_tree$delete((void*)1, NULL));
    assert_int_equal(CORAL_ERROR_ARGUMENT_PTR_IS_NULL, coral_error);
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete_leaf_nodes(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree *object;
    assert_true(coral$red_black_tree$alloc(&object));
    assert_true(coral$red_black_tree$init(object, coral_compare_size_t));
    size_t *a, *b;
    assert_true(coral$red_black_tree$node_of(sizeof(*a), (void **) &a));
    assert_true(coral$red_black_tree$node_of(sizeof(*b), (void **) &b));
    *a = 9;
    *b = 4;
    assert_true(coral$red_black_tree$insert(object, NULL, a));
    assert_true(coral$red_black_tree$insert(object, a, b));
    bool color;
    void *a_, *b_, *_;
    assert_true(object->root == a
                && coral$red_black_tree$node_get_color(a, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(a, &b_)
                && b == b_
                && coral$red_black_tree$node_get_parent(b, &a_)
                && a == a_
                && coral$red_black_tree$node_get_color(b, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_right(a, &_)
                && NULL == _);
    /* delete leaf */
    assert_true(coral$red_black_tree$delete(object, b));
    assert_true(object->root == a
                && coral$red_black_tree$node_get_color(a, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(a, &_)
                && NULL == _
                && coral$red_black_tree$node_get_right(a, &_)
                && NULL == _);
    /* delete root */
    assert_true(coral$red_black_tree$delete(object, a));
    assert_null(object->root);
    assert_true(coral$red_black_tree$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete_node_with_single_child(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree *object;
    assert_true(coral$red_black_tree$alloc(&object));
    assert_true(coral$red_black_tree$init(object, coral_compare_size_t));
    size_t *a, *b, *c, *d, *e;
    assert_true(coral$red_black_tree$node_of(sizeof(*a), (void **) &a));
    assert_true(coral$red_black_tree$node_of(sizeof(*b), (void **) &b));
    assert_true(coral$red_black_tree$node_of(sizeof(*c), (void **) &c));
    assert_true(coral$red_black_tree$node_of(sizeof(*d), (void **) &d));
    assert_true(coral$red_black_tree$node_of(sizeof(*e), (void **) &e));
    *a = 1;
    *b = 7;
    *c = 4;
    *d = 3;
    *e = 9;
    assert_true(coral$red_black_tree$insert(object, NULL, c));
    assert_true(coral$red_black_tree$insert(object, c, d));
    assert_true(coral$red_black_tree$insert(object, c, b));
    assert_true(coral$red_black_tree$insert(object, d, a));
    assert_true(coral$red_black_tree$insert(object, b, e));
    bool color;
    void *a_, *b_, *c_, *d_, *e_, *_;
    /*
     *         c(4B)
     *         /  \
     *     d(3B)  b(7B)
     *      /        \
     *    a(1R)      e(9R)
     */
    assert_true(object->root == c
                && coral$red_black_tree$node_get_color(c, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(c, &d_)
                && d == d_
                && coral$red_black_tree$node_get_color(d, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_parent(d, &c_)
                && c == c_
                && coral$red_black_tree$node_get_right(c, &b_)
                && b == b_
                && coral$red_black_tree$node_get_color(b, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_parent(b, &c_)
                && c == c_
                && coral$red_black_tree$node_get_right(d, &_)
                && NULL == _
                && coral$red_black_tree$node_get_left(d, &a_)
                && a == a_
                && coral$red_black_tree$node_get_color(a, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_left(a, &_)
                && NULL == _
                && coral$red_black_tree$node_get_right(a, &_)
                && NULL == _
                && coral$red_black_tree$node_get_parent(a, &d_)
                && d == d_
                && coral$red_black_tree$node_get_left(b, &_)
                && NULL == _
                && coral$red_black_tree$node_get_right(b, &e_)
                && e == e_
                && coral$red_black_tree$node_get_color(e, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_left(e, &_)
                && NULL == _
                && coral$red_black_tree$node_get_right(e, &_)
                && NULL == _
                && coral$red_black_tree$node_get_parent(e, &b_)
                && b == b_);
    /* single left node */
    assert_true(coral$red_black_tree$delete(object, d));
    /*
     *         c(4B)
     *         /  \
     *     a(1B)  b(7B)
     *               \
     *               e(9R)
     */
    assert_true(object->root == c
                && coral$red_black_tree$node_get_color(c, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(c, &a_)
                && a == a_
                && coral$red_black_tree$node_get_color(a, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_parent(a, &c_)
                && c == c_
                && coral$red_black_tree$node_get_left(a, &_)
                && NULL == _
                && coral$red_black_tree$node_get_right(a, &_)
                && NULL == _
                && coral$red_black_tree$node_get_right(c, &b_)
                && b == b_
                && coral$red_black_tree$node_get_color(b, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_parent(b, &c_)
                && c == c_
                && coral$red_black_tree$node_get_left(b, &_)
                && NULL == _
                && coral$red_black_tree$node_get_right(b, &e_)
                && e == e_
                && coral$red_black_tree$node_get_color(e, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_parent(e, &b_)
                && b == b_);
    /* single right node */
    assert_true(coral$red_black_tree$delete(object, b));
    /*
     *         c(4B)
     *         /  \
     *     a(1B)  e(9B)
     */
    assert_true(object->root == c
                && coral$red_black_tree$node_get_color(c, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(c, &a_)
                && a == a_
                && coral$red_black_tree$node_get_color(a, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(a, &_)
                && NULL == _
                && coral$red_black_tree$node_get_right(a, &_)
                && NULL == _
                && coral$red_black_tree$node_get_parent(a, &c_)
                && c == c_
                && coral$red_black_tree$node_get_right(c, &e_)
                && e == e_
                && coral$red_black_tree$node_get_color(e, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(e, &_)
                && NULL == _
                && coral$red_black_tree$node_get_right(e, &_)
                && NULL == _
                && coral$red_black_tree$node_get_parent(e, &c_)
                && c == c_);
    assert_true(coral$red_black_tree$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_delete_node_with_two_children(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree *object;
    assert_true(coral$red_black_tree$alloc(&object));
    assert_true(coral$red_black_tree$init(object, coral_compare_size_t));
    size_t *a, *b, *c, *d, *e;
    assert_true(coral$red_black_tree$node_of(sizeof(*a), (void **) &a));
    assert_true(coral$red_black_tree$node_of(sizeof(*b), (void **) &b));
    assert_true(coral$red_black_tree$node_of(sizeof(*c), (void **) &c));
    assert_true(coral$red_black_tree$node_of(sizeof(*d), (void **) &d));
    assert_true(coral$red_black_tree$node_of(sizeof(*e), (void **) &e));
    *a = 1;
    *b = 7;
    *c = 4;
    *d = 3;
    *e = 9;
    assert_true(coral$red_black_tree$insert(object, NULL, c));
    assert_true(coral$red_black_tree$insert(object, c, d));
    assert_true(coral$red_black_tree$insert(object, c, b));
    assert_true(coral$red_black_tree$insert(object, d, a));
    assert_true(coral$red_black_tree$insert(object, b, e));
    bool color;
    void *a_, *b_, *c_, *d_, *e_, *_;
    /*
     *         c(4B)
     *         /  \
     *     d(3B)  b(7B)
     *      /        \
     *    a(1R)      e(9R)
     */
    assert_true(object->root == c
                && coral$red_black_tree$node_get_color(c, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(c, &d_)
                && d == d_
                && coral$red_black_tree$node_get_color(d, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_right(d, &_)
                && NULL == _
                && coral$red_black_tree$node_get_parent(d, &c_)
                && c == c_
                && coral$red_black_tree$node_get_right(c, &b_)
                && b == b_
                && coral$red_black_tree$node_get_color(b, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(b, &_)
                && NULL == _
                && coral$red_black_tree$node_get_parent(b, &c_)
                && c == c_
                && coral$red_black_tree$node_get_left(d, &a_)
                && a == a_
                && coral$red_black_tree$node_get_color(a, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_left(a, &_)
                && NULL == _
                && coral$red_black_tree$node_get_right(a, &_)
                && NULL == _
                && coral$red_black_tree$node_get_parent(a, &d_)
                && d == d_
                && coral$red_black_tree$node_get_right(b, &e_)
                && e == e_
                && coral$red_black_tree$node_get_color(e, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_left(e, &_)
                && NULL == _
                && coral$red_black_tree$node_get_right(e, &_)
                && NULL == _
                && coral$red_black_tree$node_get_parent(e, &b_)
                && b == b_);
    /* delete node with two children */
    assert_true(coral$red_black_tree$delete(object, c));
    /*
     *         b(7B)
     *         /  \
     *     d(3B)  e(9B)
     *      /
     *    a(1R)
     */
    assert_true(object->root == b
                && coral$red_black_tree$node_get_color(b, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(b, &d_)
                && d == d_
                && coral$red_black_tree$node_get_color(d, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_right(d, &_)
                && NULL == _
                && coral$red_black_tree$node_get_parent(d, &b_)
                && b == b_
                && coral$red_black_tree$node_get_left(d, &a_)
                && a == a_
                && coral$red_black_tree$node_get_color(a, &color)
                && CORAL_RED_BLACK_TREE_COLOR_RED == color
                && coral$red_black_tree$node_get_left(a, &_)
                && NULL == _
                && coral$red_black_tree$node_get_right(a, &_)
                && NULL == _
                && coral$red_black_tree$node_get_parent(a, &d_)
                && d == d_
                && coral$red_black_tree$node_get_right(b, &e_)
                && e == e_
                && coral$red_black_tree$node_get_color(e, &color)
                && CORAL_RED_BLACK_TREE_COLOR_BLACK == color
                && coral$red_black_tree$node_get_left(e, &_)
                && NULL == _
                && coral$red_black_tree$node_get_right(e, &_)
                && NULL == _
                && coral$red_black_tree$node_get_parent(e, &b_)
                && b == b_);
    assert_true(coral$red_black_tree$invalidate(object, NULL));
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

static void check_tree(void **state) {
    coral_error = CORAL_ERROR_NONE;
    struct coral$red_black_tree *object;
    assert_true(coral$red_black_tree$alloc(&object));
    assert_true(coral$red_black_tree$init(object, coral_compare_size_t));
    size_t *p = NULL;
    for (size_t i = 0; i < 10000; i++) {
        size_t *n;
        assert_true(coral$red_black_tree$node_of(sizeof(*n), (void **)&n));
        *n = i;
        assert_true(coral$red_black_tree$insert(object, p, n));
        p = n;
    }
    for (size_t i = 0; i < 10000; i++) {
        assert_true(coral$red_black_tree$delete(object, object->root));
    }
    assert_null(object->root);
    free(object);
    coral_error = CORAL_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_node_alloc_error_on_null_argument_ptr),
            cmocka_unit_test(check_node_alloc_error_on_memory_allocation_failed),
            cmocka_unit_test(check_node_alloc),
            cmocka_unit_test(check_node_destroy_no_error_on_null_object_ptr),
            cmocka_unit_test(check_node_init_error_on_null_object_ptr),
            cmocka_unit_test(check_node_init),
            cmocka_unit_test(check_node_of_error_on_null_argument_ptr),
            cmocka_unit_test(check_node_of_error_on_memory_allocation_failed),
            cmocka_unit_test(check_node_of),
            cmocka_unit_test(check_node_set_parent_error_on_null_object_ptr),
            cmocka_unit_test(check_node_set_parent),
            cmocka_unit_test(check_node_get_parent_error_on_null_object_ptr),
            cmocka_unit_test(check_node_get_parent_error_on_null_argument_ptr),
            cmocka_unit_test(check_node_get_parent),
            cmocka_unit_test(check_node_get_color_black_on_null_object_ptr),
            cmocka_unit_test(check_node_get_color_error_on_null_argument_ptr),
            cmocka_unit_test(check_node_get_color),
            cmocka_unit_test(check_node_set_color_error_on_null_object_ptr),
            cmocka_unit_test(check_node_set_color),
            cmocka_unit_test(check_node_get_left_error_on_null_object_ptr),
            cmocka_unit_test(check_node_get_left_error_on_null_argument_ptr),
            cmocka_unit_test(check_node_get_left),
            cmocka_unit_test(check_node_set_left_error_on_null_object_ptr),
            cmocka_unit_test(check_node_set_left),
            cmocka_unit_test(check_node_get_right_error_on_null_object_ptr),
            cmocka_unit_test(check_node_get_right_error_on_null_argument_ptr),
            cmocka_unit_test(check_node_get_right),
            cmocka_unit_test(check_node_set_right_error_on_null_object_ptr),
            cmocka_unit_test(check_node_set_right),
            cmocka_unit_test(check_rotate_left_error_on_Y_is_null),
            cmocka_unit_test(check_rotate_left_error_on_Y_has_no_parent),
            cmocka_unit_test(check_rotate_left_error_on_Y_is_not_right_child_of_X),
            cmocka_unit_test(check_rotate_left_a_b_c_pL_X_Y),
            cmocka_unit_test(check_rotate_left_a_b_c_pR_X_Y),
            cmocka_unit_test(check_rotate_left_a_b_c_X_Y),
            cmocka_unit_test(check_rotate_left_a_c_pL_X_Y),
            cmocka_unit_test(check_rotate_left_a_c_pR_X_Y),
            cmocka_unit_test(check_rotate_left_a_c_X_Y),
            cmocka_unit_test(check_rotate_right_error_on_Y_is_null),
            cmocka_unit_test(check_rotate_right_error_on_Y_has_no_parent),
            cmocka_unit_test(check_rotate_right_error_on_Y_is_not_left_child_of_X),
            cmocka_unit_test(check_rotate_right_a_b_c_pL_X_Y),
            cmocka_unit_test(check_rotate_right_a_b_c_pR_X_Y),
            cmocka_unit_test(check_rotate_right_a_b_c_X_Y),
            cmocka_unit_test(check_rotate_right_a_c_pL_X_Y),
            cmocka_unit_test(check_rotate_right_a_c_pR_X_Y),
            cmocka_unit_test(check_rotate_right_a_c_X_Y),
            cmocka_unit_test(check_rotate_left_right_error_on_Y_is_null),
            cmocka_unit_test(check_rotate_left_right_error_on_Y_has_no_parent),
            cmocka_unit_test(check_rotate_left_right_error_on_X_has_no_parent),
            cmocka_unit_test(check_rotate_left_right_error_on_Y_is_not_right_child_of_X),
            cmocka_unit_test(check_rotate_left_right_error_on_X_is_not_left_child_of_Z),
            cmocka_unit_test(check_rotate_left_right_a_b_c_d_pL_X_Y_Z),
            cmocka_unit_test(check_rotate_left_right_a_b_c_d_pR_X_Y_Z),
            cmocka_unit_test(check_rotate_right_left_error_on_Y_is_null),
            cmocka_unit_test(check_rotate_right_left_error_on_Y_has_no_parent),
            cmocka_unit_test(check_rotate_right_left_error_on_X_has_no_parent),
            cmocka_unit_test(check_rotate_right_left_error_on_Y_is_not_left_child_of_X),
            cmocka_unit_test(check_rotate_right_left_error_on_X_is_not_right_child_of_Z),
            cmocka_unit_test(check_rotate_right_left_a_b_c_d_pL_X_Y_Z),
            cmocka_unit_test(check_rotate_right_left_a_b_c_d_pR_X_Y_Z),
            cmocka_unit_test(check_alloc_error_on_null_argument_ptr),
            cmocka_unit_test(check_alloc),
            cmocka_unit_test(check_init_error_on_null_object_ptr),
            cmocka_unit_test(check_init_error_on_null_argument_ptr),
            cmocka_unit_test(check_invalidate_on_null_object_ptr),
            cmocka_unit_test(check_invalidate),
            cmocka_unit_test(check_search_error_on_null_object_ptr),
            cmocka_unit_test(check_search_error_on_null_argument_ptr),
            cmocka_unit_test(check_search_error_on_object_not_found),
            cmocka_unit_test(check_search_empty_tree),
            cmocka_unit_test(check_search),
            cmocka_unit_test(check_insert_error_on_null_object_ptr),
            cmocka_unit_test(check_insert_error_on_null_argument_ptr),
            cmocka_unit_test(check_insert_error_on_object_already_exists),
            cmocka_unit_test(check_insert_case_0),
            cmocka_unit_test(check_insert_case_1),
            cmocka_unit_test(check_insert_case_2b_configuration_3),
            cmocka_unit_test(check_insert_case_2b_configuration_2),
            cmocka_unit_test(check_insert_case_2b_configuration_1),
            cmocka_unit_test(check_insert_case_2b_configuration_0),
            cmocka_unit_test(check_insert_case_2a),
            cmocka_unit_test(check_insert),
            cmocka_unit_test(check_get_first_error_on_null_object_ptr),
            cmocka_unit_test(check_get_first_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_first_error_on_empty_tree),
            cmocka_unit_test(check_get_first),
            cmocka_unit_test(check_get_last_error_on_null_object_ptr),
            cmocka_unit_test(check_get_last_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_last_error_on_empty_tree),
            cmocka_unit_test(check_get_last),
            cmocka_unit_test(check_get_next_error_on_null_object_ptr),
            cmocka_unit_test(check_get_next_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_next),
            cmocka_unit_test(check_get_prev_error_on_null_object_ptr),
            cmocka_unit_test(check_get_prev_error_on_null_argument_ptr),
            cmocka_unit_test(check_get_prev),
            cmocka_unit_test(check_delete_error_on_null_object_ptr),
            cmocka_unit_test(check_delete_error_on_null_argument_ptr),
            cmocka_unit_test(check_delete_leaf_nodes),
            cmocka_unit_test(check_delete_node_with_single_child),
            cmocka_unit_test(check_delete_node_with_two_children),
            cmocka_unit_test(check_tree)
    };
    cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
