#include "constants.h"

vertexf vertexfMake(float x, float y) {
	vertexf retVal = {x, y};
	return retVal;
}

vertexi vertexiMake(int x, int y) {
	vertexi retVal = {x, y};
	return retVal;
}

vertexi vertexftoi(vertexf vertex) {
	return vertexiMake((int)vertex.x, (int)vertex.y);
}

bool cmpvertexi(vertexi first, vertexi second) {
	if(first.x==second.x && first.y==second.y)
		return true;
	return false;
}

bool cmpvertexf(vertexf first, vertexf second) {
	vertexi _first = vertexftoi(first);
	vertexi _second = vertexftoi(second);
	if(cmpvertexi(_first, _second))
		return true;
	return false;
}

m_vector m_vectorMake(float x, float y) {
	m_vector retVal = {x, y};
	return retVal;
}

m_vector ZeroVector() {
	return m_vectorMake(0, 0);
}
