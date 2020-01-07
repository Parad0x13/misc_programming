#ifndef __CONSTANTS__
#define __CONSTANTS__

#include <windows.h>

#define GRAVITY -9.81
#define TERMINAL_VELOCITY 20.0

#define NO_DIRECTION	100
#define UP				101
#define DOWN			102
#define LEFT			103
#define RIGHT			104
#define NO_DIM			1000
#define X_DIM			1001
#define Y_DIM			1002

extern HANDLE hIn;
extern HANDLE hOut;
extern bool GameActive;

typedef char *string;

typedef struct _vertexi {
	int x, y;
}vertexi;

typedef struct _vertexf {
	float x, y;
}vertexf;

vertexf vertexfMake(float x, float y);
vertexi vertexiMake(int x, int y);
vertexi vertexftoi(vertexf vertex);
bool cmpvertexi(vertexi first, vertexi second);
bool cmpvertexf(vertexf first, vertexf second);

typedef struct _velocity {
	float x, y;
}velocity;

extern vertexi cameraFocus;

typedef struct _m_vector {
	float x, y;
}m_vector;

m_vector m_vectorMake(float x, float y);
m_vector ZeroVector();

#endif
