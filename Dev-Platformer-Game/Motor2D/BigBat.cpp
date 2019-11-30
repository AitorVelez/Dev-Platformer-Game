#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "BigBat.h"
#include "j1Window.h"
#include "ModuleEntities.h"
#include "j1Scene.h"
#include "Player.h"
#include "j1Audio.h"
#include "ModulePathfinding.h"

BigBat::BigBat(int x, int y, ENTITY_TYPE type) : Entity(x, y, type)
{
	pos.x = x;
	pos.y = y;
	spawn.x = x;
	spawn.y = y;
	spawnPos = App->map->WorldToMap(x, y);
	pugi::xml_document	config_file;
	pugi::xml_node* node = &App->LoadEntities(config_file);
	node = &node->child("enemies").child("bigBat");

	for (pugi::xml_node animations = node->child("animations").child("animation"); animations; animations = animations.next_sibling("animation"))
	{
		std::string tmp(animations.attribute("name").as_string());

		if (tmp == "fly")
			LoadAnimation(animations, &fly);
		else if (tmp == "death")
			LoadAnimation(animations, &death);

		animation = &fly;
	}
	batData.jumpSpeed = 0.0f;
	batData.speed = 0.0f;

	collider = App->collision->AddCollider({ x, y, 26, 30 }, COLLIDER_ENEMY, this, App->entities);

	Start();
}

BigBat::~BigBat() { CleanUp(); }

bool BigBat::Start()
{
	LoadTexture();
	path_texture = App->tex->Load("textures/cross_texture.png");
	return true;
}

