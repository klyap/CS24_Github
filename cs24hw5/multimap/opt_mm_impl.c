#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "multimap.h"


/*============================================================================
 * TYPES
 *
 *   These types are defined in the implementation file so that they can
 *   be kept hidden to code outside this source file.  This is not for any
 *   security reason, but rather just so we can enforce that our testing
 *   programs are generic and don't have any access to implementation details.
 *============================================================================*/

 /* Represents a value that is associated with a given key in the multimap. */

/* Represents a key and its associated values in the multimap, as well as
 * pointers to the left and right child nodes in the multimap. */
typedef struct multimap_node {
    /* The key-value that this multimap node represents. */
    int key;

    /* A linked list of the values associated with this key in the multimap. */
    int *values;

    /* The tail of the linked list of values, so that we can retain the order
     * they were added to the list.
     */
    int values_i;

    /* The left child of the multimap node.  This will reference nodes that
     * hold keys that are strictly less than this node's key.
     */
    struct multimap_node *left_child;

    /* The right child of the multimap node.  This will reference nodes that
     * hold keys that are strictly greater than this node's key.
     */
    struct multimap_node *right_child;
    
    /* Color for red-black tree. 1 means red, 0 means black */
    int red;
    
} multimap_node;


/* The entry-point of the multimap data structure. */
struct multimap {
    multimap_node *root;
};


/*============================================================================
 * HELPER FUNCTION DECLARATIONS
 *
 *   Declarations of helper functions that are local to this module.  Again,
 *   these are not visible outside of this module.
 *============================================================================*/

multimap_node * alloc_mm_node();

multimap_node * find_mm_node(multimap_node *root, int key,
                             int create_if_not_found);
multimap_node * rotate_left_mm_node(multimap_node *curr);
multimap_node * rotate_right_mm_node(multimap_node *curr);

void balance_mm_node(multimap_node *node);

void free_multimap_values(int *values);
void free_multimap_node(multimap_node *node);


/*============================================================================
 * FUNCTION IMPLEMENTATIONS
 *============================================================================*/
/*
 * Rotate node left for left leaning red-black tree.
 */
multimap_node * rotate_left_mm_node(multimap_node *curr) {
    multimap_node *node = curr->right_child;
    curr->right_child = node->left_child;
    node->left_child = curr;
    node->red = curr->red;
    curr->red = 1;
    return node;
}

/*
 * Rotate node right for left leaning red-black tree.
 */
multimap_node * rotate_right_mm_node(multimap_node *curr) {
    multimap_node *node = curr->left_child;
    curr->left_child = node->right_child;
    node->right_child = curr;
    node->red = curr->red;
    curr->red = 1;
    return node;
}

void flip_color(multimap_node *curr){
    curr->red = 1;
    curr->left_child->red = 0;
    curr->right_child->red = 0;
}

void balance_mm_node(multimap_node *node){
   
    if (node->left_child != NULL){
        if (node->right_child != NULL){
            if (node->right_child != NULL && node->left_child != NULL 
                && node->right_child->red == 1 && node->left_child->red == 0){
                node = rotate_left_mm_node(node);
            }
            
            if (node->left_child->red == 1 && node->right_child->red == 1){
                flip_color(node);
            }
            
        } else {
            if (node->left_child->red == 1 && 
                node->left_child->left_child->red == 1){
                node = rotate_right_mm_node(node);
            }
        }
    }
    
}


/* Allocates a multimap node, and zeros out its contents so that we know what
 * the initial value of everything will be.
 */
multimap_node * alloc_mm_node() {
    multimap_node *node = malloc(sizeof(multimap_node));
    bzero(node, sizeof(multimap_node));

    return node;
}


/* This helper function searches for the multimap node that contains the
 * specified key.  If such a node doesn't exist, the function can initialize
 * a new node and add this into the structure, or it will simply return NULL.
 * The one exception is the root - if the root is NULL then the function will
 * return a new root node.
 */
