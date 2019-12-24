#ifndef _J1_START_MENU_
#define _J1_START_MENU_

#include "j1Module.h"


class UIImage;
class UIObject;

class j1StartMenu : public j1Module {

public:
	j1StartMenu();
	~j1StartMenu() {}


	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float);
	bool PostUpdate();
	bool CleanUp();

	UIObject * background = nullptr;
	UIObject * play_button = nullptr;

	UIObject* text_start = nullptr;

	iPoint mouse_position, mouse_pos;
};


#endif // !_J1_START_MENU_
