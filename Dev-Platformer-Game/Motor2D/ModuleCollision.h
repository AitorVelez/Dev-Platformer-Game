#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#define MAX_COLLIDERS 200

#include "j1Module.h"
#include "SDL/include/SDL_rect.h"

class Entity;

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_PLAYER,
	COLLIDER_ENEMY,

	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;
	j1Module* callback = nullptr;
	int life_units;

	int offset = 0;

	Entity* owner = nullptr;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, Entity* owner, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback),
		owner(owner)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

class ModuleCollision : public j1Module
{
public:

	ModuleCollision();
	~ModuleCollision();

	bool Start();

	virtual bool  PreUpdate();
	virtual bool Update(float dt);
	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, Entity* owner, j1Module* callback = nullptr);
	bool EraseCollider(Collider* collider);
	void DebugDraw();
	bool debug = false;
private:

	Collider* colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];

};

#endif // __ModuleCollision_H__