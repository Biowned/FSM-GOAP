#include "ScenePlanning.h"

using namespace std;

ScenePlanning::ScenePlanning()
{
	draw_grid = false;

	num_cell_x = SRC_WIDTH / CELL_SIZE;
	num_cell_y = SRC_HEIGHT / CELL_SIZE;
	initMaze();
	loadTextures("../res/coin.png");

	srand((unsigned int)time(NULL));

	Agent *agent = new Agent;
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agents.push_back(agent);


	// set agent position coords to the center of a random cell
	Vector2D rand_cell(-1,-1);
	while (!isValidCell(rand_cell)) 
		rand_cell = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
	agents[0]->setPosition(cell2pix(Vector2D(num_cell_x/2,num_cell_y-5)));

	// set the coin in a random cell (but at least 3 cells far from the agent)
	coinPosition = Vector2D(5,5);
	while ((!isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, rand_cell)<3)) 
		coinPosition = Vector2D((float)(rand() % num_cell_x), (float)(rand() % 5));
	
	// PathFollowing next Target
	currentTarget = Vector2D(0, 0);
	currentTargetIndex = -1;
	initGraph();
	agents[0]->currState = agents[0]->home;
	//agents[0]->currState->Enter(agents[0]);
}

ScenePlanning::~ScenePlanning()
{
	if (background_texture)
		SDL_DestroyTexture(background_texture);
	if (coin_texture)
		SDL_DestroyTexture(coin_texture);

	for (int i = 0; i < (int)agents.size(); i++)
	{
		delete agents[i];
	}
}

void ScenePlanning::update(float dtime, SDL_Event *event)
{
	if (dtime >(60.f / 1000.f)) {
		dtime = (60.f / 1000.f);
	}
	if (agents[0]->goalPos != prevPos || first) {
		path.points.push_back(agents[0]->goalPos);
		prevPos = agents[0]->goalPos;
		first = false;
	}
	/* Keyboard & Mouse events */
	switch (event->type) {
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
			draw_grid = !draw_grid;
		break;
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			Vector2D cell = pix2cell(Vector2D((float)(event->button.x), (float)(event->button.y)));
			if (isValidCell(cell))
			{
				if (path.points.size() > 0)
					if (path.points[path.points.size() - 1] == cell2pix(cell))
						break;

				path.points.push_back(cell2pix(cell));
			}
		}
		break;
	default:
		break;
	}
	if ((currentTargetIndex == -1) && (path.points.size()>0))
		currentTargetIndex = 0;

	if (currentTargetIndex >= 0)
	{	
		float dist = Vector2D::Distance(agents[0]->getPosition(), path.points[currentTargetIndex]);
		if (dist < path.ARRIVAL_DISTANCE)
		{
			//printf("GODHERE");
			if (currentTargetIndex == path.points.size() - 1)
			{
				
				if (dist < 3)
				{
					
					path.points.clear();
					currentTargetIndex = -1;
					agents[0]->setVelocity(Vector2D(0,0));
					agents[0]->currState->Enter(agents[0]);
					
					
					// if we have arrived to the coin, replace it ina random cell!
					if (pix2cell(agents[0]->getPosition()) == coinPosition)
					{
						coinPosition = Vector2D(-1, -1);
						while ((!isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, pix2cell(agents[0]->getPosition()))<3))
							coinPosition = Vector2D((float)(rand() % num_cell_x), (float)(rand() % 5));
					}
				}
				else
				{
					Vector2D steering_force = agents[0]->Behavior()->Arrive(agents[0], currentTarget, path.ARRIVAL_DISTANCE, dtime);
					agents[0]->update(steering_force, dtime, event);
				}
				return;
			}
			currentTargetIndex++;
		}

		currentTarget = path.points[currentTargetIndex];
		Vector2D steering_force = agents[0]->Behavior()->Seek(agents[0], currentTarget, dtime);
		agents[0]->update(steering_force, dtime, event);
	} 
	else
	{
		agents[0]->update(Vector2D(0,0), dtime, event);
		agents[0]->currState->Update(agents[0]);
	}
}

