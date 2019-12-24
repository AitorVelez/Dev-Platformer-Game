#ifndef _UI_OBJECT_
#define _UI_OBJECT_

#include "SDL/include/SDL.h"
#include "j1Textures.h"
#include "p2Defs.h"
#include "p2Point.h"
#include "j1Gui.h"

class j1Module;


class UIObject {

public:
	UIObject(int x, int y);

	~UIObject();


	virtual void Update();
	virtual void Draw();


	UIObject* object;
	int x;
	int y;
	int margin_right;
	int margin_left;
	SDL_Rect rect;
	p2SString text;
	SDL_Texture* texture = nullptr;
	SDL_Texture* texture_fonts = nullptr;
	SDL_Rect size = { 0,0,0,0 };

	bool debug = false;

	bool is_hud;
};




#endif // !_UIOBJECT_