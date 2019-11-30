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
	Animation death = Animation();

	EntityData batData;

	iPoint nextPoint;
	iPoint spawnPos;

	iPoint batPos;
	iPoint playerPos;
	fPoint playerPosition;

	fPoint spawn;
};

#endif // !_BIGBAT_H__
