#include "j1App.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Window.h"



j1Player::j1Player()
{
	name.create("player");

	pugi::xml_document	config_file;
	pugi::xml_node* node = &App->LoadEntities(config_file);
	node = &node->child("player");

	for (pugi::xml_node animations = node->child("animations").child("animation"); animations; animations = animations.next_sibling("animation"))
	{
		std::string tmp(animations.attribute("name").as_string());

		if (tmp == "idle")
			LoadAnimation(animations, &idle);
		else if (tmp == "running")
			LoadAnimation(animations, &running);
		else if (tmp == "death")
			LoadAnimation(animations, &death);
		else if (tmp == "slide")
			LoadAnimation(animations, &slide);
		else if (tmp == "fall")
			LoadAnimation(animations, &fall);
		else if (tmp == "jumping")
			LoadAnimation(animations, &jumping);
		else if (tmp == "wall_slide")
			LoadAnimation(animations, &wall_slide);
		else if (tmp == "punch1")
			LoadAnimation(animations, &punch1);
		else if (tmp == "punch_barrage")
			LoadAnimation(animations, &punch_barrage);
	}
}


j1Player::~j1Player()
{
}


void j1Player::LoadTexture()
{
	texture = App->tex->Load("textures/Player/player_spritesheet.png");
}

void j1Player::LoadAnimation(pugi::xml_node animation_node, Animation* animation)
{
	bool ret = true;

	for (pugi::xml_node frame = animation_node.child("frame"); frame && ret; frame = frame.next_sibling("frame"))
		animation->PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("w").as_int(), frame.attribute("h").as_int() });

	animation->speed = animation_node.attribute("speed").as_float();
	animation->loop = animation_node.attribute("loop").as_bool();
	animation->offset = animation_node.attribute("offset").as_int();
}