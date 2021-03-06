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
#include "j1Window.h"
#include "Player.h"
#include "j1CreditsScene.h"
#include "j1SettingsScene.h"
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

	SDL_Rect button_off_mouse = { 1193,210,168,63 };
	SDL_Rect button_on_mouse = { 1189,286,170,65 };
	SDL_Rect return_rect_on = { 1440,296,142,59 };
	SDL_Rect return_rect_off = { 1440,220,141,59 };

	//background
	background = App->gui->CreateUIImage(0, 0, background_rect, texture, false);

	//play button
	play_button = App->gui->CreateUIButton(App->win->width*0.5-82, 220, button_off_mouse, button_on_mouse, button_off_mouse, texture);

	//start text
	text_start = App->gui->CreateUILabel(App->win->width*0.5 - 40, 245, "START", false);

	//exit button
	exit_button = App->gui->CreateUIButton(App->win->width - 200, 560, return_rect_off, return_rect_on, return_rect_off, texture);

	//exit text
	text_exit = App->gui->CreateUILabel(App->win->width-150, 585, "EXIT", false);

	//credits button
	credits_button = App->gui->CreateUIButton(App->win->width*0.5 - 82, 430, button_off_mouse, button_on_mouse, button_off_mouse, texture);

	//credits text
	text_credits = App->gui->CreateUILabel(App->win->width*0.5 - 40, 455, "CREDITS", false);

	//settings button
	settings_button = App->gui->CreateUIButton(App->win->width*0.5 - 82, 360, button_off_mouse, button_on_mouse, button_off_mouse, texture);

	//settings text
	text_settings = App->gui->CreateUILabel(App->win->width*0.5 - 40, 385, "SETTINGS", false);

	//continue button
	continue_button = App->gui->CreateUIButton(App->win->width*0.5 - 82, 290, button_off_mouse, button_on_mouse, button_off_mouse, texture);

	//continue text
	text_continue = App->gui->CreateUILabel(App->win->width*0.5 - 40, 315, "CONTINUE", false);

	//web button
	//web_button = App->gui->CreateUIButton(App->win->width - 975, 560, return_rect_off, return_rect_on, return_rect_off, texture);

	//exit text
	//text_web = App->gui->CreateUILabel(App->win->width - 950, 585, "WEBSITE", false);

	//title text
	//text_title = App->gui->CreateUILabel(-App->render->camera.x + 30, 30, "C  I  T  Y     H  E  R  O", false);

	//App->startmenu->active = false;
	//App->scene->active = true;
	//App->scene->Start();
	
	App->audio->PlayMusic("audio/music/MenuTheme.ogg");
	buttonfx = App->audio->LoadFx("audio/fx/Button.wav");

	return true;
}

bool j1StartMenu::PreUpdate() {

	return true;
}
bool j1StartMenu::Update(float) {
	BROFILER_CATEGORY("UpdateStartMenu", Profiler::Color::White)

	mouse_pos = App->input->GetMousePosition(mouse_position);

	//check if mouse is on play button
	if (mouse_pos.x > play_button->x&&mouse_pos.x<play_button->x + play_button->button_on.w&&mouse_pos.y>play_button->y&&mouse_pos.y < play_button->y + play_button->button_on.h)
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			App->startmenu->active = false;
			App->scene->active = true;
			App->entities->active = true;				
			App->gui->CleanUp();
			App->gui->HUDCleanUp();
			App->startmenu->CleanUp();
			App->scene->Start();
			App->scene->time_pause = false;
			App->audio->PlayFx(buttonfx);
			//App->entities->player->SpawnPlayer();
			
		
		}
	}
		
	//check if mouse is on settings button
	if (mouse_pos.x > settings_button->x&&mouse_pos.x<settings_button->x + settings_button->button_on.w&&mouse_pos.y>settings_button->y&&mouse_pos.y < settings_button->y + settings_button->button_on.h)
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			App->startmenu->active = false;
			App->settingsscene->active = true;
			App->startmenu->CleanUp();
			App->gui->CleanUp();
			App->gui->HUDCleanUp();
			App->settingsscene->Start();
			App->audio->PlayFx(buttonfx);

		}
	}
	//check if mouse is on credits button
	if (mouse_pos.x > credits_button->x&&mouse_pos.x<credits_button->x + credits_button->button_on.w&&mouse_pos.y>credits_button->y&&mouse_pos.y < credits_button->y + credits_button->button_on.h)
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			App->startmenu->active = false;
			App->creditsscene->active = true;
			App->gui->CleanUp();
			App->gui->HUDCleanUp();
			App->startmenu->CleanUp();
			App->creditsscene->Start();
			App->audio->PlayFx(buttonfx);

		}

	}
	//check if mouse is on continue button plus savegame
	if (mouse_pos.x > continue_button->x&&mouse_pos.x<continue_button->x + continue_button->button_on.w&&mouse_pos.y>continue_button->y&&mouse_pos.y < continue_button->y + continue_button->button_on.h)
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			App->scene->loading = true;
			App->startmenu->active = false;
			App->scene->active = true;
			App->entities->active = true;
			App->startmenu->CleanUp();
			App->scene->Start();			
			App->LoadGame("save_game.xml");
			App->audio->PlayFx(buttonfx);
			App->scene->time_pause = false;		
			App->audio->PlayFx(buttonfx);
		}
	}

		//check if mouse is on exit button
	if (mouse_pos.x > exit_button->x&&mouse_pos.x<exit_button->x + exit_button->button_on.w&&mouse_pos.y>exit_button->y&&mouse_pos.y < exit_button->y + exit_button->button_on.h)
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			App->audio->PlayFx(buttonfx);
			close_game = true;
		}
	}



	return true;
}
bool j1StartMenu::PostUpdate() {

	bool ret = true;

	if (close_game)
		ret = false;

	return ret;

}
bool j1StartMenu::CleanUp() {


	App->gui->CleanUp();


	return true;
}
