// Bryan Tremblay 02Sep13
/*	While reading Richard Dawkins' "The Selfish Gene" I came across a part where
	he described Evolutionary Stable Systems (ESS)' and described how they could
	be interpreted in computer software. I thought it would be easy to do and
	I guess I don't understand exactly how to accomplish the task since the
	software only works partially. */

#include <stdio.h>
#include <malloc.h>
#include <time.h>

#define POPULATION_COUNT	120
#define POPULATION_RATIO_DOVE		(5.0f/12.0f)
#define POPULATION_RATIO_HAWK		(7.0f/12.0f)
#define POPULATION_RATIO_RETALIATOR	0.0f

#define SCORE_WIN			50
#define SCORE_LOOSE			0
#define SCORE_INJURY		-100
#define SCORE_TIMEWASTE		-10

enum Birds {
	Dove,
	Hawk,
	Retaliator,
};
int BirdTypeCount = 3;
float selectionPercent[] = {POPULATION_RATIO_DOVE, POPULATION_RATIO_HAWK, POPULATION_RATIO_RETALIATOR};

enum Outcomes {
	Win,
	Loss,
	Injury,
	TimeWaste,
};

int rnd(int range) {return rand()%range;}
void seed() {srand((unsigned)time(NULL));}

int currentGlobalTag = 100;
unsigned long iteration = 0;

class Entity {
public:
	Entity() {
		tag = currentGlobalTag++;
		doveScore = 0;doveInteractions = 0;
		hawkScore = 0;hawkInteractions = 0;
		retaliatorScore = 0;retaliatorInteractions = 0;
	};
	~Entity() {};

	void fight(Entity *enemy) {
		if(enemy != this) {
			float randomValue = rand()%100/100.0f;
			int toBe;
			for(int d=0;d<BirdTypeCount;d++) {
				toBe = d;
				if(selectionPercent[d] > randomValue)break;
				randomValue -= selectionPercent[d];
			}
			if(toBe == Dove)this->type = Dove;
			if(toBe == Hawk)this->type = Hawk;
			if(toBe == Retaliator)this->type = Retaliator;
			randomValue = rand()%100/100.0f;
			for(int d=0;d<BirdTypeCount;d++) {
				toBe = d;
				if(selectionPercent[d] > randomValue)break;
				randomValue -= selectionPercent[d];
			}
			if(toBe == Dove)enemy->type = Dove;
			if(toBe == Hawk)enemy->type = Hawk;
			if(toBe == Retaliator)enemy->type = Retaliator;

			if(type == Dove && enemy->type == Dove) {
				Entity *winner = rnd(2)?this:enemy;
				Entity *looser = (winner==this)?enemy:this;
				winner->processOutcome(Win);
				looser->processOutcome(Loss);
				winner->processOutcome(TimeWaste);
				looser->processOutcome(TimeWaste);
			}
			else if(type == Dove && enemy->type == Hawk) {
				this->processOutcome(Loss);
				enemy->processOutcome(Win);
			}
			else if(type == Dove && enemy->type == Retaliator) {
				Entity *winner = rnd(2)?this:enemy;
				Entity *looser = (winner==this)?enemy:this;
				winner->processOutcome(Win);
				looser->processOutcome(Loss);
				winner->processOutcome(TimeWaste);
				looser->processOutcome(TimeWaste);
			}
			else if(type == Hawk && enemy->type == Dove) {
				this->processOutcome(Win);
				enemy->processOutcome(Loss);
			}
			else if(type == Hawk && enemy->type == Hawk) {
				Entity *winner = rnd(2)?this:enemy;
				Entity *looser = (winner==this)?enemy:this;
				winner->processOutcome(Win);
				looser->processOutcome(Loss);
				looser->processOutcome(Injury);
			}
			else if(type == Hawk && enemy->type == Retaliator) {
				Entity *winner = rnd(2)?this:enemy;
				Entity *looser = (winner==this)?enemy:this;
				winner->processOutcome(Win);
				looser->processOutcome(Loss);
				looser->processOutcome(Injury);
			}
			else if(type == Retaliator && enemy->type == Dove) {
				Entity *winner = rnd(2)?this:enemy;
				Entity *looser = (winner==this)?enemy:this;
				winner->processOutcome(Win);
				looser->processOutcome(Loss);
				winner->processOutcome(TimeWaste);
				looser->processOutcome(TimeWaste);
			}
			else if(type == Retaliator && enemy->type == Hawk) {
				Entity *winner = rnd(2)?this:enemy;
				Entity *looser = (winner==this)?enemy:this;
				winner->processOutcome(Win);
				looser->processOutcome(Loss);
				looser->processOutcome(Injury);
			}
			else if(type == Retaliator && enemy->type == Retaliator) {
				Entity *winner = rnd(2)?this:enemy;
				Entity *looser = (winner==this)?enemy:this;
				winner->processOutcome(Win);
				looser->processOutcome(Loss);
				winner->processOutcome(TimeWaste);
				looser->processOutcome(TimeWaste);
			}
			if(this->type == Dove)this->doveInteractions++;
			if(this->type == Hawk)this->hawkInteractions++;
			if(this->type == Retaliator)this->retaliatorInteractions++;
			if(enemy->type == Dove)enemy->doveInteractions++;
			if(enemy->type == Hawk)enemy->hawkInteractions++;
			if(enemy->type == Retaliator)enemy->retaliatorInteractions++;
		}
		//else printf("Problem has ocurred, %i is trying to fight itself. Fight will not commence\n", tag);
	}

