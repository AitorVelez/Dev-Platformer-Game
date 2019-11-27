#include "ModuleEntities.h"
#include "Entity.h"
#include "Player.h"
#include "BigBat.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Map.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Audio.h"
#include "j1App.h"
#include "ModuleCollision.h"
#include "j1Scene.h"

ModuleEntities::ModuleEntities() 
{
	name.create("entities");
}

ModuleEntities::~ModuleEntities() {}

bool ModuleEntities::Awake(pugi::xml_node& conf)
{
	bool ret = true;
	return ret;
}

bool ModuleEntities::Start() 
{
	return true;
}

bool ModuleEntities::PreUpdate()
{
	p2List_item<Entity*>* item = entities.start;
	while (item != nullptr)
	{
		if (item->data->to_destroy)
		{
			delete item->data;
			entities.del(item);
		}
		item = item->next;
	}
	return true;
}

bool ModuleEntities::Update(float dt)
{
	for (int i = 0; i < entities.count(); ++i)
	{
		if (entities.At(i) != nullptr)
		{
			entities.At(i)->data->Update(dt);
		}
	}
	for (int i = 0; i < entities.count(); ++i)
	{
		if (entities.At(i) != nullptr)
		{
			entities.At(i)->data->Draw();
		}	
	}
	return true;
}

bool ModuleEntities::CleanUp()
{
	p2List_item<Entity*>* item = entities.start;
	while (item != nullptr)
	{
		delete item->data;
		entities.del(item);
		item = item->next;

	}
	return true;
}

bool ModuleEntities::SpawnEntity(int x, int y, ENTITY_TYPE type)
{
	bool ret = false;

	switch (type)
	{
		case PLAYER:
		{
		player = new Player(x, y, PLAYER);
		entities.add(player);
		ret = true;
		break;
		}
		case BIGBAT:
		{
		BigBat* bat = new BigBat(x, y, BIGBAT);
		entities.add(bat);
		ret = true;
		break;
		}
		default:
		{ 
		break;
		}
		
	}

	return ret;
}

Player* ModuleEntities::GetPlayer() const
{
	for (uint i = 0; i < entities.count(); ++i)
	{
		if (entities.At(i) != nullptr)
		{
			if (entities.At(i)->data->type == PLAYER)
			{
				return (Player*)entities.At(i);
			}
		}
	}
	return nullptr;
}

void ModuleEntities::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_ENEMY)
	{
		if (player->is_punching)
		{
			c2->owner->to_destroy = true;
			c2->to_delete = true;
		}
		else
		{
			--App->entities->player->lives;
			if (App->entities->player->lives > 0)
			{
				c1->to_delete = true;
				App->entities->player->SpawnPlayer();
				App->entities->player->collider = App->collision->AddCollider({ (int)App->entities->player->pos.x, (int)App->entities->player->pos.y, 21, 30 }, COLLIDER_TYPE::COLLIDER_PLAYER, App->entities->player, App->entities);
			}				
			else
			{
				App->scene->LoadScene(1);
			}
		}
	}
}