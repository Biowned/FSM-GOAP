#pragma once
#include "State.h"
class Bank :
	public State
{
public:
	Bank();
	~Bank();
	void Enter(Agent*);
	void Update(Agent*);
	void Exit(Agent*,State*);
	
};

