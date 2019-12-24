#ifndef _J1_SETTINGSSCENE_
#define _J1_SETTINGSSCENE_

#include "j1Module.h"


class UIImage;
class UILabel;
class UIObject;
class UIButton;


class j1SettingsScene : public j1Module {

public:
	j1SettingsScene();
	~j1SettingsScene() {}


	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float);
	bool PostUpdate();
	bool CleanUp();

	UIObject* background = nullptr;
	UIObject* title = nullptr;
	UIObject* return_button = nullptr;

	SDL_Rect background_rect = { 39,845,1024,770 };
	SDL_Rect return_rect_on = { 1440,296,142,59 };
	SDL_Rect return_rect_off = { 1440,220,141,59 };
	UIObject* menu_label = nullptr;

	SDL_Texture* texture;

	iPoint mouse_position, mouse_pos;

};


#endif 
