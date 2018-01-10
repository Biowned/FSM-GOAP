#include "Bank.h"



Bank::Bank()
{
	nameState = "Bank";
}


Bank::~Bank()
{
}

void Bank::Enter(Agent* a) {
	printf("BANK\n");
	
}

void Bank::Update(Agent* a) {
	a->gold--;
	a->wealth++;
	a->totalWealth++;
	printf("BankUpdate\n");
	if (a->wealth >= 10) {
		a->wealthy = true;
	}
	if (a->gold <= 0 && a->wealthy) {
		printf("Total Wealth: %d \n", a->totalWealth);
		printf("Have to Home\n");
		a->goalPos = Vector2D(656, 624);
		Exit(a, a->home);
		
	}
	else if (a->gold <= 0 && !a->wealthy) {
		printf("Wealth Daily: %d \n", a->wealth);
		printf("Have to Mine\n");
		a->goalPos = Vector2D(82, 177);
		Exit(a, a->mine);
		
	}
	SDL_Delay(300);
}

void Bank::Exit(Agent* a,State* s) {

	a->changeState(s);
}
/*State* Bank::Instance() {
	if (currentState == 0)
	{
		currentState = new Bank;
	}
	return currentState;
}*/