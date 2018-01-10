#pragma once
#include "State.h"

class Home :
	public State
{
public:
	Home();
	~Home();
	void Enter(Agent* a);
	void Update(Agent* a);
	void Exit(Agent* a,State* s);
	//pos = Vector2D()
	//State* Instance();
	
private:
	int rest;
};

