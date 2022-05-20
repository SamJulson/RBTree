// RBTree.c
// Written by Samuel Julson

///////////////////////////////////////////////////////////////////////////////
//
// START PREPROCESSOR DIRECTIVES
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
//
// START ListNode STRUCTURES
//
////////////////////////////////////////////////////////////////////////////////

typedef struct List_Node {
	struct List_Node* next;					// Next node to go to
	void* value;							// Value of that node
} ListNode;

////////////////////////////////////////////////////////////////////////////////
//
// START LinkedList STRUCTURES
//
////////////////////////////////////////////////////////////////////////////////

typedef struct Linked_List {
	int size;								// Number of nodes in the list
	ListNode* head;							// First node on the list
} LinkedList;

////////////////////////////////////////////////////////////////////////////////
//
// START LinkedListIterator STRUCTURES
//
////////////////////////////////////////////////////////////////////////////////

typedef struct Linked_List_Iterator {
	LinkedList* list;						// List to iterate on
	ListNode* currNode;						// Current node
	ListNode* nextNode;						// Next node to iterate to
} LinkedListIterator;

////////////////////////////////////////////////////////////////////////////////
//
// START RBNode STRUCTURES
//
////////////////////////////////////////////////////////////////////////////////

typedef enum RB_Node_Color {
	UNDEFINED,
	RED,
	BLACK,
} RBNodeColor;

typedef struct RB_Node {
	RBNodeColor color;					// Color of this node: RED or BLACK
	struct RB_Node* parent;					// Parent of this node
	struct RB_Node* children[2];			// Children of this node
	void* key;								// The key of this node
	void* value;							// The value of this node
} RBNode;

////////////////////////////////////////////////////////////////////////////////
//
// START RBTree STRUCTURES
//
////////////////////////////////////////////////////////////////////////////////

typedef int (*Comparator)(void*, void*);	// Comparison function between keys

typedef struct RBTree {
	RBNode* root;							// The root of the tree
	Comparator keyCompareFunction;			// The comparison function for keys
} RBTree;

////////////////////////////////////////////////////////////////////////////////
//
// Start RBTreeIterator STRUCTURES
//
////////////////////////////////////////////////////////////////////////////////

typedef struct RBTreeIterator {
	RBTree* tree;							// Tree to iterate on
	RBNode* currNode;						// Current node
	RBNode* nextNode;						// Next node to iterate to
} RBTreeIterator;

////////////////////////////////////////////////////////////////////////////////
//
// START ListNode FUNCTION DECLARATIONS
//
////////////////////////////////////////////////////////////////////////////////

ListNode* 			ListNode_create(ListNode*, void*);
void 				ListNode_delete(ListNode*);
ListNode*			ListNode_getNext(ListNode*);
void* 				ListNode_getValue(ListNode*);
void 				ListNode_setNext(ListNode*, ListNode*);
void 				ListNode_setValue(ListNode*, void*);

////////////////////////////////////////////////////////////////////////////////
//
// START LinkedList FUNCTION DECLARATIONS
//
////////////////////////////////////////////////////////////////////////////////

LinkedList* 		LinkedList_create();
void				LinkedList_delete(LinkedList*);
int					LinkedList_add(LinkedList*, int, void*);
void*				LinkedList_remove(LinkedList*, int);
int 				LinkedList_isEmpty(LinkedList*);
void*				LinkedList_get(LinkedList*, int);
int 				LinkedList_size(LinkedList*);
int 				LinkedList_contains(LinkedList*, void*);

////////////////////////////////////////////////////////////////////////////////
//
// START LinkedListIterator STRUCTURES
//
////////////////////////////////////////////////////////////////////////////////

LinkedListIterator*	LinkedListIterator_create(LinkedList*);
void 				LinkedListIterator_delete(LinkedListIterator*);
void*				LinkedListIterator_getValue(LinkedListIterator*);
void 				LinkedListIterator_getNext(LinkedListIterator*);
int 				LinkedListIterator_hasNext(LinkedListIterator*);

////////////////////////////////////////////////////////////////////////////////
//
// START RBNode FUNCTION DECLARATIONS
//
////////////////////////////////////////////////////////////////////////////////

