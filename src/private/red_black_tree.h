#ifndef _CORAL_PRIVATE_RED_BLACK_TREE_H_
#define _CORAL_PRIVATE_RED_BLACK_TREE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Red-Black Tree: https://en.wikipedia.org/wiki/Red-black_tree */

#define CORAL_RED_BLACK_TREE_COLOR_RED                          0
#define CORAL_RED_BLACK_TREE_COLOR_BLACK                        1

#define CORAL_RED_BLACK_TREE_ERROR_Y_IS_NULL                    1
#define CORAL_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT              2
#define CORAL_RED_BLACK_TREE_ERROR_Y_IS_NOT_LEFT_CHILD_OF_X     3
#define CORAL_RED_BLACK_TREE_ERROR_Y_IS_NOT_RIGHT_CHILD_OF_X    4
#define CORAL_RED_BLACK_TREE_ERROR_X_HAS_NO_PARENT              5
#define CORAL_RED_BLACK_TREE_ERROR_X_IS_NOT_LEFT_CHILD_OF_Z     6
#define CORAL_RED_BLACK_TREE_ERROR_X_IS_NOT_RIGHT_CHILD_OF_Z    7

struct coral$red_black_tree$node {
    void *parent; /* color bit is stored in the lowest bit */
    void *left;
    void *right;
};

struct coral$red_black_tree$node *coral$red_black_tree$node_from(void *node);

void *coral$red_black_tree$node_to(struct coral$red_black_tree$node *node);

/**
 * @brief Create node instance.
 * @param [in] size bytes needed to be allocated for node instance.
 * @param [out] out receive the created node instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the node instance.
 */
bool coral$red_black_tree$node_of(size_t size, void **out);

/**
 * @brief Allocate memory for the node instance.
 * @param [in] size bytes needed to be allocated for node instance.
 * @param [out] out receive the allocated node instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate the node instance.
 */
bool coral$red_black_tree$node_alloc(size_t size, void **out);

/**
 * @brief Initialize the node instance.
 * @param [in] node instance to be initialized.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if node is <i>NULL</i>.
 */
bool coral$red_black_tree$node_init(void *node);

/**
 * @brief Destroy the node instance.
 * @param [in] node instance to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if node is <i>NULL</i>.
 */
bool coral$red_black_tree$node_destroy(void *node);

/**
 * @brief Retrieve parent of node.
 * @param [in] node whose parent we are to retrieve.
 * @param [out] out receive the parent of node.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if node is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 */
bool coral$red_black_tree$node_get_parent(void *node, void **out);

/**
 * @brief Set the parent of node.
 * @param [in] node whose parent we are to set.
 * @param [in] parent which will be the node's parent.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if node is <i>NULL</i>.
 */
bool coral$red_black_tree$node_set_parent(void *node, void *parent);

/**
 * @brief Retrieve the color of node.
 * @param [in] node whose color we are to retrieve.
 * @param [out] out receive the color of the node.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if node is <i>NULL</i>.
 */
bool coral$red_black_tree$node_get_color(void *node, bool *out);

/**
 * @brief Set the color of the node.
 * @param [in] node whose color we are to set.
 * @param [in] colour which will be the node's color.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if node is <i>NULL</i>.
 */
bool coral$red_black_tree$node_set_color(void *node, bool colour);

/**
 * @brief Retrieve left child of node.
 * @param [in] node whose left child node we are to retrieve.
 * @param [out] out receive the left child node.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_END_OF_SEQUENCE if trying to get left child node from a
 * NIL node.
 */
bool coral$red_black_tree$node_get_left(void *node, void **out);

/**
 * @brief Set the left child of node.
 * @param [in] node whose left child we are to set.
 * @param [in] left which will be the left child of node.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if node is <i>NULL</i>.
 */
bool coral$red_black_tree$node_set_left(void *node, void *left);

