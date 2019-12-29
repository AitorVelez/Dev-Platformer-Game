#ifndef _UI_SLIDER_
#define _UI_SLIDER_

#include "UIObject.h"

class UISlider : public UIObject {

public:
	UISlider(int x, int y, SDL_Rect thumb_off, SDL_Rect thumb_on, int margin_right, int margin_left, SDL_Texture *tex, UIObject* object);
	~UISlider();
	void Update();
	void Draw();
	int x_difference = 0;
	int slider_volume = 0;
	int final_volume = 0;

};



#endif _UI_SLIDER_