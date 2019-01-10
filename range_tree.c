///////////////////////////////////////////////////////////////////////////////
// File:             (range_tree.c)
// Semester:         (cs537) Fall 2018
//
// Author:           (Ryan McBride rmcbride2@wisc.edu, Nate Lenar lenar@wisc.edu)
// CS Login:         (rmcbride, lenar)
// Lecturer's Name:  (Bart Miller)
//
//////////////////////////// 80 columns wide ////////////////////////////////////

//red black tree
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "range_tree.h"

struct node* uncle(struct node* nephew){
	struct node* parent = nephew->parent;
	struct node* grandparent = parent->parent;
	if(grandparent == NULL){
		return NULL;
	}
	if(parent == grandparent->left){
		return grandparent->right;
	}
	else{
		return grandparent->left;
	}
}
struct node* grandparent(struct node* grandchild){
	if(grandchild->parent == NULL){
		return NULL;
	}

	struct node* grandparent = grandchild->parent->parent;
	return grandparent;
}
struct node* full_insert(struct node* root, struct node* insert_node){
	insert(root, insert_node);
	balance(root, insert_node);
	root = insert_node;
	while(root->parent != NULL){
		root = root->parent;
	}
	return root;
}

void insert(struct node* root, struct node* insert_node){
	if(root == NULL){
		insert_node->parent = NULL;
		insert_node->left = malloc(sizeof(struct node));
		insert_node->left->leaf = 1;
		insert_node->left->color = black;
		insert_node->right = malloc(sizeof(struct node));
		insert_node->right->leaf = 1;
		insert_node->right->color = black;
		insert_node->color = red;
		return;
	}
	else{
		if(insert_node->adr < root->adr){
			if(root->left->leaf == 0){
				insert(root->left, insert_node);
				return;
			}
			else{
				free(root->left);
				root->left = insert_node;
			}
		}
		else{
			if(root->right->leaf == 0){
				insert(root->right, insert_node);
				return;
			}
			else{
				free(root->right);
				root->right = insert_node;
			}	
		}
	}
	insert_node->parent = root;
	insert_node->left = malloc(sizeof(struct node));
	insert_node->left->leaf = 1;
	insert_node->left->color = black;
	insert_node->right = malloc(sizeof(struct node));
	insert_node->right->leaf = 1;
	insert_node->right->color = black;
	insert_node->color = red;

	return;
}



void balance(struct node* root, struct node* insert_node){
		if(insert_node->parent == NULL){
		       insert_node->color = black;
		}
		else if(insert_node->parent->color == black){
			return;
		}
		else if(uncle(insert_node)->color == red){
			insert_node->parent->color = black;
			uncle(insert_node)->color = black;
			grandparent(insert_node)->color = red;
			balance(root,insert_node->parent->parent);
		}
		else{
			if(insert_node == (grandparent(insert_node)->left->right)){
				left_rotate(insert_node->parent);
				insert_node = insert_node->left;
			}
			else if(insert_node == (grandparent(insert_node)->right->left)){
				right_rotate(insert_node->parent);
				insert_node = insert_node->right;
			}
			if(insert_node->parent != NULL){
				struct node* parent = insert_node->parent;
				struct node* grandpa = grandparent(insert_node);
			
				if(insert_node == insert_node->parent->left){
					right_rotate(grandpa);
				}
				else{
					left_rotate(grandpa);
				}
				parent->color = black;
				grandpa->color = red;
			}
		}
}



void left_rotate(struct node* rotate){
	struct node* child = rotate->right;
	struct node* parent = rotate->parent;
	rotate->right  = child->left;
	child->left = rotate;
	rotate->parent = child;
	if(rotate->right != NULL){

		rotate->right->parent = rotate;
	}
	if(parent != NULL){
		if(rotate == parent->left){
			parent->left = child;
		}
		else if(rotate == parent->right){

			parent->right = child;
		}
	}
	child->parent = parent;
}	

void right_rotate(struct node* rotate){
	struct node* child = rotate->left;
	struct node* parent = rotate->parent;
	rotate->left  = child->right;
	child->right = rotate;
	rotate->parent = child;
	if(rotate->left != NULL){
		rotate->left->parent = rotate;
	}
	if(parent != NULL){
		if(rotate == parent->left){
			parent->left = child;
		}
		else if(rotate == parent->right){
			parent->right = child;
		}
	}
	child->parent = parent;
}

void replace_value(struct node* replaced, struct node* replacee){
	replacee->color = replaced->color;
	replaced = replacee;
	if(replaced == NULL){
		printf("Error: replaced value is NULL\n");
	}
}

void switch_node(struct node* switched, struct node* switchee){
	switchee->parent = switched->parent;
	if(switched == switched->parent->left){
		switched->parent->left = switched;
	}
	else{
		switched->parent->right = switchee;
	}
	return;
}