RBNode* 			RBNode_create(RBNodeColor, RBNode*, RBNode*, RBNode*, void*, void*);
void 				RBNode_delete(RBNode*);
RBNodeColor 		RBNode_getColor(RBNode*);
RBNode* 			RBNode_getParent(RBNode*);
RBNode* 			RBNode_getLeftChild(RBNode*);
RBNode* 			RBNode_getRightChild(RBNode*);
void* 				RBNode_getKey(RBNode*);
void* 				RBNode_getValue(RBNode*);
void 				RBNode_setColor(RBNode*, RBNodeColor);
void 				RBNode_setParent(RBNode*, RBNode*);
void				RBNode_setLeftChild(RBNode*, RBNode*);
void				RBNode_setRightChild(RBNode*, RBNode*);
void 				RBNode_setKey(RBNode*, void*);
void 				RBNode_setValue(RBNode*, void*);

////////////////////////////////////////////////////////////////////////////////
//
// START RBTree FUNCTION DECLARATIONS
//
////////////////////////////////////////////////////////////////////////////////

RBTree* 			RBTree_create(Comparator);
void 				RBTree_delete(RBTree*);
void 				RBTree_delete_recursion(RBNode*);
RBNode* 			RBTree_getRoot(RBTree*);
Comparator 			RBTree_getKeyCompareFunction(RBTree*);
void 				RBTree_setRoot(RBTree*, RBNode*);
void 				RBTree_setKeyCompareFunction(RBTree*, Comparator);
void 				RBTree_repairAfterInsert(RBTree*, RBNode*);
void* 				RBTree_search(RBTree*, void*);
int 				RBTree_remove(RBTree*, void*);
void 				RBTree_repairAfterRemove(RBTree*);
void 				RBTree_rotateLeft(RBTree*, RBNode*);
void 				RBTree_rotateRight(RBTree*, RBNode*);

////////////////////////////////////////////////////////////////////////////////
//
// START RBTreeIterator FUNCTION DECLARATIONS
//
////////////////////////////////////////////////////////////////////////////////

RBTreeIterator*		RBTreeIterator_create(RBTree*);
void 				RBTreeIterator_delete(RBTreeIterator*);
void* 				RBTreeIterator_getKey(RBTreeIterator*);
void*				RBTreeIterator_getValue(RBTreeIterator*);
void 				RBTreeIterator_getNext(RBTreeIterator*);
int 				RBTreeIterator_hasNext(RBTreeIterator*);

////////////////////////////////////////////////////////////////////////////////
//
// START ListNode FUNCTION DEFINITIONS
//
////////////////////////////////////////////////////////////////////////////////

ListNode* ListNode_create(ListNode* next, void* value) {
	ListNode* newListNode = malloc(sizeof(ListNode));
	if (NULL == newListNode) {
		return NULL;
	}
	newListNode->next = next;
	newListNode->value = value;
	return newListNode;
}

void ListNode_delete(ListNode* node) {
	if (NULL == node) {
		return;
	}
	free(node);
}

ListNode* ListNode_getNext(ListNode* node) {
	if (NULL == node) {
		return NULL;
	}
	return node->next;
}

void* ListNode_getValue(ListNode* node) {
	if (NULL == node) {
		return NULL;
	}
	return node->value;
}

void ListNode_setNext(ListNode* node, ListNode* next) {
	if (NULL == node) {
		return;
	}
	node->next = next;
}

void ListNode_setValue(ListNode* node, void* value) {
	if (NULL == node) {
		return;
	}
	node->value = value;
}

////////////////////////////////////////////////////////////////////////////////
//
// START LinkedList FUNCTION DEFINITIONS
//
////////////////////////////////////////////////////////////////////////////////

LinkedList* LinkedList_create() {
	LinkedList* newLinkedList = malloc(sizeof(LinkedList));
	if (NULL == newLinkedList) {
		return NULL;
	}
	newLinkedList->size = 0;
	newLinkedList->head = NULL;
	return newLinkedList;
}

void LinkedList_delete(LinkedList* list) {
	if (NULL == list) {
		return;
	}
	ListNode* currNode = list->head;
	ListNode* nextNode = ListNode_getNext(currNode);
	while (NULL != currNode) {
		currNode = nextNode;
		nextNode = ListNode_getNext(currNode);
		free(currNode);
	}
	free(list);
}

int	LinkedList_add(LinkedList* list, int index, void* value) {
	if (NULL == list) {
		return 1;
	}

	if (index > LinkedList_size(list) || index < 0) {
		return 1;
	}

	ListNode* prevNode = NULL;
	ListNode* currNode = list->head;
	for (; index != 0; index--) {
		prevNode = currNode;
		currNode = ListNode_getNext(prevNode);
	}

	ListNode* newNode = ListNode_create(currNode, value);
	if (NULL != prevNode) {
		ListNode_setNext(prevNode, newNode);
	}	

	return 0;
}

