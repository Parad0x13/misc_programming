#include "Draws.h"
#include "game.h"
int TimerCouter=0;
int LLRR=0;
int IsTimeThis=0;
int XXX=0;

void BeginGame();
void GameIt();
void KeyCheck();
int KeyCheckEsc();
void reDrawIt();
void AjustVar();
void AjustTimer();
void AjustRL();


void BeginGame(){
XXX=getmaxx();
GameIt();
}

void GameIt(){
Again:
do {
AjustVar();
setactivepage(1);
setvisualpage(0);
reDrawIt();
setactivepage(0);
setvisualpage(1);
reDrawIt();
AjustRL();
}while (!kbhit());
if (KeyCheckEsc()==0) goto Again;
}

void KeyCheck(){
if (kbhit()) KeyCheckEsc();
}

int KeyCheckEsc(){
int toReturn=0;

if (getch()==27) toReturn=1;
return toReturn;
}


void reDrawIt(){
DrawOutSpaces();
DrawSpaces();
DrawCargoCheep(XXX,20,IsTimeThis,LLRR);
DrawGame();
}

void AjustVar(){
if (TimerCouter >20)AjustTimer();
TimerCouter=++TimerCouter;
}

void AjustTimer(){
TimerCouter =0;

DrawAmount ();

if (IsTimeThis==0)IsTimeThis=1;
else IsTimeThis=0;


}


void AjustRL(){
if (LLRR==0)XXX=XXX-1;
if (LLRR==1)XXX=XXX+1;
if (LLRR==0 && XXX<-250) LLRR=1;
if (LLRR==1 && XXX>getmaxx()+150) LLRR=0;
}

