#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <string>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <GL/glut.h>

using namespace std;

int rnd(int range) {return rand()%range;}
void seed() {srand((unsigned)time(NULL));}

void glInit();
void OnIdle();
void keyboard(unsigned char key, int x, int y);
void display(void);

int masterTag = 100;
string bases = "UDLR";		// Adenine, Thymine, Cytosine, Guanine
string geneStart = "UDLR";
string geneEnd = "RLDU";

int screenWidth, screenHeight;

typedef struct {
	unsigned char r, g, b;
}RGB;

RGB rgbWith(char _r, char _g, char _b) {
	RGB retVal;
	retVal.r = _r;retVal.g = _g;retVal.b = _b;
	return retVal;
}

unsigned char *easel;
void setEasel(int x, int y, RGB color, int border) {
	for(int sizeY=-border;sizeY<=border;sizeY++) {
		for(int sizeX=-border;sizeX<=border;sizeX++) {
			int finalX = (sizeX+x);
			int finalY = (sizeY+y);
			if(finalX <= screenWidth && finalX >= 0 && finalY <= screenHeight && finalY >= 0) {
				easel[(finalY*screenWidth+finalX)*3 + 0] = color.r;
				easel[(finalY*screenWidth+finalX)*3 + 1] = color.g;
				easel[(finalY*screenWidth+finalX)*3 + 2] = color.b;
			}
		}
	}
}

class ProteinStructure {
public:
	ProteinStructure(int _width, int _height) {
		width = _width;
		height = _height;
		data = (int *)calloc(width*height, sizeof(int));
	}
	ProteinStructure(string code) {
		int minX = 0, maxX = 0, minY = 0, maxY = 0, x = 0, y = 0;
		for(int index=0;index<code.size();index++) {
			if(code[index] == bases[0])y++;
			if(code[index] == bases[1])y--;
			if(code[index] == bases[2])x--;
			if(code[index] == bases[3])x++;
			minX = min(x, minX);
			maxX = max(x, maxX);
			minY = min(y, minY);
			maxY = max(y, maxY);
		}
		width = abs(maxX-minX)+1;
		height = abs(maxY-minY)+1;
		int xOffset = abs(minX), yOffset = abs(maxY);
		data = (int *)calloc(width*height, sizeof(int));
		data[yOffset*width+xOffset] = 1;	// Otherwise the initial location wouldn't be active
		x = 0, y =0;
		for(int index=0;index<code.size();index++) {
			if(code[index] == bases[0])y++;
			if(code[index] == bases[1])y--;
			if(code[index] == bases[2])x--;
			if(code[index] == bases[3])x++;
			int actualX = x+xOffset;
			int actualY = yOffset-y;
			data[actualY*width+actualX] = 1;
		}
	}
	~ProteinStructure();
	ProteinStructure *rotateCW() {
		ProteinStructure *retVal = new ProteinStructure(height, width);
		for(int y=0;y<retVal->height;y++) {
			for(int x=0;x<retVal->width;x++)
				retVal->data[y*retVal->width+x] = data[(height-1-x)*width+y];
		}
		return retVal;
	}
	void render() {
		for(int y=0;y<height;y++) {
			for(int x=0;x<width;x++) {
				if(data[y*width+x] == 0)cout << ".";
				if(data[y*width+x] == 1)cout << "X";
				if(data[y*width+x] == 2)cout << "S";
			}
			cout << endl;
		}
		cout << endl;
	}
	int width, height, *data;
};

