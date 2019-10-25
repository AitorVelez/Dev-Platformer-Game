#ifndef _j1PLAYER_H__
#define _j1PLAYER_H__

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;

enum COLLISION_TYPE
{
	GROUND,
	AIR,
	DEATH,
	WIN
};

struct PlayerInfo 
{
	iPoint position;

};

class j1Player :
	public j1Module
{
public:

	j1Player();
	virtual	~j1Player();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void LoadTexture();

	void LoadAnimation(pugi::xml_node animation_node, Animation* animation);

	void FindPlayerSpawn();

	void SpawnPlayer();

	COLLISION_TYPE CheckCollision(int x) const;

public:

	SDL_Texture* texture = nullptr;

	Animation* animation = nullptr;

	Animation idle = Animation();
	Animation running = Animation();
	Animation jumping = Animation();
	Animation death = Animation();
	Animation slide = Animation();
	Animation fall = Animation();
	Animation wall_slide = Animation();
	Animation punch1 = Animation();
	Animation punch_barrage = Animation();

	PlayerInfo player;

	iPoint spawn_pos;

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	bool god_mode = false;
};

#endif