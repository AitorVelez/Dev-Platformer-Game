#ifndef _BIGBAT_H__
#define _BIGBAT_H__

#include "Entity.h"
#include "ModuleEntities.h"
#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"
#include "j1Render.h"

struct SDL_Texture;

class BigBat : public Entity
{
public:

	BigBat(int x, int y, ENTITY_TYPE type);
	~BigBat();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	bool Load(pugi::xml_node& data);

	bool Save(pugi::xml_node& data) const;

	void LoadTexture();

	void LoadAnimation(pugi::xml_node animation_node, Animation* animation);

public:
	Animation fly = Animation();
	//Animation idleSwordUp = Animation();
	//Animation running = Animation();
	//Animation attack = Animation();
	Animation death = Animation();
	//Animation falling = Animation();

	EntityData batData;

	iPoint batPos;
	iPoint playerPos;
	fPoint playerPosition;
	iPoint nextPoint;
	iPoint spawnPos;

	fPoint spawn;
};

#endif // !_BIGBAT_H__
