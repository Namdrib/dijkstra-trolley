#ifndef HELPERS_CPP
#define HELPERS_CPP

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Node.h"

using namespace std;


#define all(c) (c).begin(), (c).end()

enum SEARCH_STRATEGY {
	BFS,
	DIJKSTRA
};

// Overload vector output
template <typename T>
ostream& operator << (ostream &os, vector<T> &v)
{
	for (size_t i=0; i<v.size(); i++)
	{
		os << v[i];
		if (i < v.size()-1) os << ", ";
	}
	return os;
}

// Only keep the lowest cost, unless there's no link
void updateInitialCost(size_t &oldCost, size_t newCost)
{
	if (oldCost < 0) oldCost = newCost;
	else oldCost = min(oldCost, newCost);
}

// Ensure query lies in the range [lowerBound, upperBound]
// If it is less than lowerBound, set it to lowerBound;
// If it is greater than upperBound, set it to upperBound;
void clampBetween(const size_t lowerBound, size_t &query, const size_t upperBound)
{
	query = max(query, lowerBound);
	query = min(query, upperBound);
}

// assume file is of format
// n
// a1 b1 c1
// a2 b2 c2
// ...
// am bm cm
// where n is number of nodes, and each row `ai bi ci` represents:
// node a links bidirectionally to link b with cost c
// cost of zero indicates a connection.
// Treat no path as a -1
// File may have comments at the start, but not after the line containing `n`
vector<vector<size_t>> readFileIntoAdjMat(string filename)
{
	// Open the file
	istream *isPtr = nullptr; // Read from here
	ifstream ifs;

	// Switch input stream between cin and a file
	if (filename != "")
	{
		ifs.open(filename);
		if (!ifs.is_open())
		{
			cerr << "Error, could not open " << filename << endl;
			exit(EXIT_FAILURE);
		}
		isPtr = &ifs;
	}
	else
	{
		isPtr = &cin;
	}

	string line;
	for (getline(ifs, line); !isdigit(line[0]); getline(ifs, line))
	{
		;
	}

	// Number of nodes
	size_t numNodes = stoi(line);

	// The output
	vector<vector<size_t>> adjMat(numNodes, vector<size_t>(numNodes, SIZE_MAX)); // By default, no connections

	// Read each line and then put the paths into the adjmat
	// Make bi-directional links
	for (size_t a, b, c; ifs >> a >> b >> c; )
	{
		updateInitialCost(adjMat[a][b], c);
		updateInitialCost(adjMat[b][a], c);
	}

	if (filename != "")
	{
		ifs.close();
		// delete isPtr;
	}

	return adjMat;
}



// Pop (get and remove) the next value in the queue
// This depends on the strategy.
// If strategy is bfs, just remove first element
// If strategy is dijkstra, remove least-cumulative-cost element
Node* getNextInQueue(vector<Node*> &fringe, const SEARCH_STRATEGY &strategy)
{
	// Find the index
	size_t index = (strategy == BFS) ? 0 : distance(fringe.begin(), min_element(all(fringe), NodeDistanceLessThan()));

	// Retrieve and return it.
	Node *out = fringe[index];
	fringe.erase(fringe.begin() + index);
	return out;
}

void printPathAndCost(Node *result)
{
	if (result == nullptr)
	{
		cout << "No path found for this setup" << endl;
		return;
	}

	size_t cost = 0;
	vector<size_t> nodeOrder;

	// Build the path.
	while (result != nullptr)
	{
		// cout << "cost from " << result->id << ": " << result->f << endl;
		cost += result->f;
		nodeOrder.push_back(result->id);
		result = result->parent;
	}

	reverse(all(nodeOrder));
	cout << "Path: " << nodeOrder << endl;
	cout << "Total cost: " << cost << endl;
}

// Perform graph search on adjMat.
// Start at startNode and finish at endNode
// If there is no path, return a nullptr, otherwise return the final node
// which has a "parent chain" back to the start
// strategy facilitates a way to choose between BFS and Dijkstra's algorithm
// See enum SEARCH_STRATEGY at the to of this file
Node* doSearch(vector<vector<size_t>> adjMat, size_t startNode, size_t endNode, SEARCH_STRATEGY strategy)
{
	vector<size_t> closed; // stores ids of explored nodes
	vector<Node*> fringe;  // stores nodes that are "pending" exploration

	// Add the start node to the fringe
	Node* initial = new Node(startNode, 0);
	fringe.push_back(initial);

	while (!fringe.empty())
	{
		Node* current = getNextInQueue(fringe, strategy);

		// Found the target! Return
		if (current->id == endNode) return current;

		// Not in closed
		if (find(all(closed), current->id) != closed.end()) continue;
		closed.push_back(current->id);

		// Add all neighbours to fringe
		for (size_t i=0; i<adjMat[current->id].size(); i++)
		{
			// Only consider those with an existing link (cost >= 0)
			size_t stepCost = adjMat[current->id][i];
			if (stepCost == SIZE_MAX)
			{
				continue;
			}

			#ifdef DEBUG
				cout << "From " << current->id << " -> " << i << " ... ";
			#endif // DEBUG

			// Already in closed - don't do again
			if (find(all(closed), i) != closed.end())
			{
				#ifdef DEBUG
					cout << "ALREADY CLOSED!" << endl;
				#endif // DEBUG
				continue;
			}

			// The node we're currently probing
			Node* temp = new Node(i, stepCost);
			temp->parent = current;

			#ifdef DEBUG
				cout << "EXPLORING!" << endl;
				printPathAndCost(temp);
			#endif // DEBUG

			// Update running cost if using DIJKSTRA strategy
			if (strategy == DIJKSTRA)
			{
				temp->g = temp->parent->g + temp->f;
			}

			fringe.push_back(temp);
		}
	}

	// Should only reach here if there was no path between startNode, endNode
	return nullptr;
}

#endif // HELPERS_CPP
