#ifndef __COLLISIONDETECTION__
#define __COLLISIONDETECTION__

// Brute force method
Item *actorCollidesWithItem(Actor *actor) {
	for(int d=0;d<activeChunkCount;d++) {
		for(int x=0;x<CHUNK_WIDTH;x++) {
			for(int y=0;y<CHUNK_HEIGHT;y++) {
				Item *item = activeChunks[d]->items[y][x];
				if(item->alive) {
					if(cmpVertexi(vertexftoi(actor->testLoc), vertexiMake(x, y))) {
						return item;
					}
				}
			}
		}
	}
	return NULL;
}

#endif
