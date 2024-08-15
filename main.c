#include <stdio.h>
#include <string.h>

#include "twee.h"

struct record
{
	int   id;
	char  name[32];
	short score;
} names[] = {
	{1, "hello", 3},
	{2, "sailor", 6},
	{3, "nothing", 7},
	{4, "death", 8},
	{5, "life", 10},
	{6, "taurus", 3},
	{7, "iris", 6},
	{8, "venus", 7},
	{9, "saturn", 8},
	{10, "solar", 10},
	{11, "proxima", 10}
};

#define ARRAY_SIZE(a) (int)(sizeof(a) / sizeof(a[0]))

void twee_dump(twee_node_t *a, int d);

void frch(void *a)
{
	struct record *r = (struct record *)a;
	printf("%d : %s : %d\n", r->id, r->name, r->score);
}

int cmp(void *a, void *b)
{
	char *x = (char *)((char *)a + sizeof(int));
	char *y = (char *)((char *)b + sizeof(int));

	return strncmp(x, y, 32);
}

int main(void)
{
	twee_t a = {
		.root = 0,
		.stride = sizeof(struct record),
		.cmp = cmp,

		.allocations = 0,
		.capacity = 0,
		.size = 0
	};

	for(int i = 0; i < ARRAY_SIZE(names); i++)
	{
		twee_insert(&a, names + i);
	}

	printf("Tree dump before deletion: \n\n");
	twee_dump(a.root, 0);
	puts("\n\n");

	struct record r = {
		.id = 0,
		.name = "sailor",
		.score = 0
	};
	twee_delete(&a, &r);

	printf("Tree dump after deletion: \n\n");
	twee_dump(a.root, 0);
	puts("\n");

	printf("Printing in the sorted order of search: \n");
	twee_foreach(&a, frch);
	puts("\n");

	printf("Printing in the order of input: \n");
	twee_foreach_serial(&a, frch);
	puts("\n");
}
