#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "ModuleEntities.h"
#include "Player.h"
#include "ModulePathfinding.h"
#include "ModulePathfindingWalker.h"
#include "UIButton.h"
#include "UIImage.h"
#include "UILabel.h"
#include "j1Gui.h"
#include "j1StartMenu.h"
#include <chrono>
#include <time.h>
#include <ctime>
#include <iostream>

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	if (App->scene->active)
	{

		App->map->Load("Map1.tmx");

		timer.Start();

		camerafx = App->audio->LoadFx("audio/fx/Camera.wav");
		photofx = App->audio->LoadFx("audio/fx/Photo.wav");

		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data, true))
			App->pathfinding->SetMap(w, h, data);

		int w2, h2;
		uchar* data2 = NULL;
		if (App->map->CreateWalkabilityMap(w2, h2, &data2, false))
			App->pathfindingWalker->SetMap(w2, h2, data2);

		RELEASE_ARRAY(data);

		App->audio->PlayMusic("audio/music/CityHeroTheme.ogg");
		App->audio->MusicVolume(App->audio->music_volume);

		if (!loading)
		{
			iPoint spawnEntity;
			p2List_item<MapLayer*>* layer = App->map->data.layers.end;
			for (int i = 0; i < (layer->data->width * layer->data->height); i++)
			{
				if (layer->data->data[i] == 204)
				{
					spawnEntity = App->map->TileToWorld(i);
					App->entities->SpawnEntity(spawnEntity.x, spawnEntity.y, PLAYER);		
				}
				else if (layer->data->data[i] == 269)
				{
					spawnEntity = App->map->TileToWorld(i);
					App->entities->SpawnEntity(spawnEntity.x, spawnEntity.y, BIGBAT);
				}
				else if (layer->data->data[i] == 336)
				{
					spawnEntity = App->map->TileToWorld(i);
					App->entities->SpawnEntity(spawnEntity.x, spawnEntity.y, WALKING_ENEMY);
				}
				else if (layer->data->data[i] == 271)
				{
					spawnEntity = App->map->TileToWorld(i);
					App->entities->SpawnEntity(spawnEntity.x, spawnEntity.y, COIN);
				}
			}
		}

		texture = App->gui->atlas;

		//background_rect = { 1944,847,1022,7680 };
		button_off_mouse = { 1193,210,168,63 };
		button_on_mouse = { 1189,286,170,65 };
	
		camPos = App->render->camera.x;
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (!pause_menu)
	{
		cont_timer = time_start + timer.ReadSec() - cont_pause_timer;
	}
	//get mouse pos
	mouse_pos = App->input->GetMousePosition(mouse_position);

	//start from first level
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		LoadScene(1);
	}

	//start from second level
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) 
	{
		LoadScene(2);
	}

	//start from current level
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		LoadScene(current_map);
	}

	//save game
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		App->SaveGame("save_game.xml");
	}

	//load game
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		loading = true;
		App->LoadGame("save_game.xml");
	}

	//volume down
	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		change_volume = false;
		App->audio->ChangeVolume(change_volume);
	}

	//volume up
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) 
	{
		change_volume = true;
		App->audio->ChangeVolume(change_volume);
	}

	//draw logic
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) 
	{
		App->map->draw_logic = !App->map->draw_logic;
	}

	//activate/disactivate godmode
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) 
	{
		App->entities->player->god_mode = !App->entities->player->god_mode;
	}

	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN && !pause_menu)
	{
		App->audio->PlayFx(camerafx);
		foto_mode = !foto_mode;
	}

	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	/*if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 1;*/

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 1;

	App->map->Draw();

	float camera_speed = App->entities->player->playerData.speed * dt;

	if (App->entities->player->pos.x - (-App->render->camera.x + (App->render->camera.w / 2)) >= 0)
	{
		if (camPos - App->render->camera.w > -(App->map->data.width * App->map->data.tile_width))
			camPos -= camera_speed;
	}

	if (App->entities->player->pos.x - (-App->render->camera.x + (App->render->camera.w / 3)) <= 0)
	{
		if (camPos < 0)
			camPos += camera_speed;
	}

	//PAUSE
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && !pause_menu && time_pause == false)
	{
		pause_menu = true;
		time_pause = true;

		App->entities->active = false;

		//resume button
		resume_button = App->gui->CreateUIButton(400, 200, button_off_mouse, button_on_mouse, button_off_mouse, texture);

		//resume text
		text_resume = App->gui->CreateUILabel(-App->render->camera.x + 450, 225, "RESUME", false);

		//exit button
		exit_button = App->gui->CreateUIButton(400, 360, button_off_mouse, button_on_mouse, button_off_mouse, texture);

		//exit text
		text_exit = App->gui->CreateUILabel(-App->render->camera.x + 415, 385, "SAVE & EXIT", false);

		//main menu button
		main_menu_button = App->gui->CreateUIButton(400, 280, button_off_mouse, button_on_mouse, button_off_mouse, texture);


		//main menu text
		text_main_menu = App->gui->CreateUILabel(-App->render->camera.x + 430, 305, "MAIN MENU", false);

		pause_timer.Start();
	}

	//PAUSE BUTTONS FUNCTIONALITY
	if (App->entities->active == false) 
	{
		//check if mouse is on resume button
		if (mouse_pos.x > resume_button->x&&mouse_pos.x<resume_button->x + resume_button->button_on.w&&mouse_pos.y>resume_button->y&&mouse_pos.y < resume_button->y + resume_button->button_on.h)
		{
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				pause_menu = false;
				App->gui->CleanUp();
				App->entities->active = true;
				time_pause = false;
				cont_pause_timer += pause_timer.ReadSec();
			}
		}
		//check if mouse is on exti button
		if (mouse_pos.x > exit_button->x&&mouse_pos.x<exit_button->x + exit_button->button_on.w&&mouse_pos.y>exit_button->y&&mouse_pos.y < exit_button->y + exit_button->button_on.h)
		{
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				App->SaveGame("save_game.xml");
				close_game = true;
				cont_pause_timer = 0.0f;
			}
		}
		//check if mouse is on main_menu button
		if (mouse_pos.x > main_menu_button->x&&mouse_pos.x<main_menu_button->x + main_menu_button->button_on.w&&mouse_pos.y>main_menu_button->y&&mouse_pos.y < main_menu_button->y + main_menu_button->button_on.h)
		{
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				time_pause = true;
				pause_menu = false;
				App->scene->active = false;
				App->startmenu->active = true;
				App->startmenu->Start();
				App->scene->CleanUp();
				App->gui->HUDCleanUp();
				App->entities->CleanUp();
				cont_pause_timer = 0.0f;
			}
		}
	}

	int x = 0;
	int y = 0;
	//App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);

	App->render->camera.x = camPos;

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN && foto_mode)
	{
		SDL_RenderReadPixels(App->render->renderer, NULL, SDL_PIXELFORMAT_ARGB8888, App->win->screen_surface->pixels, App->win->screen_surface->pitch);
		auto now = std::chrono::system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(now);
		tm time;
		localtime_s(&time, &now_c);

		int secs = time.tm_sec;
		int mins = time.tm_min;
		int hour = time.tm_hour;
		int day = time.tm_mday;
		int month = time.tm_mon + 1;
		int year = time.tm_year + 1900;

		static char photo_name[60];

		App->audio->PlayFx(photofx);

		sprintf_s(photo_name, 60, "screenshots/City Hero on %i-%02i-%02i at %02i.%02i.%02i.png", year, month, day, hour, mins, secs);

		SDL_SaveBMP(App->win->screen_surface, photo_name);
	}
	if (close_game)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	/*delete heart1;
	delete heart2;
	delete heart3;*/
	LOG("Freeing scene");

	return true;
}

