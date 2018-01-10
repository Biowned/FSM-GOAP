#pragma once
#include <iostream>
#include <minmax.h>
#include <SDL.h>
#include <SDL_image.h>
#include "SDL_SimpleApp.h"
#include "Vector2D.h"
#include "utils.h"
#include "SteeringBehavior.h"
#include"Path.h"
#include <queue>
#include <map>

#include "State.h"

class State;
class Agent
{
	friend class SteeringBehavior;

private:
	SteeringBehavior *steering_behavior;
	Vector2D position;
	Vector2D velocity;
	Vector2D target;

	float mass;
	float orientation;
	float max_force;
	float max_velocity;

	SDL_Color color;

	SDL_Texture *sprite_texture;
	bool draw_sprite;
	int sprite_num_frames;
	int sprite_w;
	int sprite_h;

public:
	Agent();
	~Agent();
	State* home;
	State* saloon;
	State* mine;
	State* bank;
	State* currState;
	void changeState(State*);
	
	int gold;
	int wealth;
	int tireness;
	int totalWealth;
	float thirst;
	bool full;
	bool thirsty;
	bool rested;
	bool wealthy;
	
	SteeringBehavior *Behavior();
	Vector2D getPosition();
	Vector2D getTarget();
	Vector2D getVelocity();
	
	float getMaxVelocity();
	void setPosition(Vector2D position);
	void setTarget(Vector2D target);
	void setVelocity(Vector2D velocity);
	void setMass(float mass);
	void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void update(Vector2D steering_force, float dtime, SDL_Event *event);
	void draw();
	bool Agent::loadSpriteTexture(char* filename, int num_frames=1);
	//Path Astar(Vector2D, Vector2D);
	Vector2D initPos;
	Vector2D goalPos = Vector2D(656,624);
};
