#pragma once
#include "State.h"

class Mine :
	public State
{
public:
	Mine();
	~Mine();
	void Enter(Agent*);
	void Update(Agent*);
	void Exit(Agent*,State* s);
	//State* Instance();
	
private:
	int gold;
};