void j1Scene::LoadScene(int map) 
{
	timer.Start();
	cont_pause_timer = 0.0f;
	App->map->CleanUp();
	App->tex->FreeTextures();
	App->entities->CleanUp();
	//App->player->LoadTexture();	

	App->gui->atlas = App->tex->Load(App->gui->atlas_file_name.GetString());
	texture = App->gui->atlas;

	if (map == 1) 
	{
		App->map->Load("Map1.tmx");
		current_map = 1;
	}
	else if (map == 2)
	{
		App->map->Load("Map2.tmx");
		current_map = 2;
	}

	if (!loading)
	{
		iPoint spawnEntity;
		p2List_item<MapLayer*>* layer = App->map->data.layers.end;
		for (int i = 0; i < (layer->data->width * layer->data->height); i++)
		{
			if (layer->data->data[i] == 204)
			{
				spawnEntity = App->map->TileToWorld(i);
				App->entities->SpawnEntity(spawnEntity.x, spawnEntity.y, PLAYER);
			}
			else if (layer->data->data[i] == 269)
			{
				spawnEntity = App->map->TileToWorld(i);
				App->entities->SpawnEntity(spawnEntity.x, spawnEntity.y, BIGBAT);
			}
			else if (layer->data->data[i] == 336)
			{
				spawnEntity = App->map->TileToWorld(i);
				App->entities->SpawnEntity(spawnEntity.x, spawnEntity.y, WALKING_ENEMY);
			}
			else if (layer->data->data[i] == 271)
			{
				spawnEntity = App->map->TileToWorld(i);
				App->entities->SpawnEntity(spawnEntity.x, spawnEntity.y, COIN);
			}
		}
	}
	/*if (App->entities->player->lives >= 1)
		App->scene->heart1 = App->gui->CreateUIImage(800, 20, { 1565, 489, 36, 32 }, NULL, this);

	if (App->entities->player->lives >= 2)
		App->scene->heart2 = App->gui->CreateUIImage(850, 20, { 1565, 489, 36, 32 }, NULL, this);

	if (App->entities->player->lives >= 3)
		App->scene->heart3 = App->gui->CreateUIImage(900, 20, { 1565, 489, 36, 32 }, NULL, this);*/
	/*App->player->FindPlayerSpawn();
	App->player->SpawnPlayer();*/
	//loading = false;
}

