#include "Home.h"
#include "Mine.h"
#include <stdio.h>


Home::Home()
{
	nameState = "Home";
}


Home::~Home()
{
	
}

void Home::Enter(Agent* a) {
	printf("HOME\n");
	if (a->wealthy) {
		a->wealth = 0;
		a->wealthy = false;
	}
	
}
void Home::Update(Agent* a) {
	if (a->tireness > 0)
	a->tireness--;
	
	printf("HomeUpdate\n");
	if(a->tireness < 1 && a->gold < 3){
		printf("Have to Mine\n");
		a->goalPos = Vector2D(82, 177);
		Exit(a,a->mine);
	}
	else if(a->tireness <= 0 && a->gold >= 3)
	{
		printf("Have to Bank\n");
		a->goalPos = Vector2D(209, 624);
		Exit(a, a->bank);
	}
	SDL_Delay(300);
}

void Home::Exit(Agent* a,State* s) {
	//State* s = new Home;
	a->changeState(s);
}
/*State* Home::Instance() {
	if (currentState == 0)
	{
		currentState = new Home;
	}
	return currentState;
}*/
