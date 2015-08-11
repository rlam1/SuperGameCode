#include "stdafx.h"
#include "Sprite.h"


Sprite::Sprite(std::string resLocation)
{
}

Sprite::Sprite()
{
	sourceImage = nullptr;
    GenErrorImage();
}


Sprite::~Sprite()
{
	al_destroy_bitmap(sourceImage);
}

/*
    This makes a blue square with a yellow triangle inside that
    has an exclamation mark. This image is used anytime an image
    file on disk fails to load.

    ---------------
    |      ^      |
    |     / \     |
    |    / ! \    |  <- Kinda like this...
    |   /     \   |
    |   -------   |
    ---------------
*/
bool Sprite::GenErrorImage()
{
	sourceImage = al_create_bitmap(frameWidth, frameHeight);

	if (sourceImage == nullptr)
		return false;

	ALLEGRO_COLOR background = al_color_html("#3f51b5");
	ALLEGRO_COLOR shadow = al_color_html("#1a237e");
	ALLEGRO_COLOR sign = al_color_html("#ffeb3b");

	float w = frameWidth;
	float h = frameHeight;

	al_set_target_bitmap(sourceImage);
	al_clear_to_color(background);

	// Shadow (3 triangles)
	al_draw_filled_triangle(w / 2.0, h / 4.0, w, (h * 3.0) / 4.0, w / 2.0, h, shadow);
	al_draw_filled_triangle(w, (h * 3.0) / 4.0, w, h, w / 2.0, h, shadow);
	al_draw_filled_triangle(w / 2.0, h / 4.0, w / 2.0, h, w / 4.0, (h * 3.0) / 4.0, shadow);

	// Alert sign triangle
	al_draw_filled_triangle(w / 2.0, h / 4.0, ((w * 3.0) / 4), ((h * 3.0) / 4.0), w / 4.0, ((h * 3.0) / 4.0), sign);

	// Exclamation point
	al_draw_filled_rectangle((w * 15.0) / 32.0, ((h * 14.0) / 32.0), ((w * 17.0) / 32.0), ((h * 19.0) / 32.0), background);
	al_draw_filled_rectangle((w * 15.0) / 32.0, (h * 5.0) / 8, (w * 17.0) / 32.0, (h * 11.0) / 16.0, background);

	al_set_target_backbuffer(al_get_current_display());

	return true;
}

void Sprite::parseADF(std::string resLoc)
{
    ALLEGRO_CONFIG *file = al_load_config_file(resLoc.c_str());
    if (file == nullptr)
    {
        std::cerr << "Error: ADF file " << resLoc << " not found!, fallback to errorImage" << std::endl;
        frameWidth = 30; frameHeight = 30;
        frameDelay = 1;
        GenErrorImage();

        rows = 30 / frameHeight;
        columns = 30 / frameWidth;
        frames = rows * columns;

        animList.emplace_front(AnimState::IDLE, 0, 1);
    }
}
