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


	p2SString text;
	SDL_Texture* texture = nullptr;


};




#endif // !_UIOBJECT_