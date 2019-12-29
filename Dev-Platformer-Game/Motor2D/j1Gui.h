#ifndef _J1_GUI_
#define _J1_GUI_

#include "j1Module.h"
#include "p2Point.h"
#include "p2List.h"
#include "SDL/include/SDL.h"

class UIObject;

enum UIObjectType {

	NoObjectType,
	ImageObject,
	LabelObject,
};

class j1Gui : public j1Module {

public:

	j1Gui();

	~j1Gui();

	bool Awake(pugi::xml_node&);

	bool Start();
	bool PreUpdate();
	bool Update(float);
	bool PostUpdate();
	bool CleanUp();
	bool HUDCleanUp();

	SDL_Texture* atlas = nullptr;

	p2SString atlas_file_name;

	bool debug = false;


	UIObject* CreateUIImage(int x, int y, SDL_Rect rect, SDL_Texture* texture, bool is_hud);
	UIObject* CreateUILabel(int x, int y, p2SString text, bool is_hud);
	UIObject* CreateUIButton(int x, int y, SDL_Rect mouse_off, SDL_Rect mouse_on, SDL_Rect mouse_click, SDL_Texture* texture);
	UIObject* CreateUISlider(int x, int y, SDL_Rect thumb_off, SDL_Rect thumb_on, int margin_right, int margin_left, SDL_Texture* tex, UIObject* object);


private:

	p2List<UIObject*> UIObjects;

	p2List<UIObject*> HUDObjects;	
};




#endif // !_J1_GUI_

