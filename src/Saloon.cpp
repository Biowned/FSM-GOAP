#include "Saloon.h"



Saloon::Saloon()
{
	nameState = "Saloon";
}


Saloon::~Saloon()
{
}

void Saloon::Enter(Agent* a) {
	printf("SALOON\n");
	a->totalWealth--;
	
}
void Saloon::Update(Agent* a) {
	printf("SaloonUpdate\n");
	a->thirst--;
	if (a->thirst <= 0) {
		a->thirst = 0;
	}
	//printf("%1f", a->thirst);
	if (a->thirst <= 0 && a->tireness >= 30) {
		printf("Have to Home\n");
		a->goalPos = Vector2D(656, 624);
		Exit(a, a->home);
	}
	else if (a->thirst <= 0 && a->gold >= 3) {
		printf("Have to Bank\n");
		a->goalPos = Vector2D(209, 624);
		Exit(a, a->bank);
	}
	else if (a->thirst <= 0 && a->gold < 3) {
		printf("Have to Mine\n");
		a->goalPos = Vector2D(82, 177);
		Exit(a, a->mine);
	}
	SDL_Delay(300);
}

void Saloon::Exit(Agent* a,State* s) {
	a->changeState(s);
}
/*State* Saloon::Instance() {
	if (currentState == 0)
	{
		currentState = new Saloon;
	}
	return currentState;
}*/

