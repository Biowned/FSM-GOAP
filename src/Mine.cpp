#include "Mine.h"



Mine::Mine()
{
	nameState = "Mine";
}


Mine::~Mine()
{

}

void Mine::Enter(Agent* a) {
	printf("MINE\n");
	
}
void Mine::Update(Agent* a) {
	printf("MineUpdate\n");
	a->gold++;
	a->tireness++;
	a->thirst+=0.5f;
	
	if (a->thirst >= 5) {
		printf("Have to Saloon\n");
		a->goalPos = Vector2D(1038, 624);
		Exit(a, a->saloon);
	}
	else if (a->gold == 3) {
		printf("Have to Bank\n");
		a->goalPos = Vector2D(209, 624);
		Exit(a, a->bank);
	}
	else if (a->tireness >= 8) {
		printf("Have to Home\n");
		a->goalPos = Vector2D(656, 624);
		Exit(a, a->home);
	}
	SDL_Delay(500);
}

void Mine::Exit(Agent* a, State* s) {
	a->changeState(s);
}

/*State* Mine::Instance() {
	if(currentState == 0)
	{
		currentState = new Mine;
	}
	return currentState;
}*/

