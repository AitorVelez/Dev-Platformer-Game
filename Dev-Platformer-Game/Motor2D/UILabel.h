#ifndef _UI_LABEL_
#define _UI_LABEL_

#include "UIObject.h"

class UILabel : public UIObject {

public:

	UILabel(int x, int y, p2SString text, bool is_hud);
	~UILabel();

	void Draw();

};

#endif 
