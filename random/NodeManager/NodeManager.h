// Bryan Tremblay 15Oct13
// V1.1
// Simple Node Manager that uses Dijkstra's Method to find the shortest path
// TODO: What happens if you try to find paths between non-connected nodes?
// TODO: Ensure solver works with negative values
#ifndef __NODEMANAGER__
#define __NODEMANAGER__

#include <iostream>
#include <assert.h>
#include <vector>
#include <limits.h>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <map>

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
		elems.push_back(item);
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string>elems;
	split(s, delim, elems);
	return elems;
}

class NodeConnection;
class Node {
public:
	Node(std::string identifier) : identifier(identifier) {}
	std::string identifier;
};

typedef enum {
	MonoDirectional,
	BiDirectional,
}DIRECTIONS;

class NodeConnection {
public:
	NodeConnection(Node *a, Node *b, double weight) : a(a), b(b), weight(weight) {
		direction = BiDirectional;
	}
	bool contains(Node *node) {
		if(a == node || b == node)return true;
		return false;
	}
	bool contains(Node *_a, Node *_b) {
		if(a == _a && b == _b)return true;
		if(a == _b && b == _a)return true;
		return false;
	}
	Node *neighborOf(Node *node) {
		assert(contains(node));
		if(a == node)return b;
		if(b == node)return a;
	}
	bool directionWorks(Node *_a, Node *_b) {
		assert(contains(_a, _b));
		if(direction == BiDirectional)return true;
		if(direction == MonoDirectional && _a == a)return true;
		return false;
	}
	void log() {
		std::string directionString;
		if(direction == BiDirectional)directionString = " <-> ";
		else if(direction == MonoDirectional)directionString = " -> ";
		std::cout << a->identifier << directionString << b->identifier << std::endl;
	}
	Node *a, *b;
	double weight;
	int direction;
};

class NodeConnectionGroup {
public:
	NodeConnectionGroup() {}
	void add(NodeConnection *connection) {
		// Do error checking here
		connections.push_back(connection);
		constructNodeOrder();
	}
	void reverse() {
		std::reverse(connections.begin(), connections.end());
		std::reverse(nodeOrder.begin(), nodeOrder.end());
	}
	Node *commonBetween(NodeConnection *a, NodeConnection *b) {
		if(b->contains(a->a))return a->a;
		if(b->contains(a->b))return a->b;
		return NULL;
	}
	void constructNodeOrder() {
		nodeOrder.clear();
		for(int d=0;d<connections.size();d++) {
			NodeConnection *a = NULL, *b = NULL;
			Node *common = NULL;
			a = connections[d];
			if(d < connections.size()-1)b = connections[d+1];
			if(b != NULL)common = commonBetween(a, b);
			if(d == 0 && common != NULL)nodeOrder.push_back(a->neighborOf(common));
			if(common != NULL)nodeOrder.push_back(common);
			if(d == connections.size()-1 && connections.size() >= 2) {
				Node *theCommon = commonBetween(connections[d], connections[d-1]);
				nodeOrder.push_back(a->neighborOf(theCommon));
			}
		}
	}
	double sum() {
		double retVal = 0;
		for(int d=0;d<connections.size();d++)
			retVal += connections[d]->weight;
		return retVal;
	}
	void log() {
		for(int d=0;d<nodeOrder.size();d++) {
			std::cout << nodeOrder[d]->identifier;
			if(d < nodeOrder.size()-1)
				std::cout << " <=" << connections[d]->weight << "=> ";
		}
		std::cout << std::endl << "Total Sum: " << sum() << std::endl;
	}
	std::vector<NodeConnection *>connections;
	std::vector<Node *>nodeOrder;
};