void* LinkedList_remove(LinkedList* list, int index) {
	// TODO: Implement this later
	return NULL;
}

int LinkedList_isEmpty(LinkedList* list) {
	if (NULL == list) {
		return 1;
	}
	return (0 > list->size) ? 1 : 0;
}

void* LinkedList_get(LinkedList* list, int index) {
	// TODO: Implement this later
	return NULL;
}

int LinkedList_size(LinkedList* list) {
	if (NULL == list) {
		return 0;
	}
	return list->size;
}

int LinkedList_contains(LinkedList* list, void* value) {
	// TODO: Implement this later
	return 1;
}

////////////////////////////////////////////////////////////////////////////////
//
// START LinkedListIterator FUNCTION DEFINITIONS
//
////////////////////////////////////////////////////////////////////////////////

LinkedListIterator*	LinkedListIterator_create(LinkedList* list) {
	LinkedListIterator* newIter = malloc(sizeof(LinkedListIterator));
	if (NULL == newIter) {
		return NULL;
	}
	newIter->list = list;
	newIter->currNode = NULL;
	newIter->nextNode = LinkedList_get(list, 0);
	return NULL;
}

void LinkedListIterator_delete(LinkedListIterator* iter) {
	if (NULL == iter) {
		return;
	}
	free(iter);
}

void* LinkedListIterator_getValue(LinkedListIterator* iter) {
	if (NULL == iter) {
		return NULL;
	}
	return ListNode_getValue(iter->currNode);
}

void LinkedListIterator_getNext(LinkedListIterator* iter) {
	if (NULL == iter) {
		return;
	}
	if (NULL == iter->currNode) {
		iter->nextNode = NULL;
		return;
	}
	iter->currNode = iter->nextNode;
	iter->nextNode = ListNode_getNext(iter->currNode);
	return;
}

int LinkedListIterator_hasNext(LinkedListIterator* iter) {
	if (NULL == iter) {
		return 0;
	}
	return (NULL == iter->nextNode) ? 0 : 1;
}

////////////////////////////////////////////////////////////////////////////////
//
// START RBNode FUNCTION DEFINITIONS
//
////////////////////////////////////////////////////////////////////////////////

RBNode* RBNode_create(RBNodeColor color, RBNode* parent, RBNode* left, RBNode* right, void* key, void* value) {
  RBNode* newNode = malloc(sizeof(RBNode));
  if (NULL == newNode) {
  	return NULL;
  }
  newNode->color = color;
  newNode->parent = parent;
  newNode->children[0] = left;
  newNode->children[1] = right;
  newNode->key = key;
  newNode->value = value;
  return newNode;
}

void RBNode_delete(RBNode* node) {
	if (NULL != node) {
		free(node);
	}
}

RBNodeColor RBNode_getColor(RBNode* node) {
	if (NULL == node) {
		return UNDEFINED;
	}
	return node->color; 
}

RBNode* RBNode_getParent(RBNode* node) {
	if (NULL == node) {
		return NULL;
	}
	return node->parent; 
}

RBNode* RBNode_getLeftChild(RBNode* node) {
	if (NULL == node) {
		return NULL;
	}
	return node->children[0]; 
}

RBNode* RBNode_getRightChild(RBNode* node) {
	if (NULL == node) {
		return NULL;
	}
	return node->children[1]; 
}

void* RBNode_getKey(RBNode* node) {
	if (NULL == node) {
		return NULL;
	}
	return node->key;
}

void* RBNode_getValue(RBNode* node) {
	if (NULL == node) {
		return NULL;
	}
	return node->value;
}

void RBNode_setColor(RBNode* node, RBNodeColor color) {
	if (NULL == node) {
		return;
	}
	node->color = color;
}

void RBNode_setParent(RBNode* node, RBNode* parent) {
	if (NULL == node) {
		return;
	}
	node->parent = parent;
}

void RBNode_setLeftChild(RBNode* node, RBNode* child) {
	if (NULL == node) {
		return;
	}
	node->children[0] = child;
}

void RBNode_setRightChild(RBNode* node, RBNode* child) {
	if (NULL == node) {
		return;
	}
	node->children[1] = child;
}

void RBNode_setKey(RBNode* node, void* key) {
	if (NULL == node) {
		return;
	}
	node->key = key;
}

