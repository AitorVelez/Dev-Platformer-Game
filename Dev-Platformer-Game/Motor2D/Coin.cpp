#include "Coin.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Textures.h"

Coin::Coin(int x, int y, ENTITY_TYPE type) : Entity(x, y, type)
{
	pos.x = x;
	pos.y = y;
	spawn.x = x;
	spawn.y = y;
	spawnPos = App->map->WorldToMap(x, y);
	pugi::xml_document	config_file;
	pugi::xml_node* node = &App->LoadEntities(config_file);
	node = &node->child("enemies").child("coin");

	for (pugi::xml_node animations = node->child("animations").child("animation"); animations; animations = animations.next_sibling("animation"))
	{
		std::string tmp(animations.attribute("name").as_string());

		if (tmp == "coinRot")
			LoadAnimation(animations, &coinRot);

		animation = &coinRot;
	}

	collider = App->collision->AddCollider({ x, y, 32, 32 }, COLLIDER_COIN, this, App->entities);

	Start();
}


Coin::~Coin() { CleanUp(); }

bool Coin::Start()
{
	LoadTexture();
	return true;
}

bool Coin::CleanUp()
{
	App->tex->UnLoad(texture);
	animation = nullptr;
	texture = nullptr;
	return true;
}

bool Coin::Load(pugi::xml_node & data)
{
	pos.x = data.child("position").attribute("x").as_float();
	pos.y = data.child("position").attribute("y").as_float();

	return false;
}

bool Coin::Save(pugi::xml_node & data) const
{
	pugi::xml_node position = data.append_child("position");

	position.append_attribute("x") = (float)pos.x;
	position.append_attribute("y") = (float)pos.y;

	return false;
}

void Coin::LoadTexture()
{
	texture = App->tex->Load("textures/Enemies/coin_spritesheet.png");
}

void Coin::LoadAnimation(pugi::xml_node animation_node, Animation * animation)
{
	for (pugi::xml_node frame = animation_node.child("frame"); frame; frame = frame.next_sibling("frame"))
		animation->PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("w").as_int(), frame.attribute("h").as_int() });

	animation->speed = animation_node.attribute("speed").as_float();
	animation->loop = animation_node.attribute("loop").as_bool();
	animation->offset = animation_node.attribute("offset").as_int();
}
