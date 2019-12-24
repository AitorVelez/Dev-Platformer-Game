#include "UIButton.h"
#include "UIObject.h"
#include "j1App.h"
#include "j1StartMenu.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Input.h"
#include "p2Log.h"
#include "Brofiler/Brofiler.h"

UIButton::UIButton(int x, int y, SDL_Rect mouse_off, SDL_Rect mouse_on, SDL_Rect mouse_click, SDL_Texture* tex) :UIObject(x, y)
{
	this->x = x;
	this->y = y;
	this->button_off = mouse_off;
	this->button_on = mouse_on;
	this->button_clicked = mouse_click;
	this->texture = tex;
}

UIButton::~UIButton() {}

void UIButton::Update()
{
	BROFILER_CATEGORY("UpdateUIButton", Profiler::Color::Yellow)

	mouse_pos = App->input->GetMousePosition(mouse_position);


	//button_start position

	if (mouse_pos.x >= x && mouse_pos.x <= x + button_off.w && mouse_pos.y >= y && mouse_pos.y <= y + button_off.h) {

		current_rect = button_off;
		is_on = false;

	}
	else
	{
		current_rect = button_on;
		is_on = true;
	}

}

void UIButton::Draw()
{
	BROFILER_CATEGORY("DrawUIButton", Profiler::Color::Brown)

		App->render->Blit(texture, x, y, &current_rect, 0);

	




}
