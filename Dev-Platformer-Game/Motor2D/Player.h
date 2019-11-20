#ifndef _PLAYER_H__
#define _PLAYER_H__

#include "Entity.h"
#include "ModuleEntities.h"
#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"
#include "j1Render.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player(int x, int y, ENTITY_TYPE type);
	~Player();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	int GetPlayerTile(fPoint pos) const;

	bool Load(pugi::xml_node& data);

	bool Save(pugi::xml_node& data) const;

	void LoadTexture();

	void LoadAnimation(pugi::xml_node animation_node, Animation* animation);

	void FindPlayerSpawn();

	void SpawnPlayer();

public:
	Animation idle = Animation();
	Animation running = Animation();
	Animation jumping = Animation();
	Animation death = Animation();
	Animation slide = Animation();
	Animation fall = Animation();
	Animation wall_slide = Animation();
	Animation punch1 = Animation();
	Animation punch_barrage = Animation();

	bool god_mode = false;

	bool is_jumping;
	bool is_falling;
	bool can_jump;

	bool looking_right = false;
	bool looking_left = false;

	int cont = 0;

	iPoint spawn_pos;

	EntityData playerData;
};

#endif // !_PLAYER_H_