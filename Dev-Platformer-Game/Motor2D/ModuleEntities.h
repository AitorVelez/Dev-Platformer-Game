#ifndef _MODULEENTITIES_H_
#define _MODULEENTITIES_H_

#include "j1Module.h"
#include "p2List.h"

class Entity;

class Player;

struct Collider;

enum ENTITY_TYPE
{
	NONE,
	PLAYER,
	WALKING_ENEMY,
	BIGBAT,
	COIN
};

class ModuleEntities : public j1Module
{

public:
	ModuleEntities();
	~ModuleEntities();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

	bool SpawnEntity(int x, int y, ENTITY_TYPE type);

	Player* GetPlayer() const;

	void OnCollision(Collider* c1, Collider* c2);

public:
	p2List<Entity*> entities;

	Player* player;

};

#endif // !_MODULEENTITIES_H_
