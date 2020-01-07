#include <stdio.h>
#include "Object.h"
#include "Quadtree.h"

int main() {
	Quadtree quadtree(0.0f, 0.0f, 80.0f, 25.0f, 0, 3);
	Object *thing = new Object(5, 5, 1, 1);
	printf("Hi %f\n", thing->x);
}
