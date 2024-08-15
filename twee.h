#ifndef _TWEE_H_
#define _TWEE_H_

/*
 * TODO: we should use offset integers instead of pointers that would help implement a
 * transform buffer to better utilize space.
 */
typedef struct twee_node_t
{
	int height;			// if height is -1 node is invalid

	struct twee_node_t *left;
	struct twee_node_t *right;

	char value[];
} twee_node_t;

typedef struct
{
	twee_node_t *root;
	int stride;

	int(*cmp)(void *, void *);

	int capacity;
	int size;

	char *allocations;
} twee_t;

void twee_insert(twee_t *a, void *value);
void twee_delete(twee_t *a, void *value);
void twee_foreach(twee_t *a, void(*f)(void *v));

void twee_get(twee_t *a, int indx, void *out);
void twee_set(twee_t *a, int indx, void *out, void *in);

void twee_foreach_serial(twee_t *a, void(*f)(void *v));

void *twee_allocate(twee_t *a);
void twee_deallocate(twee_t *a, twee_node_t *value);

#endif
