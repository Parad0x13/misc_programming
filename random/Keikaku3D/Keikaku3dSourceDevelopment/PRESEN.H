
void StartPresent();
void DrawShape(int x,int y,int x2,int y2,int ToFill,char color,char fillColor);
void DrawBackG(int x,int y,int x2,int y2,char color);
void FillOuside (int x,int y,int x2,int y2,int steep,char color);
void outText(int x,int y , char color ,char color2 ,char far *textstring );
void Shad (int x,int y,int x2,int y2);


void StartPresent(){
int xmax;
int ymax;
setcolor(getmaxcolor());
xmax = getmaxx();
ymax = getmaxy();
setbkcolor(LIGHTGRAY);
DrawBackG (0,0,xmax,ymax,WHITE);
setcolor(RED);
moveto(5,10);
settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
setusercharsize(3, 1, 3, 1);
outtext("Asteroids Mineral Explorers");


Shad (125,125,xmax-75,ymax-75);

DrawShape(100,100,xmax-100,ymax-100,1,BLUE,WHITE);


outText( 120,102 , BLACK ,WHITE,"Asteroids Mineral Explorers" );
outText(120,230 , BLACK ,WHITE,"press any key to start");



}


void DrawShape(int x,int y,int x2,int y2,int ToFill,char color,char fillColor){
int xmax;
int ymax;
int r=20;
int r2=8;

setcolor(color);
setfillstyle(SOLID_FILL,color);


bar(x,y+r,x2,y2-r);
bar(x+r,y,x2-r,y2);
fillellipse(x+r,y+r,r,r);
fillellipse(x2-r,y2-r,r,r);
fillellipse(x2-r,y+r,r,r);
fillellipse(x+r,y2-r,r,r);

if (ToFill==1)FillOuside(x+r,y+r,x2-r,y2-r,r2,fillColor);
}


void DrawBackG(int x,int y,int x2,int y2,char color){

setfillstyle(HATCH_FILL,color);
bar(x,y, x2, y2);

}


void FillOuside (int x,int y,int x2,int y2,int steep,char color){
int xx;
int yy;
setcolor(color);
for (xx=x; xx<x2; xx=xx+steep)line (xx,y,xx,y2);
for (yy=y; yy<y2; yy=yy+steep)line (x,yy,x2,yy);
line (x2,y,x2,y2);
line (x,y2,x2,y2);
}

void outText(int x,int y , char color ,char color2 ,char far *textstring ){
setcolor(color);
moveto(x,y);
settextstyle( SMALL_FONT, HORIZ_DIR, 1);
setusercharsize(1, 1, 1, 1);
outtext(textstring);
moveto(x+1,y+1);
outtext(textstring);
}


void Shad (int x,int y,int x2,int y2){
DrawShape(x,y,x2,y2,0,DARKGRAY,0);
DrawShape(x-12,y-12,x2-12,y2-12,0,LIGHTGRAY,0);
}