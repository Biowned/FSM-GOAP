#pragma once
#include "Vector2D.h"
#include <map>

using namespace std;
struct position
{
	int x;
	int y;
};

class Edge
{
public:
	Edge(Vector2D, Vector2D, float);
	~Edge();
	Vector2D first;
	Vector2D second;
	float cost;
	//	Edge();

private:

};
