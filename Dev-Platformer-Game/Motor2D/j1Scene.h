#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Timer.h"

struct SDL_Texture;
class UIObject;
class UILabel;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Save
	bool Save(pugi::xml_node& data)const;

	//Load
	bool Load(pugi::xml_node& savegame);

	void LoadScene(int map);

	int current_map = 1;

	float camPos;

	iPoint mouse_position, mouse_pos;

	SDL_Rect return_rect_on = { 1443, 298,142,58 };
	SDL_Rect return_rect_off = { 1443,222,141,58 };

	SDL_Rect button_off_mouse;
	SDL_Rect button_on_mouse;

	UIObject* resume_button = nullptr;
	UIObject* exit_button = nullptr;
	UIObject * main_menu_button = nullptr;
	
	UIObject* text_resume = nullptr;
	UIObject* text_exit = nullptr;
	UIObject* text_main_menu = nullptr;
	UIObject* lifes_title = nullptr;
	UIObject* coins_title = nullptr;


	/*UIImage* heart1 = nullptr;
	UIImage* heart2 = nullptr;
	UIImage* heart3 = nullptr;*/

	bool time_pause = false;

	bool loading = false;

	SDL_Texture* texture;

	j1Timer timer;
	j1Timer pause_timer;

	float cont_pause_timer = 0.0f;
	float cont_timer = 0.0f;
	float time_start = 0.0f;
	bool pause_menu = false;

private:

	bool change_volume = false;
	bool close_game = false;
	bool foto_mode = false;

	unsigned int camerafx;
	unsigned int photofx;

	//j1Timer timer;
	//j1Timer pause_timer;

	//float cont_pause_timer = 0.0f;
	//float cont_timer = 0.0f;
	//float time_start = 0.0f;
};

#endif // __j1SCENE_H__