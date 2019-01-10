#ifndef MALLOC_H
#define MALLOC_H

void* malloc537(size_t size);
void free537(void* ptr);
void* realloc537(void* ptr, size_t size);
void memcheck537(void* ptr, size_t size);
struct node* search_tree(struct node* root, void* ptr);


#endif
