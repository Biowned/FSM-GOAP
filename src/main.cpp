#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "SDL_SimpleApp.h"
#include "ScenePlanning.h"
#include "State.h"
#include "GOAP.h"

#include "Action.h"
#include "Planner.h"
#include "WorldState.h"

#include <iostream>
#include <vector>

#define FRAMES_PER_SEC 30

using namespace std;
using namespace goap;

int main(int argc, char ** argv)
{

	std::cout << "GOAP test running...\n";
	std::vector<Action> actions;

	// Constants for the various states are helpful to keep us from
	// accidentally mistyping a state name.
	const int agent_viu = 10;
	const int agent_te_arma = 15;
	const int arma_carregada = 20;
	const int agent_te_bomba = 30;
	const int enemic_visible = 35;
	const int enemic_alineat = 40;
	const int enemic_aprop = 45;
	const int enemic_viu = 50;
	const int enemic_aprop_bomba = 55;
	const int bomba_plantada = 60;
	const int fugir = 65;

	Action disparar("Disparar", 2);
	disparar.setPrecondition(arma_carregada, true);
	disparar.setPrecondition(enemic_alineat, true);
	disparar.setEffect(enemic_viu, false);
	disparar.setEffect(arma_carregada, false);
	actions.push_back(disparar);

	Action buscarArma("Buscar Arma", 5);
	buscarArma.setPrecondition(agent_viu, true);
	buscarArma.setPrecondition(enemic_visible, false);
	buscarArma.setPrecondition(agent_te_arma, false);
	buscarArma.setEffect(agent_te_arma, true);
	actions.push_back(buscarArma);

	Action buscarBomba("Buscar Bomba", 1);
	buscarBomba.setPrecondition(agent_viu, true);
	buscarBomba.setPrecondition(enemic_visible, false);
	buscarBomba.setPrecondition(agent_te_bomba, false);
	buscarBomba.setEffect(agent_te_bomba, true);
	actions.push_back(buscarBomba);

	Action apropar("Apropar", 5);
	apropar.setPrecondition(enemic_aprop, false);
	apropar.setPrecondition(enemic_visible, true);
	apropar.setPrecondition(arma_carregada, true);
	apropar.setEffect(enemic_aprop, true);
	actions.push_back(apropar);

	Action apuntar("Apuntar", 5);
	apuntar.setPrecondition(enemic_aprop, true);
	apuntar.setPrecondition(enemic_visible, true);
	apuntar.setPrecondition(arma_carregada, true);
	apuntar.setPrecondition(enemic_alineat, false);
	apuntar.setEffect(enemic_alineat, true);
	actions.push_back(apuntar);

	Action carregar("Carregar", 5);
	carregar.setPrecondition(agent_te_arma, true);
	carregar.setPrecondition(arma_carregada, false);
	carregar.setPrecondition(fugir, false);
	carregar.setEffect(arma_carregada, true);
	actions.push_back(carregar);

	Action buscarEnemic("Buscar Enemic", 5);
	buscarEnemic.setPrecondition(arma_carregada, true);
	buscarEnemic.setPrecondition(enemic_visible, false);
	buscarEnemic.setPrecondition(agent_te_bomba, true);
	buscarEnemic.setEffect(enemic_visible, true);
	actions.push_back(buscarEnemic);

	Action detonar("Detonar Bomba", 10);
	detonar.setPrecondition(agent_viu, true);
	detonar.setPrecondition(bomba_plantada, true);
	detonar.setPrecondition(enemic_visible, true);
	detonar.setPrecondition(enemic_aprop_bomba, true);
	detonar.setEffect(enemic_viu, false);
	detonar.setEffect(bomba_plantada, false);
	actions.push_back(detonar);

	Action plantarBomba("Plantar Bomba", 1);
	plantarBomba.setPrecondition(agent_te_bomba, true);
	plantarBomba.setPrecondition(bomba_plantada, false);
	plantarBomba.setPrecondition(enemic_visible, false);
	plantarBomba.setEffect(agent_te_bomba, false);
	plantarBomba.setEffect(bomba_plantada, true);
	actions.push_back(plantarBomba);


	Action fugint("Fugint", 1);
	fugint.setPrecondition(agent_viu, true);
	fugint.setPrecondition(agent_te_arma, false);
	fugint.setPrecondition(enemic_visible, true);
	fugint.setEffect(enemic_visible, false);
	actions.push_back(fugint);

	srand(time(NULL));
	WorldState initial_state;
	initial_state.setVariable(agent_viu, true);
	initial_state.setVariable(agent_te_arma, rand() % 2);
	initial_state.setVariable(arma_carregada, rand() % 2);
	initial_state.setVariable(agent_te_bomba, rand() % 2);
	initial_state.setVariable(enemic_visible, rand() % 2);
	initial_state.setVariable(enemic_viu, true);
	initial_state.setVariable(enemic_alineat, rand() % 2);
	initial_state.setVariable(enemic_aprop, rand() % 2);
	initial_state.setVariable(fugir, rand() % 2);
	initial_state.setVariable(enemic_aprop_bomba, rand() % 2);
	initial_state.setVariable(bomba_plantada, rand() % 2);

	WorldState goal_target_dead;
	goal_target_dead.setVariable(enemic_viu, false);
	goal_target_dead.priority_ = 50;

	// Fire up the A* planner
	Planner as;
	try {
		std::vector<Action> the_plan = as.plan(initial_state, goal_target_dead, actions);
		std::cout << "Found a Path:\n";
		for (std::vector<Action>::reverse_iterator rit = the_plan.rbegin(); rit != the_plan.rend(); ++rit) {
			std::cout << rit->name() << std::endl;
		}
		std::cout << "\nFinite State Machine test:\n";
	}
	catch (const std::exception&) {
		std::cout << "Sorry, could not find a path!\n";
	}

	bool quit = false;
	SDL_Event event;
	int this_tick = 0;
	int next_tick = 1000/FRAMES_PER_SEC;

	SDL_SimpleApp *app = SDL_SimpleApp::Instance();

	Scene *curr_scene = new ScenePlanning;
	app->setWindowTitle(curr_scene->getTitle());

	while (!quit)
	{
		this_tick = SDL_GetTicks();
		if (this_tick < next_tick)
		{
			SDL_Delay(next_tick - this_tick);
		}
		next_tick = this_tick + (1000 / FRAMES_PER_SEC);

		// run app frame by frame
		event = app->run(curr_scene);

		/* Keyboard events */
		switch (event.type)
		{
		case SDL_KEYDOWN:
			if (event.key.keysym.scancode == SDL_SCANCODE_1)
			{
				delete(curr_scene);
				curr_scene = new ScenePlanning;
				app->setWindowTitle(curr_scene->getTitle());
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_2)
			{
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_3)
			{
			}
			if ((event.key.keysym.scancode == SDL_SCANCODE_Q) || (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
			{
				quit = true;
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_F)
			{
				app->setFullScreen();
			}
			break;
		case SDL_QUIT:
			quit = true;
			break;
		}

	}

	return 0;
}