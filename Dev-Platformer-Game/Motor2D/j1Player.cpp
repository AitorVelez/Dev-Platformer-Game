#include "j1App.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Window.h"
#include "p2Log.h"



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


bool j1Player::Start()
{
	LoadTexture();
	App->audio->LoadFx("audio/fx/Jump.wav");
	App->audio->LoadFx("audio/fx/Die.wav");

	animation = &idle;
	FindPlayerSpawn();
	SpawnPlayer();

	is_jumping = false;
	looking_right = true;

	LOG("PLAYER SPAWN: %u %u", player.position.x, player.position.y);

	return true;
}

bool j1Player::Update(float dt)
{
	animation = &idle;

	float falling_speed = player.gravity;
	if (can_jump)
		falling_speed -= 1.5;

	if (looking_right)
		flip = SDL_FLIP_NONE;
	else if (looking_left)
		flip = SDL_FLIP_HORIZONTAL;

	fPoint tempPos = player.position;

	if (!god_mode)
	{
		tempPos.y += falling_speed;
		if (CheckCollision(GetPlayerTile({ tempPos.x + 5, tempPos.y + animation->GetCurrentFrame().h })) == COLLISION_TYPE::AIR
			&& CheckCollision(GetPlayerTile({ tempPos.x + 10, tempPos.y + animation->GetCurrentFrame().h })) == COLLISION_TYPE::AIR
			&& !is_jumping)
		{
			can_jump = false;
			is_falling = true;
			player.position = tempPos;
			if (!can_jump)
				animation = &fall;
		}
		else
		{
			is_falling = false;
			can_jump = true;
		}

		if (CheckCollision(GetPlayerTile({ tempPos.x + 5, tempPos.y + animation->GetCurrentFrame().h })) == COLLISION_TYPE::DEATH
			&& CheckCollision(GetPlayerTile({ tempPos.x + 10, tempPos.y + animation->GetCurrentFrame().h })) == COLLISION_TYPE::DEATH)
		{
			App->audio->PlayFx(2);
			SpawnPlayer();
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			looking_left = false;
			looking_right = true;

			tempPos = player.position;

			tempPos.x += player.speed;

			if (CheckCollision(GetPlayerTile({ tempPos.x + animation->GetCurrentFrame().w, tempPos.y })) == COLLISION_TYPE::AIR
				&& CheckCollision(GetPlayerTile({ tempPos.x + animation->GetCurrentFrame().w, tempPos.y + animation->GetCurrentFrame().h })) == COLLISION_TYPE::AIR)
			{
				player.position.x = tempPos.x;
				if (is_falling == false)
					animation = &running;
			}
			else if (CheckCollision(GetPlayerTile({ tempPos.x + animation->GetCurrentFrame().w, tempPos.y })) == COLLISION_TYPE::GROUND
				&& CheckCollision(GetPlayerTile({ tempPos.x + animation->GetCurrentFrame().w, tempPos.y + animation->GetCurrentFrame().h })) == COLLISION_TYPE::GROUND
				&& is_falling)
			{
				animation = &wall_slide;
				can_jump = true;
			}

			else if (CheckCollision(GetPlayerTile({ tempPos.x + animation->GetCurrentFrame().w, tempPos.y })) == COLLISION_TYPE::WIN
				&& CheckCollision(GetPlayerTile({ tempPos.x + animation->GetCurrentFrame().w, tempPos.y + animation->GetCurrentFrame().h })) == COLLISION_TYPE::WIN)
			{
				if (App->scene->current_map == 1)
				{
					App->scene->LoadScene(2);
					App->scene->current_map = 2;
				}
				else
				{
					App->scene->LoadScene(1);
					App->scene->current_map = 1;
				}
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			looking_left = true;
			looking_right = false;
			tempPos = player.position;

			tempPos.x -= player.speed;
			if (CheckCollision(GetPlayerTile({ tempPos.x, tempPos.y })) == COLLISION_TYPE::AIR
				&& CheckCollision(GetPlayerTile({ tempPos.x, tempPos.y + animation->GetCurrentFrame().h })) == COLLISION_TYPE::AIR)
			{
				if (tempPos.x >= App->render->camera.x)
					player.position.x = tempPos.x;
				if (is_falling == false)
					animation = &running;
			}
			else if (CheckCollision(GetPlayerTile({ tempPos.x, tempPos.y })) == COLLISION_TYPE::GROUND
				&& CheckCollision(GetPlayerTile({ tempPos.x, tempPos.y + animation->GetCurrentFrame().h })) == COLLISION_TYPE::GROUND
				&& is_falling)
			{
				animation = &wall_slide;
				can_jump = true;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && is_jumping == false && can_jump)
		{
			App->audio->PlayFx(1);
			can_jump = false;
			jumping.Reset();
			is_jumping = true;
			cont = 0;
		}
		if (is_jumping)
		{
			tempPos = player.position;

			tempPos.y -= player.jumpSpeed;
			if (CheckCollision(GetPlayerTile({ tempPos.x + 5, tempPos.y })) == COLLISION_TYPE::AIR
				&& CheckCollision(GetPlayerTile({ tempPos.x + 10, tempPos.y })) == COLLISION_TYPE::AIR)
			{
				if (tempPos.y >= App->render->camera.y)
					player.position.y = tempPos.y;
				animation = &jumping;
			}
			if (cont == 35)
			{
				is_jumping = false;
			}
		}
	}

	else if (god_mode)
	{
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			looking_left = false;
			looking_right = true;

			tempPos = player.position;

			tempPos.x += player.speed;
			player.position.x = tempPos.x;

			if (CheckCollision(GetPlayerTile({ tempPos.x + animation->GetCurrentFrame().w, tempPos.y })) == COLLISION_TYPE::WIN
				&& CheckCollision(GetPlayerTile({ tempPos.x + animation->GetCurrentFrame().w, tempPos.y + animation->GetCurrentFrame().h })) == COLLISION_TYPE::WIN)
			{
				if (App->scene->current_map == 1)
				{
					App->scene->LoadScene(2);
					App->scene->current_map = 2;
				}
				else
				{
					App->scene->LoadScene(1);
					App->scene->current_map = 1;
				}
			}

			animation = &running;
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			looking_left = true;
			looking_right = false;

			tempPos = player.position;

			tempPos.x -= player.speed;

			if (tempPos.x >= App->render->camera.x)
				player.position.x = tempPos.x;

			animation = &running;
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			tempPos = player.position;

			tempPos.y -= player.speed;

			if (tempPos.y >= App->render->camera.y)
				player.position.y = tempPos.y;
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			tempPos = player.position;

			tempPos.y += player.speed;

			if (tempPos.y + animation->GetCurrentFrame().h <= (App->render->camera.y + App->win->height))
				player.position.y = tempPos.y;
		}
	}

	App->render->Blit(texture, player.position.x, player.position.y, &animation->GetCurrentFrame(), 1.0f, flip);
	cont++;
	return true;
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

void j1Player::FindPlayerSpawn()
{
	p2List_item<MapLayer*>* layer = App->map->data.layers.end;
	for (int i = 0; i < (layer->data->width * layer->data->height); i++)
	{
		if (layer->data->data[i] == 204)
		{
			spawn_pos = App->map->TileToWorld(i);
		}
	}
}

void j1Player::SpawnPlayer() 
{
	player.position.x = spawn_pos.x;
	player.position.y = spawn_pos.y;
	App->render->camera.x = 0;
}

COLLISION_TYPE j1Player::CheckCollision(int x) const
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

int j1Player::GetPlayerTile(fPoint pos) const
{
	iPoint position = App->map->WorldToMap(pos.x, pos.y);

	int tile_number = position.y * App->map->data.width + position.x;

	return tile_number;
}

bool j1Player::Load(pugi::xml_node& data)
{
	player.position.x = data.child("position").attribute("x").as_float();
	player.position.y = data.child("position").attribute("y").as_float();

	return true;
}

bool j1Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node position = data.append_child("position");

	position.append_attribute("x") = (float)player.position.x;
	position.append_attribute("y") = (float)player.position.y;

	return true;
}

bool j1Player::CleanUp()
{
	App->tex->UnLoad(texture);
	return true;
}