bool j1Scene::Save(pugi::xml_node& data) const 
{
	pugi::xml_node map = data.append_child("Map");

	pugi::xml_node time = data.append_child("Time");

	map.append_attribute("CurrentMap") = current_map;
	time.append_attribute("CurrentTime") = cont_timer;

	for (p2List_item<Entity*>* entity = App->entities->entities.start; entity != App->entities->entities.end; entity = entity->next)
	{
		entity->data->Save(data);
	}

	return true;
}

bool j1Scene::Load(pugi::xml_node& savegame)
{
	current_map = savegame.child("Map").attribute("CurrentMap").as_int();
	time_start = savegame.child("Time").attribute("CurrentTime").as_float();

	App->map->CleanUp();
	App->tex->FreeTextures();
	App->entities->CleanUp();

	switch (current_map)
	{
	case 1:
		LoadScene(1);
		break;
	case 2:
		LoadScene(2);
		break;
	default:
		break;
	}

	for (pugi::xml_node it = savegame.child("entity"); it.empty() == false; it = it.next_sibling())
	{
		ENTITY_TYPE type = (ENTITY_TYPE)it.attribute("type").as_int();		

		iPoint spawnEntity;
		spawnEntity.x = it.attribute("x").as_int();
		spawnEntity.y = it.attribute("y").as_int();

		switch (type)
		{
		case NONE:
			break;
		case PLAYER:
			App->entities->SpawnEntity(spawnEntity.x, spawnEntity.y, PLAYER);
			break;
		case WALKING_ENEMY:
			App->entities->SpawnEntity(spawnEntity.x, spawnEntity.y, WALKING_ENEMY);
			break;
		case BIGBAT:
			App->entities->SpawnEntity(spawnEntity.x, spawnEntity.y, BIGBAT);
			break;
		case COIN:
			App->entities->SpawnEntity(spawnEntity.x, spawnEntity.y, COIN);
			break;
		default:
			break;
		}
	}

	/*ENTITY_TYPE type = (ENTITY_TYPE)savegame.child("entity").attribute("type").as_int();

	iPoint spawnEntity;
	spawnEntity.x = savegame.child("entity").attribute("x").as_int();
	spawnEntity.y = savegame.child("entity").attribute("y").as_int();

	switch (type)
	{
	case NONE:
		break;
	case PLAYER:
		App->entities->SpawnEntity(spawnEntity.x, spawnEntity.y, PLAYER);
		break;
	case WALKING_ENEMY:
		App->entities->SpawnEntity(spawnEntity.x, spawnEntity.y, WALKING_ENEMY);
		break;
	case BIGBAT:
		App->entities->SpawnEntity(spawnEntity.x, spawnEntity.y, BIGBAT);
		break;
	case COIN:
		App->entities->SpawnEntity(spawnEntity.x, spawnEntity.y, COIN);
		break;
	default:
		break;
	}*/

	/*for (p2List_item<Entity*>* entity = App->entities->entities.start; entity != App->entities->entities.end; entity = entity->next)
	{
		entity->data->Load(savegame);
	}*/

	return true;
}