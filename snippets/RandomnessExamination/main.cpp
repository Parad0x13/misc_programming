#include <GL/glut.h>
#include <iostream>
#include <ctime>

#define SQUARE 300
#define BEGIN_WIDTH SQUARE
#define BEGIN_HEIGHT SQUARE
#define DefaultSpeed 200
#define DefaultDelta 1
int SPEED = DefaultSpeed;
unsigned char DELTA = DefaultDelta;
/*	DELTA 1 produces distinct patterns
	DELTA 2 produces less distinct patterns
	DELTA 64 seems to produce what total randomness
	DELTA 128 produces what seems to be total randomness
	(300x300) produces a box in the upper right, two rectangles extending from it to the
		edges of the screen and a constant random box in the center
	301-303x301-303 skews distinct bars
	304x304 has distinct squares but on the lower right corner instead of upper left
		like 300x300 does
	308x304 has distinct squares but larger ones than 300x300 and 304x304
*/
/*	Settings: Square 300, Speed 200, Delta 1
		Given enough time there will be a secondary pattern
		where along with the squares it will seem as though
		everything is 'skewing' slightly. Many iterations
		are needed for this effect to become apparent	*/

using namespace std;

void glInit();
void OnIdle();
void keyboard(unsigned char key, int x, int y);
void display(void);

int screenWidth, screenHeight;
size_t fullSpeed = SPEED*SPEED;

class Color {
public:
	Color(char r, char g, char b) : r(r), g(g), b(b) {}
	char r, g, b;
};

class Easel {
	int width, height;
	unsigned char *data;
public:
	Easel(int width, int height) : width(width), height(height) {
		data = new unsigned char[width*height*4];
		clear();
	}
	void increase(int x, int y, int increment) {
		int actualY = (height-1)-y;
		int index = actualY*width+x;
		index <<= 2;
		data[index + 0] += increment;
		data[index + 1] += increment;
		data[index + 2] += increment;
	}
	void clear() {memset(data, 0x00, width*height*4);}
	void render() {glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);}
}*easel;

int main(int argc, char **argv) {
	srand(time(NULL));

	glutInit(&argc, argv);
	glutIdleFunc(&OnIdle);
	glutInitWindowSize(BEGIN_WIDTH, BEGIN_HEIGHT);
	glutCreateWindow("Randomness Examination");
	glutKeyboardFunc(&keyboard);
	glutDisplayFunc(&display);
	screenWidth = glutGet(GLUT_WINDOW_WIDTH);
	screenHeight = glutGet(GLUT_WINDOW_HEIGHT);
	easel = new Easel(screenWidth, screenHeight);

	cout << "Running Randomness Examination" << endl;
	cout << "Square size: " << SQUARE << endl;
	cout << "Performing " << (double)fullSpeed << " iterations each cycle" << endl;
	cout << "Increasing RGB value by " << (int)DELTA << " each iteration" << endl;

	glutMainLoop();

	return EXIT_SUCCESS;
}

void OnIdle() {
	for(size_t d=0;d<fullSpeed;d++)
		easel->increase(rand()%screenWidth, rand()%screenHeight, DELTA);
}

#define RESET 999999
void affectSpeed(int speedDelta) {
	if(speedDelta == RESET)SPEED = DefaultSpeed;
	else SPEED += speedDelta;
	fullSpeed = SPEED*SPEED;
}

void keyboard(unsigned char key, int x, int y) {
	bool shouldClear = false, shouldLogDelta = false, shouldLogSpeed = false;
	if(key == '\x1B')exit(EXIT_SUCCESS);
	else if(key == '=' || key == '+') {DELTA++;shouldLogDelta = true;}
	else if(key == '-') {DELTA--;shouldLogDelta = true;}
	else if(key == ']') {affectSpeed(10);shouldLogSpeed = true;}
	else if(key == '[') {affectSpeed(-10);shouldLogSpeed = true;}
	else if(key == 'c')shouldClear = true;
	else if(key == 'r') {
		DELTA = DefaultDelta;
		affectSpeed(RESET);
		shouldClear = shouldLogDelta = shouldLogSpeed = true;
	}

	if(shouldClear)easel->clear();
	if(shouldLogDelta)cout << "Delta has changed to " << (int)DELTA << endl;
	if(shouldLogSpeed)cout << "Speed has changed to " << SPEED << " performing " << fullSpeed << " iterations each cycle" << endl;
}

void display() {
	easel->render();
	glFlush();
	glutPostRedisplay();
}