	void processOutcome(int outcome) {
		switch(outcome) {
			case Win:
				if(type == Dove)doveScore += SCORE_WIN;
				if(type == Hawk)hawkScore += SCORE_WIN;
				if(type == Retaliator)retaliatorScore += SCORE_WIN;
				break;
			case Loss:
				if(type == Dove)doveScore += SCORE_LOOSE;
				if(type == Hawk)hawkScore += SCORE_LOOSE;
				if(type == Retaliator)retaliatorScore += SCORE_LOOSE;
				break;
			case Injury:
				if(type == Dove)doveScore += SCORE_INJURY;
				if(type == Hawk)hawkScore += SCORE_INJURY;
				if(type == Retaliator)retaliatorScore += SCORE_INJURY;
				break;
			case TimeWaste:
				if(type == Dove)doveScore += SCORE_TIMEWASTE;
				if(type == Hawk)hawkScore += SCORE_TIMEWASTE;
				if(type == Retaliator)retaliatorScore += SCORE_TIMEWASTE;
				break;
			default:
				break;
		}
	}

	float averageForType(int _type) {
		if(_type == Dove)
			return (float)doveScore/(float)doveInteractions;
		else if(_type == Hawk)
			return (float)hawkScore/(float)hawkInteractions;
		else if(_type == Retaliator)
			return (float)retaliatorScore/(float)retaliatorInteractions;
		else return -101010;
	}

	float averageForAllTypes() {
		return (float)(doveScore+hawkScore+retaliatorScore)/(float)(doveInteractions+hawkInteractions+retaliatorInteractions);
	}

	int doveScore, doveInteractions;
	int hawkScore, hawkInteractions;
	int retaliatorScore, retaliatorInteractions;
	int tag, type;
};

Entity *population[POPULATION_COUNT];

void init() {
	for(int d=0;d<POPULATION_COUNT;d++)
		population[d] = new Entity();
}

int main() {
	seed();
	init();
	for(;;) {
		Entity *a = population[rnd(POPULATION_COUNT)];
		Entity *b = population[rnd(POPULATION_COUNT)];
		a->fight(b);

		iteration++;
		//printf("Dove: %f\nHawk: %f\n", population[0]->averageForType(Dove), population[0]->averageForType(Hawk));
		if(iteration%1000000 == 0) {
			double all = 0;
			for(int d=0;d<POPULATION_COUNT;d++)all += population[0]->averageForAllTypes();
			all /= POPULATION_COUNT;
			printf("Aggregate: %f\n", all);
		}
	}
}
