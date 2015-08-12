#pragma once
#include "Init.h"

#include <forward_list>

#include <type_traits>

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
    IDLE = 1,
    MOVE,
    ATTACK0,
    ATTACK1,
    HARMED,
    SPECIAL0,
    SPECIAL1
};

std::ostream& operator << (std::ostream& os, const AnimState& obj);

enum class AnimDir : unsigned char {
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

    void printData();

private:
	ALLEGRO_BITMAP *sourceImage;
    std::string sourceImgPath;
    int frameWidth, frameHeight;
    int frameDelay;
    int rows, columns, frames;

    struct _animations {
        AnimState type;
        int startCol;
        unsigned char sides;

        _animations(AnimState _type, int _startCol, unsigned _sides) :
            type(_type), startCol(_startCol), sides(_sides) {}
    };
    std::forward_list<_animations> animList;

	bool GenErrorImage();
    bool parseADF(std::string resLoc);
    void fallbackToDefaultADF(std::string resLoc);
};  

