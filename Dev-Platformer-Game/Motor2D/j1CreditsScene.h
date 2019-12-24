#ifndef _J1_CREDITSSCENE_
#define _J1_CREDITSSCENE_

#include "j1Module.h"
#include "p2Point.h"

class UIImage;
class UIObject;
class UILabel;

class j1CreditsScene :public j1Module
{

public:

	j1CreditsScene();
	~j1CreditsScene() {}

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float);
	bool PostUpdate();
	bool CleanUp();

	UIObject* background = nullptr;

};

#endif

