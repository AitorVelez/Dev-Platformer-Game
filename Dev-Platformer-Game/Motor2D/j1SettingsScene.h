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
	UIObject* volume_bar = nullptr;
	UIObject* volume_thumb = nullptr;
	UIObject* volume_music_label = nullptr;
	UIObject* volume_fx_label = nullptr;
	UIObject* volume_thumb_fx = nullptr;
	UIObject* volume_bar_fx = nullptr;

	SDL_Rect background_rect = { 39,845,1024,770 };
	SDL_Rect return_rect_on = { 1440,296,142,59 };
	SDL_Rect return_rect_off = { 1440,220,141,59 };
	SDL_Rect thumb_rect_off = { 1642,35,84,84 };
	SDL_Rect thumb_rect_on = { 1642,125,84,85 };
	SDL_Rect volumen_rect = { 1100,458,431,88 };


	UIObject* menu_label = nullptr;

	SDL_Texture* texture;

	iPoint mouse_position, mouse_pos;

};


#endif 