/**
 * @brief Retrieve right child of node.
 * @param [in] node whose left right node we are to retrieve.
 * @param [out] out receive the right child node.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_END_OF_SEQUENCE if trying to get right child node from a
 * NIL node.
 */
bool coral$red_black_tree$node_get_right(void *node, void **out);

/**
 * @brief Set the right child of node.
 * @param [in] node whose right child we are to set.
 * @param [in] right which will be the right child of node.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if node is <i>NULL</i>.
 */
bool coral$red_black_tree$node_set_right(void *node, void *right);

/**
 * @brief Retrieve the next node.
 * @param [in] node whose next node we are to retrieve.
 * @param [out] out receive the next node.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if node is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_END_OF_SEQUENCE if there is no next node.
 */
bool coral$red_black_tree$get_next(void *node, void **out);

/**
 * @brief Retrieve the previous node.
 * @param [in] node whose previous node we are to retrieve.
 * @param [out] out receive the previous node.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if node is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_END_OF_SEQUENCE if there is no previous node.
 */
bool coral$red_black_tree$get_prev(void *node, void **out);

/**
 * @brief Perform left rotation.
 * @verbatim
 *      p                 p
 *      |                 |
 *     (x)               (y)
 *     / \       =>      / \
 *    a  (y)           (x)  c
 *       / \           / \
 *      b   c         a   b
 * @endverbatim
 * @param [in] node (y) in the picture above before rotation is performed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_RED_BLACK_TREE_ERROR_Y_IS_NULL
 * @throws CORAL_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT
 * @throws CORAL_RED_BLACK_TREE_ERROR_Y_IS_NOT_RIGHT_CHILD_OF_X
 */
bool coral$red_black_tree$rotate_left(void *node);

/**
 * @brief Perform right rotation.
 * @verbatim
 *      p                 p
 *      |                 |
 *     (x)               (y)
 *     / \       =>      / \
 *   (y)  c             a  (x)
 *   / \                   / \
 *  a   b                 b   c
 * @endverbatim
 * @param [in] node (y) in the picture above before rotation is performed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_RED_BLACK_TREE_ERROR_Y_IS_NULL
 * @throws CORAL_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT
 * @throws CORAL_RED_BLACK_TREE_ERROR_Y_IS_NOT_LEFT_CHILD_OF_X
 */
bool coral$red_black_tree$rotate_right(void *node);

/**
 * @brief Perform left-right rotation.
 * @verbatim
 *      p                 p
 *      |                 |
 *     (z)               (y)
 *     / \       =>      / \
 *   (x)  d            (x)  (z)
 *   / \               / \  / \
 *  a  (y)            a   bc   d
 *     / \
 *    b   c
 * @endverbatim
 * @param [in] node (y) in the picture above before rotation is performed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_RED_BLACK_TREE_ERROR_Y_IS_NULL
 * @throws CORAL_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT
 * @throws CORAL_RED_BLACK_TREE_ERROR_Y_IS_NOT_RIGHT_CHILD_OF_X
 * @throws CORAL_RED_BLACK_TREE_ERROR_X_HAS_NO_PARENT
 * @throws CORAL_RED_BLACK_TREE_ERROR_X_IS_NOT_LEFT_CHILD_OF_Z
 */
bool coral$red_black_tree$rotate_left_right(void *node);

/**
 * @brief Perform right-left rotation.
 * @verbatim
 *      p                 p
 *      |                 |
 *     (z)               (y)
 *     / \       =>      / \
 *    a  (x)           (z)  (x)
 *       / \           / \  / \
 *     (y)  d         a   bc   d
 *     / \
 *    b   c
 * @endverbatim
 * @param [in] node (y) in the picture above before rotation is performed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_RED_BLACK_TREE_ERROR_Y_IS_NULL
 * @throws CORAL_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT
 * @throws CORAL_RED_BLACK_TREE_ERROR_Y_IS_NOT_LEFT_CHILD_OF_X
 * @throws CORAL_RED_BLACK_TREE_ERROR_X_HAS_NO_PARENT
 * @throws CORAL_RED_BLACK_TREE_ERROR_X_IS_NOT_RIGHT_CHILD_OF_Z
 */
