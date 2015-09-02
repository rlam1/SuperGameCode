#pragma once
#include "Init.h"
#include "Bitfield.h"

#include <forward_list>
#include <algorithm>

#include <type_traits> // Used to access raw number of enum class inside overloaded << operator

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

/*
    TODO:
        - Consider using a <map> instead of <forward_list> to store the
          animation data, with the AnimState as key to facilitate finding
          the instructions to render the character.
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

enum class AnimDir {
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

    virtual void Update();
    virtual void Render(float scX, float scY);
    virtual void SendNewState(AnimState state, AnimDir direction);

    void printData();

private:
	ALLEGRO_BITMAP *sourceImage;
    std::string sourceImgPath;
    int frameWidth, frameHeight;
    int rows, columns, frames;

    int frameDelay, frameCount, curFrame;
    AnimState curState;
    AnimDir curDir;

    struct _animations {
        AnimState type;
        int startRow;
        unsigned char sides;

        _animations(AnimState _type, int _startRow, unsigned _sides) :
            type(_type), startRow(_startRow), sides(_sides) {}
    };
    std::forward_list<_animations> animList;

	bool GenErrorImage();
    bool parseADF(std::string resLoc);
    void fallbackToDefaultADF(std::string resLoc);
};  
