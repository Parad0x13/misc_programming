#include "DOSScreen.h"

DOSScreen *screen;

int main() {
	screen = new DOSScreen(DOS_SCREEN_WIDTH, DOS_SCREEN_HEIGHT);

	screen->clear('.', LIGHTBLUE);
	screen->set(5, 10, 'A');
	screen->set(6, 10, 'B', YELLOW);
	screen->set(7, 10, 'C', BROWN, LIGHTMAGENTA);

	screen->swapBuffers();

	return false;
}
