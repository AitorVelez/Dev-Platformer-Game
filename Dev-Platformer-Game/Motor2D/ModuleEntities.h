#ifndef _MODULEENTITIES_H_
#define _MODULEENTITIES_H_

#include "j1Module.h"
#include "p2List.h"

class Entity;

class Player;

enum ENTITY_TYPE
{
	NONE,
	PLAYER,
	BLACKBANDIT,
	SKELETON,
	BIGBAT
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

public:
	p2List<Entity*> entities;

	Player* player;
};

#endif // !_MODULEENTITIES_H_
