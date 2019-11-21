#include "Entity.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "Animation.h"
#include "j1Textures.h"
#include "j1Render.h"

Entity::Entity(int x, int y, ENTITY_TYPE type) :  pos(x, y), type(type) {}


Entity::~Entity()
{
	App->tex->UnLoad(texture);
	App->tex->UnLoad(path_texture);
}

COLLISION_TYPE Entity::CheckCollision(int x) const
{
	p2List_item<MapLayer*>* layer_colliders = App->map->data.layers.end;

	switch (layer_colliders->data->data[x])
	{
	default:
		break;

	case 202:
		return COLLISION_TYPE::GROUND;
		break;

	case 203:
		return COLLISION_TYPE::DEATH;
		break;

	case 270:
		return COLLISION_TYPE::WIN;
		break;
	}

	return COLLISION_TYPE::AIR;
}

void Entity::Draw()
{
	SDL_Rect anim_rect = animation->GetCurrentFrame();
	if (animation != nullptr)
	{
		App->render->Blit(texture, pos.x, pos.y, &anim_rect, 1, flip);
	}
}

int Entity::GetEntityTile(fPoint pos) const
{
	iPoint position = App->map->WorldToMap(pos.x, pos.y);

	int tile_number = position.y * App->map->data.width + position.x;

	return tile_number;
}