struct node* return_inorder_successor(struct node* pred){
	struct node* successor;
	if(pred->left->leaf == 0 && pred->right->leaf == 0){
		successor = return_inorder_successor(pred->left);
	}
	else if(pred->left->leaf == 0){
		successor = pred->left;
	}
	else if(pred->right->leaf == 0){
		successor = pred->right;
	}
	else{
		return pred;
	}
	return successor;
}

struct node* return_inorder_predecessor(struct node* successor){
	struct node* return_node = successor;
	
	if(return_node->left->leaf != 1){
		return_node = return_node->left;
		while(return_node->right->leaf != 1){
			return_node = return_node->right;
		}	
	}

	return return_node;
}

void remove_node(struct node* removed){
	struct node* replacee;
	if((removed->left->leaf == 0) && (removed->right->leaf ==0)){
		replacee = return_inorder_successor(removed->right);
		replace_value(removed, replacee);
	}
	else{
		replacee = removed;
	}
	struct node* child;
	if(replacee->left->leaf == 0){
		child = replacee->left;
	}
	else{
		child = replacee->right;
	}
	if(replacee->color == red){
		switch_node(replacee, child);
	}
	else if((replacee->color == black) && (child->color = red)){
		child->color = black;
		switch_node(replacee, child);
	}
	else{
		struct node* uncle1 = uncle(replacee);
		child = replacee->right;
		switch_node(replacee, child);
		if(child->parent != NULL){
			if(uncle1->color == red){
				child->parent->color = red;
				uncle1->color = black;
				if(child == child->parent->left){
					left_rotate(child->parent);
				}
				else{
					right_rotate(child->parent);
				}
			}
			if((child->parent->color == black) && 
				(uncle1->color == black) &&
				(uncle1->left->color == black) &&
				(uncle1->right->color == black)){
			uncle1->color = red;
			}
		}
	}
	free(removed);
	return;
}

void balance_remove1(struct node* imbalance){
	if(imbalance->parent != NULL){
		balance_remove2(imbalance);
	}
}

void balance_remove2(struct node* imbalance){
	struct node* brother;
	int lr = 0;
	if(imbalance == imbalance->parent->left){
		brother = imbalance->parent->right;
		lr = 1; 
	}
	else{
		brother = imbalance->parent->left;
	}
	if(brother->color == red){
		imbalance->parent->color = red;
		brother->color = black;
		if(lr == 1){
			left_rotate(imbalance->parent);
		}
		else{
			right_rotate(imbalance->parent);
		}
	}
	//balance 3
	if(imbalance == imbalance->parent->left){
		brother = imbalance->parent->right;
		lr = 1;
	}
	else{
		brother = imbalance->parent->left;
	}
	if((imbalance->parent->color == black) && 
	  (brother->color == black) &&
	  (brother->left->color == black) &&
	  (brother->right->color == black)){

		brother->color = red;
		balance_remove1(imbalance->parent);
	}
	//balance 4
	if(imbalance == imbalance->parent->left){
		brother = imbalance->parent->right;
		lr = 1;
	}
	else{
		brother = imbalance->parent->left;
	}

	if((imbalance->parent->color == red) && 
	   (brother->color == black) &&
	   (brother->left->color == black) &&
	   (brother->right->color == black)){

			brother->color = red;
			imbalance->parent->color = black;
	}

	//balance 5
	if(imbalance == imbalance->parent->left){
		brother = imbalance->parent->right;
		lr = 1;
	}
	else{
		brother = imbalance->parent->left;
	}

	if(brother->color == black){
		if((lr == 1) && 
	  	   (brother->right->color == black) &&
	  	   (brother->left->color == red)){
			brother->color = red;
			brother->left->color = black;
			right_rotate(brother);
		}
		else if((lr == 0) &&
			(brother->left->color == black) &&
		 	(brother->right->color == red)){
			
			brother->color = red;
			brother->right->color = black;
		left_rotate(brother);
		}
		if(imbalance == imbalance->parent->left){
			brother = imbalance->parent->right;
			lr = 1;
		}
		else{
			brother = imbalance->parent->left;
		}

		brother->color = imbalance->parent->color;
		imbalance->parent->color = black;
		if(lr == 1){
			brother->right->color = black;
			left_rotate(imbalance->parent);
		}
		else{
			brother->left->color = black;
			right_rotate(imbalance->parent);

		}
	}		
}

int numberb;
int numberr;

void print(struct node* currNode){
	struct node* diffnode;
	struct node* rightnode;
	if(currNode->left->leaf == 0){
		diffnode = currNode->left;
		print(diffnode);
	}
	if(currNode->right->leaf == 0){
		rightnode = currNode->right;
		print(rightnode);
	}
	if(currNode->color == black){
		numberb++;
	}
	else{
		numberr++;
	}
	printf("number of blacks: %d\n\n\n", numberb);
	printf("number of reds: %d\n\n\n", numberr);
	printf("this is node's adr: %p \n this is node's color: %d\n\n", currNode->adr, currNode->color);
	return;

}