void ScenePlanning::draw()
{
	
	drawMaze();
	drawCoin();

	
	if (draw_grid)
	{
		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 255, 255, 255, 127);
		/*for (int i = 0; i < SRC_WIDTH; i+=CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), i, 0, i, SRC_HEIGHT);
		}
		for (int j = 0; j < SRC_HEIGHT; j = j += CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), 0, j, SRC_WIDTH, j);
		}*/
		for (int i = 0; i < edges.size(); i++) {
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), (int)(edges[i].first.x), (int)(edges[i].first.y), (int)(edges[i].second.x), (int)(edges[i].second.y));
		}
	}

	for (int i = 0; i < (int)path.points.size(); i++)
	{
		draw_circle(TheApp::Instance()->getRenderer(), (int)(path.points[i].x), (int)(path.points[i].y), 15, 255, 255, 0, 255);
		if (i > 0)
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), (int)(path.points[i - 1].x), (int)(path.points[i - 1].y), (int)(path.points[i].x), (int)(path.points[i].y));
	}
	SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 255, 0, 0, 255);
	

	draw_circle(TheApp::Instance()->getRenderer(), (int)currentTarget.x, (int)currentTarget.y, 15, 255, 0, 0, 255);

	agents[0]->draw();

	Text currState("Current State: " + agents[0]->currState->nameState, Vector2D(30, 30), TheApp::Instance()->getRenderer(), 24, false);
	currState.RenderText();

	Text dailyGoal("Daily Goal: " + std::to_string(agents[0]->wealth) +"/10", Vector2D(TheApp::Instance()->getWinSize().x - 250, 30), TheApp::Instance()->getRenderer(), 24, false);
	dailyGoal.RenderText();

	Text totalWealth("Total Wealth: " + std::to_string(agents[0]->totalWealth), Vector2D(TheApp::Instance()->getWinSize().x - 250, 60), TheApp::Instance()->getRenderer(), 24, false);
	totalWealth.RenderText();

	Text pempins("Pocket Pempins: " + std::to_string(agents[0]->gold) + "/3", Vector2D(TheApp::Instance()->getWinSize().x - 250, 90), TheApp::Instance()->getRenderer(), 24, false);
	pempins.RenderText();

	Text thirst("Thirst: " + std::to_string((int)agents[0]->thirst) + "/5", Vector2D(TheApp::Instance()->getWinSize().x - 250, 120), TheApp::Instance()->getRenderer(), 24, false);
	thirst.RenderText();

	Text tireness("Tireness: " + std::to_string(agents[0]->tireness) + "/8", Vector2D(TheApp::Instance()->getWinSize().x - 250, 150), TheApp::Instance()->getRenderer(), 24, false);
	tireness.RenderText();
}

const char* ScenePlanning::getTitle()
{
	return "SDL Steering Behaviors :: Planning Demo";
}

void ScenePlanning::drawMaze()
{
	if (!draw_grid)
	{

		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 0, 255, 255);
		for (unsigned int i = 0; i < maze_rects.size(); i++)
			SDL_RenderFillRect(TheApp::Instance()->getRenderer(), &maze_rects[i]);
	}
	else
	{
		//SDL_RenderCopy(TheApp::Instance()->getRenderer(), background_texture, NULL, NULL );
	}
}

void ScenePlanning::drawCoin()
{
	Vector2D coin_coords = cell2pix(coinPosition);
	int offset = CELL_SIZE / 2;
	SDL_Rect dstrect = {(int)coin_coords.x-offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE};
	SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
}

