#include "j1App.h"
#include "j1Fonts.h"
#include "UILabel.h"
#include "j1Textures.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1StartMenu.h"
#include "Brofiler/Brofiler.h"

UILabel::UILabel(int x, int y, p2SString text, bool is_hud) : UIObject(x, y)
{
	this->x = x;
	this->y = y;
	this->text = text;
	this->is_hud = is_hud;

	int w = 0, h = 0;
	App->fonts->CalcSize(this->text.GetString(), w, h, App->fonts->default);
	size.w = w;
	size.h = h;
	texture_fonts = App->fonts->Print(text.GetString(), { 255, 255, 255, 255 }, App->fonts->default);

}


UILabel::~UILabel()
{
	text.Clear();
	App->tex->UnLoad(texture_fonts);
	text = nullptr;
	texture_fonts = nullptr;
	delete texture_fonts;
}

void UILabel::Draw() {

	BROFILER_CATEGORY("UILabelDraw", Profiler::Color::Green)

		App->render->Blit(texture_fonts, x, y);


}