multimap_node * find_mm_node(multimap_node *root, int key,
                             int create_if_not_found) {
    multimap_node *node;

    /* If the entire multimap is empty, the root will be NULL. */
    if (root == NULL) {
        if (create_if_not_found) {
            root = alloc_mm_node();
            root->key = key;
        }
        return root;
    }

    /* Now we know the multimap has at least a root node, so start there. */
    node = root;
    while (1) {
        if (node->key == key)
            break;

        if (node->key > key) {   /* Follow left child */
            if (node->left_child == NULL && create_if_not_found) {
                /* No left child, but caller wants us to create a new node. */
                multimap_node *new = alloc_mm_node();
                new->key = key;
                new->red = 1;
                node->left_child = new;
                
                node = node->left_child;
                
                /* Balance the tree if needed */
                balance_mm_node(node);
            } else {
                node = node->left_child;
            }
            
        }
        else {                   /* Follow right child */
            if (node->right_child == NULL && create_if_not_found) {
                /* No right child, but caller wants us to create a new node. */
                
                multimap_node *new = alloc_mm_node();
                new->key = key;
                new->red = 1;
                node->right_child = new;
                
                node = node->right_child;
                /* Balance the tree if needed */
                balance_mm_node(node);
                
            } else {
                node = node->right_child;
            }
            
        }
       
        if (node == NULL)
            break;
        
    }
   
    return node;
}


/* This helper function frees all values in a multimap node's value-list. */
void free_multimap_values(int *values) {
    free(values);
}


/* This helper function frees a multimap node, including its children and
 * value-list.
 */
void free_multimap_node(multimap_node *node) {
    if (node == NULL)
        return;

    /* Free the children first. */
    free_multimap_node(node->left_child);
    free_multimap_node(node->right_child);

    /* Free the list of values. */
    free_multimap_values(node->values);

#ifdef DEBUG_ZERO
    /* Clear out what we are about to free, to expose issues quickly. */
    bzero(node, sizeof(multimap_node));
#endif
    free(node);
}


/* Initialize a multimap data structure. */
multimap * init_multimap() {
    multimap *mm = malloc(sizeof(multimap));
    mm->root = NULL;
    return mm;
}


/* Release all dynamically allocated memory associated with the multimap
 * data structure.
 */
void clear_multimap(multimap *mm) {
    assert(mm != NULL);
    free_multimap_node(mm->root);
    mm->root = NULL;
}


/* Adds the specified (key, value) pair to the multimap. */
void mm_add_value(multimap *mm, int key, int value) {
    multimap_node *node;
    //int *new_value;

    assert(mm != NULL);
    /* Look up the node with the specified key.  Create if not found. */
    node = find_mm_node(mm->root, key, /* create */ 1);
    if (mm->root == NULL)
        mm->root = node;

    assert(node != NULL);
    assert(node->key == key);

    /* Add the new value to the multimap node. */
    if (node->values_i > 0){
        /* Case 1: There are existing values */
        if (node->values_i % 50 == 40){
            /* Expand values list if we are running out of space */
            int *bigger_array = (int *) realloc(node->values, 
                                (node->values_i/50 + 2) * 50 * sizeof(int));
            
            node->values = bigger_array;
        }
        
        /* Add new value to list */
        *(node->values + node->values_i) = value;
        node->values_i += 1;
        
    } else {
        /* Case 2: need to do initital value list allocation */
        int *new_value = malloc(50 * sizeof(int));
        *new_value = value;
        node->values = new_value;
        node->values_i = 1;
        
    }
    
}


/* Returns nonzero if the multimap contains the specified key-value, zero
 * otherwise.
 */
int mm_contains_key(multimap *mm, int key) {
    return find_mm_node(mm->root, key, /* create */ 0) != NULL;
}


/* Returns nonzero if the multimap contains the specified (key, value) pair,
 * zero otherwise.
 */
int mm_contains_pair(multimap *mm, int key, int value) {
    multimap_node *node;
    int *curr;
    
    node = find_mm_node(mm->root, key, /* create */ 0);

    if (node == NULL)
        return 0;

    curr = node->values;
    if (curr == NULL){
        return 0;
    }
    
    /* Iterate through values until you find it or reach end of list */
    for (int i = 0; i < node->values_i; i++){
        if (*(node->values + i) == value)
            return 1;
    }

    return 0;
}


/* This helper function is used by mm_traverse() to traverse every pair within
 * the multimap.
 */
void mm_traverse_helper(multimap_node *node, void (*f)(int key, int value)) {
    
    int *curr;
    
    if (node == NULL)
        return;

    mm_traverse_helper(node->left_child, f);

    curr = node->values;
    
    /* Iterate through all values in the list */
    for (int i = 0; i < node->values_i; i++){
        f(node->key, *(curr + i));
    }


    mm_traverse_helper(node->right_child, f);
}


/* Performs an in-order traversal of the multimap, passing each (key, value)
 * pair to the specified function.
 */
void mm_traverse(multimap *mm, void (*f)(int key, int value)) {
    mm_traverse_helper(mm->root, f);
}

