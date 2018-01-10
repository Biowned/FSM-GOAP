#pragma once
#include "Agent.h"

class Agent;
class State
{
public:
	State();
	~State();
	std::string nameState;
	//static State* currentState;
	virtual void Enter(Agent* a) = 0;
	virtual void Update(Agent* a) = 0;
	virtual void Exit(Agent* a,State* s) = 0;
private:
	//virtual State* Instance() = 0;
};

