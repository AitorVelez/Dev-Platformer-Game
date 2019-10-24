#ifndef _j1PLAYER_H__
#define _j1PLAYER_H__

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "Animation.h"

class j1Player :
	public j1Module
{
public:

	j1Player();
	~j1Player();

	void LoadTexture();

	void LoadAnimation(pugi::xml_node animation_node, Animation* animation);

public:

	SDL_Texture* texture = nullptr;
};

#endif