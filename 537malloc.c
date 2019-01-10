///////////////////////////////////////////////////////////////////////////////
// File:             (537malloc.c)
// Semester:         (cs537) Fall 2018
//
// Author:           (Ryan McBride rmcbride2@wisc.edu, Nate Lenar lenar@wisc.edu)
// CS Login:         (rmcbride, lenar)
// Lecturer's Name:  (Bart Miller)
//
//////////////////////////// 80 columns wide ////////////////////////////////////


#include <stdlib.h>
#include <stdio.h>
#include "range_tree.h"
#include "537malloc.h"

static struct node* root = NULL;

void* malloc537(size_t size){
	if(size == 0){
		printf("caution: malloc'd size of zero\n");
	}
	void* addr = malloc(size);
	struct node* malloc_node = malloc(sizeof(*malloc_node));
	
	malloc_node->size = size;
	malloc_node->adr = addr;
	malloc_node->alloc = 1;
	
	root = full_insert(root, malloc_node);

	if(malloc_node->parent  ==  NULL){
		//addr = malloc(size);
		return addr;
	}
//	addr = malloc(size);
	if(malloc_node == malloc_node->parent->left){//might need to add next if into this boolean
		if((malloc_node->adr + malloc_node->size) > malloc_node->parent->adr){
			struct node* xnode;
			struct node* p;
			p = malloc_node->parent;
			xnode = p;
			//find highest relative in tree that malloc_node's range overwrites
			while( (malloc_node->adr + malloc_node->size) > p->adr){
				if(p->parent == NULL){
					break;
				}
				if(p == p->parent->left){
					xnode = p;
					p = p->parent;
		
				}
				else if (p == p->parent->right){
					xnode = p;
					break;
				}
			}
			//delete nodes that malloc_node overwrites if they're freed
			while( (xnode != malloc_node) && ((malloc_node->adr + malloc_node->size) > xnode->adr) ){
				if(xnode->alloc == 0){
					struct node* tempNode = xnode;
					xnode = return_inorder_successor(xnode);//sets new xnode
					remove_node(tempNode);
					//free the memory for tempnode
				}
				else{
					printf("error: xnode is alloced\n");
					exit(-1);			
				}	
			}
		} //end the larger if here.
	}
	if( ((malloc_node->adr + malloc_node->size) < malloc_node->parent->adr) || (malloc_node == malloc_node->parent->right)){//why not just if
		struct node* xnode;

		xnode = return_inorder_successor(malloc_node);
		while( (xnode != malloc_node) && ((malloc_node->adr + malloc_node->size) > xnode->adr) ){
			if(xnode->alloc == 0){
				remove_node(xnode);//delete xnode;
				//free xnode then set new one
				xnode = return_inorder_successor(malloc_node);//set new xnode
			}
			else{
				printf("error xnode is alloced\n");
				exit(1);
			}
		}
	}

	//check if malloc_node's memory uses freed memory in a different node in tree
	struct node* prev_node = return_inorder_predecessor(malloc_node);

	if( (prev_node != malloc_node) && ((prev_node->adr + prev_node->size) > malloc_node->adr)){
		if(prev_node->alloc == 0){
			prev_node->size = malloc_node->adr - prev_node->adr;
		}
		else{
			printf("ERROR: prev_node is alloced\n");
			exit(1);
		}
	}	
	return addr;
}

void free537(void* ptr){
	if(ptr == NULL){
		printf("ERROR: tried to free a null pointer\n");
		exit(-1);
	}
	struct node* xnode = search_tree(root,ptr);// = search(ptr);
	if(xnode == NULL){
		printf("ERROR: this memory address doesn't exist\n");
		exit(-1);
	}
	else if(ptr > xnode->adr){
		printf("ERROR: tried to free inside malloc'd block\n");
		exit(-1);
	}
	else if(xnode->alloc == 0){
		printf("ERROR: memory already freed\n");
		exit(-1);
	}
	else{
		xnode->alloc = 0;
		free(ptr);
	}
}

void* realloc537(void *ptr, size_t size){
	void* addr = ptr;
	if(ptr == NULL){
		addr = malloc537(size);
	}
	else if(size == 0 && ptr != NULL){
		printf("Warning: size is zero\n");
		free537(ptr);
		return ptr;	
		//addr = ptr;//what to return
	}
	else{
		struct node* xnode;
		xnode = search_tree(root,ptr);//search for ptr in tree
		if(xnode != NULL ){
			if(ptr == xnode->adr){
				addr = realloc(ptr,size);
				xnode->adr = addr;
				xnode->size = size;
			}
		}

	}
	return addr;
}

void memcheck537(void* ptr, size_t size){
	struct node* node = search_tree(root,ptr);//= search for ptr in tree
	if(node == NULL){
		printf("ERROR: memory doesn't exist\n");
		exit(-1);
	}
	if(ptr+size > (node->adr + node->size)){
		printf("ERROR: range is out of bounds");
		exit(-1);
	}	
}

struct node* search_tree(struct node* root,void* ptr){
	struct node* currNode = root;
	while( !((ptr >= currNode->adr) && (ptr < (currNode->adr + currNode->size))) ){
		while(ptr >= (currNode->adr + currNode->size)){
			if(currNode->right->leaf == 0){
				currNode = currNode->right;
			}
			else if( currNode->left->leaf == 1){
				return NULL;
			}
		}
		while(ptr < currNode->adr){
			if(currNode->left->leaf == 0){
				currNode = currNode->left;
			}
			else if(currNode->right->leaf == 1){
				return NULL;
			}
		}
	}
	return currNode;
}



