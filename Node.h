#ifndef NODE_H
#define NODE_H

#include <cstdlib>

class Node
{
public:
	Node();

	Node(size_t id, size_t cost);

	bool operator == (const Node &rhs);

	size_t id; // which node on the map (1 is first)
	size_t f; // cost
	size_t g; // running cost

	Node* parent;
};

struct NodeIDSame
{
	bool operator()(Node *a, Node *b)
	{
		return a->id == b->id;
	}
};

struct NodeDistanceLessThan
{
	bool operator()(Node *a, Node *b)
	{
		return (a->g < b->g);
	}
};

#endif // NODE_H
