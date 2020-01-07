/*	Bryan Tremblay's freeglut boilerplate code V1.0
	Need to implement:
		- Screen resize properly
		- Ability to use regular OpenGL instead of Easel	*/
#include <GL/glut.h>
#include <assert.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

void glInit();
void OnIdle();
void keyboard(unsigned char key, int x, int y);
void display(void);

int screenWidth, screenHeight;

class Color {
public:
	Color() {
		r = rand()%255;
		g = rand()%255;
		b = rand()%255;
	}
	Color(char r, char g, char b) : r(r), g(g), b(b) {}
	char r, g, b;
};

class Easel {
	int width, height;
	unsigned char *data;
public:
	Easel(int width, int height) : width(width), height(height) {
		data = new unsigned char[width*height*3];
	}
	void set(int x, int y, Color *color, int border) {
		for(int sizeY=-border;sizeY<=border;sizeY++) {
			for(int sizeX=-border;sizeX<=border;sizeX++) {
				int finalX = (sizeX+x);
				int finalY = (sizeY+y);
				set(finalX, finalY, color);
			}
		}
	}
	void set(int x, int y, Color *color) {
		if(existsInEasel(x, y)) {
			int actualY = (height-1)-y;
			data[(actualY*width+x)*3 + 0] = color->r;
			data[(actualY*width+x)*3 + 1] = color->g;
			data[(actualY*width+x)*3 + 2] = color->b;
		}
	}
	bool existsInEasel(int x, int y) {
		if(x < 0 || x > width || y < 0 || y > height)return false;
		return true;
	}
	void clear() {memset(data, 0x00, width*height*3);}
	void render() {glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, data);}
}*easel;

int main(int argc, char **argv) {
	srand(time(0));

	glutInit(&argc, argv);
	glutIdleFunc(&OnIdle);
	glutCreateWindow("Freeglut Boilerplate");
	glutKeyboardFunc(&keyboard);
	glutDisplayFunc(&display);
	glInit();
	easel = new Easel(screenWidth, screenHeight);

	glutMainLoop();

	return EXIT_SUCCESS;
}

void glInit() {
	screenWidth = glutGet(GLUT_WINDOW_WIDTH);
	screenHeight = glutGet(GLUT_WINDOW_HEIGHT);
	float ratio = screenWidth * 1.0 / screenHeight;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, screenWidth, 0, screenHeight, -1, 1);
	glMatrixMode(GL_MODELVIEW);
}

void OnIdle() {
	//
}

void keyboard(unsigned char key, int x, int y) {
	switch(key) {
		case '\x1B':
			exit(EXIT_SUCCESS);
			break;
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);

	easel->clear();
	easel->set(10, 10, new Color(255, 0, 0), 3);
	easel->set(20, 10, new Color(0, 255, 0), 3);
	easel->set(30, 10, new Color(0, 0, 255), 3);
	easel->set(40, 10, new Color(), 3);
	easel->render();

	glFlush();
	glutPostRedisplay();
}