class Protein {
public:
	Protein(int minLength, int maxLength) {
		code = Protein::randomProteinCode(minLength, maxLength);
		setupStructures();
	}
	Protein(string _code) {
		code = _code;
		setupStructures();
	}
	~Protein();
	static string randomProteinCode(int minLength, int maxLength) {
		string retVal;
		bool proteinComplete = false;
		while(!proteinComplete) {
			char next = bases[rnd(4)];
			retVal.push_back(next);
			string last4 = string(retVal.end()-geneEnd.length(), retVal.end());
			if(last4.compare(geneEnd) == 0) {
				retVal.erase(retVal.end(), retVal.end());
				if(retVal.size() >= minLength)proteinComplete = true;
			}
			if(retVal.size() >= maxLength)proteinComplete = true;
		}
		return retVal;
	}
	static float strengthOfProtein(Protein *protein) {
		// Special Proteins. These have direct effects on Critters
		// Ranging from chance of 1.0f anything not one of these proteins
		// will register between 0.0f and 0.1f
		// Each protein tier will increase activity 0.1f
		static Protein *OBlock = new Protein("DRU");	// (O)Power Level: 1
		static Protein *SBlock = new Protein("RUR");	// (S)Power Level: 2
		static Protein *ZBlock = new Protein("RDR");	// (Z)Power Level: 3
		static Protein *JBlock = new Protein("RRD");	// (J)Power Level: 4
		static Protein *LBlock = new Protein("URR");	// (L)Power Level: 5
		static Protein *TBlock = new Protein("RRLD");	// (Z)Power Level: 6
		static Protein *IBlock = new Protein("RRR");	// (I)Power Level: 7
		int totalStrengthLevels = 7;
		int currentStrength = 0;

		if(protein->equals(OBlock))currentStrength += 1;
		if(protein->equals(SBlock))currentStrength += 2;
		if(protein->equals(ZBlock))currentStrength += 3;
		if(protein->equals(JBlock))currentStrength += 4;
		if(protein->equals(LBlock))currentStrength += 5;
		if(protein->equals(TBlock))currentStrength += 6;
		if(protein->equals(IBlock))currentStrength += 7;
		return (float)currentStrength/(float)totalStrengthLevels;
	}
	void setupStructures() {
		structures[0] = new ProteinStructure(code);
		for(int d=1;d<4;d++)structures[d] = structures[d-1]->rotateCW();
	}
	bool equals(Protein *protein) {
		bool rotationHasMatched = false;
		for(int rot=0;rot<4;rot++) {
			if(structures[0]->width == protein->structures[rot]->width && structures[0]->height == protein->structures[rot]->height) {
				int mismatches = 0;
				for(int d=0;d<structures[0]->width*structures[0]->height;d++) {
					if(structures[0]->data[d] != protein->structures[rot]->data[d]){mismatches++;break;}
				}
				if(mismatches == 0){rotationHasMatched = true;break;}
			}
		}
		return rotationHasMatched;
	}
	void log() {cout << "Protein: " << code << endl;}
	void render() {render(1);}
	void render(int rotations) {
		assert(rotations <= 4);
		for(int d=0;d<rotations;d++)structures[d]->render();
	}
	string code;
	ProteinStructure *structures[4];
};

class Entity {
public:
	Entity() {
		alive = true;
		color = rgbWith(rnd(255), rnd(255), rnd(255));
		x = rnd(screenWidth);
		y = rnd(screenHeight);
	}
	void render(int borderSize) {setEasel(x, y, color, borderSize);}
	void render() {render(0);}
	bool alive;
	int x, y;
	RGB color;
};

class Food : public Entity {
public:
	Food() {
		color = rgbWith(64, 255, 64);
	}
};
vector<Food *>foods;