class NodeManager {
public:
	NodeManager() {}
	bool addNode(Node *node) {
		if(nodes.count(node->identifier) == 0) {
			nodes[node->identifier] = node;
			return true;
		}
		else {
			std::cout << "Tried adding a node with identifier " << node->identifier << " that already exists" << std::endl;
			return false;
		}
	}
	void process(std::string input) {
		nodes.clear();
		connections.clear();
		std::string::iterator end_pos = std::remove(input.begin(), input.end(), ' ');
		input.erase(end_pos, input.end());
		std::vector<std::string>words = split(input, ',');
		assert(words.size()%3 == 0);
		for(int d=0;d<words.size();d+=3)
			addLink(words[d+0], words[d+1], atoi(words[d+2].c_str()), BiDirectional);
	}
	bool addNode(Node *node, bool forcedCreate) {return addNode(node);}
	bool addLink(std::string a, std::string b, double distance, int direction) {
		if(nodes.count(a) == 0)addNode(new Node(a), true);
		if(nodes.count(b) == 0)addNode(new Node(b), true);
		NodeConnection *link = linkBetween(a, b);
		if(link == NULL) {
			NodeConnection *connection = new NodeConnection(nodes[a], nodes[b], distance);
			connection->direction = direction;
			connections.push_back(connection);
		}
		else {
			if(link->direction == BiDirectional)
				std::cout << "Bi-Directional connection already exists between those two nodes" << std::endl;
			assert(link->direction == MonoDirectional && direction == MonoDirectional);
			// If both are not MonoDirectional then unpredicted behaviour is possible
			NodeConnection *connection = new NodeConnection(nodes[a], nodes[b], distance);
			connection->direction = direction;
			connections.push_back(connection);
		}
	}
	void removeLink(NodeConnection *link) {
		for(int d=0;d<connections.size();d++) {
			if(connections[d] == link) {
				connections.erase(connections.begin() + d);
				break;
			}
		}
	}
	void removeLink(std::string a, std::string b) {
		NodeConnection *link = linkBetween(a, b);
		if(link != NULL)removeLink(link);
	}
	void removeAllLinksTo(std::string identifier) {
		std::vector<NodeConnection *>toRemove = linksTo(identifier);
		for(int d=0;d<toRemove.size();d++)removeLink(toRemove[d]);
	}
	NodeConnection *linkBetween(std::string a, std::string b) {
		assert(nodes.count(a) > 0 && nodes.count(b) > 0);
		Node *nodeA = nodes[a];
		Node *nodeB = nodes[b];
		for(int d=0;d<connections.size();d++) {
			NodeConnection *connection = connections[d];
			if(connection->contains(nodeA, nodeB) && connection->directionWorks(nodeB, nodeA))return connection;
		}
		return NULL;
	}
	std::vector<NodeConnection *> linksTo(std::string identifier) {
		std::vector<NodeConnection *>retVal;
		if(nodes.count(identifier) > 0) {
			Node *node = nodes[identifier];
			for(int d=0;d<connections.size();d++) {
				if(connections[d]->contains(node))
					retVal.push_back(connections[d]);
			}
		}
		return retVal;
	}
	std::vector<NodeConnection *> removeLinksUnableToTravelToFrom(std::vector<NodeConnection *> links, std::string identifier) {
		std::vector<NodeConnection *> retVal;
		Node *node = nodes[identifier];
		for(int d=0;d<links.size();d++) {
			if(links[d]->directionWorks(node, links[d]->neighborOf(node)))
				retVal.push_back(links[d]);
		}
		return retVal;
	}
	void removeNode(std::string node) {
		if(nodes.count(node) > 0) {
			removeAllLinksTo(node);
			std::map<std::string, Node *>::iterator it = nodes.find(node);
			nodes.erase(it);
		}
	}
	NodeConnectionGroup *shortestConnectionsBetween(std::string a, std::string b) {
		NodeConnectionGroup *retVal = new NodeConnectionGroup();
		assert(nodes.count(a) > 0 && nodes.count(b) > 0);
		Node *nodeA = nodes[a];
		Node *nodeB = nodes[b];
		// Dijkstra's Algorithm requires we relax each edge. Edges can only be relaxed
		// if all nodes between a and b are visited and relative weights calculated
		std::vector<Node *>visited;
		std::vector<Node *>unvisited;
		for(std::map<std::string, Node *>::iterator it = nodes.begin();it != nodes.end();it++)
			unvisited.push_back(it->second);
		std::map<Node *, double>relativeWeights;
		for(int d=0;d<unvisited.size();d++)relativeWeights[unvisited[d]] = INT_MAX;
		// Dijkstra's Algorithm then sets the initial node's weight to 0
		relativeWeights[nodeA] = 0;
		Node *currentNode;
		// To map the shortest route after all required connections have been relaxed we start
		// with the last node then work our way back through the parent nodes that relaxed
		// the connection last.
		std::map<Node *, Node*>parents;

		// Dijkstra requires that we continue on until the end node is reached
		while(currentNode != nodeB) {
			assert(unvisited[0] != NULL);
			Node *lowestWeightNode = unvisited[0];
			// Dijkstra requires that we process the next unvisited node with least relative weight
			for(int d=0;d<unvisited.size();d++) {
				if(relativeWeights[unvisited[d]] < relativeWeights[lowestWeightNode])
					lowestWeightNode = unvisited[d];
			}
			currentNode = lowestWeightNode;
			// Dijkstra requires we move this node to the visited set
			std::vector<Node *>::iterator position = std::find(unvisited.begin(), unvisited.end(), currentNode);
			if(position != unvisited.end())unvisited.erase(position);
			visited.push_back(currentNode);
			// Dijkstra then requires that we relax all connections that are incident to the node
			// To relax an incident connection we sum the current relative weight of the current node
			//		and the weight of the connection between them. If that weight is less than the node
			//		on the other side of the connection's weight the node is relaxed even further
			std::vector<NodeConnection *>toRelax = linksTo(currentNode->identifier);
			toRelax = removeLinksUnableToTravelToFrom(toRelax, currentNode->identifier);
			for(int d=0;d<toRelax.size();d++) {
				Node *neighbor = toRelax[d]->neighborOf(currentNode);
				double calculatedWeight = relativeWeights[currentNode] + toRelax[d]->weight;
				if(calculatedWeight < relativeWeights[neighbor]) {
					relativeWeights[neighbor] = calculatedWeight;
					parents[neighbor] = currentNode;
				}
			}
		}

		currentNode = nodeB;
		while(currentNode != nodeA) {
			if(parents.count(currentNode) > 0) {
				Node *parent = parents[currentNode];
				NodeConnection *connection = linkBetween(currentNode->identifier, parent->identifier);
				retVal->add(connection);
				currentNode = parent;
			}
		}
		retVal->reverse();

		return retVal;
	}
	NodeConnectionGroup *longestConnectionsBetween(std::string a, std::string b) {
		double maxWeight = 0;
		for(size_t d=0;d<connections.size();d++) {
			if(connections[d]->weight > maxWeight)maxWeight = connections[d]->weight;
		}
		// It's possible that you could overflow the double weight... maybe I should check for that
		double ceilingWeight = ceil(maxWeight)+10;
		for(size_t d=0;d<connections.size();d++) {
			connections[d]->weight = ceilingWeight - connections[d]->weight;
		}
		NodeConnectionGroup *retVal = shortestConnectionsBetween(a, b);
		for(size_t d=0;d<retVal->connections.size();d++) {
			retVal->connections[d]->weight = ceilingWeight - retVal->connections[d]->weight;
		}

		return retVal;
	}
	void log() {
		std::cout << "Existing Nodes:" << std::endl;
		typedef std::map<std::string, Node *>::iterator it_type;
		for(it_type iterator = nodes.begin();iterator != nodes.end();iterator++)
			std::cout << "\t" << iterator->first << ":" << iterator->second << std::endl;
		std::cout << "Total of " << nodes.size() << " nodes" << std::endl;
		std::cout << "Existing Connections:" << std::endl;
		for(int d=0;d<connections.size();d++)
			std::cout << "\t" << connections[d]->a->identifier << " <-> " << connections[d]->b->identifier << ": " << connections[d]->weight << std::endl;
	}
	std::map<std::string, Node *>nodes;
	std::vector<NodeConnection *>connections;
};

#endif
