#ifndef REDBLACK_H
#define REDBLACK_H
#include <stdlib.h>

enum colortype{black, red};

struct node{
	int alloc;
	void* adr;
	int size;
	struct node* left;
	struct node* right;
	struct node* parent;
	enum colortype color;
	int leaf;
};

//struct tree{
//	struct node* root;
//	int total_height;
//};

struct node* uncle(struct node* nephew);

struct node* grandparent(struct node* grandchild);

struct node* full_insert(struct node* root, struct node* insert_node);

void insert(struct node* root, struct node* insert_node);

void balance(struct node* root, struct node* insert_node);

void left_rotate(struct node* rotate);

void right_rotate(struct node* rotate);

struct node* return_inorder_successor(struct node* pred);

struct node* return_inorder_predecessor(struct node* successor);

void replace_value(struct node* replaced, struct node* replacee);

void remove_node(struct node* removed);

void balance_remove1(struct node* imbalance);

void balance_remove2(struct node* imbalance);

void print(struct node* currNode);


//reordering functions still needed
#endif