bool BigBat::Update(float dt)
{
	batData.jumpSpeed = 0.0f;
	batData.speed = 0.0f;
	playerPosition = App->entities->player->pos;

	batPos = App->map->WorldToMap(pos.x, pos.y);
	playerPos = App->map->WorldToMap(playerPosition.x, playerPosition.y);

	if (batPos.x < playerPos.x + 8 && batPos.x > playerPos.x - 8 && batPos.y < playerPos.y + 8 && batPos.y > playerPos.y - 8)
	{
		if (App->pathfinding->CreatePath(batPos, playerPos) != -1)
		{
			const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

			if (App->map->draw_logic)
			{
				for (uint i = 0; i < path->Count(); ++i)
				{
					iPoint nextPoint = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
					App->render->Blit(path_texture, nextPoint.x, nextPoint.y);
				}
			}
			if (path->Count() > 0)
			{
				iPoint pathPoint = iPoint(path->At(0)->x, path->At(0)->y);
				if (pathPoint.x < batPos.x)
				{
					flip = SDL_RendererFlip::SDL_FLIP_NONE;
					batData.speed = -70 * dt;
				}
				else if (pathPoint.x > batPos.x)
				{
					flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
					batData.speed = 70 * dt;
				}
				if (pathPoint.y < batPos.y)
				{
					batData.jumpSpeed = -70 * dt;
				}
				else if (pathPoint.y > batPos.y)
				{
					batData.jumpSpeed = 70 * dt;
				}
			}
		}
	}
	else
	{
		if (App->pathfinding->CreatePath(batPos, spawnPos) != -1)
		{
			const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

			if (App->map->draw_logic)
			{
				for (uint i = 0; i < path->Count(); ++i)
				{
					iPoint nextPoint = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
					App->render->Blit(path_texture, nextPoint.x, nextPoint.y);
				}
			}
			if (path->Count() > 0)
			{
				iPoint pathPoint = iPoint(path->At(0)->x, path->At(0)->y);
				if (pathPoint.x < batPos.x)
				{
					flip = SDL_RendererFlip::SDL_FLIP_NONE;
					batData.speed = -70 * dt;
				}
				else if (pathPoint.x > batPos.x)
				{
					flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
					batData.speed = 70 * dt;
				}
				if (pathPoint.y < batPos.y)
				{
					batData.jumpSpeed = -70 * dt;
				}
				else if (pathPoint.y > batPos.y)
				{
					batData.jumpSpeed = 70 * dt;
				}
			}
		}
	}
	pos.x += batData.speed;
	pos.y += batData.jumpSpeed;


	/*if ((animation->GetCurrentFrame().x >= App->entities->player->animation->GetCurrentFrame().x && App->entities->player->animation->GetCurrentFrame().x <= App->entities->player->animation->GetCurrentFrame().x + App->entities->player->animation->GetCurrentFrame().w && animation->GetCurrentFrame().y >= App->entities->player->animation->GetCurrentFrame().y && App->entities->player->animation->GetCurrentFrame().y <= App->entities->player->animation->GetCurrentFrame().y + App->entities->player->animation->GetCurrentFrame().h)
		&& (animation->GetCurrentFrame().x + animation->GetCurrentFrame().w >= App->entities->player->animation->GetCurrentFrame().x && animation->GetCurrentFrame().x + animation->GetCurrentFrame().w <= App->entities->player->animation->GetCurrentFrame().x + App->entities->player->animation->GetCurrentFrame().w && animation->GetCurrentFrame().y >= App->entities->player->animation->GetCurrentFrame().y && animation->GetCurrentFrame().y <= App->entities->player->animation->GetCurrentFrame().y + App->entities->player->animation->GetCurrentFrame().h)
		&& (animation->GetCurrentFrame().x >= App->entities->player->animation->GetCurrentFrame().x && animation->GetCurrentFrame().x <= App->entities->player->animation->GetCurrentFrame().x + App->entities->player->animation->GetCurrentFrame().w && animation->GetCurrentFrame().y + animation->GetCurrentFrame().h >= App->entities->player->animation->GetCurrentFrame().y && animation->GetCurrentFrame().y + animation->GetCurrentFrame().h <= App->entities->player->animation->GetCurrentFrame().y + App->entities->player->animation->GetCurrentFrame().h)
		&& (animation->GetCurrentFrame().x + animation->GetCurrentFrame().w >= App->entities->player->animation->GetCurrentFrame().x && animation->GetCurrentFrame().x + animation->GetCurrentFrame().w <= App->entities->player->animation->GetCurrentFrame().x + App->entities->player->animation->GetCurrentFrame().w && animation->GetCurrentFrame().x + animation->GetCurrentFrame().w >= App->entities->player->animation->GetCurrentFrame().y && animation->GetCurrentFrame().x + animation->GetCurrentFrame().w <= App->entities->player->animation->GetCurrentFrame().y + App->entities->player->animation->GetCurrentFrame().h)
		&& !App->entities->player->playerAttacking && !App->entities->player->god_mode)
	{
		App->audio->PlayFx(2);
		App->entities->player->SpawnPLayer();
	}*/

	/*iPoint contactPointRight = { animation->GetCurrentFrame().x + animation->GetCurrentFrame().w, animation->GetCurrentFrame().y + (animation->GetCurrentFrame().h / 2) };
	iPoint contactPointLeft = { animation->GetCurrentFrame().x, animation->GetCurrentFrame().y + (animation->GetCurrentFrame().h / 2) };*/

	//if (batPos.y == playerPos.y && (batPos.x == playerPos.x + App->entities->player->animation->GetCurrentFrame().w || batPos.x == playerPos.x) && !App->entities->player->god_mode)
	//{
	//	if (!App->entities->player->playerAttacking)
	//	{
	//		App->audio->PlayFx(2);
	//		App->entities->player->SpawnPLayer();
	//		pos = spawn;
	//	}
	//	else to_destroy = true;
	//}

	collider->SetPos(pos.x, pos.y);

	return true;
}

bool BigBat::CleanUp()
{
	App->tex->UnLoad(texture);
	animation = nullptr;
	texture = nullptr;
	collider->to_delete = true;
	return true;
}

bool BigBat::Load(pugi::xml_node& data)
{
	pos.x = data.child("position").attribute("x").as_float();
	pos.y = data.child("position").attribute("y").as_float();

	return true;
}

bool BigBat::Save(pugi::xml_node& data) const
{
	pugi::xml_node position = data.append_child("position");

	position.append_attribute("x") = (float)pos.x;
	position.append_attribute("y") = (float)pos.y;

	return true;
}

void BigBat::LoadTexture()
{
	texture = App->tex->Load("textures/Enemies/Enemy_Bat.png");
}

void BigBat::LoadAnimation(pugi::xml_node animation_node, Animation* animation)
{
	for (pugi::xml_node frame = animation_node.child("frame"); frame; frame = frame.next_sibling("frame"))
		animation->PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("w").as_int(), frame.attribute("h").as_int() });

	animation->speed = animation_node.attribute("speed").as_float();
	animation->loop = animation_node.attribute("loop").as_bool();
	animation->offset = animation_node.attribute("offset").as_int();
	
}