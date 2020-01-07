// Bryan Tremblay V1.1
#include <iostream>
#include "NodeManager.h"

using namespace std;

int main() {
	NodeManager *manager = new NodeManager();

	string youtube1 = "a,b,4,a,c,2,b,d,5,b,c,1,c,d,8,c,e,10,d,z,6,e,z,3,d,e,2";
	string youtube2 = "a,b,16,a,c,9,a,d,35,b,e,25,b,d,12,c,d,15,c,f,22,d,e,14,d,g,19,d,f,17,f,g,14,g,e,8";
	string youtube3 = "a,b,3,a,h,4,a,e,5,b,e,5,b,c,2,e,f,4,e,h,7,h,i,2,c,d,3,c,f,2,i,f,4,i,j,6,f,g,4,d,g,7,g,j,4,d,z,2,j,z,5,g,z,6,b,f,7,h,f,5,c,g,6,f,j,3";
	string pathSum1 = "a,b,3,a,c,3,b,d,7,b,e,7,c,e,4,c,f,4,d,g,2,d,h,2,e,h,4,e,i,4,f,i,6,f,j,6,g,k,8,h,k,5,i,k,9,j,k,3";

	manager->process(youtube3);
	NodeConnectionGroup *shortestRoute = manager->shortestConnectionsBetween("a", "j");
	shortestRoute->log();

	manager->process(pathSum1);
	NodeConnectionGroup *longestRoute = manager->longestConnectionsBetween("a", "k");
	longestRoute->log();

	return false;
}
