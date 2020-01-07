#ifndef __KB__
#define __KB__

#include <windows.h>
#include "main.h"
#include "game.h"

#define MAX_VK_SIZE 256

extern unsigned int Keys[MAX_VK_SIZE];

void initKB();
void addKBHold(unsigned int key);
void removeKBHold(unsigned int key);
int keyPressed(unsigned int key);
void processInput();
void processInput_Record(INPUT_RECORD InRec);
void processKeyPresses();

#endif
