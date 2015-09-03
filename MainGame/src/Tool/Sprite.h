#pragma once
#include "Init.h"
#include "Bitfield.h"

#include <unordered_map>

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

enum AnimDir {
    DOWN,
    UP,
    LEFT,
    RIGHT
};

enum _dirBits {
    bDOWN = 1 << 0,
    bUP = 1 << 1,
    bLEFT = 1 << 2,
    bRIGHT = 1 << 3
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
        int startRow;
        int sides;

        _animations(int _startRow, unsigned _sides) :
            startRow(_startRow), sides(_sides) {}

        _animations() :
            startRow(0), sides(1) {}
    };
    std::unordered_map<AnimState, _animations> animList;

    int animDirToBits(int animDir);
	bool GenErrorImage();
    bool parseADF(std::string resLoc);
    void fallbackToDefaultADF(std::string resLoc);
};  
