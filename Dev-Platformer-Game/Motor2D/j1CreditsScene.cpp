#include "j1CreditsScene.h"
#include "j1StartMenu.h"
#include "UIButton.h"
#include "UIObject.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "Brofiler/Brofiler.h"

j1CreditsScene::j1CreditsScene()
{
	name.create("credits_scene");
}

bool j1CreditsScene::Awake(pugi::xml_node &)
{
	return true;
}

bool j1CreditsScene::Start()
{
	SDL_Texture* texture = App->gui->atlas;

	SDL_Rect back_rect = { 39,845,1024,770 };

	//background
	background = App->gui->CreateUIImage(0, 0, back_rect, texture, false);

	return true;
}

bool j1CreditsScene::PreUpdate()
{
	return true;
}

bool j1CreditsScene::Update(float)
{
	BROFILER_CATEGORY("UpdateCreditsScene", Profiler::Color::BurlyWood)

	return true;
}

bool j1CreditsScene::PostUpdate()
{
	return true;
}

bool j1CreditsScene::CleanUp()
{
	return true;
}