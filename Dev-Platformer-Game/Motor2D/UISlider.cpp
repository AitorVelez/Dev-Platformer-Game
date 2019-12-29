#include "UISlider.h"
#include "UIObject.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "UIObject.h"
#include "j1Render.h"
#include "j1StartMenu.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "Brofiler/Brofiler.h"

#define MAX_VOL 128
#define VOL_CORRECTION 40

UISlider::UISlider(int x, int y, SDL_Rect thumb_off, SDL_Rect thumb_on, int margin_right, int margin_left, SDL_Texture *tex, UIObject* object) :UIObject(x, y)
{
	this->x = x;
	this->y = y;
	this->button_off = thumb_off;
	this->button_on = thumb_on;
	this->texture = tex;
	this->margin_left = margin_left;
	this->margin_right = margin_right;
	this->object = object;

}

UISlider::~UISlider() {}

void UISlider::Update()
{
	BROFILER_CATEGORY("UISliderUpdate", Profiler::Color::Blue)
		//get mouse pos
		mouse_pos = App->input->GetMousePosition(mouse_position);
	//check pos
	if (mouse_pos.x >= x && mouse_pos.x <= x + button_off.w && mouse_pos.y >= y && mouse_pos.y <= y + button_off.h)
	{
		current_rect = button_off;
		if ((App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) && x<margin_right - button_off.w && x>margin_left)
		{
			current_rect = button_on;
			x = mouse_pos.x - 40;
		}
		else if ((App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) && x >= margin_right - button_off.w)
		{
			x -= 1;
		}
		else if ((App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) && x <= margin_left)
		{
			x += 1;
		}

	}
	else
	{
		x_difference = mouse_pos.x - x;
		current_rect = button_on;

	}

	//new music volume
	slider_volume = ((object->x + MAX_VOL)*x) / (object->x + object->thumb_rect_off.w);
	final_volume = slider_volume - object->x;
	Mix_VolumeMusic(final_volume - VOL_CORRECTION);

}

void UISlider::Draw()
{
	BROFILER_CATEGORY("UISliderDraw", Profiler::Color::Beige)

		App->render->Blit(texture, x, y, &current_rect, 0);
	if (App->startmenu->active == true)
		App->render->DrawQuad({ x,y,rect.w,rect.h }, 255, 0, 0, 255, false, false);

}


