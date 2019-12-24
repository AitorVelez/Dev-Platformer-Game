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
	SDL_Rect return_rect_on = { 1440,296,142,59 };
	SDL_Rect return_rect_off = { 1440,220,141,59 };

	//background
	background = App->gui->CreateUIImage(0, 0, back_rect, texture, false);

	//return button
	return_button = App->gui->CreateUIButton(20, 550, return_rect_off, return_rect_on, return_rect_off, texture);

	//return to menu label
	menu_text = App->gui->CreateUILabel(-App->render->camera.x + 65, 575, "MENU", false);

	//github repository button
	github_button = App->gui->CreateUIButton(20, 350, return_rect_off, return_rect_on, return_rect_off, texture);

	//github label
	github_label = App->gui->CreateUILabel(-App->render->camera.x + 32, 375, "REPOSITORY", false);

	return true;
}

bool j1CreditsScene::PreUpdate()
{
	return true;
}

bool j1CreditsScene::Update(float)
{
	BROFILER_CATEGORY("UpdateCreditsScene", Profiler::Color::BurlyWood)

	mouse_pos = App->input->GetMousePosition(mouse_position);

		//check if mouse is on menu button
	if (mouse_pos.x > return_button->x&&mouse_pos.x<return_button->x + return_button->button_on.w&&mouse_pos.y>return_button->y&&mouse_pos.y < return_button->y + return_button->button_on.h)
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
				App->creditsscene->active = false;
				App->startmenu->active = true;
				App->startmenu->Start();
				App->creditsscene->CleanUp();
		}
	}

	//check if mouse is on repository button
	if (mouse_pos.x > github_button->x&&mouse_pos.x<github_button->x + github_button->button_on.w&&mouse_pos.y>github_button->y&&mouse_pos.y < github_button->y + github_button->button_on.h)
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			//go to the link
			system("start https://github.com/AitorVelez/Dev-Platformer-Game");
		}
	}

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