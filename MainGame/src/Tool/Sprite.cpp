#include "stdafx.h"
#include "Sprite.h"


Sprite::Sprite(std::string resLocation)
{
	pathToImage = resLocation;
	isImageLoaded = LoadImageFile();
	isErrorImageLoaded = false;

	if (!isImageLoaded)
	{
		isErrorImageLoaded = GenErrorImage();
	} else
	{
		size.x = al_get_bitmap_width(sourceImage);
		size.y = al_get_bitmap_height(sourceImage);
	}
}

Sprite::Sprite(Vec2D size)
{
	pathToImage = "";
	sourceImage = nullptr;
	isImageLoaded = false;
	this->size = size;

	isErrorImageLoaded = GenErrorImage();
}

Sprite::Sprite()
{
	pathToImage = "";
	sourceImage = nullptr;
	isImageLoaded = false;
	size.x = 30;
	size.y = 30;

	isErrorImageLoaded = GenErrorImage();
}


Sprite::~Sprite()
{
	al_destroy_bitmap(sourceImage);
}


void Sprite::SetImagePath(std::string resLocation)
{
    //TODO: Make the logic in this code use private LoadImageFile function.

	if (pathToImage == resLocation)
		return;

    //Temporal storage for original image data
    std::string oldPath = pathToImage;
	oldImage = sourceImage;  // This one is class scope to keep pointer if function is terminated.
    bool oldErrorImage = isErrorImageLoaded;
    bool oldImageLoaded = isImageLoaded;

	pathToImage = resLocation;

	sourceImage = al_load_bitmap(pathToImage.c_str());
    if (sourceImage == nullptr)
    {
        isImageLoaded = false;
    } else
    {
        isErrorImageLoaded = false;
        isImageLoaded = true;
        al_destroy_bitmap(oldImage);

        size.x = al_get_bitmap_width(sourceImage);
        size.y = al_get_bitmap_height(sourceImage);

        return;
    }

	if (isImageLoaded == false)         //Restore original Sprite status
	{
		al_destroy_bitmap(sourceImage); //TODO: Check if this is necesarry at all since image loading failed anyways...

		sourceImage = oldImage;
        pathToImage = oldPath;

		isErrorImageLoaded = oldErrorImage;
        isImageLoaded = oldImageLoaded;
	}
}


Vec2D Sprite::GetSize()
{
	return size;
}


ALLEGRO_BITMAP* Sprite::GetSprite()
{
    if (isImageLoaded || isErrorImageLoaded)
    {
        return sourceImage;
    }
    else
    {
        return nullptr;
    }
}


bool Sprite::LoadImageFile()
{
	sourceImage = al_load_bitmap(pathToImage.c_str());

	if (sourceImage == nullptr)
	{
		return false;
	}

	return true;
}


bool Sprite::GenErrorImage()
{
	sourceImage = al_create_bitmap((int) size.x, (int) size.y);

	if (sourceImage == nullptr)
		return false;

	ALLEGRO_COLOR background = al_color_html("#3f51b5");
	ALLEGRO_COLOR shadow = al_color_html("#1a237e");
	ALLEGRO_COLOR sign = al_color_html("#ffeb3b");

	float w = size.x;
	float h = size.y;

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