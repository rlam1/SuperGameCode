#pragma once
#include "Vec.h"
#include "Init.h"

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