void ScenePlanning::initMaze()
{

	// Initialize a list of Rectagles describing the maze geometry (useful for collision avoidance)
	SDL_Rect rect = { 0, 0, 1280, 32 };
	maze_rects.push_back(rect);
	rect = { 128, 160, 1152, 32 };
	maze_rects.push_back(rect);
	rect = { 0, 736, 1280, 32 };
	maze_rects.push_back(rect);
	
	rect = { 0,32,32,736 };
	maze_rects.push_back(rect);
	rect = { 1248,32,32,736 };
	maze_rects.push_back(rect);

	rect = { 416,512,32,268 };
	maze_rects.push_back(rect);
	rect = { 832,512,32,268 };
	maze_rects.push_back(rect);
	
	rect = { 32,480,128,32 };
	maze_rects.push_back(rect);
	rect = { 288,480,288,32 };
	maze_rects.push_back(rect);
	rect = { 704,480,288,32 };
	maze_rects.push_back(rect);
	rect = { 1120,480,128,32 };
	maze_rects.push_back(rect);

	// Initialize the terrain matrix (for each cell a zero value indicates it's a wall)
	
	// (1st) initialize all cells to 1 by default
	for (int i = 0; i < num_cell_x; i++)
	{
		vector<int> terrain_col(num_cell_y, 1); 
		terrain.push_back(terrain_col);
	}
	// (2nd) set to zero all cells that belong to a wall
	int offset = CELL_SIZE / 2;
	for (int i = 0; i < num_cell_x; i++)
	{
		for (int j = 0; j < num_cell_y; j++)
		{
			Vector2D cell_center ((float)(i*CELL_SIZE + offset), (float)(j*CELL_SIZE + offset));
			for (unsigned int b = 0; b < maze_rects.size(); b++)
			{
				if (Vector2DUtils::IsInsideRect(cell_center, (float)maze_rects[b].x, (float)maze_rects[b].y, (float)maze_rects[b].w, (float)maze_rects[b].h))
				{
					terrain[i][j] = 0;
				    break;
				}  
			}
			
		}
	}

}

void ScenePlanning::initGraph() {
	for (int i = 0; i < num_cell_x; i++)
	{
		for (int j = 0; j < num_cell_y; j++)
		{
			if (terrain[i][j] != 0) {
				// va esquerrra
				if (i > 0 && terrain[i - 1][j] != 0) {
					Edge aresta3 = Edge(cell2pix(Vector2D(i, j)), cell2pix(Vector2D(i - 1, j)), 1);
					edges.push_back(aresta3);
					//edges[make_pair(cell2pix(Vector2D(i, j)), cell2pix(Vector2D(i - 1, j)))] = 1;
				}
				// va dreta
				if (i < num_cell_x - 1 && terrain[i + 1][j] != 0) {
					Edge aresta2 = Edge(cell2pix(Vector2D(i, j)), cell2pix(Vector2D(i + 1, j)), 1);
					edges.push_back(aresta2);
					//edges[make_pair(cell2pix(Vector2D(i, j)), cell2pix(Vector2D(i + 1, j)))] = 1;
				}
				// va adalt
				if (j > 0 && terrain[i][j - 1] != 0) {
					Edge aresta = Edge(cell2pix(Vector2D(i, j)), cell2pix(Vector2D(i, j - 1)), 1);
					edges.push_back(aresta);
					//edges[make_pair(cell2pix(Vector2D(i, j)), cell2pix(Vector2D(i, j - 1)))] = 1;
				}
				// va abaix
				if (j < num_cell_y - 1 && terrain[i][j + 1] != 0) {
					Edge aresta4 = Edge(cell2pix(Vector2D(i, j)), cell2pix(Vector2D(i, j + 1)), 1);
					edges.push_back(aresta4);
					//edges[make_pair(cell2pix(Vector2D(i, j)), cell2pix(Vector2D(i, j + 1)))] = 1;
				}
			}
		}
	}
}

bool ScenePlanning::loadTextures( char* filename_coin)
{
	SDL_Surface *image = IMG_Load(filename_coin);
	/*if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	background_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);*/

	//image = IMG_Load(filename_coin);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	coin_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	return true;
}

Vector2D ScenePlanning::cell2pix(Vector2D cell)
{
	int offset = CELL_SIZE / 2;
	return Vector2D(cell.x*CELL_SIZE + offset, cell.y*CELL_SIZE + offset);
}

