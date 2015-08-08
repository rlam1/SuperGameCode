#pragma once
#include "Vec.h"
#include "Init.h"

/*
    Feature List
    - Loads spritesheet
    - Loads animation configuration
    - Each tick it updates the animation state
        - When initialized it loads an idle state,
          and loops it each tick
        - The update message can contain a change of state
          that causes a new different loop to happen
    - Upon request draws current animation frame
        - When it is time to draw the animation, it needs to know
          where to put the frame on the world.
*/

enum class AnimState {
    IDLE,
    MOVING,
    ATTACKING,
    HARMED,
    SPECIAL0,
    SPECIAL1
};

enum class AnimDir : char {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Sprite {
public:
	Sprite(std::string resLocation);
	Sprite();
	virtual ~Sprite();

private:
	ALLEGRO_BITMAP* sourceImage;
	Vec2D size;

	bool GenErrorImage();
};

