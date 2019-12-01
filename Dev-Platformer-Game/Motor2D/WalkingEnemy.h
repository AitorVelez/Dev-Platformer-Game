#ifndef _WALKINGENEMY_H__
#define _WALKINGENEMY_H__

#include "Entity.h"
#include "ModuleEntities.h"
#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"
#include "j1Render.h"

struct SDL_Texture;

class WalkingEnemy : public Entity
{
public:

	WalkingEnemy(int x, int y, ENTITY_TYPE type);
	~WalkingEnemy();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	bool Load(pugi::xml_node& data);

	bool Save(pugi::xml_node& data) const;

	void LoadTexture();

	void LoadAnimation(pugi::xml_node animation_node, Animation* animation);

public:
	Animation idle = Animation();
	Animation idleSwordUp = Animation();
	Animation running = Animation();
	Animation attack = Animation();
	Animation death = Animation();
	Animation falling = Animation();

	EntityData banditData;

	iPoint nextPoint;
	iPoint spawnPos;
	iPoint banditPos;
	iPoint playerPos;
	fPoint playerPosition;

	fPoint spawn;
};

#endif // !_BLACKBANDIT_H_