void RBNode_setValue(RBNode* node, void* value) {
	if (NULL == node) {
		return;
	}
	node->value = value;
}

////////////////////////////////////////////////////////////////////////////////
//
// START RBTree FUNCTION DEFINITIONS
//
////////////////////////////////////////////////////////////////////////////////

RBTree* RBTree_create(Comparator keyCompareFunction) {
	RBTree* newTree = malloc(sizeof(RBTree));
	newTree->root = NULL;
	newTree->keyCompareFunction = keyCompareFunction;
	return newTree;
}

void RBTree_delete(RBTree* tree) {
	if (NULL != tree) {
	// Recurse down the tree, deleting
		RBTree_delete_recursion(tree->root);
		free(tree);
	}
}

void RBTree_delete_recursion(RBNode* node) {
	if (NULL == node) {
		return;
	}
	// Delete the left subtree
    if (NULL != RBNode_getLeftChild(node)) {
    	RBTree_delete_recursion(RBNode_getLeftChild(node));
    }
    // Delete the right subtree
    if (NULL != RBNode_getRightChild(node)) {
    	RBTree_delete_recursion(RBNode_getRightChild(node));
    }
    // Delete the current node
    RBNode_delete(node);
    return;
}

RBNode* RBTree_getRoot(RBTree* tree) {
	if (NULL == tree) {
		return NULL;
	}
	return tree->root; 
}

Comparator RBTree_getKeyCompareFunction(RBTree* tree) {
	if (NULL == tree) {
		return NULL;
	}
	return tree->keyCompareFunction; 
}

void RBTree_setRoot(RBTree* tree, RBNode* root) {
	if (NULL == tree) {
		return;
	}
	tree->root = root; 
}

void RBTree_setKeyCompareFunction(RBTree* tree, Comparator keyCompareFunction) {
	if (NULL == tree) {
		return;
	}
	tree->keyCompareFunction = keyCompareFunction; 
}

int RBTree_compareKeys(RBTree* tree, void* key1, void* key2) {
	if (NULL == tree) {
		return 0;
	}
	return tree->keyCompareFunction(key1, key2);
}

int RBTree_insert(RBTree* tree, void* key, void* value) {
	if (NULL == tree || NULL == key || NULL == value) {
		return 1;
	}
	RBNode* currTreeParent = NULL;
    RBNode* currTreeNode = RBTree_getRoot(tree);
    // Find the insertion point of the key
    while (NULL != currTreeNode) {
    	// New parent is the current node
    	currTreeParent = currTreeNode;
    	// If the key is less than the current parent's key
    	if (RBTree_compareKeys(tree, RBNode_getKey(currTreeNode), key) < 0) {
    		// Current node is the parent's left child
    		currTreeNode = RBNode_getLeftChild(currTreeNode);
    	} else {
    		// Current node is the parent's right child
    		currTreeNode = RBNode_getRightChild(currTreeNode);
    	}
    }
    RBNode* newNode;
    if (NULL == currTreeParent) {
    	// New node is the root.
    	newNode = RBNode_create(RED, NULL, NULL, NULL, key, value);
    	RBTree_setRoot(tree, newNode);
    } else if (RBTree_compareKeys(tree, RBNode_getKey(currTreeParent), key) < 0) {
    	// New node is the left child of the current parent node
    	newNode = RBNode_create(RED, currTreeParent, NULL, NULL, key, value);
    	RBNode_setLeftChild(currTreeParent, newNode);
    } else {
    	// New node is the right child of the current parent node
    	newNode = RBNode_create(RED, currTreeParent, NULL, NULL, key, value);
    	RBNode_setRightChild(currTreeParent, newNode);
    }
    // Restructure tree to keep the tree sorted
    RBTree_repairAfterInsert(tree, newNode);
    return 0;
}

