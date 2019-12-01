#include "Player.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "j1Window.h"

Player::Player(int x, int y, ENTITY_TYPE type) : Entity(x, y, type)
{
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
		else if (tmp == "kick1")
			LoadAnimation(animations, &kick1);
	}

	collider = App->collision->AddCollider({ x, y, 21, 30 }, COLLIDER_PLAYER, this, App->entities);

	Start();
}

Player::~Player() { CleanUp(); }

bool Player::Start()
{
	LoadTexture();
	App->audio->LoadFx("audio/fx/Jump.wav");
	App->audio->LoadFx("audio/fx/Die.wav");

	animation = &idle;
	FindPlayerSpawn();
	SpawnPlayer();

	playerData.speed = 100;
	playerData.jumpSpeed = 100;
	playerData.gravity = 150;

	is_jumping = false;
	looking_right = true;

	return true;
}

bool Player::Update(float dt)
{
	animation = &idle;

	float falling_speed = playerData.gravity * dt;
	if (can_jump)
		falling_speed -= 1.5;

	if (looking_right)
		flip = SDL_FLIP_NONE;
	else if (looking_left)
		flip = SDL_FLIP_HORIZONTAL;

	fPoint tempPos = pos;

	if (!god_mode)
	{
		tempPos.y += falling_speed;
		if (CheckCollision(GetPlayerTile({ tempPos.x + 5, tempPos.y + animation->GetCurrentFrame().h })) == COLLISION_TYPE::AIR
			&& CheckCollision(GetPlayerTile({ tempPos.x + 10, tempPos.y + animation->GetCurrentFrame().h })) == COLLISION_TYPE::AIR
			&& !is_jumping)
		{
			can_jump = false;
			is_falling = true;
			pos = tempPos;
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
			if (!god_mode)
			{
				App->audio->PlayFx(2);
				--lives;
				if (lives > 0)
					SpawnPlayer();
				else
				{
					App->scene->LoadScene(1);
				}
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			looking_left = false;
			looking_right = true;

			tempPos = pos;

			tempPos.x += playerData.speed * dt;

			if (CheckCollision(GetPlayerTile({ tempPos.x + animation->GetCurrentFrame().w, tempPos.y })) == COLLISION_TYPE::AIR
				&& CheckCollision(GetPlayerTile({ tempPos.x + animation->GetCurrentFrame().w, tempPos.y + animation->GetCurrentFrame().h })) == COLLISION_TYPE::AIR)
			{
				pos.x = tempPos.x;
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
			tempPos = pos;

			tempPos.x -= playerData.speed * dt;
			if (CheckCollision(GetPlayerTile({ tempPos.x, tempPos.y })) == COLLISION_TYPE::AIR
				&& CheckCollision(GetPlayerTile({ tempPos.x, tempPos.y + animation->GetCurrentFrame().h })) == COLLISION_TYPE::AIR)
			{
				if (tempPos.x >= App->render->camera.x)
					pos.x = tempPos.x;
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
			jumpingCount = 0;
		}
		if (is_jumping)
		{
			tempPos = pos;

			tempPos.y -= playerData.jumpSpeed * dt;
			if (CheckCollision(GetPlayerTile({ tempPos.x + 5, tempPos.y })) == COLLISION_TYPE::AIR
				&& CheckCollision(GetPlayerTile({ tempPos.x + 10, tempPos.y })) == COLLISION_TYPE::AIR)
			{
				if (tempPos.y >= App->render->camera.y)
					pos.y = tempPos.y;
				animation = &jumping;
			}
			if (jumpingCount >= 0.65)
			{
				is_jumping = false;
			}
		}
		// PUNCHES AND KICKS
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN && !is_punching && !is_jumping && !is_falling)
		{
			
			is_punching = true;
			punch1.Reset();
			punch1.ResetLoops();			
		}
		if (is_punching)
		{
			if (flip == SDL_FLIP_HORIZONTAL)
			{
				offset = -13;
				collider->rect.w = 34;
				collider->offset = -13;
			}
			else
			{
				collider->rect.w = 34;
				collider->offset = 0;
			}

			animation = &punch1;
			if (animation->Finished())
			{
				is_punching = false;
				ability_boost = false;
				animation = &idle;
				collider->rect.w = 21;
				collider->offset = 0;
				offset = 0;
			}
		}


		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN && !is_punching && is_falling)   //multiple kicks while falling, need to think about attack design
		{
			is_kicking = true;
			kick1.Reset();
			kick1.ResetLoops();
		}
		if (is_kicking)
		{
			animation = &kick1;

			if (animation->Finished())
			{
				is_kicking = false;
				animation = &idle;
			}
		}
	}

	else if (god_mode)
	{
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			looking_left = false;
			looking_right = true;

			tempPos = pos;

			tempPos.x += playerData.speed;
			pos.x = tempPos.x;

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

			tempPos = pos;

			tempPos.x -= playerData.speed;

			if (tempPos.x >= App->render->camera.x)
				pos.x = tempPos.x;

			animation = &running;
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			tempPos = pos;

			tempPos.y -= playerData.speed;

			if (tempPos.y >= App->render->camera.y)
				pos.y = tempPos.y;
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			tempPos = pos;

			tempPos.y += playerData.speed;

			if (tempPos.y + animation->GetCurrentFrame().h <= (App->render->camera.y + App->win->height))
				pos.y = tempPos.y;
		}
	}
	jumpingCount += dt;

	collider->SetPos(pos.x, pos.y);

	return true;
}

void Player::LoadTexture()
{
	texture = App->tex->Load("textures/Player/player_spritesheet.png");
}

void Player::LoadAnimation(pugi::xml_node animation_node, Animation* animation)
{
	bool ret = true;

	for (pugi::xml_node frame = animation_node.child("frame"); frame && ret; frame = frame.next_sibling("frame"))
		animation->PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("w").as_int(), frame.attribute("h").as_int() });

	animation->speed = animation_node.attribute("speed").as_float();
	animation->loop = animation_node.attribute("loop").as_bool();
	animation->offset = animation_node.attribute("offset").as_int();
}

void Player::FindPlayerSpawn()
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

void Player::SpawnPlayer()
{
	pos.x = spawn_pos.x;
	pos.y = spawn_pos.y;
	App->render->camera.x = 0;
	App->scene->camPos = 0.0f;
	collider->SetPos(pos.x, pos.y);
}

int Player::GetPlayerTile(fPoint pos) const
{
	iPoint position = App->map->WorldToMap(pos.x, pos.y);

	int tile_number = position.y * App->map->data.width + position.x;

	return tile_number;
}

bool Player::Load(pugi::xml_node& data)
{
	pos.x = data.child("position").attribute("x").as_float();
	pos.y = data.child("position").attribute("y").as_float();

	return true;
}

bool Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node position = data.append_child("position");

	position.append_attribute("x") = pos.x;
	position.append_attribute("y") = pos.y;

	return true;
}

bool Player::CleanUp()
{
	App->tex->UnLoad(texture);
	animation = nullptr;
	texture = nullptr;
	collider->to_delete = true;
	return true;	
}