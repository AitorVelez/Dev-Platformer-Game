#ifndef _ENTITY_H_
#define	_ENTITY_H_

#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "ModuleEntities.h"
#include "ModuleCollision.h"

class Animation;

enum COLLISION_TYPE
{
	GROUND,
	AIR,
	DEATH,
	WIN
};

struct EntityData
{
	float speed = 2.0f;
	float jumpSpeed = 2.0f;
	float gravity = 3.0f;
};

class Entity 
{

public:

	Entity(int x, int y, ENTITY_TYPE type);

	virtual ~Entity();

	virtual bool Update(float dt) { return true; };

	virtual bool Load(pugi::xml_node& data) { return true; };

	virtual bool Save(pugi::xml_node& data) const { return true; };

	virtual void LoadTexture() {};

	virtual void FindEntitySpawn(ENTITY_TYPE type) {};

	virtual void SpawnEntity() {};

	int GetEntityTile(fPoint pos) const;

	virtual void Draw();

	virtual COLLISION_TYPE CheckCollision(int x) const;

public:

	Animation* animation = nullptr;

	ENTITY_TYPE type = ENTITY_TYPE::NONE;

	SDL_Texture* texture = nullptr;

	SDL_Texture* path_texture = nullptr;

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	fPoint pos = { 0.0f, 0.0f };
	fPoint previousPos = { 0.0f, 0.0f };

	bool to_destroy = false;

	Collider* collider = nullptr;
};

#endif // !_ENTITY_H_