int Par_RBTree_insert(RBTree* tree, void* key, void* value) {
	if (NULL == tree || NULL == key || NULL == value) {
		return 1;
	}
	RBNode* currTreeParent = NULL;
    RBNode* currTreeNode = RBTree_getRoot(tree);
    // Find the insertion point of the key
    while (NULL != currTreeNode) {
    	// New parent is the current node
    	currTreeParent = currTreeNode;
    	// If the key is less than the current parent's key
    	if (RBTree_compareKeys(tree, RBNode_getKey(currTreeNode), key) < 0) {
    		// Current node is the parent's left child
    		currTreeNode = RBNode_getLeftChild(currTreeNode);
    	} else {
    		// Current node is the parent's right child
    		currTreeNode = RBNode_getRightChild(currTreeNode);
    	}
    }
    RBNode* newNode;
    if (NULL == currTreeParent) {
    	// New node is the root.
    	newNode = RBNode_create(RED, NULL, NULL, NULL, key, value);
    	RBTree_setRoot(tree, newNode);
    } else if (RBTree_compareKeys(tree, RBNode_getKey(currTreeParent), key) < 0) {
    	// New node is the left child of the current parent node
    	newNode = RBNode_create(RED, currTreeParent, NULL, NULL, key, value);
    	RBNode_setLeftChild(currTreeParent, newNode);
    } else {
    	// New node is the right child of the current parent node
    	newNode = RBNode_create(RED, currTreeParent, NULL, NULL, key, value);
    	RBNode_setRightChild(currTreeParent, newNode);
    }
    // Restructure tree to keep the tree sorted
    RBTree_repairAfterInsert(tree, newNode);
    return 0;
}

void RBTree_repairAfterInsert(RBTree* tree, RBNode* node) {
	if (NULL == tree || NULL == node) {
		return;
	}
	while (RBNode_getColor(RBNode_getParent(node)) == RED) {
		if (RBNode_getParent(node) == RBNode_getLeftChild(RBNode_getParent(RBNode_getParent(node)))) {
			RBNode* uncle = RBNode_getRightChild(RBNode_getParent(RBNode_getParent(node)));
			if (RBNode_getColor(uncle) == RED) {
				RBNode_setColor(RBNode_getParent(node), BLACK);
				RBNode_setColor(uncle, BLACK);
				RBNode_setColor(RBNode_getParent(RBNode_getParent(node)), RED);
				node = RBNode_getParent(RBNode_getParent(node));
			} else {
				if (node == RBNode_getRightChild(RBNode_getParent(node))) {
					node = RBNode_getParent(node);
					RBTree_rotateLeft(tree, node);
				}
				RBNode_setColor(RBNode_getParent(node), BLACK);
				RBNode_setColor(RBNode_getParent(RBNode_getParent(node)), RED);
				RBTree_rotateRight(tree, RBNode_getParent(RBNode_getParent(node)));
			}
		} else {
			RBNode* uncle = RBNode_getLeftChild(RBNode_getParent(RBNode_getParent(node)));
			if (RBNode_getColor(uncle) == RED) {
				RBNode_setColor(RBNode_getParent(node), BLACK);
				RBNode_setColor(uncle, BLACK);
				RBNode_setColor(RBNode_getParent(RBNode_getParent(node)), RED);
				node = RBNode_getParent(RBNode_getParent(node));
			} else {
				if (node == RBNode_getLeftChild(RBNode_getParent(node))) {
					node = RBNode_getParent(node);
					RBTree_rotateRight(tree, node);
				}
				RBNode_setColor(RBNode_getParent(node), BLACK);
				RBNode_setColor(RBNode_getParent(RBNode_getParent(node)), RED);
				RBTree_rotateLeft(tree, RBNode_getParent(RBNode_getParent(node)));
			}
		}
	}
	RBNode_setColor(RBTree_getRoot(tree), BLACK);
}

void* RBTree_search(RBTree* tree, void* key) {
	if (NULL == tree || NULL == key) {
		return NULL;
	}
	RBNode* currNode = RBTree_getRoot(tree);
	while (NULL != currNode) {
		switch(RBTree_compareKeys(tree, RBNode_getKey(currNode), key)) {
			case 0:
				return RBNode_getValue(currNode);
			case -1:
				currNode = RBNode_getLeftChild(currNode);
				break;
			case 1:
				currNode = RBNode_getRightChild(currNode);
				break;
			default:
				return NULL;  // NEVER SHOULD HAVE COME HERE!
		}
	}
	return NULL;
}

int RBTree_remove(RBTree* tree, void* key) {
	// TODO: Implement this if you want to make it a nice library
	return 1;
}

void RBTree_repairAfterRemove(RBTree* tree) {
	// TODO: Implement this if you want to make this a nice library
	;
}

