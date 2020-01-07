#ifndef __OBJECT_H__
#define __OBJECT_H__

class Object {
public:
	Object(float _x, float _y, float _width, float _height);

	float x, y, width, height;
};

Object::Object(float _x, float _y, float _width, float _height) :
	x(_x), y(_y), width(_width), height(_height) {
		//
}

#endif
