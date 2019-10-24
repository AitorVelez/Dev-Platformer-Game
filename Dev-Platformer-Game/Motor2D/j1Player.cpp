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
}


j1Player::~j1Player()
{
}


void j1Player::LoadTexture()
{
	texture = App->tex->Load("textures/Player/player_spritesheet.png");
}

