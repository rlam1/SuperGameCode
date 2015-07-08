#pragma once
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_color.h>
#include <allegro5\allegro_primitives.h>

#include <string>
#include <iostream>

#include "Vec.h"
#include "Init.h"

class Sprite {
public:
	Sprite(std::string resLocation);
	Sprite(Vec2D size);
	Sprite();
	virtual ~Sprite();

	void SetImagePath(std::string resLocation);
	Vec2D GetSize();
	ALLEGRO_BITMAP* GetSprite();
private:
	ALLEGRO_BITMAP* sourceImage;
    ALLEGRO_BITMAP *oldImage;
	Vec2D size;
	bool isImageLoaded;
	bool isErrorImageLoaded;
	std::string pathToImage;

	bool LoadImageFile();
	bool GenErrorImage();
};

