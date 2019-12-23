#include "UIImage.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1StartMenu.h"
#include "Brofiler/Brofiler.h"

UIImage::UIImage(int x, int y, SDL_Rect rec, SDL_Texture* tex, bool is_hud) : UIObject(x, y) {

	this->x = x;
	this->y = y;
	rect = rec;
	this->texture = tex;
	this->is_hud = is_hud;

}
UIImage::~UIImage() {}


void UIImage::Draw() 
{

	BROFILER_CATEGORY("DrawUIImage", Profiler::Color::Red)

		App->render->Blit(texture, x, y, &rect, 0);
	/*if (App->startmenu->debug == true)
		App->render->DrawQuad({ x,y,rect.w,rect.h }, 255, 0, 0, 255, false, false);*/
}