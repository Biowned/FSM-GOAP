#pragma once
#include <vector>
#include <time.h>
#include "Scene.h"
#include "Agent.h"
#include "Path.h"
#include "Graph.h"
#include"Home.h"
#include"Mine.h"
#include"Bank.h"
#include"Saloon.h"
#include "Text.h"
class ScenePlanning :
	public Scene
{
public:
	ScenePlanning();
	~ScenePlanning();
	void update(float dtime, SDL_Event *event);
	void draw();
	const char* getTitle();
	Vector2D cell2pix(Vector2D cell);
	Vector2D pix2cell(Vector2D pix);
	
private:
	std::vector<Agent*> agents;
	Vector2D coinPosition;
	Vector2D currentTarget;
	int currentTargetIndex;
	Path path;
	int num_cell_x;
	int num_cell_y;
	bool draw_grid;
	std::vector<SDL_Rect> maze_rects;
	void drawMaze();
	void drawCoin();
	SDL_Texture *background_texture;
	SDL_Texture *coin_texture;
	void initMaze();
	bool loadTextures(char* filename_coin);
	void initGraph();
	std::vector< std::vector<int> > terrain;
	vector<Edge> edges;
	int D;
	vector<Vector2D> goldPositions;
	Path aStar(Vector2D init, Vector2D fin);
	bool isValidCell(Vector2D cell);
	float heuristicA(Vector2D goal, Vector2D next);
	vector<Vector2D>neighbours(Vector2D current);
	Vector2D prevPos = Vector2D(656, 624);
	bool first = true;
};

class CompareDist
{
public:
	bool operator()(std::pair<Vector2D, int > n1, std::pair<Vector2D, int> n2) {
		return n1.second > n2.second;
	}
};
