#pragma once
#include "State.h"
class Saloon :
	public State
{
public:
	Saloon();
	~Saloon();
	void Enter(Agent*);
	void Update(Agent*);
	void Exit(Agent*,State* s);
	//State* Instance();
	
};

