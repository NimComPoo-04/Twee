#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "twee.h"

/*
 * Prototype implementation of BST data structure which maintains 2 different
 * ordering schemes of the same data elements, without significantly affecting the time and
 * space complexity.
 *
 * The orders preserved in this prototype are:
 *
 * i> The order of insertion
 * ii> The sorted order according the cmp function
 *
 * In this case we implement balanced binary search tree (AVL Tree) to be more specific.
 *
 * The main way this is different from a regular tree implementation is that we organize the
 * allocated nodes into an dynamic array. The meta data required to maintain this array is constant
 * over increasing input sizes. Since we have control over the allocations we can simply order
 * this allocations following predefined requirements.
 *
 * One important case where this may be important is in displaying video game players registry. it may be
 * useful to order the players in the order that they joined the game. But its also essential that the
 * 
 */

static inline int tree_get_height(twee_node_t *root)
{
	if(!root) return 0;
	return root->height;
}

static void tree_rotate_left(twee_node_t **root);
static void tree_rotate_right(twee_node_t **root)
{
	twee_node_t *old_head = root[0];
	twee_node_t *new_head = root[0]->left;

	if(!new_head->left && new_head->right)
	{
		tree_rotate_left(&(root[0]->left));
		new_head = root[0]->left;
	}

	root[0] = new_head;
	old_head->left = new_head->right;
	new_head->right = old_head;

	{
		const int lh = tree_get_height(old_head->left);
		const int rh = tree_get_height(old_head->right);
		old_head->height = (lh > rh ? lh : rh) + 1;
	}

	{
		const int lh = tree_get_height(new_head->left);
		const int rh = tree_get_height(new_head->right);
		new_head->height = (lh > rh ? lh : rh) + 1;
	}
}

static void tree_rotate_left(twee_node_t **root)
{
	twee_node_t *old_head = root[0];
	twee_node_t *new_head = root[0]->right;

	if(!new_head->right && new_head->left)
	{
		tree_rotate_right(&(root[0]->right));
		new_head = root[0]->right;
	}

	root[0] = new_head;
	old_head->right = new_head->left;
	new_head->left = old_head;

	{
		const int lh = tree_get_height(old_head->left);
		const int rh = tree_get_height(old_head->right);
		old_head->height = (lh > rh ? lh : rh) + 1;
	}

	{
		const int lh = tree_get_height(new_head->left);
		const int rh = tree_get_height(new_head->right);
		new_head->height = (lh > rh ? lh : rh) + 1;
	}
}

static void tree_rebalance(twee_node_t **root)
{
	int diff = tree_get_height(root[0]->left) - tree_get_height(root[0]->right);

	if(diff < -1)     tree_rotate_left(root);
	else if(diff > 1) tree_rotate_right(root);
}

static int twee_node_insert(twee_node_t **root, twee_t *s, void *value)
{
	int ret = 0;

	if(*root == NULL)
	{
		// calloc(sizeof(twee_node_t) + s->stride, 1);
		*root = twee_allocate(s);

		root[0]->left = NULL;
		root[0]->right = NULL;

		memcpy(root[0]->value, value, s->stride);

		root[0]->height = 1;

		ret = 1;
	}
	else if(s->cmp(root[0]->value, value) > 0)
	{
		ret = twee_node_insert(&(root[0]->left), s, value);
	}
	else if(s->cmp(root[0]->value, value) <= 0)
	{
		ret = twee_node_insert(&(root[0]->right), s, value);
	}

	const int lh = tree_get_height(root[0]->left);
	const int rh = tree_get_height(root[0]->right);
	root[0]->height = (lh > rh ? lh : rh) + 1;

	tree_rebalance(root);

	return ret;
}

static int twee_node_delete(twee_node_t **root, twee_t *s, void *value)
{
	if(!root[0]) return 0;

	int ret = 0;

	if(s->cmp(root[0]->value, value) == 0)
	{
		if(root[0]->left)
		{
			if(root[0]->right == NULL)
			{
				twee_node_t *l = root[0];
				root[0] = root[0]->left;
				twee_deallocate(s, l);
			}
			else
			{
				twee_node_t *k = root[0]->left;
				twee_node_t *pk = root[0];

				while(k->right != NULL)
				{
					pk = k;
					k = k->right;
				}

				if(pk == root[0])
				{
					pk->left = k->right;
				}
				else
				{
					pk->right = k->left;
				}

				k->left = root[0]->left;
				k->right = root[0]->right;

				twee_deallocate(s, root[0]);
				root[0] = k;
			}
		}
		else
		{
			twee_node_t *l = root[0];
			root[0] = root[0]->right;
			twee_deallocate(s, l);
		}

		ret = 1;
	}
	else if(s->cmp(root[0]->value, value) > 0)
	{
		ret = twee_node_delete(&(root[0]->left), s, value);
	}
	else if(s->cmp(root[0]->value, value) <= 0)
	{
		ret = twee_node_delete(&(root[0]->right), s, value);
	}

	if(ret)
	{
		const int lh = tree_get_height(root[0]->left);
		const int rh = tree_get_height(root[0]->right);
		root[0]->height = (lh > rh ? lh : rh) + 1;

		tree_rebalance(root);
	}

	return ret;
}

void twee_insert(twee_t *a, void *value)
{
	twee_node_insert(&(a->root), a, value);
}

void twee_delete(twee_t *a, void *value)
{
	twee_node_delete(&(a->root), a, value);
}

void twee_node_foreach(twee_node_t *a, void(*f)(void *v))
{
	if(!a) return;

	twee_node_foreach(a->left, f);
	f(a->value);
	twee_node_foreach(a->right, f);
}

void twee_foreach(twee_t *a, void(*f)(void *v))
{
	twee_node_foreach(a->root, f);
}

void twee_dump(twee_node_t *a, int d)
{
	if(!a) return;

	twee_dump(a->left, d + 1);
	printf("%*c%s:%d\n", 8 * d, ' ', a->value + sizeof(int), a->height);
	twee_dump(a->right, d + 1);
}

/*
 * TODO: The allocations here will be wasteful, since it will not touch any deleted elements. We should
 * implement an bucket array instead.
 * */
void *twee_allocate(twee_t *a)
{
	if(a->size >= a->capacity)
	{
		if(a->capacity == 0)
			a->capacity =  a->stride + sizeof(twee_node_t);
		else
			a->capacity = 2 * a->capacity;

		a->allocations = realloc(a->allocations, a->capacity);
	}

	void *v = (a->allocations + a->size);
	a->size += a->stride + sizeof(twee_node_t);

	return v;
}

void twee_deallocate(twee_t *a, twee_node_t *value)
{
	(void)a;			// we don't need this, just here for symetry
	value->height = -1;

	value->left = NULL;
	value->right = NULL;
}

void twee_get(twee_t *a, int indx, void *out)
{
	if(indx >= a->size / a->stride || indx < 0)
		return;
	const int size = a->stride + sizeof(twee_node_t);
	memcpy(out, a->allocations + indx * size, size);
}

void twee_set(twee_t *a, int indx, void *out, void *in)
{
	if(indx >= a->size / a->stride || indx < 0)
		return;
	const int size = a->stride + sizeof(twee_node_t);
	memcpy(out, a->allocations + indx * size, size);
	memcpy(a->allocations + indx * size, in, size);
}

void twee_foreach_serial(twee_t *a, void(*f)(void *v))
{
	for(int i = 0; i < a->size; i += a->stride + sizeof(twee_node_t))
	{
		if(((twee_node_t *)(a->allocations + i))->height == -1)
			continue;
		f(((twee_node_t *)(a->allocations + i))->value);
	}
}
