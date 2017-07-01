#include "Node.h"

Node::Node()
{
	id = 0;
	f = 0;
	g = 0;
	parent = nullptr;
}

Node::Node(size_t id, size_t cost) : Node()
{
	this->id = id;
	f = cost;
}

bool Node::operator == (const Node &rhs)
{
	return this->id == rhs.id;
}