void RBTree_rotateLeft(RBTree* tree, RBNode* node) {
	if (NULL == tree || NULL == node) {
		return;
	}
	// Get the new subtree root
	RBNode* newParent = RBNode_getRightChild(node);
	// Move the left child of the new subtree root as the right child of the old subtree root
	RBNode_setRightChild(node, RBNode_getLeftChild(newParent));
	RBNode_setParent(RBNode_getLeftChild(newParent), node);
	// Set the subtree root to the new subtree root
	RBNode_setParent(newParent, RBNode_getParent(node));
	if (NULL == RBNode_getParent(node)) {
		// If the old subtree was the root of the tree, set the new root as such
		RBTree_setRoot(tree, newParent);
	} else if (node == RBNode_getLeftChild(RBNode_getParent(node))) {
		// If the old subtree root was the left child of its parent, set the new subtree root as such
		RBNode_setLeftChild(RBNode_getParent(node), newParent);
	} else {
		// If the old subtree root was the right child of its parent, set the new subtree root as such
		RBNode_setRightChild(RBNode_getParent(node), newParent);
	}
	// Set the old subtree root as the left child of the new subtree root
	RBNode_setLeftChild(newParent, node);
	RBNode_setParent(node, newParent);
}

void RBTree_rotateRight(RBTree* tree, RBNode* node) {
	if (NULL == tree || NULL == node) {
		return;
	}
	// Get the new subtree root
	RBNode* newParent = RBNode_getLeftChild(node);
	// Move the right child of the new subtree root as the left child of the old subtree root
	RBNode_setLeftChild(node, RBNode_getRightChild(newParent));
	RBNode_setParent(RBNode_getRightChild(newParent), node);
	// Set the subtree root to the new subtree root
	RBNode_setParent(newParent, RBNode_getParent(node));
	if (NULL == RBNode_getParent(node)) {
		// If the old subtree was the root of the tree, set the new root as such
		RBTree_setRoot(tree, newParent);
	} else if (node == RBNode_getRightChild(RBNode_getParent(node))) {
		// If the old subtree root was the right child of its parent, set the new subtree root as such
		RBNode_setRightChild(RBNode_getParent(node), newParent);
	} else {
		// If the old subtree root was the left child of its parent, set the new subtree root as such
		RBNode_setLeftChild(RBNode_getParent(node), newParent);
	}
	// Set the old subtree root as the right child of the new subtree root
	RBNode_setRightChild(newParent, node);
	RBNode_setParent(node, newParent);
}

////////////////////////////////////////////////////////////////////////////////
//
// START RBTreeIterator FUNCTION DEFINITIONS
//
////////////////////////////////////////////////////////////////////////////////

RBTreeIterator* RBTreeIterator_create(RBTree* tree) {
	RBTreeIterator* newIter = malloc(sizeof(RBTreeIterator));
	if (newIter == NULL) {
		return NULL;
	}
	newIter->tree = tree;
	newIter->currNode = NULL;
	newIter->nextNode = RBTree_getRoot(tree);
	while (NULL != RBNode_getLeftChild(newIter->nextNode)) {
		newIter->nextNode = RBNode_getLeftChild(newIter->nextNode);
	}
	return newIter;
}

void* RBTreeIterator_getKey(RBTreeIterator* iter) {
	if (NULL == iter) {
		return NULL;
	}
	return RBNode_getKey(iter->currNode);
}

void* RBTreeIterator_getValue(RBTreeIterator* iter) {
	if (NULL == iter) {
		return NULL;
	}
	return RBNode_getValue(iter->currNode);
}

int RBTreeIterator_hasNext(RBTreeIterator* iter) {
	if (NULL == iter) {
		return 0;
	}
	return (NULL == iter->nextNode) ? 0 : 1;
}

void RBTreeIterator_getNext(RBTreeIterator* iter) {
	iter->currNode = iter->nextNode;
	if (NULL == iter->currNode) {
		iter->nextNode = NULL;
		return;
	}

	if (NULL != RBNode_getRightChild(iter->currNode)) {
		iter->nextNode = RBNode_getRightChild(iter->currNode);
		while (NULL != RBNode_getLeftChild(iter->nextNode)) {
			iter->nextNode = RBNode_getLeftChild(iter->nextNode);
		}
	} else {
		RBNode* n = iter->currNode;
		RBNode* p = RBNode_getParent(n);
		while (NULL != p && n == RBNode_getRightChild(p)) {
			n = p;
			p = RBNode_getParent(p);
		}
		iter->nextNode = p;
	}
}

void RBTreeIterator_delete(RBTreeIterator* iter) {
	if (NULL != iter) {
		free(iter);
	}
}

int intCompare(void* int1, void* int2) {
	if (*(int*)int1 == *(int*)int2) {
		return 0;
	} else if (*(int*)int1 < *(int*)int2) {
		return 1;
	} else {
		return -1;
	}
}
