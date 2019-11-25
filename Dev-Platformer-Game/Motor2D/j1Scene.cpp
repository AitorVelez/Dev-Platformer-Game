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
	App->map->Load("Map1.tmx");
	App->audio->PlayMusic("audio/music/CityHeroTheme.ogg");
	App->audio->MusicVolume(App->audio->music_volume);

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

	int camera_speed = App->entities->player->playerData.speed;

	if (App->entities->player->pos.x - (-App->render->camera.x + (App->render->camera.w / 2)) >= 0)
	{
		if (App->render->camera.x - App->render->camera.w > -(App->map->data.width*App->map->data.tile_width))
			App->render->camera.x -= camera_speed;
	}

	if (App->entities->player->pos.x - (-App->render->camera.x + (App->render->camera.w / 3)) <= 0)
	{
		if (App->render->camera.x < 0)
			App->render->camera.x += camera_speed;
	}

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	/*p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);*/
	//p2SString title("City Hero   Level: %i      Music volume: %i / 130", App->scene->current_map, App->audio->music_volume);

	//App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::LoadScene(int map) 
{
	App->map->CleanUp();
	App->tex->FreeTextures();
	//App->player->LoadTexture();
	

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
	}
	/*App->player->FindPlayerSpawn();
	App->player->SpawnPlayer();*/
}

bool j1Scene::Save(pugi::xml_node& data) const 
{
	pugi::xml_node map = data.append_child("Map");

	map.append_attribute("CurrentMap") = current_map;

	return true;
}

bool j1Scene::Load(pugi::xml_node& savegame)
{
	current_map = savegame.child("Map").attribute("CurrentMap").as_int();

	App->map->CleanUp();

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

	return true;

}