#pragma once
#include "Init.h"

#include <forward_list>

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
    MOVE,
    ATTACK0,
    ATTACK1,
    HARMED,
    SPECIAL0,
    SPECIAL1
};

enum class AnimDir : char {
    DOWN,
    UP,
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
    int frameWidth, frameHeight;
    int frameDelay;
    int rows, columns, frames;

    struct _animations {
        AnimState type;
        int startCol;
        unsigned char sides;
    };
    std::forward_list<_animations> animList;

	bool GenErrorImage();
    void parseADF(std::string resLoc);
};  

