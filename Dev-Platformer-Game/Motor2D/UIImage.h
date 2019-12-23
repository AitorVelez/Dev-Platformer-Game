#ifndef _UI_IMAGE_
#define _UI_IMAGE_

#include "UIObject.h"
#include "p2Point.h"

class UIImage : public UIObject {

public:

	UIImage(int x, int y, SDL_Rect rect, SDL_Texture* tex, bool is_hud);
	~UIImage();

	void Draw();

};

#endif
