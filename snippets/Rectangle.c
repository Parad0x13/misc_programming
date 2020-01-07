// Simple software that crudly checks if two rectangles intersect
#include <stdio.h>

typedef struct vertex2f {
	float x, y;
}vertex2f;

vertex2f vertex2fMake(float x , float y) {
	vertex2f retVal;
	retVal.x = x;
	retVal.y = y;
	return retVal;
}

typedef struct size2f {
	float width, height;
}size2f;

size2f size2fMake(float width, float height) {
	size2f retVal;
	retVal.width = width;
	retVal.height = height;
	return retVal;
};

typedef struct rect2f {
	vertex2f loc;
	size2f size;
}rect2f;

rect2f rect2fMake(vertex2f loc, size2f size) {
	rect2f retVal;
	retVal.loc = loc;
	retVal.size = size;
	return retVal;
}

int rect2fIntersect(rect2f rectOne, rect2f rectTwo);

int main() {
	rect2f first, second;
	first = rect2fMake(vertex2fMake(5, 5), size2fMake(2, 2));
	second = rect2fMake(vertex2fMake(3.1, 5), size2fMake(2, 2));
	printf("Intersects: %i\n", rect2fIntersect(first, second));
	return 0;
}

int rect2fIntersect(rect2f rectOne, rect2f rectTwo) {
	float left = rectOne.loc.x;
	float right = rectOne.loc.x+rectOne.size.width;
	float top = rectOne.loc.y;
	float bottom = rectOne.loc.y+rectOne.size.height;
	float left2 = rectTwo.loc.x;
	float right2 = rectTwo.loc.x+rectTwo.size.width;
	float top2 = rectTwo.loc.y;
	float bottom2 = rectTwo.loc.y+rectTwo.size.height;
	if(right<left2||left>right2||bottom>top2||top<bottom2)return 0;
	return 1;
}
