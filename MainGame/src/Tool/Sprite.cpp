#include "stdafx.h"
#include "Sprite.h"

#include <array>

std::array<std::string, 8> sections = { "", "IDLE", "MOVE", "ATTACK0", "ATTACK1", "HARMED", "SPECIAL0", "SPECIAL1" };
std::array<std::string, 6> entries = { "resPath", "frameWidth", "frameHeight", "frameDelay", "startRow", "sides" };

std::ostream& operator << (std::ostream& os, const AnimState& obj)
{
    int type = static_cast<std::underlying_type<AnimState>::type>(obj);
    if (type >= 1 && type < 8)
    {
        os << sections[type];
        return os;
    } else
    {
        os << "Corrupt";
        return os;
    }
}

Sprite::Sprite(std::string resLocation)
{
    if (parseADF(resLocation) == false)
    {
        std::cerr << "Warning: Loading of " << resLocation << " failed. Error Image generated." << std::endl;
        return;
    }
    sourceImage = al_load_bitmap(sourceImgPath.c_str());
    if (sourceImage == nullptr)
    {
        animList.clear();
        fallbackToDefaultADF(sourceImgPath);
    }
    rows = al_get_bitmap_height(sourceImage) / frameHeight;
    columns = al_get_bitmap_width(sourceImage) / frameWidth;
    frames = rows * columns;
    frameCount = 0;

    // Quick additional sanity check
    for (auto &anim : animList)
    {
        if (anim.startRow <= 0 || anim.startRow > rows - 1)
        {
            std::cerr << "Warning: Animation out of bounds, will default to start at origin. " << resLocation << std::endl
                << "         rows = " << rows << ", and animation wants to start at " << anim.startRow << std::endl;
            anim.startRow = 0;
        }
    }

    printData();
}

Sprite::Sprite()
{
	sourceImage = nullptr;
    frameCount = 0;
    fallbackToDefaultADF("INTENTIONAL ERROR TRIGGER, DISREGARD ERROR");
}


Sprite::~Sprite()
{
	al_destroy_bitmap(sourceImage);
    animList.clear();
}

void Sprite::Update()
{
    frameCount++;
    if (frameCount >= frameDelay)
    {
        curFrame++;
        if (curFrame >= columns)
        {
            curFrame = 0;
        }

        frameCount = 0;
    }
}

void Sprite::Render(float scX, float scY)
{
    // TODO: ADD CHECK TO VERIFY SPRITESHEET ACTUALLY CONTAINS THE CURRENT DIRECTION
    //       WE GUARANTEE THE ANIMSTATE IS VALID BUT NOT THE DIRECTION AT THIS POINT
    al_draw_bitmap_region(sourceImage, curFrame * frameWidth, 2 * frameHeight, frameWidth, frameHeight, scX, scY, 0);
}

void Sprite::SendNewState(AnimState state, AnimDir direction)
{
    curDir = direction;
    
    for (const auto &elem : animList)
    {
        if (elem.type == state)
        {
            curState = state;
            break;
        } else
        {
            curState = AnimState::IDLE;
            std::cerr << "Warning: Spritesheet at " << sourceImgPath << " has no " << state << " animation defined!" << std::endl
                << "         Defaulting to IDLE animation." << std::endl;
        }
    }

    curFrame = 0;
    frameCount = 0;
}