Vector2D ScenePlanning::pix2cell(Vector2D pix)
{
	return Vector2D((float)((int)pix.x/CELL_SIZE), (float)((int)pix.y / CELL_SIZE));
}

bool ScenePlanning::isValidCell(Vector2D cell)
{
	if ((cell.x < 0) || (cell.y < 0) || (cell.x >= terrain.size()) || (cell.y >= terrain[0].size()) )
		return false;
	return !(terrain[(unsigned int)cell.x][(unsigned int)cell.y] == 0);
}

bool fncomp(Vector2D lhs, Vector2D rhs) {
	return (lhs.x + (lhs.y / 10000)) < (rhs.x + (rhs.y / 10000));

}

Path ScenePlanning::aStar(Vector2D init, Vector2D fin) {
	Path pathfinding;
	int visitedNodes = 0;

	Vector2D pos = cell2pix(init);

	std::priority_queue<std::pair<Vector2D, int>, std::vector<std::pair<Vector2D, int>>, CompareDist> frontier;

	bool(*fn_pts)(Vector2D, Vector2D) = fncomp;
	std::map<Vector2D, int, bool(*)(Vector2D, Vector2D)> costSoFar(fn_pts);

	//list<Vector2D, int> costSoFar;
	//fillDraw.clear();
	bool(*fn_pt)(Vector2D, Vector2D) = fncomp;
	std::map<Vector2D, Vector2D, bool(*)(Vector2D, Vector2D)> cameFrom(fn_pt);



	cameFrom[pos] = Vector2D(0, 0);
	frontier.emplace(make_pair(pos, 1));
	while (!frontier.empty())
	{
		visitedNodes++;
		Vector2D current = frontier.top().first;

		if (current == cell2pix(fin)) {

			cout << "A* - Nodes Visited: " << visitedNodes << endl;
			pathfinding.points.push_back(current);
			while (current != pos)
			{
				current = cameFrom[current];

				pathfinding.points.push_back(current);
			}

			reverse(pathfinding.points.begin(), pathfinding.points.end());
			return pathfinding;


		}


		vector<Vector2D> NB = neighbours(current);

		vector<pair<Vector2D, int>> newNodes;
		for each (Vector2D Next in NB)
		{
			//for (map<Vector2D, Vector2D>::iterator it = cameFrom.begin(); it != cameFrom.end(); it++) {
			int cost = 0;
			for (int i = 0; i < edges.size(); i++) {
				if (edges[i].first == current && edges[i].second == Next) {
					cost = edges[i].cost;
					break;
				}

			}
			int newCost = costSoFar[current] + cost;
			if (costSoFar.find(Next) == costSoFar.end() || newCost < costSoFar[Next]) {
				costSoFar[Next] = newCost;
				newNodes.push_back(std::make_pair(Next, newCost + heuristicA(fin, pix2cell(Next))));
				cameFrom[Next] = current;
				//fillDraw.push_back(Next);
			}
			


		}

		frontier.pop();
		for (int i = 0; i < newNodes.size(); i++) {
			frontier.emplace(newNodes[i]);
		}
	}



	//return pathfinding;

}

float ScenePlanning::heuristicA(Vector2D goal, Vector2D next) {
	float dx = abs(next.x - goal.x);
	if (dx > num_cell_x / 2) {
		dx = num_cell_x - dx;
	}
	float dy = abs(next.y - goal.y);
	return D * (dx + dy);
}

vector<Vector2D> ScenePlanning::neighbours(Vector2D current) {
	int d = 10000;
	//Vector2D curCell = pix2cell(current);
	vector<Vector2D> neighboursVec;
	for (int i = 0; i < edges.size(); i++) {
		if (edges[i].first == current) {
			if (d > edges[i].cost)
			{
				d = edges[i].cost;
				D = edges[i].cost;

			}
			neighboursVec.push_back(edges[i].second);

		}
		else if (neighboursVec.size() >= 2) {
			break;
		}
	}

	return neighboursVec;
}