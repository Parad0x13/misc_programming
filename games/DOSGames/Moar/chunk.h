#ifndef __CHUNK__
#define __CHUNK__

#define CHUNK_WIDTH		10
#define CHUNK_HEIGHT	10
#define MAX_ACTORS		10

class Chunk {
public:
	Chunk();
	int nextAvailableActorIndex();
	vertexi loc;
	Item *items[CHUNK_HEIGHT][CHUNK_WIDTH];
	Actor *actors[MAX_ACTORS];
};

Chunk::Chunk() {
	for(int x=0;x<CHUNK_WIDTH;x++) {
		for(int y=0;y<CHUNK_HEIGHT;y++) {
			items[y][x] = new Item;
			items[y][x]->alive = false;
		}
	}
	for(int d=0;d<MAX_ACTORS;d++) {
		actors[d] = new Actor;
		actors[d]->alive = false;
	}
}

Item *itemAtChunkLocation(Chunk *chunk, vertexi loc) {
	if(chunk->items[loc.y][loc.x]->alive)
		return chunk->items[loc.y][loc.x];
	return NULL;
}

Chunk *startingChunk() {
	Chunk *retVal = new Chunk;
	retVal->loc = vertexiMake(0, 0);

	retVal->items[6][0] = Stone_Item();
	retVal->items[6][1] = Stone_Item();
	retVal->items[6][2] = Stone_Item();
	retVal->items[6][3] = Stone_Item();
	retVal->items[6][4] = Stone_Item();
	retVal->items[6][5] = Stone_Item();
	retVal->items[6][6] = Stone_Item();
	retVal->items[6][7] = Stone_Item();
	retVal->items[6][8] = Stone_Item();
	retVal->items[6][9] = Stone_Item();
	return retVal;
}

int activeChunkCount;
Chunk *activeChunks[1];
Actor *player;

void generateWorld() {
	activeChunks[0] = startingChunk();
	activeChunkCount++;

	int index = activeChunks[0]->nextAvailableActorIndex();
	delete activeChunks[0]->actors[index];
	activeChunks[0]->actors[index] = Player_Actor();
	activeChunks[0]->actors[index]->loc = vertexfMake(4, 1);
	player = activeChunks[0]->actors[index];
}

int Chunk::nextAvailableActorIndex() {
	for(int d=0;d<MAX_ACTORS;d++) {
		if(!actors[d]->alive)
			return d;
	}
	return -1;
}

#endif