bool coral$red_black_tree$rotate_right_left(void *node);

struct coral$red_black_tree {
    void *root;

    int (*compare)(const void *, const void *);
};

/**
 * @brief Allocate memory for red black tree instance.
 * @param [out] out receive the allocated red black tree instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_MEMORY_ALLOCATION_FAILED if there is insufficient
 * memory to allocate space for a red black tree instance.
 */
bool coral$red_black_tree$alloc(struct coral$red_black_tree **out);

/**
 * @brief Initialise the red black tree instance.
 * @param [in] object instance to be initialised.
 * @param [in] compare comparison which must return an integer less than,
 * equal to, or greater than zero if the <u>first node</u> is considered
 * to be respectively less than, equal to, or greater than the <u>second
 * node</u>.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if compare is <i>NULL</i>.
 */
bool coral$red_black_tree$init(struct coral$red_black_tree *object,
                               int (*compare)(const void *first,
                                              const void *second));

/**
 * @brief Invalidate the red black tree instance.
 * <p>The nodes in the red black tree are destroyed and each will invoke the
 * provided <i>on node destroy</i> callback. The actual <u>red black tree
 * instance is not deallocated</u> since it may have been embedded in a larger
 * structure.
 * @param [in] object instance to be invalidated.
 * @param [in] on_node_destroy called just before the node is to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 */
bool coral$red_black_tree$invalidate(struct coral$red_black_tree *object,
                                     void (*on_node_destroy)(void *));

/**
 * @brief First value in the red black tree instance.
 * @param [in] object instance from which the first value is to be retrieved.
 * @param [out] out receive the first value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if the red black tree is empty.
 */
bool coral$red_black_tree$get_first(struct coral$red_black_tree *object,
                                    void **out);

/**
 * @brief Last value in the red black tree instance.
 * @param [in] object instance from which the last value is to be retrieved.
 * @param [out] out receive the last value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if out is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if the red black tree is empty.
 */
bool coral$red_black_tree$get_last(struct coral$red_black_tree *object,
                                   void **out);

/**
 * @brief Search for value.
 * @param [in] object instance to be searched.
 * @param [in] root scope the search if not specified then whole red black
 * tree will be searched for value.
 * @param [in] value to find in the tree instance.
 * @param [out] out receive the exact matched node or insertion point.
 * @return If exact match found true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if either value or out is
 * <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_NOT_FOUND if exact match not found insertion
 * point will still be reported.
 */
bool coral$red_black_tree$search(struct coral$red_black_tree *object,
                                 const void *root, const void *value,
                                 void **out);

/**
 * @brief Insert node.
 * @param [in] object instance of red black tree which node is to be inserted.
 * @param [in] insertion_point obtained by performing search.
 * @param [in] node which is to be inserted at insertion point.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NULL if either insertion point (while
 * the red black tree instance is not empty) or node is <i>NULL</i>.
 * @throws CORAL_ERROR_OBJECT_ALREADY_EXISTS if node is already present in
 * the red black tree instance.
 */
bool coral$red_black_tree$insert(struct coral$red_black_tree *object,
                                 void *insertion_point, void *node);

/**
 * @brief Delete node.
 * @param [in] object instance of red black tree which node is to be deleted.
 * @param [in] node which is to be deleted.
 * @return On success true, otherwise false if an error has occurred.
 * @throws CORAL_ERROR_OBJECT_PTR_IS_NULL if object is <i>NULL</i>.
 * @throws CORAL_ERROR_ARGUMENT_PTR_IS_NUL if node is <i>NULL</i>.
 */
bool coral$red_black_tree$delete(struct coral$red_black_tree *object,
                                 void *node);

#endif /* _CORAL_PRIVATE_RED_BLACK_TREE_H_ */