void Sprite::printData()
{
    std::cout << "****************SPRITE DATA DUMP****************" << std::endl
        << "resPath=" << sourceImgPath << ", loaded at 0x" << sourceImage << std::endl
        << "frames: Width=" << frameWidth << " | Height=" << frameHeight << " | Delay=" << frameDelay << std::endl
        << "total frames=" << frames << " rows=" << rows << " columns=" << columns << std::endl
        << "Animations: " << std::endl;

    for (const auto &anim : animList)
    {
        std::cout << std::endl << " Type=" << anim.type << std::endl
            << "  Starting Row=" << anim.startRow << std::endl
            << "  Active sides(BITFIELD)=" << (int)anim.sides << std::endl;
    }
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

void Sprite::fallbackToDefaultADF(std::string resLoc)
{
    std::cerr << "Error: Resource " << resLoc << " not found!, fallback to errorImage" << std::endl;
    frameWidth = 30; frameHeight = 30;
    frameDelay = 1;
    sourceImgPath = "";
    GenErrorImage();

    rows = 30 / frameHeight;
    columns = 30 / frameWidth;
    frames = rows * columns;

    animList.emplace_front(AnimState::IDLE, 0, 1);
    return;
}

/*
Load the file
Read first section
while we have a valid section:
    If the first section is the blank section ("") then enter the section
        read the first entry
        while we have a valid entry
            map the entry to its internal container value
            read next entry
    If the section is any other section, map the name to its animation state.
        read the first entry
        while we have a valid entry
            map the entry to its internal container value
            read next entry
    read next section
*/
bool Sprite::parseADF(std::string resLoc)
{
    ALLEGRO_CONFIG *file = al_load_config_file(resLoc.c_str());
    if (file == nullptr)
    {
        fallbackToDefaultADF(resLoc);
        return false;
    }

    ALLEGRO_CONFIG_SECTION *section = nullptr;
    ALLEGRO_CONFIG_ENTRY   *entry   = nullptr;
    const char *sectionName = nullptr;
    const char *entryName   = nullptr;

    sectionName = al_get_first_config_section(file, &section);

    while (section != nullptr)
    {
        if (sectionName == sections[0]) // Global section
        {
            entryName = al_get_first_config_entry(file, sectionName, &entry);
            while (entry != nullptr)
            {
                if (entryName == entries[0])
                {
                    sourceImgPath = al_get_config_value(file, sectionName, entryName);
                } else if (entryName == entries[1])
                {
                    frameWidth = std::atoi(al_get_config_value(file, sectionName, entryName));
                } else if (entryName == entries[2])
                {
                    frameHeight = std::atoi(al_get_config_value(file, sectionName, entryName));
                } else if (entryName == entries[3])
                {
                    frameDelay = std::atoi(al_get_config_value(file, sectionName, entryName));
                } else
                {
                    std::cerr << "Warning: Unknown key-pair for value at " << sectionName << "." << entryName << std::endl;
                }

                entryName = al_get_next_config_entry(&entry);
            }
        } else // All other sections
        {
            AnimState state;
            int start;
            unsigned char sides;

            if (sectionName == sections[1])
            {
                state = AnimState::IDLE;
            } else if (sectionName == sections[2])
            {
                state = AnimState::MOVE;
            } else if (sectionName == sections[3])
            {
                state = AnimState::ATTACK0;
            } else if (sectionName == sections[4])
            {
                state = AnimState::ATTACK1;
            } else if (sectionName == sections[5])
            {
                state = AnimState::HARMED;
            } else if (sectionName == sections[6])
            {
                state = AnimState::SPECIAL0;
            } else if (sectionName == sections[7])
            {
                state = AnimState::SPECIAL1;
            } else
            {
                std::cerr << "Warning: Unknown section name: " << sectionName << " default to IDLE" << std::endl;
                state = AnimState::IDLE;
            }

            entryName = al_get_first_config_entry(file, sectionName, &entry);
            while (entry != nullptr)
            {
                if (entryName == entries[4]) // Start Row
                {
                    start = std::atoi(al_get_config_value(file, sectionName, entryName));
                } else if (entryName == entries[5]) // Sides bitfield
                {
                    sides = std::atoi(al_get_config_value(file, sectionName, entryName));
                } else
                {
                    std::cerr << "Warning: Unknown key-pair for value at " << sectionName << "." << entryName << std::endl;
                }

                entryName = al_get_next_config_entry(&entry);
            }

            animList.emplace_front(state, start, sides);
        }

        sectionName = al_get_next_config_section(&section);
    }

    al_destroy_config(file);
    return true;
}
