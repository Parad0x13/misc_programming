#ifndef __KB__
#define __KB__

#include <windows.h>

#define MAX_VK_SIZE 256

void initKB();
void addKBHold(unsigned int key);
void removeKBHold(unsigned int key);
int keyPressed(unsigned int key);
void processInput();
void processInput_Record(INPUT_RECORD InRec);
void processKeyPresses();

extern unsigned int Keys[MAX_VK_SIZE];
extern bool jumping;

#endif