class Critter : public Entity {
public:
	Critter(Critter *parentA, Critter *parentB) {
		age = 0;
		maxAge = 10000;
		hunger = 0;
		health = 100;
		geneHeaderLength = 4;
		proteinLength = 8;
		geneFooterLength = 4;
		geneLength = geneHeaderLength + proteinLength + geneFooterLength;
		geneCount = 5;
		color = rgbWith(0, 255, 255);
		tag = masterTag++;
		if(parentA != NULL && parentB != NULL) {
			for(int d=0;d<geneCount;d++)
				DNA += rnd(2)?parentA->getGene(d):parentB->getGene(d);
		}
		else {
			for(int d=0;d<geneCount;d++) {
				DNA += geneStart;
				DNA += Protein::randomProteinCode(proteinLength, proteinLength);
				DNA += geneEnd;
			}
		}
		geneProteins = new Protein*[geneCount];
		for(int d=0;d<geneCount;d++) {
			string geneCode = getGene(d);
			string proteinCode = geneCode.substr(geneHeaderLength, proteinLength);
			geneProteins[d] = new Protein(proteinCode);
		}
		initDNAProperties();
	}
	~Critter();
	void initDNAProperties() {
		eatingProbability = Protein::strengthOfProtein(geneProteins[0]);
		movingProbability[0] = Protein::strengthOfProtein(geneProteins[1]);
		movingProbability[1] = Protein::strengthOfProtein(geneProteins[2]);
		movingProbability[2] = Protein::strengthOfProtein(geneProteins[3]);
		movingProbability[3] = Protein::strengthOfProtein(geneProteins[4]);
		movingProbability[4] = 4.0f-(movingProbability[0]+movingProbability[1]+movingProbability[2]+movingProbability[3]);
	}
	string getGene(int geneNumber) {
		int beginIndex = geneLength * geneNumber;
		int endIndex = beginIndex + geneLength;
		assert(DNA.size() >= endIndex);
		return DNA.substr(beginIndex, geneLength);
	}
	void logDNA() {
		cout << "Tag(" << tag << "):" << endl;
		for(int d=0;d<geneCount;d++) {
			string geneCode = getGene(d);
			cout << "Gene(" << d << "): " << "...." << geneCode.substr(geneHeaderLength, proteinLength) << "...." << endl;
		}
	}
	void tick() {
		tryEating();
		tryMoving();
		age++;
		if(age > maxAge)alive = false;
	}
	void tryEating() {
		for(int d=0;d<foods.size();d++) {
			if(foods[d]->x == x && foods[d]->y == y) {
				foods[d]->alive = false;
				age = 0;
			}
		}
	}
	void tryMoving() {
		int toBe = -1;
		float sum = 0;
		for(int d=0;d<5;d++)sum += movingProbability[d];
		int finalSum = sum*100.0f;	// For some weird reason 0.01f*100.0f = 0 for ints...
		if(finalSum > 0) {
			float randomValue = rnd(finalSum)/100.0f;
			for(int d=0;d<5;d++) {
				randomValue -= movingProbability[d];
				if(randomValue < 0){toBe = d;break;}
			}
		}
		if(toBe == 0)y++;
		if(toBe == 1)y--;
		if(toBe == 2)x--;
		if(toBe == 3)x++;
	}
	string DNA;
	int geneHeaderLength, proteinLength, geneFooterLength, geneLength, geneCount;
	int age, maxAge, hunger, health, tag;
	Protein **geneProteins;
	float eatingProbability;
	float movingProbability[5];	// Up, Down, Left, Right, StandStill
};
vector<Critter *>critters;

int main(int argc, char **argv) {
	seed();

	glutInit(&argc, argv);
	glutIdleFunc(&OnIdle);
	glutCreateWindow("GLUT Test");
	glutKeyboardFunc(&keyboard);
	glutDisplayFunc(&display);
	glInit();

	for(int d=0;d<100;d++)critters.push_back(new Critter(NULL, NULL));
	for(int d=0;d<1000;d++)foods.push_back(new Food());

	glutMainLoop();

	return EXIT_SUCCESS;
}

void glInit() {
	screenWidth = glutGet(GLUT_WINDOW_WIDTH);
	screenHeight = glutGet(GLUT_WINDOW_HEIGHT);
	easel = new unsigned char[screenWidth*screenHeight*3];
	float ratio = screenWidth * 1.0 / screenHeight;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, screenWidth, 0, screenHeight, -1, 1);
	glMatrixMode(GL_MODELVIEW);
}

void OnIdle() {
	for(int d=0;d<critters.size();d++)critters[d]->tick();

	vector<Entity *>toRemove;
	for(int d=0;d<critters.size();d++) {
		if(!critters[d]->alive)toRemove.push_back(critters[d]);
	}
	for(int d=0;d<toRemove.size();d++) {
		int index = 0;
		for(int dd=0;dd<critters.size();dd++) {
			if(toRemove[d] == critters[dd])index = dd;
		}
		critters.erase(critters.begin()+index);
	}
	toRemove.clear();
	for(int d=0;d<foods.size();d++) {
		if(!foods[d]->alive)toRemove.push_back(foods[d]);
	}
	for(int d=0;d<toRemove.size();d++) {
		int index = 0;
		for(int dd=0;dd<foods.size();dd++) {
			if(toRemove[d] == foods[dd])index = dd;
		}
		foods.erase(foods.begin()+index);
	}
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

	memset(easel, 0x00, screenWidth*screenHeight*3);
	for(int d=0;d<critters.size();d++)critters[d]->render(1);
	for(int d=0;d<foods.size();d++)foods[d]->render(2);
	glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE, easel);

	glFlush();
	glutPostRedisplay();
}