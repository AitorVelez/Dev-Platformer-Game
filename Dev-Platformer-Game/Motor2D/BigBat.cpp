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
	animation = &fly;
	App->render->Blit(texture, pos.x, pos.y, &animation->GetCurrentFrame(), 1.0f, flip);

	return true;
}

bool BigBat::CleanUp()
{
	App->tex->UnLoad(texture);
	animation = nullptr;
	texture = nullptr;
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