#include "j1App.h"
#include "j1Module.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "j1StartMenu.h"
#include "UIObject.h"
#include "j1Scene.h"
#include "Entity.h"
#include "j1Audio.h"
#include "p2Log.h"
#include "j1Fonts.h"
#include "Player.h"
#include "Brofiler/Brofiler.h"

j1StartMenu::j1StartMenu() : j1Module() {

	name.create("startmenu");
}


bool j1StartMenu::Awake(pugi::xml_node& cofing) {

	return true;
}
bool j1StartMenu::Start() {

	SDL_Texture* texture = App->gui->atlas;

	SDL_Rect background_rect = { 40,36,1024,768 };

	//background
	background = App->gui->CreateUIImage(0, 0, background_rect, texture, false);

	//App->startmenu->active = false;
	//App->scene->active = true;
	//App->scene->Start();

	return true;
}
bool j1StartMenu::PreUpdate() {

	return true;
}
bool j1StartMenu::Update(float) {
	BROFILER_CATEGORY("UpdateStartMenu", Profiler::Color::White)

	return true;
}
bool j1StartMenu::PostUpdate() {


	return true;

}
bool j1StartMenu::CleanUp() {


	App->gui->CleanUp();


	return true;
}
